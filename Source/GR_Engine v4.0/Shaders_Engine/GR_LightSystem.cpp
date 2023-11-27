/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_LightSystem.cpp
// It's The GreenRay Lighting Shader Engine.
// Made Specially For Shader Deferred Lighting Technology Implementation.
// Date Creation: 29 August 2005
// Last Changing: 21 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_LightSystem.h>

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Prototypes
////////////////////////////////////////////////////////////////////////////////
CGRLightGlobals* gl_LObj = NULL; //Global GreenRay Lighting Intreface

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRMeshXShader Class
///////////////////////////////////////////////////////////////////////////////
CGRMeshXShader::CGRMeshXShader()
{
  //Set Default Parameters
  m_pShader = NULL;
  g_pMaterials = NULL;
} //EndConstructionDirectives

CGRMeshXShader::~CGRMeshXShader()
{
  ReleaseClass(m_pShader);
  m_pGRMesh = NULL; //NULL Model Object Pointer
  d_pDevice = NULL; //NULL Device Pointer
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRMeshXShader::Create()
// Desc: Creates The Mesh Using The Specified FX File For Rendering
//-----------------------------------------------------------------------------
HRESULT CGRMeshXShader::Create(LPDIRECT3DDEVICE9 pDevice,CGRMeshX* pGRMesh,bool Textured)
{
  HRESULT Result;
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pGRMesh = pGRMesh; //Apropritate Model Object Pointer To Class
  LPCSTR FxFile;

  //Create An Effect (Outputs The Mesh Into The Fat Buffer)
  if (Textured) FxFile = "\Shaders\\GeomTextured.fx";
  else FxFile = "\Shaders\\GeomSolidColor.fx";
  //else FxFile = "\Shaders\\GeomSpots.fx";

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

  //Load The Texture Into LightTexture
  //D3DXCreateTextureFromFile(d_pDevice,"\Textures\\LightMaps\\FlashLight.bmp",&LightTexture);

  return S_OK;
} //EndCreateFunction

//-----------------------------------------------------------------------------
// Name: CGRMeshXShader::Reset()
// Desc: Called When The Device Has Changed In Some Way
//-----------------------------------------------------------------------------
void CGRMeshXShader::Reset()
{
  if (m_pShader) m_pShader->OnLostDevice();
} //EndResetProcedure

//-----------------------------------------------------------------------------
// Name: CGRMeshXShader::Render()
// Desc: Render The Mesh Handle MRT Or Single Render Target
//-----------------------------------------------------------------------------
void CGRMeshXShader::Render(UINT RenderPass)
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


  /*  D3DXMATRIX World;
  D3DXMatrixIdentity(&World);

  //Set The Texture Adjustment Matrix Offsets
  float XOffset = 0.5f+(0.5f/(float)256); //Width Of The Light Texture
  float YOffset = 0.5f+(0.5f/(float)256); //Height Of The Light Texture

  //Create The Texture Adjustment Matrix
  D3DXMATRIX TexMatrix;
  ZeroMemory(&TexMatrix,sizeof(D3DXMATRIX));
  TexMatrix._11 = 0.5f;
  TexMatrix._22 = -0.5f;
  TexMatrix._33 = 0.5f;
  TexMatrix._41 = XOffset;
  TexMatrix._42 = YOffset;
  TexMatrix._43 = 0.5f;
  TexMatrix._44 = 1.0f;	

  //Create The Projection Matrix For The Light
  D3DXMATRIX LightProj;
  D3DXMatrixPerspectiveFovLH(&LightProj,D3DX_PI/4,1.0f,0.0f,1000.0f);

  m_pShader->SetMatrix("WorldViewProjection",&matWorldViewProjection);
  m_pShader->SetMatrix("World",&m_WorldMatrix);
  m_pShader->SetMatrix("TexTransform",&(m_WorldMatrix*matView*LightProj*TexMatrix));
  m_pShader->SetValue("EyePos",&gl_LObj->m_pCamPos,sizeof(D3DXVECTOR3));
  m_pShader->SetTexture("LightTex",LightTexture);
  m_pShader->SetFloat("LightFalloff",75.0f);*/



  m_pShader->CommitChanges();




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



  //Render State For Until Rendering Mesh Object
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  //Render The Mesh
  LPD3DXMESH m_pMesh = m_pGRMesh->GetLocalMesh();

  UINT iPass,iPasses;
  m_pShader->Begin(&iPasses,0);
    
  for (iPass = 0; iPass < iPasses; ++iPass)
  {
    m_pShader->BeginPass(iPass);

    //Set And Draw Each Of The Materials In The Mesh
    for (DWORD I = 0; I < m_pGRMesh->m_dwNumMaterials; ++I)
    {   
      //Set Texture For The Mesh Object
      if (m_pGRMesh->m_pTextures[I]) 
	  {
	    m_pShader->SetTexture("DiffuseMapTexture",m_pGRMesh->m_pTextures[I]);  
        m_pShader->SetTexture("SpecularMapTexture",m_pGRMesh->m_pSpecTextures[I]);
        if (m_pGRMesh->m_pHeightTextures[I] != NULL) m_pShader->SetTexture("NormalMapTexture",m_pGRMesh->m_pHeightTextures[I]);
		else m_pShader->SetTexture("NormalMapTexture",m_pGRMesh->m_pNormTextures[I]);
	  }
	  m_pShader->CommitChanges();

      m_pMesh->DrawSubset(I); 
	}
    m_pShader->EndPass();
  }
  m_pShader->End();
} //EndRenderProcedure

//-----------------------------------------------------------------------------
// Name: CGRMeshXShader::RenderWeapon()
// Desc: Render The Mesh Handle MRT Or Single Render Target
//-----------------------------------------------------------------------------
void CGRMeshXShader::RenderWeapon(UINT RenderPass)
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
    LPD3DXMESH m_pMesh = m_pGRMesh->GetLocalMesh();

    UINT iPass,iPasses;
    m_pShader->Begin(&iPasses,0/*D3DXFX_DONOTSAVESTATE*/);
    
    for (iPass = 0; iPass < iPasses; ++iPass)
    {
      m_pShader->BeginPass(iPass);

      //Set And Draw Each Of The Materials In The Mesh
      for (DWORD I = 0; I < /*m_pGRMesh->m_dwNumMaterials;*/ 11; ++I)
      {   
        //Set Texture For The Mesh Object
        //if (m_pGRMesh->m_pTextures[I]) m_pShader->SetTexture("DiffuseMapTexture",m_pGRMesh->m_pTextures[I]);  
	    //m_pShader->CommitChanges();

        m_pMesh->DrawSubset(I);
	  }
      m_pShader->EndPass();
    }
    m_pShader->End();
  } //End Render Weapon Model

  //When All Render Passed Then Release Viewports 
  ReleaseClass(pBackBuffer);
  d_pDevice->SetViewport(&OriginalVP);
} //EndRenderWeaponProcedure





//-----------------------------------------------------------------------------
// Name: CGRMeshXShader::RenderShell()
// Desc: Render The Weapon Shell Mesh Handle MRT Or Single Render Target
//-----------------------------------------------------------------------------
void CGRMeshXShader::RenderShell(D3DXMATRIX MatWorld)
{
  //m_WorldMatrix = MatWorld;

  //Scale Shell By Change Y Position
  D3DXMATRIX MatScale;
  D3DXMatrixScaling(&MatScale, 0.6f, 0.6f, 0.8f);
  D3DXMatrixMultiply(&m_WorldMatrix,&MatScale,&MatWorld);

  if (m_WorldMatrix._42 <= 10.0f) 
  {
	D3DXMatrixScaling(&MatScale, 1.2f, 1.2f, 1.6f); 
	D3DXMatrixMultiply(&m_WorldMatrix,&MatScale,&MatWorld);
  }

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
  m_pShader->CommitChanges();


  //Render Some Weapon Shell Model
  {
    //Render State For Until Rendering Mesh Object
    d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
    d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //Render The Mesh
    LPD3DXMESH m_pMesh = m_pGRMesh->GetLocalMesh();

    UINT iPass,iPasses;
    m_pShader->Begin(&iPasses,0/*D3DXFX_DONOTSAVESTATE*/);
    
    for (iPass = 0; iPass < iPasses; ++iPass)
    {
      m_pShader->BeginPass(iPass);

      //Set And Draw Each Of The Materials In The Mesh
      for (DWORD I = 0; I < m_pGRMesh->m_dwNumMaterials; ++I)
      {   
        //Set Texture For The Mesh Object
        if (m_pGRMesh->m_pTextures[I])
		{
		  m_pShader->SetTexture("DiffuseMapTexture",m_pGRMesh->m_pTextures[I]); 
		  m_pShader->SetTexture("SpecularMapTexture",m_pGRMesh->m_pSpecTextures[I]);
		  if (m_pGRMesh->m_pHeightTextures[I] != NULL) m_pShader->SetTexture("NormalMapTexture",m_pGRMesh->m_pHeightTextures[I]);
		  else m_pShader->SetTexture("NormalMapTexture",m_pGRMesh->m_pNormTextures[I]);
		}
		m_pShader->CommitChanges();

        m_pMesh->DrawSubset(I);
	  }
      m_pShader->EndPass();
    }
    m_pShader->End();
  } //End Render Weapon Shell Model
} //EndRenderShellProcedure





//-----------------------------------------------------------------------------
// Name: CGRMeshXShader::RenderShadow()
// Desc: Render The Mesh Handle Into A Shadow Map
//-----------------------------------------------------------------------------
void CGRMeshXShader::RenderShadow(UINT ShNum)
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

  //Render State For Until Rendering Mesh Object
  //d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  //d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  //Render The Mesh
  LPD3DXMESH m_pMesh = m_pGRMesh->GetLocalMesh();

  UINT iPass,iPasses;
  m_pShader->Begin(&iPasses,0);
    
  for (iPass = 0; iPass < iPasses; ++iPass)
  {
    m_pShader->BeginPass(iPass);

    //Set And Draw Each Of The Materials In The Mesh
    for (DWORD I = 0; I < m_pGRMesh->m_dwNumMaterials; ++I)
    {   
      //Set Texture For The Mesh Object
      //if (m_pGRMesh->m_pTextures[I]) m_pShader->SetTexture("DiffuseMapTexture",m_pGRMesh->m_pTextures[I]);  
	  //m_pShader->CommitChanges();
      m_pMesh->DrawSubset(I);
	}
	m_pShader->EndPass();  
  }
  m_pShader->End();
} //EndRenderShadowProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRLightSystem Class
///////////////////////////////////////////////////////////////////////////////
CGRLightSystem::CGRLightSystem(LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pDevice,
							   int WndWidth,int WndHeight)
{
  //Set Sefault Parameters
  d_pD3D = pD3D;        //Apropritate Device Interface Pointer To Class
  d_pDevice = pDevice;  //Apropritate Device Pointer To Class

  m_FrameBuffer = NULL;
  m_pBackBuffer = NULL;
  m_ShowFatFx = NULL;
  m_DisplayFatType = DFT_COMBINE_COLOR;
  m_LightHemisphere = NULL;
  m_bFlash = false;

  for (UINT I = 0; I < MAX_SHADOWS; I++)
  {
    m_pShadowMap[I]	 = NULL;
	m_pShadowMapSurf[I] = NULL;
	m_LightDirectional[I] = NULL;
  }

  m_pShadowMapZ = NULL;
  m_pAttenuationFuncs = NULL;
  m_pDotProductFuncs = NULL;
  m_pMaterialFuncs = NULL;	

  for (UINT I = 0; I < MAX_POINT_LIGHTS; I++) m_LightPoint[I] = NULL;
  for (UINT I = 0; I < MAX_LIGHTS; I++) m_ActiveLights[I] = NULL;
////////////////////////////////////////////////////////////////////////////////

  //Create Global Light Object Interface
  gl_LObj = new CGRLightGlobals();
  gl_LObj->m_pD3D = d_pD3D;
  gl_LObj->m_pD3DDevice = d_pDevice;
  D3DXCreateEffectPool(&gl_LObj->m_EffectPool);
  D3DXCreateEffectPool(&gl_LObj->m_LightEffectPool);
////////////////////////////////////////////////////////////////////////////////

  //Empty Material List
  EmptyMaterialList();

  //A Pixel Shader That Show/Combines Each Buffer In The Fat Framebuffer
  if (SUCCEEDED(D3DXCreateEffectFromFile(d_pDevice,"\Shaders\\ShowFat.fx",
										   0,                          //Macros
										   0,                          //Include
										   0,                          //Flags
										   gl_LObj->m_LightEffectPool, //Effect Pool
										   &m_ShowFatFx,
										   0)))
  {
    m_ShowFatFx->SetTechnique("T0");

	//Create Lights
    UINT CurrentLight = 0;
    m_LightHemisphere = new CGRHemisphereLight();
    m_LightHemisphere->Create();
    m_ActiveLights[CurrentLight++] = m_LightHemisphere;

    for (UINT I = 0; I < MAX_SHADOWS; I++)
    {
	  m_LightDirectional[I] = new CGRDirectionalLight();
	  m_LightDirectional[I]->Create();
	  m_ActiveLights[CurrentLight++] = m_LightDirectional[I];

	  if (I == 0)
	  {
	    m_LightDirectional[0]->m_LightColor = D3DXVECTOR4(1,1,1,0);
	    m_LightDirectional[0]->m_LightIntensity = 0.3f;
	  } 
	  else
	  {
	    m_LightDirectional[1]->m_LightColor = D3DXVECTOR4(1,1,1,0);
	    m_LightDirectional[1]->m_LightIntensity = 0.3f;
	  }
    }

    for (UINT I = 0; I < MAX_POINT_LIGHTS; I++)
    {
	  m_LightPoint[I] = new CGRPointLight();
	  m_LightPoint[I]->Create();
	  m_LightPoint[I]->m_LightIntensity = 0.7f;
	  m_LightPoint[I]->m_LightID = (unsigned char)I;
	  m_ActiveLights[CurrentLight++] = m_LightPoint[I];
      m_LightPoint[I]->m_ShowVolume = false;
    }

	D3DCAPS9 D3DCaps;
    d_pDevice->GetDeviceCaps(&D3DCaps);
      
	//Check MRT Support
    if (D3DCaps.NumSimultaneousRTs == 4) m_FrameBuffer = new CGRGBufferMRT(d_pD3D,d_pDevice);
    else m_FrameBuffer = new CGRGBufferSingle(d_pD3D,d_pDevice);

    //Create All The Render-Targets
    if (m_FrameBuffer->CreateBuffers(WndWidth,WndHeight) == false) 
	{ Log("GR_LightSystem.cpp: Error Creating FrameBuffer \n"); }

    gl_LObj->m_Framebuffer = m_FrameBuffer;

    for (UINT I = 0; I < MAX_SHADOWS; I++)
    {
	  //Create Shadow Map Texture And Retrieve Surface
	  if (FAILED(d_pDevice->CreateTexture(SHADOW_MAP_SIZE,SHADOW_MAP_SIZE,1, 
	  D3DUSAGE_RENDERTARGET,SHADOW_MAP_FORMAT,D3DPOOL_DEFAULT,&m_pShadowMap[I],NULL)))
      { Log("GR_LightSystem.cpp: Error Creating Shadow Map Texture \n"); }
	  if (FAILED(m_pShadowMap[I]->GetSurfaceLevel(0,&m_pShadowMapSurf[I]))) 
      { Log("GR_LightSystem.cpp: Error Creating Shadow Map Surface \n"); }
    }

    //Create Depth Buffer For Shadow Map Rendering
    if (FAILED(d_pDevice->CreateDepthStencilSurface(SHADOW_MAP_SIZE,SHADOW_MAP_SIZE, 
    D3DFMT_D16,D3DMULTISAMPLE_NONE,0,TRUE,&m_pShadowMapZ,NULL))) 
    { Log("GR_LightSystem.cpp: Error Creating Depth Stencil Surface \n"); }

    //Create Dot Product Texture
    if (FAILED(d_pDevice->CreateTexture(DOTP_WIDTH, 
    NUM_DOTP_FUNCTIONS*BILERP_PROTECTION,1,0,D3DFMT_V16U16, 
    D3DPOOL_MANAGED,&m_pDotProductFuncs,NULL))) 
	{ Log("GR_LightSystem.cpp: Error Creating Dot Product \n"); }
    if (FAILED(D3DXFillTexture(m_pDotProductFuncs,FillDotProductTexture,0))) 
	{ Log("GR_LightSystem.cpp: Error Fill Dot Product Texture \n"); }

    //Create Attenuation Texture
    if (FAILED(d_pDevice->CreateTexture(ATTENUATION_WIDTH, 
    NUM_ATTENUATION_FUNCTIONS*BILERP_PROTECTION,1,0, 
    D3DFMT_V16U16,D3DPOOL_MANAGED,&m_pAttenuationFuncs,NULL))) 
	{ Log("GR_LightSystem.cpp: Error Creating Attenuation Texture \n"); }

    if (FAILED(D3DXFillTexture(m_pAttenuationFuncs,FillAttenuationTexture,0))) 
	{ Log("GR_LightSystem.cpp: Error Fill Attenuation Texture \n"); }

    //Fill The Material List, All Materials Should Have Already Been Registered
    if (FAILED(d_pDevice->CreateTexture(NUM_MATERIALS,1,1,0, 
    D3DFMT_A16B16G16R16F,D3DPOOL_MANAGED,&m_pMaterialFuncs,NULL)))
	{ Log("GR_LightSystem.cpp: Error Creating Material Texture \n"); }

    if (FAILED(D3DXFillTexture(m_pMaterialFuncs,FillMaterialTexture,0))) 
	{ Log("GR_LightSystem.cpp: Error Fill Material Texture \n"); }

    //Set The G-Buffer Textures Into Showfat.fx
    m_ShowFatFx->SetTexture("Buffer0Texture",m_FrameBuffer->GetBufferTexture(FFB_BUFFER0));
    m_ShowFatFx->SetTexture("Buffer1Texture",m_FrameBuffer->GetBufferTexture(FFB_BUFFER1));
    m_ShowFatFx->SetTexture("Buffer2Texture",m_FrameBuffer->GetBufferTexture(FFB_BUFFER2));
    m_ShowFatFx->SetTexture("Buffer3Texture",m_FrameBuffer->GetBufferTexture(FFB_BUFFER3));
    m_ShowFatFx->SetTexture("GlobalShadowMapTexture",m_pShadowMap[0]);

    m_ShowFatFx->SetFloat("fRcpFrameWidth",1.f/m_FrameBuffer->GetWidth());
    m_ShowFatFx->SetFloat("fRcpFrameHeight",1.f/m_FrameBuffer->GetHeight());

    //Set The Lightmodel Textures
    m_ShowFatFx->SetTexture("MaterialMapTexture",m_pMaterialFuncs);
    m_ShowFatFx->SetTexture("DotProductFunctionMapTexture",m_pDotProductFuncs);
    m_ShowFatFx->SetTexture("AttenuationMapTexture",m_pAttenuationFuncs);

    //Store The Back Buffer Surface For Later Use
    d_pDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer);

	Log("GR_LightSystem.cpp: Fat Shader Was Created Success OK \n");
  }
} //EndConstructionDirectives

CGRLightSystem::~CGRLightSystem()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::UpdateLight()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLightSystem::UpdateLight(D3DXMATRIX CamView,D3DXVECTOR3 CamPos)
{
  if (gl_LObj != NULL && m_LightHemisphere != NULL)
  {
    //Set Perspective View For The Model In Space
    float fAspect = 1.1f;
    D3DXMATRIXA16 Mat_Proj;
    D3DXMatrixPerspectiveFovLH(&Mat_Proj,D3DXToRadian(60.0f),fAspect,1.1f,5000.0f);
    //d_pDevice->SetTransform(D3DTS_PROJECTION,&Mat_Proj);

	//Update Lighting Matrices
    D3DXMATRIX MatViewProjection;
    D3DXMatrixMultiply(&MatViewProjection,&CamView,&Mat_Proj);
    gl_LObj->m_View = CamView;
    gl_LObj->m_ViewProj = MatViewProjection;
    gl_LObj->m_Proj = Mat_Proj;

	//Update Player Light Position
	gl_LObj->m_pCamPos = CamPos;


    //These Shader Matrices Are Shared Among All Meshes So We Just Set The First Mesh
	//Create Matrix To Takes Us From Store Position Into View Space Position
	D3DXMATRIX MatProjectionInvScaled,MatScale;
	D3DXMatrixInverse(&MatProjectionInvScaled,0,&Mat_Proj);
	D3DXMatrixIdentity(&MatScale);

	MatScale._11 *= 2;
	MatScale._22 *= 2;
	MatScale._41 += -1;
	MatScale._42 += -1;

	D3DXMatrixMultiply(&MatProjectionInvScaled,&MatScale,&MatProjectionInvScaled);

	//Matrix That Takes Us From G-Buffer Position To View Space Position
	m_LightHemisphere->m_pShader->SetMatrix("matProjectionInvScaled",&MatProjectionInvScaled); 
    
    //Update Light Culling 
    UpdateCullInfo(&gl_LObj->m_CullInfo);
  } //End If
} //EndUpdateLightProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::UpdateCullInfo()
// Desc: Sets Up The Frustum Planes, Endpoints, And Center For The Frustum
//       Defined By A Given View Matrix And Projection Matrix. This Info Will 
//       Be Used When Culling Each Object In CullObject().
//-----------------------------------------------------------------------------
void CGRLightSystem::UpdateCullInfo(CULLINFO* pCullInfo)
{
  D3DXMATRIXA16 Mat;
  D3DXMatrixInverse(&Mat,NULL,&gl_LObj->m_ViewProj);

  pCullInfo->VecFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); //xyz
  pCullInfo->VecFrustum[1] = D3DXVECTOR3( 1.0f, -1.0f, 0.0f); //Xyz
  pCullInfo->VecFrustum[2] = D3DXVECTOR3(-1.0f,  1.0f, 0.0f); //xYz
  pCullInfo->VecFrustum[3] = D3DXVECTOR3( 1.0f,  1.0f, 0.0f); //XYz
  pCullInfo->VecFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, 0.99999f); //xyZ (0.99999f So We Don't Freak Out At Infinity)
  pCullInfo->VecFrustum[5] = D3DXVECTOR3( 1.0f, -1.0f, 0.99999f); //XyZ
  pCullInfo->VecFrustum[6] = D3DXVECTOR3(-1.0f,  1.0f, 0.99999f); //xYZ
  pCullInfo->VecFrustum[7] = D3DXVECTOR3( 1.0f,  1.0f, 0.99999f); //XYZ

  for (INT I = 0; I < 8; I++)
  D3DXVec3TransformCoord(&pCullInfo->VecFrustum[I],&pCullInfo->VecFrustum[I],&Mat);

  D3DXPlaneFromPoints(&pCullInfo->PlaneFrustum[CP_NEAR],&pCullInfo->VecFrustum[0], 
                      &pCullInfo->VecFrustum[1],&pCullInfo->VecFrustum[2]);       //Near
  D3DXPlaneFromPoints(&pCullInfo->PlaneFrustum[CP_FAR],&pCullInfo->VecFrustum[6], 
                      &pCullInfo->VecFrustum[7],&pCullInfo->VecFrustum[5]);       //Far
  D3DXPlaneFromPoints(&pCullInfo->PlaneFrustum[CP_LEFT],&pCullInfo->VecFrustum[2], 
                      &pCullInfo->VecFrustum[6],&pCullInfo->VecFrustum[4]);       //Left
  D3DXPlaneFromPoints(&pCullInfo->PlaneFrustum[CP_RIGHT],&pCullInfo->VecFrustum[7], 
                      &pCullInfo->VecFrustum[3],&pCullInfo->VecFrustum[5]);       //Right
  D3DXPlaneFromPoints(&pCullInfo->PlaneFrustum[CP_TOP],&pCullInfo->VecFrustum[2], 
                      &pCullInfo->VecFrustum[3],&pCullInfo->VecFrustum[6]);       //Top
  D3DXPlaneFromPoints(&pCullInfo->PlaneFrustum[CP_BOTTOM],&pCullInfo->VecFrustum[1], 
                      &pCullInfo->VecFrustum[0],&pCullInfo->VecFrustum[4]);       //Bottom
} //EndUpdateCullInfoProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::SetPointLightParam()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLightSystem::SetPointLightParam(UINT LightNum,
										D3DXVECTOR3 LightPos,
										float LightVal,
										D3DXVECTOR4 LightColor)
{
  //Light Matrices
  D3DXMATRIXA16 MatScale;

  //Set Light Scale By Light Value
  float Val = (LightNum/5.f)+LightVal;
  D3DXVECTOR3 Scale(Val,Val,Val);
  D3DXMatrixTransformation(&MatScale,0,0,&Scale,0,0,0);

  //Set Light Radius
  D3DXVECTOR3 Sph[3];
  Sph[0] = D3DXVECTOR3(1,0,0);
  Sph[1] = D3DXVECTOR3(0,1,0);
  Sph[2] = D3DXVECTOR3(0,0,1);

  D3DXVec3TransformCoordArray(Sph,sizeof(D3DXVECTOR3),Sph,sizeof(D3DXVECTOR3),&MatScale,3);
  float x = D3DXVec3Length(&Sph[0]);
  float y = D3DXVec3Length(&Sph[1]);
  float z = D3DXVec3Length(&Sph[2]);

  float Radius = x;
  if (y > Radius) Radius = y;
  if (z > Radius) Radius = z;

  m_LightPoint[LightNum]->m_Radius = Scale.x/2;

  //Set Light Color And Position In Space
  m_LightPoint[LightNum]->m_LightColor = LightColor;
  Scale = D3DXVECTOR3(Radius*4,Radius*4,Radius*4);
  D3DXMatrixTransformation(&MatScale, 0, 0, &Scale, 0, 0, &LightPos);
  m_LightPoint[LightNum]->m_WorldMatrix = MatScale;
} //EndSetPointLightParamProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::SetFlashLightParam()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLightSystem::SetFlashLightParam(UINT LightNum,
										D3DXVECTOR4 LightPos,
										float LightVal,
										D3DXVECTOR4 LightColor)
{
  //Set Light Scale By Light Value
  float Val = (LightNum/5.f)+LightVal;
  D3DXVECTOR3 Scale(Val,Val,Val);
  D3DXMATRIXA16 MatScale;
  D3DXMatrixTransformation(&MatScale,0,0,&Scale,0,0,0);

  //Set Light Radius
  D3DXVECTOR3 Sph[3];
  Sph[0] = D3DXVECTOR3(1,0,0);
  Sph[1] = D3DXVECTOR3(0,1,0);
  Sph[2] = D3DXVECTOR3(0,0,1);

  D3DXVec3TransformCoordArray(Sph,sizeof(D3DXVECTOR3),Sph,sizeof(D3DXVECTOR3),&MatScale,3);
  float x = D3DXVec3Length(&Sph[0]);
  float y = D3DXVec3Length(&Sph[1]);
  float z = D3DXVec3Length(&Sph[2]);

  float Radius = x;
  if (y > Radius) Radius = y;
  if (z > Radius) Radius = z;

  m_LightPoint[LightNum]->m_Radius = Scale.x/2;

  //Set Light Color And Position In Space
  m_LightPoint[LightNum]->m_LightColor = LightColor;
  Scale = D3DXVECTOR3(Radius*4,Radius*4,Radius*4);
  D3DXMatrixTransformation(&MatScale, 0, 0, &Scale, 0, 0, 0);
			
  //Set Startup Matrices And Flash Light Position
  D3DXMATRIXA16 MatView = gl_LObj->m_View;

  //Inverse Matrix Here
  D3DXMATRIXA16 mInvView,mWorld;
  D3DXMatrixIdentity(&mInvView);
  D3DXMatrixIdentity(&mWorld);
  D3DXMatrixInverse(&mInvView,NULL,&MatView);

  //Set Light Postition On Screen Space
  D3DXVECTOR4 Pos = LightPos;
  D3DXVec4Transform(&Pos,&Pos,&mInvView);
  mWorld._41 = Pos.x;
  mWorld._42 = Pos.y;
  mWorld._43 = Pos.z;
  //mWorld._44 = 1.5f;

  //Multiply Matrices
  mWorld*= mInvView*MatView; 

  D3DXMATRIXA16 mTransform;
  D3DXMatrixIdentity(&mTransform);
  D3DXMatrixInverse(&mTransform,NULL,&MatView);
  mTransform._41 = mWorld._41;
  mTransform._42 = mWorld._42;
  mTransform._43 = mWorld._43;

  //Attach Light Position To Camera View
  D3DXMATRIXA16 MatTrans;          
  D3DXMatrixMultiply(&MatTrans,&MatScale,&mTransform);
  m_LightPoint[LightNum]->m_WorldMatrix = MatTrans;   
} //EndSetFlashLightParamProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::LightGBuffer()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLightSystem::LightGBuffer()
{
  //Select Just The Backbuffer As The Target
  //Reuse The Depth Buffer From The Fat Framebuffer To Enable Depth Cull Optizamations
  d_pDevice->SetDepthStencilSurface(m_FrameBuffer->GetBufferSurface(FFB_DEPTH));
  d_pDevice->SetRenderTarget(0,m_pBackBuffer);
  d_pDevice->SetRenderTarget(1,0);
  d_pDevice->SetRenderTarget(2,0);
  d_pDevice->SetRenderTarget(3,0); 

  //d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  d_pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_STENCILENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	
  //Assign Point Light Parameters 
  for (UINT J = 0; J < MAX_POINT_LIGHTS; J++)
  {
    //Assign Light Parameters For Every Light Source
	UINT LightNum = J%7;
	switch (LightNum)
	{
	  case 0:
	  SetPointLightParam(LightNum,D3DXVECTOR3(10.0f,30.0f,-60.0f),148.0f,D3DXVECTOR4(1,1,1,0));	  
	  break;
      
	  case 1:
      SetPointLightParam(LightNum,D3DXVECTOR3(110.0f,30.0f,-60.0f),128.0f,D3DXVECTOR4(1,1,1,0));
	  break;

      case 2:
      SetPointLightParam(LightNum,D3DXVECTOR3(10.0f,30.0f,-100.0f),148.0f,D3DXVECTOR4(1,1,0,0));
	  break;

      case 3:
      SetPointLightParam(LightNum,D3DXVECTOR3(110.0f,30.0f,60.0f),128.0f,D3DXVECTOR4(1,1,0,0));
	  break;

	  case 4:
   	  if (m_bFlash) SetFlashLightParam(LightNum,D3DXVECTOR4(0.0f, -8.0f, 48.0f, 1.0f),68.0f,
	                                            D3DXVECTOR4(2.5f,1.1f,0.66f,0.0f));
      else SetFlashLightParam(LightNum,D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),0.0f,D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f)); 
	  break;

	  case 5:
	  if (m_bFlash) SetPointLightParam(LightNum,D3DXVECTOR3(gl_LObj->m_pCamPos.x,gl_LObj->m_pCamPos.y-2.0f,gl_LObj->m_pCamPos.z),268.0f,
		                                        D3DXVECTOR4(2.5f,1.1f,0.66f,0.0f));
	  else SetFlashLightParam(LightNum,D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f),0.0f,D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f)); 
	  break;
	}
  }

  //Update All Active Lights
  for (UINT J = 0; J < MAX_LIGHTS; J++)
  {
	if (m_ActiveLights[J] != NULL) m_ActiveLights[J]->Update();
  }

  //Render All Active Lights
  for (UINT J = 0; J < MAX_LIGHTS; J++)
  {
	if (m_ActiveLights[J] != NULL) m_ActiveLights[J]->Render();
  }

  //Update Weapon Flash State
  m_bFlash = false;
  
  d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
} //EndLightGBufferProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::BeginShadow()
// Desc: Called To Render Shadow Map For The Light. Outputs Per-Pixel Distance
//       Normalized To 0..1 Range To The Floating Point Buffer.
//-----------------------------------------------------------------------------
void CGRLightSystem::BeginShadow(UINT ShadowNum,D3DXVECTOR3 ShadowPos,float LightPosX,float LightPosY,float LightPosZ)
{
  //Light Direction
  //Set The Transform Matrices
  FLOAT	depthRange = 130.0f;

  D3DXVECTOR3 vEyePt;
  D3DXVECTOR3 vLookatPt;
  D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
  const D3DXVECTOR3 zAxis(0,0,1);

  D3DXMATRIX matLightView,matLightProj,matLightViewProj;

  D3DXMATRIX MatWarp;
  D3DXVECTOR3 vLightDir;

  D3DXMATRIX matView,matInvView;
  D3DXMATRIX Transf,Rot,MatRotX,MatRotY,MatRotZ;

  //if (ShadowNum == 0)
  //{
	//  D3DXMatrixOrthoLH(&matLightProj, 20.0f, 20.0f, 0.0f, depthRange*1);
  //} 

 // if (ShadowNum == 1)
 // {
 // } 



	  vLookatPt = D3DXVECTOR3(ShadowPos);

      D3DXMatrixRotationX(&MatRotX, LightPosX);
      D3DXMatrixRotationY(&MatRotY, LightPosY);
	  D3DXMatrixRotationZ(&MatRotZ, LightPosZ);
	  D3DXMatrixMultiply(&Rot, &MatRotX, &MatRotY);
      D3DXMatrixMultiply(&Transf, &Rot, &MatRotZ);

      //Extract The View Direction (Faces Down The Z)
      D3DXVec3TransformNormal(&vLightDir,&zAxis,&Transf);
      D3DXVec3Normalize(&vLightDir,&vLightDir);


      //Setup Shadow Map Transform
      vEyePt = vLookatPt+(vLightDir*15);
      D3DXMatrixLookAtLH(&matLightView,&vEyePt,&vLookatPt,&vUpVec);

      //Projection For Directional Light
      D3DXMatrixOrthoLH(&matLightProj, 100.0f, 100.0f, -10.0f, depthRange*1);
      D3DXMatrixMultiply(&matLightViewProj,&matLightView,&matLightProj);




      //We Need To Move Position From Eye Space To Light Space And Then Account For Texel Offset
      //invViewMat*lightViewMat*matTexAdj;
      //Biblo: A Traditionalist View Of 3-D Image Warping, Hansong Zhang
      D3DXMatrixInverse(&matInvView,0,&gl_LObj->m_View);
      D3DXMatrixMultiply(&MatWarp,&matInvView,&matLightViewProj);


      //Texture Adjustment Matrix
      FLOAT fTexelOffs = (0.5f/SHADOW_MAP_SIZE);
      D3DXMATRIX matTexAdj(0.5f,        0.0f,			 0.0f,	   0.0f,
					   0.0f,            -0.5f,		     0.0f,	   0.0f,
					   0.0f,            0.0f,			 1.0f,	   0.0f,
					   0.5f+fTexelOffs, 0.5f+fTexelOffs, -0.0065f, 1.0f);

      D3DXMatrixMultiply(&MatWarp,&MatWarp,&matTexAdj);


	  gl_LObj->m_ShadowView = matLightView;
      gl_LObj->m_ShadowViewProj = matLightViewProj;





  if (ShadowNum == 0)
  {
	//m_LightPoint[0]->m_pShader->SetMatrixArray("matShadowWarp", gl_LObj->MatWarp, 2);
	  m_LightPoint[1]->m_pShader->SetMatrix("matShadowWarp0", &MatWarp);
	//g_pEffect->SetValue("g_avLightPositionView", avLightViewPosition, sizeof(D3DXVECTOR4) * NUM_LIGHTS);
	m_LightPoint[1]->m_pShader->SetTexture("ShadowMapTexture",m_pShadowMap[ShadowNum]);
  } 
  else
  {
	//m_LightPoint[0]->m_pShader->SetMatrixArray("matShadowWarp", gl_LObj->MatWarp, 2);
	  m_LightPoint[2]->m_pShader->SetMatrix("matShadowWarp0", &MatWarp);
	//g_pEffect->SetValue("g_avLightPositionView", avLightViewPosition, sizeof(D3DXVECTOR4) * NUM_LIGHTS);
	m_LightPoint[2]->m_pShader->SetTexture("ShadowMapTexture",m_pShadowMap[ShadowNum]);
  }

  D3DXVECTOR4 jit0 = D3DXVECTOR4(0.1f/SHADOW_MAP_SIZE, 0.1f/SHADOW_MAP_SIZE, 0, 0);
  D3DXVECTOR4 jit1 = D3DXVECTOR4(0.1f/SHADOW_MAP_SIZE, 0.9f/SHADOW_MAP_SIZE, 0, 0);
  D3DXVECTOR4 jit2 = D3DXVECTOR4(0.9f/SHADOW_MAP_SIZE, 0.1f/SHADOW_MAP_SIZE, 0, 0);
  D3DXVECTOR4 jit3 = D3DXVECTOR4(0.9f/SHADOW_MAP_SIZE, 0.9f/SHADOW_MAP_SIZE, 0, 0);

  m_LightPoint[0]->m_pShader->SetVector("f4ShadowJitter0",&jit0);
  m_LightPoint[0]->m_pShader->SetVector("f4ShadowJitter1",&jit1);
  m_LightPoint[0]->m_pShader->SetVector("f4ShadowJitter2",&jit2);
  m_LightPoint[0]->m_pShader->SetVector("f4ShadowJitter3",&jit3);








  //Save Old Viewport
  d_pDevice->GetViewport(&m_pShadowViewport);

  //Set New Render Target
  d_pDevice->SetRenderTarget(0,m_pShadowMapSurf[ShadowNum]);
  d_pDevice->SetDepthStencilSurface(m_pShadowMapZ);

  //Setup Shadow Map Viewport
  D3DVIEWPORT9 ShadowViewport;
  ShadowViewport.X = 0;
  ShadowViewport.Y = 0;
  ShadowViewport.Width = SHADOW_MAP_SIZE;
  ShadowViewport.Height = SHADOW_MAP_SIZE;
  ShadowViewport.MinZ = 0.0f;
  ShadowViewport.MaxZ = 1.0f;
  d_pDevice->SetViewport(&ShadowViewport);

  //Clear Viewport
  d_pDevice->Clear(0L,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0xFFFFFFFF,1.0f,0L);

  //for (UINT I = 0; I < MAX_OBJECTS; I++)
  //{
  //}
} //EndBeginShadowProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::EndShadow()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLightSystem::EndShadow()
{
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);

  //Restore Old Viewport
  d_pDevice->SetViewport(&m_pShadowViewport);
} //EndEndShadowProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::BeginLight()
// Desc: Called Once Per Frame, The Call Is The Entry Point For 3D
//       Rendering. This Function Sets Ip Render States, Clears The
//       Viewport, And Renders The Scene.
//-----------------------------------------------------------------------------
void CGRLightSystem::BeginLight()
{
  //Render The Shadows Into The Shadow Maps
  //for (UINT I = 0; I < MAX_SHADOWS; I++) RenderShadowMap(I);

  //Render The G-Buffers, For MRT This Should Be A Single Pass
  for (UINT PassNum = 0; PassNum < m_FrameBuffer->GetRenderPassCount(); PassNum++)
  {
    if (m_FrameBuffer->GetType() == MT_MRT)
	{
	  //Clear Render Targets
      m_FrameBuffer->Clear(FFB_BUFFER0|FFB_BUFFER1|FFB_BUFFER2|FFB_BUFFER3|FFB_DEPTH|FFB_STENCIL);
	  m_FrameBuffer->SelectBuffersAsRenderTarget(FFB_BUFFER0|FFB_BUFFER1|FFB_BUFFER2|FFB_BUFFER3|FFB_DEPTH|FFB_STENCIL);
	} 
	else
	{
	  //Note PassNum = 0 To 3 == FFB_BUFFER0 to FFB_BUFFER3
	  if (PassNum == 0) m_FrameBuffer->Clear(FFB_BUFFER0|FFB_DEPTH|FFB_STENCIL);
	  else { m_FrameBuffer->Clear((1 << PassNum)); }		
	  m_FrameBuffer->SelectBuffersAsRenderTarget((1 << PassNum)|FFB_DEPTH|FFB_STENCIL);
	}
  }

  //Render The Objects
  //for (UINT I = 0; I < MAX_OBJECTS; I++)
  //{
  //}
} //EndBeginLightProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::EndLight()
// Desc: Finishing Render Light
//-----------------------------------------------------------------------------
void CGRLightSystem::EndLight()
{
  //Render Light Techniques As Finished
  LightGBuffer();
} //EndEndLightProcedure

//-----------------------------------------------------------------------------
// Name: CGRLightSystem::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLightSystem::Release()
{
  //Release Point Lights
  for (UINT I = 0; I < MAX_POINT_LIGHTS; I++) DeleteClass(m_LightPoint[I]);
  
  //Release Directional Lights
  for (UINT I = 0; I < MAX_SHADOWS; I++) DeleteClass(m_LightDirectional[I]);
  
  DeleteClass(m_LightHemisphere);
  ReleaseClass(m_ShowFatFx);
  ReleaseClass(gl_LObj->m_LightEffectPool);
  ReleaseClass(gl_LObj->m_EffectPool);

  //Release Global Light Object Interface
  if (gl_LObj != NULL)
  {
    ReleaseClass(gl_LObj); 
	DeleteClass(gl_LObj); 
  }

  d_pDevice = NULL; //NULL Device Pointer
  d_pD3D = NULL;    //NULL Device Interface Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------