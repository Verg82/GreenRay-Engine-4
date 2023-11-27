/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Interface.cpp
// It's The GreenRay User Interface Engine.
// Made Specially For Implementing User Interface.
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Interface.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRInterface Class
///////////////////////////////////////////////////////////////////////////////
CGRInterface::CGRInterface(LPDIRECT3DDEVICE9 pDevice)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pUIEffect = NULL;
  LPD3DXEFFECT TempEffect = NULL;
  m_pUIVertexDecl = NULL;
  m_pUIVertexBuffer = NULL;
  m_pUITexture = NULL;

  //Create UI Panel Vertex Declaration For Shader Input Registers
  D3DVERTEXELEMENT9 Decl[] =
  {
    {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, //Size = 12
    {0,	12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END()
  };
  
  //Create Vertex Declaration For This Shader Input Registers
  if (SUCCEEDED(CreateVertDeclaration(d_pDevice,&m_pUIVertexDecl,Decl)))
  {
    //Load Some Shader File
    if (SUCCEEDED(LoadShaderFile(d_pDevice,TEXT("\Shaders\\UserInterface.fx"),&TempEffect)))
    {
      m_pUIEffect = TempEffect;
	  m_pUITechHandle = m_pUIEffect->GetTechniqueByName("UserInterfaceTech");
    }
    else 
    {
      Log("GR_Interface.cpp: Unable To Load Shader File UserInterface.fx.. \n");
      TempEffect = NULL;
	  m_pUIEffect = NULL;
    }

    //Initialize Vertex Buffer
    if (FAILED(CreateVertexBuffer())) 
    { 
      Log("GR_Interface.cpp: Unable To Create Vertex Buffer.. \n"); 
      m_pUIVertexBuffer = NULL;
    }
  }
} //EndConstructionDirectives

CGRInterface::~CGRInterface()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRInterface::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRInterface::Release()
{
  d_pDevice = NULL;
  m_pUIVertexDecl = NULL;
  ReleaseClass(m_pUIEffect);
  ReleaseClass(m_pUITexture);
  ReleaseClass(m_pUIVertexBuffer);
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRInterface::SetOpacityLevel()
// Desc: 
//-----------------------------------------------------------------------------
void CGRInterface::SetOpacityLevel(int Opacity)
{
  if (Opacity <= 10) m_fOpacity = float(Opacity*0.001);
  if (Opacity >= 10) m_fOpacity = float(Opacity*0.01);
  if (Opacity >= 100) m_fOpacity = 1.0f;
} //EndSetOpacityLevelProcedure

//-----------------------------------------------------------------------------
// Name: CGRInterface::CreateTexture()
// Desc: 
//-----------------------------------------------------------------------------
void CGRInterface::CreateTexture(LPCSTR FileName,DWORD dwKeyColor)
{
  if (FAILED(D3DXCreateTextureFromFileEx(d_pDevice,FileName,0,0,0,0,
  D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,dwKeyColor,NULL,NULL,&m_pUITexture)))
  {
    Log("GR_Interface.cpp: Can't Create User Interface Texture.. \n");
	m_pUITexture = NULL;
  }
} //EndCreateTextureProcedure

//-----------------------------------------------------------------------------
// Name: CGRInterface::GetInplace()
// Desc: 
//-----------------------------------------------------------------------------
bool CGRInterface::GetInplace(int x,int y)
{
  //Is The Coordinate Passed In Inside (Over) This Interface Panel
  /*if ((x >= m_nX) && (x < (m_nX + m_nWidth - 1)) && (y >= m_nY) && (y < (m_nY + m_nHeight - 1)))
  {
    return true;
  }
  else
  {
    return false;
  }*/	
  return true; //Del This Before Debug
} //EndGetInplaceFunction

//-----------------------------------------------------------------------------
// Name: CGRInterface::CreateVertexBuffer()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CGRInterface::CreateVertexBuffer()
{
  HRESULT Result = S_OK;
  const FLOAT fDepth = 121.0f; //Z

  //Vertexes Description
  LAYER_VERTEX Vertexes[] = 
  {
	{-50.0f,  50.0f, fDepth, 0.0f, 0.0f}, //A
	{ 50.0f,  50.0f, fDepth, 1.0f, 0.0f}, //B
	{ 50.0f, -50.0f, fDepth, 1.0f, 1.0f}, //C

	{ 50.0f, -50.0f, fDepth, 1.0f, 1.0f}, //C
	{-50.0f, -50.0f, fDepth, 0.0f, 1.0f}, //D
	{-50.0f,  50.0f, fDepth, 0.0f, 0.0f}, //A
     //X        Y      Z     TexU  TexV
  }; //End Vertexes Description

  //Create Vertex Buffer
  if (FAILED(d_pDevice->CreateVertexBuffer(6*sizeof(LAYER_VERTEX),0, 
  LAYER_D3DFVF_VERTEX,D3DPOOL_DEFAULT,&m_pUIVertexBuffer,NULL))) Result = E_FAIL;

  //Lock The Vertex Buffer
  VOID* pBV;
  if (FAILED(m_pUIVertexBuffer->Lock(0,sizeof(Vertexes),(void**)&pBV,0))) Result = E_FAIL;
  
  memcpy(pBV,Vertexes,sizeof(Vertexes)); //Copy Vertex Data To Memory

  m_pUIVertexBuffer->Unlock(); //Unlock The Vertex Buffer

  return Result;
} //EndCreateVertexBufferFunction

//-----------------------------------------------------------------------------
// Name: CGRInterface::Render()
// Desc: Render UI
//-----------------------------------------------------------------------------
void CGRInterface::Render(D3DXMATRIX Projection)
{
  D3DSURFACE_DESC m_pSurface;
  D3DVIEWPORT9 ViewPort = {0, 0, 0, 0, 0.0f, 1.0f};
  D3DVIEWPORT9 OriginalVP;
  IDirect3DSurface9* pBackBuffer = NULL;
  UINT uPasses,uPass;
  float UI_BlendArray[4] = {0.0f, 0.0f, 0.0f, m_fOpacity};

  //If UI Effect Is Ready Then Render UI Now
  if (m_pUIEffect != NULL)
  {  
    d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

    d_pDevice->GetRenderTarget(0,&pBackBuffer);
    pBackBuffer->GetDesc(&m_pSurface);
  
    //Calculate The Viewport  
    ViewPort.Width = m_nWidth;
    ViewPort.Height = m_nHeight;
                                                               
    if (ViewPort.Width > m_pSurface.Width) ViewPort.Width = m_pSurface.Width; 
    if (ViewPort.Height > m_pSurface.Height) ViewPort.Height = m_pSurface.Height;
  
    ViewPort.X = m_nLeft;
    ViewPort.Y = m_nTop;

    d_pDevice->GetViewport(&OriginalVP);
    d_pDevice->SetViewport(&ViewPort);

    d_pDevice->SetStreamSource(0,m_pUIVertexBuffer,0,sizeof(LAYER_VERTEX));
    d_pDevice->SetFVF(NULL);
    d_pDevice->SetVertexDeclaration(m_pUIVertexDecl);

    m_pUIEffect->SetTechnique(m_pUITechHandle);
    m_pUIEffect->SetValue("UIBlend_Weight",(void*)UI_BlendArray,4*sizeof(float));
    m_pUIEffect->SetTexture("Input_Texture",m_pUITexture);
    
    //This Is Just Here To Get Around A Compiler Complaint
    m_pUIEffect->SetMatrix("Project_Matrix",&Projection);
    
    //Set The Blending State And Render UI Elements
    m_pUIEffect->Begin(&uPasses,0);
    for (uPass = 0; uPass < uPasses; uPass++)
    {
      m_pUIEffect->BeginPass(uPass);  

	  //Rendering UI Panel Layer
      d_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
	
      m_pUIEffect->EndPass();  
    }
    m_pUIEffect->End();

    ReleaseClass(pBackBuffer);
    d_pDevice->SetViewport(&OriginalVP);
  }
} //EndRenderProcedure

//-----------------------------------------------------------------------------