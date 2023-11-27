/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_MeshXLoader.cpp
// It's The GreenRay Lighting Shader Engine.
// Mesh Handler Class, Sample That Render Lots Of Different Meshes.
// Date Creation: 29 August 2005
// Last Changing: 17 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_MeshXLoader.h>

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
//Vertex Declarations For Another X-Files Converted From 3DS Max
D3DVERTEXELEMENT9 VertexDecl[] =
{
  {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  //{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
  //{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},	  
  D3DDECL_END()
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRModMeshX Class
///////////////////////////////////////////////////////////////////////////////
CGRModMeshX::CGRModMeshX()
{
  //Set Default Parameters
//  m_pMesh = NULL; 
  m_pShader = NULL; 
  m_bCastShadow = true;
  
  pVertDecl = NULL;
  g_pMaterials = NULL;

} //EndConstructionDirectives

CGRModMeshX::~CGRModMeshX()
{
  ReleaseClass(m_pShader);
  //ReleaseClass(m_pMesh);
  if (m_pObject != NULL) m_pObject = NULL; //Free Object Model
  d_pDevice = NULL; //NULL Device Pointer
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRModMeshX::Create()
// Desc: Creates The Mesh Using The Specified FX File For Rendering
//-----------------------------------------------------------------------------
HRESULT CGRModMeshX::Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR MeshName,const LPCSTR FxFile)
{
  HRESULT Result;
  d_pDevice = pDevice; //Apropritate Device Pointer To Class

  //Create Object To Represent
  m_pObject = new CGRMeshX(); //Construct Object Or Model Class
  if (m_pTexPath != NULL) m_pObject->SetTexturePath(m_pTexPath); //Set Object Texture Path

  //Create Vertex Declaration
  d_pDevice->CreateVertexDeclaration(VertexDecl,&pVertDecl);
 
  //Load An Object To Cast
  if (FAILED(m_pObject->Create(d_pDevice,MeshName,100)))
  {
    Log("GR_MeshXLoader.cpp: Error Load Model.. \n");
	return Result;
  }

  //Set A Reasonable Vertex Type
  m_pObject->SetVertexDecl(VertexDecl);
  //m_pObject->SetFVF(MODEL_VERTEX_FVF);  

  //Initialize The Vertex Buffers For The File-Based Objects
  m_pObject->RestoreObjects();


  //
// D3DXCreateBox(d_pDevice, 1.0f, 1.0f, 1.0f, &m_pMesh, NULL);


  //Create An Effect (Outputs The Mesh Into The Fat Buffer)
  if (FAILED(Result = D3DXCreateEffectFromFile(d_pDevice, 
                                               FxFile,
                                               0, //Macros
											   0, //Include
											   0, //Flags
											   gl_LObj->m_EffectPool, //Effect Pool
											   &m_pShader,0)))
  {
    Log("GR_MeshXLoader.cpp: Error Load Lighting Shader.. \n");
    return Result;
  }

  D3DXMatrixIdentity(&m_WorldMatrix);

  return S_OK;
} //EndCreateFunction

//-----------------------------------------------------------------------------
// Name: CGRModMeshX::SetTexturePath()
// Desc: This Procedure For Setting Path Where Texture There Found
//-----------------------------------------------------------------------------
void CGRModMeshX::SetTexturePath(LPCSTR Tex_Path)
{
  m_pTexPath = Tex_Path;
} //EndSetTexturePathProcedure

//-----------------------------------------------------------------------------
// Name: CGRModMeshX::Reset()
// Desc: Called When The Device Has Changed In Some Way
//-----------------------------------------------------------------------------
void CGRModMeshX::Reset()
{
  if (m_pShader) m_pShader->OnLostDevice();
} //EndResetProcedure

//-----------------------------------------------------------------------------
// Name: CGRModMeshX::Render()
// Desc: Render The Mesh Handle MRT Or Single Render Target
//-----------------------------------------------------------------------------
void CGRModMeshX::Render(UINT RenderPass)
{
  const D3DXMATRIXA16& matView = gl_LObj->m_View;
  const D3DXMATRIXA16& matViewProjection = gl_LObj->m_ViewProj;

  D3DXMATRIXA16 matWorldView,matWorldViewProjection;
  D3DXMatrixMultiply(&matWorldView,&m_WorldMatrix,&matView);
  D3DXMatrixMultiply(&matWorldViewProjection,&m_WorldMatrix,&matViewProjection);

  D3DXMATRIXA16 matTransInvWorldView;
  D3DXMatrixInverse(&matTransInvWorldView,0,&matWorldView);
  D3DXMatrixTranspose(&matTransInvWorldView,&matTransInvWorldView);

  m_pShader->SetMatrix("matWorld",&m_WorldMatrix);
  m_pShader->SetMatrix("matWorldView",&matWorldView);
  m_pShader->SetMatrix("matWorldViewProjection",&matWorldViewProjection);
  m_pShader->SetMatrix("matTranInvWorldView",&matTransInvWorldView);

  //Set Render Technique
  if (gl_LObj->m_Framebuffer->GetType() == MT_MRT) m_pShader->SetTechnique("T0");
  m_pShader->CommitChanges();

  //Render The Mesh
  LPD3DXMESH m_pmMesh = m_pObject->GetLocalMesh();

  UINT iPass,iPasses;
  m_pShader->Begin(&iPasses,0/*D3DXFX_DONOTSAVESTATE*/);
    
  for (iPass = 0; iPass < iPasses; ++iPass)
  {
    m_pShader->BeginPass(iPass);

    //Set And Draw Each Of The Materials In The Mesh
    for (DWORD I = 0; I < m_pObject->m_dwNumMaterials; ++I)
    {   
      //Set Texture For The Mesh Object
      if (m_pObject->m_pTextures[I]) m_pShader->SetTexture("DiffuseMapTexture",m_pObject->m_pTextures[I]);  
	  m_pShader->CommitChanges();

      m_pmMesh->DrawSubset(I);
	}
    m_pShader->EndPass();
  }
  m_pShader->End();
} //EndRenderProcedure

//-----------------------------------------------------------------------------
// Name: CGRModMeshX::RenderWeapon()
// Desc: Render The Mesh Handle MRT Or Single Render Target
//-----------------------------------------------------------------------------
void CGRModMeshX::RenderWeapon(UINT RenderPass)
{
  //Set Perspective View For The Model In Space
  float fAspect = 1.1f;
  D3DXMATRIXA16 Mat_Proj;
  D3DXMatrixPerspectiveFovLH(&Mat_Proj,D3DX_PI/4,fAspect,0.1f,1000.0f);

  //Translation Matrices For The Model Object
  D3DXMATRIXA16 Mat_View;
  D3DXMatrixTranslation(&Mat_View, 0.0f, 0.0f, 0.0f);

  D3DXMATRIXA16 MatViewProj;
  D3DXMatrixMultiply(&MatViewProj,&Mat_View,&Mat_Proj);

  //Transform Object Position In Space Matrices
  D3DXMATRIXA16 Pos_Matrix;
  D3DXMatrixIdentity(&Pos_Matrix);
  Pos_Matrix._41 = 1.0f;
  Pos_Matrix._42 = -1.0f;
  Pos_Matrix._43 = 4.0f;
  m_WorldMatrix = Pos_Matrix;

  //Set Matrices To Shader
  const D3DXMATRIXA16& matView = Mat_View;
  const D3DXMATRIXA16& matViewProjection = MatViewProj;

  D3DXMATRIXA16 matWorldView,matWorldViewProjection;
  D3DXMatrixMultiply(&matWorldView,&m_WorldMatrix,&matView);
  D3DXMatrixMultiply(&matWorldViewProjection,&m_WorldMatrix,&matViewProjection);

  D3DXMATRIXA16 matTransInvWorldView;
  D3DXMatrixInverse(&matTransInvWorldView,0,&matWorldView);
  D3DXMatrixTranspose(&matTransInvWorldView,&matTransInvWorldView);

  m_pShader->SetMatrix("matWorld",&m_WorldMatrix);
  m_pShader->SetMatrix("matWorldView",&matWorldView);
  m_pShader->SetMatrix("matWorldViewProjection",&matWorldViewProjection);
  m_pShader->SetMatrix("matTranInvWorldView",&matTransInvWorldView);

  //Set Render Technique
  if (gl_LObj->m_Framebuffer->GetType() == MT_MRT) m_pShader->SetTechnique("T0");
  else
  {
    if (RenderPass == 0) m_pShader->SetTechnique("T2");
	else if (RenderPass == 1) m_pShader->SetTechnique("T3");
	else if (RenderPass == 2) m_pShader->SetTechnique("T4");
	else if (RenderPass == 3) m_pShader->SetTechnique("T5");
  }
  m_pShader->CommitChanges();





  //Render Weapon
  //Setting ViewPort Variables And Parameters For Rendering Weapon Model
  D3DSURFACE_DESC m_pSurface;
  D3DVIEWPORT9 ViewPort = {0, 0, 0, 0, 0.0f, 0.1f};
  D3DVIEWPORT9 OriginalVP;
  IDirect3DSurface9* pBackBuffer = NULL;

  d_pDevice->GetRenderTarget(0,&pBackBuffer);
  pBackBuffer->GetDesc(&m_pSurface);
  
  //Calculate The Viewport  
  ViewPort.Width = 1024; //ScrWidth;
  ViewPort.Height = 768; //ScrHeight;
                                                               
  if (ViewPort.Width > m_pSurface.Width) ViewPort.Width = m_pSurface.Width; 
  if (ViewPort.Height > m_pSurface.Height) ViewPort.Height = m_pSurface.Height;
  
  ViewPort.X = 0;
  ViewPort.Y = 0;

  //Prepare The Viewport For Rendering
  d_pDevice->GetViewport(&OriginalVP);
  d_pDevice->SetViewport(&ViewPort);

  //Render Some Weapon Model
  {
    //Render State For Until Rendering Mesh Object
    d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
    d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //Render The Mesh
    LPD3DXMESH m_pmMesh = m_pObject->GetLocalMesh();

    UINT iPass,iPasses;
    m_pShader->Begin(&iPasses,0/*D3DXFX_DONOTSAVESTATE*/);
    
    for (iPass = 0; iPass < iPasses; ++iPass)
    {
      m_pShader->BeginPass(iPass);

      //Set And Draw Each Of The Materials In The Mesh
      for (DWORD I = 0; I < /*m_pObject->m_dwNumMaterials;*/ 11; ++I)
      {   
        //Set Texture For The Mesh Object
        //if (m_pObject->m_pTextures[I]) m_pShader->SetTexture("DiffuseMapTexture",m_pObject->m_pTextures[I]);  
	    //d_pDevice->SetTexture(0,m_pObject->m_pTextures[I]);
	  
	    //m_pShader->CommitChanges();

        m_pmMesh->DrawSubset(I);
	  }

	  //m_pMesh->DrawSubset(0);
      m_pShader->EndPass();
    }
    m_pShader->End();
  } //End Render Weapon Model

  //When All Render Passed Then Release Viewports 
  ReleaseClass(pBackBuffer);
  d_pDevice->SetViewport(&OriginalVP);
} //EndRenderWeaponProcedure





//-----------------------------------------------------------------------------
// Name: CGRModMeshX::RenderShell()
// Desc: Render The Weapon Shell Mesh Handle MRT Or Single Render Target
//-----------------------------------------------------------------------------
void CGRModMeshX::RenderShell(UINT RenderPass)
{
//  m_WorldMatrix = gl_LObj->m_ShellView;

  //Set Matrices To Shader
  const D3DXMATRIXA16& matView = gl_LObj->m_View;
  const D3DXMATRIXA16& matViewProjection = gl_LObj->m_ViewProj;

  //const D3DXMATRIXA16& matView = Mat_View;
  //const D3DXMATRIXA16& matViewProjection = MatViewProj;

  D3DXMATRIXA16 matWorldView,matWorldViewProjection;
  D3DXMatrixMultiply(&matWorldView,&m_WorldMatrix,&matView);
  D3DXMatrixMultiply(&matWorldViewProjection,&m_WorldMatrix,&matViewProjection);

  D3DXMATRIXA16 matTransInvWorldView;
  D3DXMatrixInverse(&matTransInvWorldView,0,&matWorldView);
  D3DXMatrixTranspose(&matTransInvWorldView,&matTransInvWorldView);

  m_pShader->SetMatrix("matWorld",&m_WorldMatrix);
  m_pShader->SetMatrix("matWorldView",&matWorldView);
  m_pShader->SetMatrix("matWorldViewProjection",&matWorldViewProjection);
  m_pShader->SetMatrix("matTranInvWorldView",&matTransInvWorldView);

  //Set Render Technique
  if (gl_LObj->m_Framebuffer->GetType() == MT_MRT) m_pShader->SetTechnique("T0");
  else
  {
    if (RenderPass == 0) m_pShader->SetTechnique("T2");
	else if (RenderPass == 1) m_pShader->SetTechnique("T3");
	else if (RenderPass == 2) m_pShader->SetTechnique("T4");
	else if (RenderPass == 3) m_pShader->SetTechnique("T5");
  }
  m_pShader->CommitChanges();





 



  //Render Some Weapon Shell Model
  {
    //Render State For Until Rendering Mesh Object
    d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
    d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //Render The Mesh
    LPD3DXMESH m_pmMesh = m_pObject->GetLocalMesh();

    UINT iPass,iPasses;
    m_pShader->Begin(&iPasses,0/*D3DXFX_DONOTSAVESTATE*/);
    
    for (iPass = 0; iPass < iPasses; ++iPass)
    {
      m_pShader->BeginPass(iPass);

      //Set And Draw Each Of The Materials In The Mesh
      for (DWORD I = 0; I < m_pObject->m_dwNumMaterials; ++I)
      {   
        //Set Texture For The Mesh Object
        if (m_pObject->m_pTextures[I]) m_pShader->SetTexture("DiffuseMapTexture",m_pObject->m_pTextures[I]);  
	    //d_pDevice->SetTexture(0,m_pObject->m_pTextures[I]);
	  
	    m_pShader->CommitChanges();

        m_pmMesh->DrawSubset(I);
	  }

	  //m_pMesh->DrawSubset(0);
      m_pShader->EndPass();
    }
    m_pShader->End();
  } //End Render Weapon Shell Model
} //EndRenderShellProcedure





//-----------------------------------------------------------------------------
// Name: CGRModMeshX::RenderShadow()
// Desc: Render The Mesh Handle Into A Shadow Map
//-----------------------------------------------------------------------------
void CGRModMeshX::RenderShadow()
{
  const D3DXMATRIXA16& matView = gl_LObj->m_ShadowView;
  const D3DXMATRIXA16& matViewProjection = gl_LObj->m_ShadowViewProj;
  D3DXMATRIXA16 matWorldView,matWorldViewProjection;

  D3DXMatrixMultiply(&matWorldView,&m_WorldMatrix,&matView);
  D3DXMatrixMultiply(&matWorldViewProjection,&m_WorldMatrix,&matViewProjection);

  m_pShader->SetMatrix("matWorldView",&matWorldView);
  m_pShader->SetMatrix("matWorldViewProjection",&matWorldViewProjection);
  m_pShader->SetTechnique("T1");
  m_pShader->CommitChanges();

  //Render The Mesh
  LPD3DXMESH m_pmMesh = m_pObject->GetLocalMesh();

  UINT iPass,iPasses;
  m_pShader->Begin(&iPasses,0);
    
  for (iPass = 0; iPass < iPasses; ++iPass)
  {
    m_pShader->BeginPass(iPass);

    //Set And Draw Each Of The Materials In The Mesh
    for (DWORD I = 0; I < m_pObject->m_dwNumMaterials; ++I)
    {   
      //Set Texture For The Mesh Object
      if (m_pObject->m_pTextures[I]) m_pShader->SetTexture("DiffuseMapTexture",m_pObject->m_pTextures[I]);  
	  //d_pDevice->SetTexture(0,m_pObject->m_pTextures[I]);
	  
	  m_pShader->CommitChanges();

      m_pmMesh->DrawSubset(I);
	}

	//m_pMesh->DrawSubset(0);
	m_pShader->EndPass();  
  }
  m_pShader->End();
} //EndRenderShadowProcedure

//-----------------------------------------------------------------------------