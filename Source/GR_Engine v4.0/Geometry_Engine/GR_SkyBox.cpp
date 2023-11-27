/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_SkyBox.cpp: Interface For The SkyBox Geometry Implementation Class.
// It's The GreenRay SkyBox Geometry Transform Engine.
// Made Specially For Implementing Only SkyBox Geometry And Effects.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_SkyBox.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRSkyBox Class
//////////////////////////////////////////////////////////////////////////////
CGRSkyBox::CGRSkyBox(LPDIRECT3DDEVICE9 pDevice)
{
  //Set Default Parameters
  d_pDevice = pDevice; //To Appropriate Device Pointer To Class
  m_pVertexBuffer = NULL; //Prepare Vertex Buffer
  m_pIndexBuffer = NULL; //Prepare Index Buffer
} //EndConstructionDirectives

CGRSkyBox::~CGRSkyBox()
{  
} //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: CGRSkyBox::Release()
// Desc: 
//------------------------------------------------------------------------------
void CGRSkyBox::Release()
{
  //Free SkyBox Textures
  for (UINT I = 0; I < 6; I++) 
  { 
    ReleaseClass(m_pTex[I]); 
	m_TexPath[I] = NULL; 
  } 

  ReleaseClass(m_pVertexBuffer); //Free Vertex Buffer
  ReleaseClass(m_pIndexBuffer);  //Free Index Buffer
  d_pDevice = NULL;              //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::SetTextures()
// Desc: Loading And Init Textures
//-----------------------------------------------------------------------------
void CGRSkyBox::SetTextures(LPCSTR ZNeg,
						    LPCSTR XNeg,
                            LPCSTR ZPos,
						    LPCSTR XPos,
						    LPCSTR YNeg,
							LPCSTR YPos)
{
  //Prepare Textures, Assign Array Of Textures NULL Value.
  memset(m_pTex,0x00,sizeof(m_pTex)); 

  m_TexPath[0] = ZNeg;
  m_TexPath[1] = XNeg;
  m_TexPath[2] = ZPos;
  m_TexPath[3] = XPos;
  m_TexPath[4] = YNeg;
  m_TexPath[5] = YPos;

  //Create Some Textures
  for (UINT I = 0; I < 6; I++)
  {
    if (FAILED(D3DXCreateTextureFromFileEx(d_pDevice,m_TexPath[I],D3DX_DEFAULT,D3DX_DEFAULT,
    1,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,0,NULL,NULL,&m_pTex[I])))
	{
	  Log("GR_SkyBox.cpp: Can't Load SkyBox Texture \n");
	}
  }
} //EndSetTexturesProcedure

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::CreateBox()
// Desc: Create SkyBox With Use Vertex And Index Buffer
//-----------------------------------------------------------------------------
HRESULT CGRSkyBox::CreateBox(float fWidth,float fHeight,float fDepth)
{
  m_pWidth = fWidth;
  m_pHeight = fHeight;
  m_pDepth = fDepth;

  //ћассив с описанием 6 сторон куба в данной структуре вершин 
  //описываютс€ координаты вершин в пространстве и текстурные
  //"барицентрические" (Barycentric) TU - TV координаты.
  SKYBOX_VERTEX Vertexes[] =
  {
    //Width   Height   Depth  tu(W) tv(H)
    //Back Wall
	{ fWidth, 0.0f,    0.0f, 0.0f, 1.0f, }, //A
    { fWidth, fHeight, 0.0f, 0.0f, 0.0f, }, //B
    { 0.0f,   fHeight, 0.0f, 1.0f, 0.0f, }, //C
	{ 0.0f,   0.0f,    0.0f, 1.0f, 1.0f, }, //D
   
	//Left Wall
    { 0.0f, 0.0f,    0.0f,   0.0f, 1.0f, }, //A2
    { 0.0f, fHeight, 0.0f,   0.0f, 0.0f, }, //B2
    { 0.0f, fHeight, fDepth, 1.0f, 0.0f, }, //C2
	{ 0.0f, 0.0f,    fDepth, 1.0f, 1.0f, }, //D2
    
	//Front Wall
	{ 0.0f,   0.0f,    fDepth, 0.0f, 1.0f, }, //A3
    { 0.0f,   fHeight, fDepth, 0.0f, 0.0f, }, //B3
    { fWidth, fHeight, fDepth, 1.0f, 0.0f, }, //C3
	{ fWidth, 0.0f,    fDepth, 1.0f, 1.0f, }, //D3
    
	//Right Wall
    { fWidth, 0.0f,    fDepth, 0.0f, 1.0f, }, //A4  
    { fWidth, fHeight, fDepth, 0.0f, 0.0f, }, //B4
    { fWidth, fHeight, 0.0f,   1.0f, 0.0f, }, //C4
	{ fWidth, 0.0f,    0.0f,   1.0f, 1.0f, }, //D4
    
	//Floor    
    { fWidth, 0.0f, 0.0f,   1.0f, 1.0f, }, //A5
    { 0.0f,   0.0f, 0.0f,   0.0f, 1.0f, }, //B5
    { 0.0f,   0.0f, fDepth, 0.0f, 0.0f, }, //C5
	{ fWidth, 0.0f, fDepth, 1.0f, 0.0f, }, //D5
     
	//Ceiling
    { fWidth, fHeight, fDepth, 1.0f, 1.0f, }, //A6 
    { 0.0f,   fHeight, fDepth, 0.0f, 1.0f, }, //B6
    { 0.0f,   fHeight, 0.0f,   0.0f, 0.0f, }, //C6
	{ fWidth, fHeight, 0.0f,   1.0f, 0.0f, }, //D6
    //  x        y       z     tu(W) tv(H)
  }; //End Vertexes Description

  //ћассив индексов содержащий и описывающий 6 сторон куба (12 треугольников)
  const unsigned short Index[] = 
  {
  0,1,2,    2,3,0,
  4,5,6,    6,7,4,
  8,9,10,   10,11,8,
  12,13,14, 14,15,12,
  16,17,18, 18,19,16,
  20,21,22, 22,23,20, 
  }; //End Indexes Description
	
  //Create Vertex Buffer
  if (FAILED(d_pDevice->CreateVertexBuffer(36*sizeof(SKYBOX_VERTEX),0, 
  D3DFVF_SKYBOXVERTEX,D3DPOOL_DEFAULT,&m_pVertexBuffer,NULL))) return E_FAIL;

  //Lock The Vertex Buffer
  VOID* pBV;
  if (FAILED(m_pVertexBuffer->Lock(0,sizeof(Vertexes),(void**)&pBV,0))) return E_FAIL;
  
  memcpy(pBV,Vertexes,sizeof(Vertexes)); //Copy Vertex Data To Memory

  m_pVertexBuffer->Unlock(); //Unlock The Vertex Buffer
    
  //Create Index Buffer
  d_pDevice->CreateIndexBuffer(36*sizeof(Index),0,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_pIndexBuffer,NULL);
    
  //Lock The Index Buffer
  VOID* pBI;
  m_pIndexBuffer->Lock(0,sizeof(Index),(void**)&pBI,0); 
    
  memcpy(pBI,Index,sizeof(Index)); //Copy Index Data To Memory
      
  m_pIndexBuffer->Unlock(); //Unlock The Index Buffer
 
  return S_OK;
} //EndCreateSkyBoxFunction

//-----------------------------------------------------------------------------
// Name: CGRSkyBox::Render()
// Desc: Rendering The SkyBox Geometry With Textures
//-----------------------------------------------------------------------------
void CGRSkyBox::Render()
{  
  //Set Render States And Disable Z-Buffer
  d_pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
  d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
  d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

  //VU Addr Set Texture Sampling Parameters
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);

  //Set Texture Mode
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);	
  //d_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
  d_pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
  d_pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
  //d_pDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);

  //Set Perspective View For Spherical Show SkyBox In Space (60 Degree) 
  float Aspect_Ratio = 1.1f;
  D3DXMATRIXA16 Mat_Proj;
  //FLOAT fAspectRatio = (float)m_d3dsdBackBuffer.Width/(float)m_d3dsdBackBuffer.Height;
  D3DXMatrixPerspectiveFovLH(&Mat_Proj,/*D3DX_PI/4*/D3DXToRadian(60.0f),Aspect_Ratio,1.0f,1000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&Mat_Proj); 

  //Translation Matrices For The SkyBox Object
  D3DXMATRIXA16 Mat_Trans,Mat_Rotate,Mat_Pos,Mat_Centered; 
  //                                        W           H           D
  D3DXMatrixTranslation(&Mat_Centered,-m_pWidth/2,-m_pHeight/2,-m_pDepth/2); 
  D3DXMatrixRotationYawPitchRoll(&Mat_Rotate,D3DX_PI,0.0f,0.0f);
  D3DXMatrixTranslation(&Mat_Pos,VecPos.x,VecPos.y,VecPos.z);
  Mat_Trans = (Mat_Centered*Mat_Rotate)*Mat_Pos; //Multiply Matrices
  d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Trans); //Transform Object Position In Space

  //Draw The SkyBox Here
  //Set Vertex And Index Buffers To Render
  d_pDevice->SetTexture(0,NULL);
 
  //”станавливаем в устройстве текущий вершинный буфер
  d_pDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(SKYBOX_VERTEX));
  //”станавливаем подготовленный ранее FVF наших вершин D3DFVF_SKYBOXVERTEX)
  d_pDevice->SetFVF(D3DFVF_SKYBOXVERTEX); 
  //”станавливаем индексы из ранее подготовленного индексного буфера:
  d_pDevice->SetIndices(m_pIndexBuffer);
  
  //Output For Created Objects
  //Render BackWall
  if (m_pTex[0] != NULL) d_pDevice->SetTexture(0,m_pTex[0]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2 );
  
  //Render LeftWall
  if (m_pTex[1] != NULL) d_pDevice->SetTexture(0,m_pTex[1]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 4, 0, 4, 0, 2 ); //Offset To 4 Base Vertex Index 

  //Render FrontWall
  if (m_pTex[2] != NULL) d_pDevice->SetTexture(0,m_pTex[2]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8, 0, 4, 0, 2 ); //Offset To 8 Base Vertex Index 

  //Render RightWall
  if (m_pTex[3] != NULL) d_pDevice->SetTexture(0,m_pTex[3]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 12, 0, 4, 0, 2 ); //Offset To 12 Base Vertex Index 

  //Render Floor
  if (m_pTex[4] != NULL) d_pDevice->SetTexture(0,m_pTex[4]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 16, 0, 4, 0, 2 ); //Offset To 16 Base Vertex Index

  //Render Ceiling
  if (m_pTex[5] != NULL) d_pDevice->SetTexture(0,m_pTex[5]);
  d_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 20, 0, 4, 0, 2 ); //Offset To 20 Base Vertex Index

  //«десь уже возвращаем исходные параметры и активируем Z-Buffer, это необходимо
  //дл€ устранени€ вли€ни€ параметров SkyBox на отрисовку последующих
  //элементов сцены которые будут рендеритьс€ после самого SkyBox-а в дальнейшем..
  d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
  d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
  
  //VU Addr ¬озвращаем исходные параметры дл€ отрисовки текстур,
  //так называемые текстурные "сэмплеры" и устанавливаем их в режим 
  //"обЄртки" - WRAP, если этого не сделать, то модель или друга€ геометри€
  //игрового "мира" имеюща€ текстуры будет рендеритьс€ некорректно, т.е 
  //изображение текстуры будет как бы раст€гиватьс€ от краЄв к центру, иными словами
  //будет сказыватьс€ вли€ние текстурного состо€ни€ CLAMP которое мы устанавливали
  //ранее дл€ отрисовки SkyBox..
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
  d_pDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
} //EndRenderProcedure

//-----------------------------------------------------------------------------