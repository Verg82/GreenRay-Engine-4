/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Object.cpp
// It's The GreenRay Object Loader Interface.
// Made Specially For Implementing Loading And Rendering 3D Objects.
// Date Creation: 29 August 2005
// Last Changing: 29 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Object.h>

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
//Vertex Declarations For Another X-Files Converted From 3DS Max
D3DVERTEXELEMENT9 g_pVertDecl[] =
{
  {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  //{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
  //{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},	  
  D3DDECL_END()
};

D3DVERTEXELEMENT9 Elements[] =
{
  {0, sizeof(float)*0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, sizeof(float)*3,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, sizeof(float)*6,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  {0, sizeof(float)*8,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
  {0, sizeof(float)*11, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
  D3DDECL_END()
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRObject Class
///////////////////////////////////////////////////////////////////////////////
CGRObject::CGRObject()
{
  m_pObject = NULL;
  pVertDecl = NULL;
  g_pMaterials = NULL;
} //EndConstructionDirectives

CGRObject::~CGRObject()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRObject::Create()
// Desc: 
//-----------------------------------------------------------------------------
void CGRObject::Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR File_Name,UINT MapAmplitude)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
 
  //Create Object To Represent
  {
    m_pObject = new CGRMeshX(); //Construct Object Or Model Class
    if (m_pTexPath != NULL) m_pObject->SetTexturePath(m_pTexPath); //Set Object Texture Path

    //Create Vertex Declaration
    d_pDevice->CreateVertexDeclaration(g_pVertDecl,&pVertDecl);
 
    //Load An Object To Cast
    if (FAILED(m_pObject->Create(d_pDevice,File_Name,MapAmplitude)))
    {
      Log("GR_Object.cpp: Error Load Map Location Model.. \n");
    }

	//Compute The Tangent Frame For The Mesh. (Needed For Normal Mapping)
	/*ID3DXMesh* Temp;
	ID3DXMesh* m_pMesh = m_pObject->GetLocalMesh();
	m_pMesh->CloneMesh(D3DXMESH_MANAGED, Elements, d_pDevice, &Temp);
	m_pMesh->Release();
	m_pMesh = Temp;
	void* Data;
	m_pMesh->LockVertexBuffer(D3DLOCK_DISCARD,&Data);
	m_pMesh->UnlockVertexBuffer();
	D3DXComputeTangentFrame(m_pMesh,NULL);*/

    //Set A Reasonable Vertex Type
    m_pObject->SetVertexDecl(g_pVertDecl);
    //m_pObject->SetFVF(d_pDevice,MODEL_VERTEX_FVF);  

    //Initialize The Vertex Buffers For The File-Based Objects
    m_pObject->RestoreObjects();
  } //End Create Object-Model
} //EndCreateProcedure

//-----------------------------------------------------------------------------
// Name: CGRObject::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRObject::Release()
{
  //Free Object Model
  if (m_pObject != NULL) m_pObject = NULL;
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRObject::SetTexturePath()
// Desc: This Procedure For Setting Path Where Texture There Found
//-----------------------------------------------------------------------------
void CGRObject::SetTexturePath(LPCSTR Tex_Path)
{
  m_pTexPath = Tex_Path;
} //EndSetTexturePathProcedure

//-----------------------------------------------------------------------------
// Name: CGRObject::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CGRObject::Render()
{
  //Render State For Until Rendering Mesh Object
  d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  //VU Addr
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);

  //Set Texturing State Parameters
  //d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
  //d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
  
  //Anisotropic Texture Filtering
  d_pDevice->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,8);
  d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
  d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
  d_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_ANISOTROPIC);

  //Set Perspective View For The Model In Space
  float Aspect_Ratio = 1.1f;
  D3DXMATRIXA16 Mat_Proj;
  D3DXMatrixPerspectiveFovLH(&Mat_Proj,/*D3DX_PI/4*/D3DXToRadian(60.0f),Aspect_Ratio,1.0f,100000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&Mat_Proj);

  //Translation Matrices For The Model Object
  D3DXMATRIXA16 Mat_Pos;
  D3DXMatrixTranslation(&Mat_Pos,0.0f, 0.0f, 0.0f);
  d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Pos); //Transform Object Position In Space

  //Set The World Matrix
  D3DXMATRIX World;
  D3DXMatrixIdentity(&World);


  //Rendering Model Here
  LPD3DXMESH m_pMesh = m_pObject->GetLocalMesh();
   
  //Set And Draw Each Of The Materials In The Mesh
  for (DWORD I = 0; I < m_pObject->m_dwNumMaterials; ++I)
  {   
    //d_pDevice->SetMaterial(&m_pMesh->m_pMaterials[I]);
    d_pDevice->SetTexture(0,m_pObject->m_pTextures[I]);
    m_pMesh->DrawSubset(I);
  }
} //EndRenderProcedure

//-----------------------------------------------------------------------------