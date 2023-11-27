/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Decals.cpp
// It's The GreenRay Decals Engine.
// Made Specially For Implementing Decals Technology.
// Date Creation: 29 August 2005
// Last Changing: 06 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Decals.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRDecal Class
//////////////////////////////////////////////////////////////////////////////
CGRDecal::CGRDecal(LPDIRECT3DDEVICE9 pDevice,LPCSTR DecalName,float Height,float Width)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  //D3DXCreatePolygon(d_pDevice,10,4,&m_pDecal,0); 
  CreateDecalSurface(Width,Height); //Create Decal Polygon

  //Set Default Parameters
  dwColor = D3DCOLOR_XRGB(255,255,255);

  //Create Decal Texture
  if (FAILED(D3DXCreateTextureFromFileEx(d_pDevice,DecalName,0,0,0,0,
  D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,dwColor,NULL,NULL,&m_pTexture)))
  {
    Log("GR_Decals.cpp: Can't Create Decal Texture.. \n");
	return;
  }
} //EndConstructionDirectives

CGRDecal::~CGRDecal()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRDecal::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRDecal::Release()
{
  //ReleaseClass(m_pDecal); //Free Decal
  if (m_pTexture != NULL) ReleaseClass(m_pTexture); 
  ReleaseClass(m_pVertexBuffer);
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::CreateDecalSurface()
// Desc: Initialize Our Vertexes Structure
//-----------------------------------------------------------------------------
HRESULT CGRDecal::CreateDecalSurface(float fWidth,float fHeight)
{
  HRESULT _Result = S_OK;
  const FLOAT fDepth = 0.0f; //Z

  //Vertexes Description
  DECAL_VERTEX Vertexes[] = 
  {
	{-fWidth,  fHeight, fDepth, 0.0f, 0.0f}, //A
	{ fWidth,  fHeight, fDepth, 1.0f, 0.0f}, //B
	{ fWidth, -fHeight, fDepth, 1.0f, 1.0f}, //C

	{ fWidth, -fHeight, fDepth, 1.0f, 1.0f}, //C
	{-fWidth, -fHeight, fDepth, 0.0f, 1.0f}, //D
	{-fWidth,  fHeight, fDepth, 0.0f, 0.0f}, //A
     //X          Y        Z    TexU  TexV
  }; //End Vertexes Description

  //Create Vertex Buffer
  if (FAILED(d_pDevice->CreateVertexBuffer(6*sizeof(DECAL_VERTEX),0, 
  D3DFVF_DECALVERTEX,D3DPOOL_DEFAULT,&m_pVertexBuffer,NULL))) _Result = E_FAIL;

  //Lock The Vertex Buffer
  VOID* pBV;
  if (FAILED(m_pVertexBuffer->Lock(0,sizeof(Vertexes),(void**)&pBV,0))) _Result = E_FAIL;
  memcpy(pBV,Vertexes,sizeof(Vertexes)); //Copy Vertex Data To Memory
  m_pVertexBuffer->Unlock(); //Unlock The Vertex Buffer
  
  return _Result;
} //EndCreateSkyBoxFunction

//-----------------------------------------------------------------------------
// Name: CGRDecal::SetCoord()
// Desc: 
//-----------------------------------------------------------------------------
void CGRDecal::SetCoord(D3DXVECTOR3 vPos,D3DXVECTOR3 vNormal,D3DXVECTOR3 vVec1,D3DXVECTOR3 vVec2)
{ 
  Pos = D3DXVECTOR3(vPos); //Get RayCast Point Position
  Normal = D3DXVECTOR3(vNormal); //Get Normals

  //Get Polygon Vertices Vectors
  Vec_1 = D3DXVECTOR3(vVec1); 
  Vec_2 = D3DXVECTOR3(vVec2);
	
  //Build Polygon Rotation Matrices
  //Set Needed Matrices
  D3DXVec3Normalize(&Normal,&Normal);

  //if (Normal = D3DXVECTOR3(0,1,0)) { Normal = D3DXVECTOR3(1,0,0); }

  D3DXVec3Cross(&Vec_1,&Normal,&Vec_2);
  D3DXVec3Cross(&Vec_2,&Vec_1,&Normal);
  D3DXVec3Normalize(&Vec_2,&Vec_2);
  D3DXVec3Normalize(&Vec_1,&Vec_1);

  Mat_Trans._11 = Vec_1.x;
  Mat_Trans._12 = Vec_1.y;
  Mat_Trans._13 = Vec_1.z;

  Mat_Trans._21 = Vec_2.x;
  Mat_Trans._22 = Vec_2.y;
  Mat_Trans._23 = Vec_2.z; 

  Mat_Trans._31 = Normal.x;
  Mat_Trans._32 = Normal.y;
  Mat_Trans._33 = Normal.z;

  Mat_Trans._41 = Pos.x;
  Mat_Trans._42 = Pos.y;
  Mat_Trans._43 = Pos.z;

  Mat_Trans._14 = 0.0f;
  Mat_Trans._24 = 0.0f;
  Mat_Trans._34 = 0.0f;
  Mat_Trans._44 = 1.0f;
} //EndSetCoordProcedure

//-----------------------------------------------------------------------------
// Name: CGRDecal::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CGRDecal::Render()
{
  //Setting Rendering Parameters
  d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
  d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);  
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

  //Enable Alpha Blending So We Can Use Transparent Textures
  d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

  //Set How The Texture Should Be Blended (Use Alpha)
  d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
  d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
  
  //Set Perspective View For The Model In Space
  float Aspect_Ratio = 1.1f;
  D3DXMATRIXA16 Mat_Proj;
  D3DXMatrixPerspectiveFovLH(&Mat_Proj,/*D3DX_PI/4*/D3DXToRadian(60.0f),Aspect_Ratio,1.1f,10000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&Mat_Proj);

  //Mat_Trans = (Mat_Rot*Mat_Pos);
  d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Trans);

  //Set Vertex And Index Buffers To Render Decal
  d_pDevice->SetTexture(0,NULL);
  d_pDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(DECAL_VERTEX));
  d_pDevice->SetFVF(D3DFVF_DECALVERTEX);

  //Set Texture And Draw Decal Mesh
  d_pDevice->SetTexture(0,m_pTexture);
  //d_pDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
  d_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
  //m_pDecal->DrawSubset(0); //Draw Decal

  //Set Default Render States
  d_pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_STENCILENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
} //EndDrawProcedure

//-----------------------------------------------------------------------------