/****************************************************************************/ 
// It's An Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Engine.cpp Visual Studio 8 Version.
// It's An GreenRay GREngine Source File.
// Contain General 3D Engine Initialization Parameters And Instructions.
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Engine.h>

///////////////////////////////////////////////////////////////////////////////
// External DLL Definitions And Prototypes 
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
#define export extern "C" __declspec(dllexport)
#else
#define export __declspec(dllexport)
#endif

#ifdef __cplusplus
#define import extern "C" __declspec(dllimport)
#else
#define import __declspec(dllimport)
#endif

///////////////////////////////////////////////////////////////////////////////
// Exporting DLL External Definitions And Prototypes 
///////////////////////////////////////////////////////////////////////////////
export HRESULT CreateGR(HWND WndLayer);
export void CreateDirect3DInterface(HWND WndLayer);
export void RenderDirect3D();
export void ReleaseDirect3D();
export void SetWindowedMode(BOOL Windowed);
export void SetScreenResolution(int Width,int Height);
export void SetSkyBoxMaterials(LPCSTR ZNeg,LPCSTR XNeg,LPCSTR ZPos,LPCSTR XPos,LPCSTR YNeg,LPCSTR YPos);
export void UseCamera(bool UseConnect);

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Prototypes
////////////////////////////////////////////////////////////////////////////////
C3DEngine* m_pGREngine = NULL;   //Global GreenRay Engine Interface

//-----------------------------------------------------------------------------
// Name: CreateGR()
// Desc: GreenRay Engine Initialization Function
//-----------------------------------------------------------------------------
HRESULT CreateGR(HWND WndLayer)
{
  //Set Default Parameters
  HRESULT m_pResult = S_OK; //Set Result By Default

  m_pGREngine = new C3DEngine(WndLayer);

  if (m_pGREngine == NULL)
  {
    return E_FAIL;
  }
  else return S_OK;
} //EndCreateGRFunction

//-----------------------------------------------------------------------------
// Name: CreateDirect3DInterface()
// Desc: Create GreenRay Engine Direct3D Interface
//-----------------------------------------------------------------------------
void CreateDirect3DInterface(HWND WndLayer)
{
  //Create Direct3D Interface
  if (m_pGREngine != NULL) 
  {
    m_pGREngine->CreateDirect3D(WndLayer);
    m_pGREngine->CreateObjects();
  }
} //EndCreateDirect3DInterfaceProcedure

//-----------------------------------------------------------------------------
// Name: RenderDirect3D()
// Desc: Render GreenRay Scene And All Objects
//-----------------------------------------------------------------------------
void RenderDirect3D()
{
  if (m_pGREngine != NULL) m_pGREngine->Render();
} //EndRenderDirect3DProcedure

//-----------------------------------------------------------------------------
// Name: ReleaseDirect3D()
// Desc: Release GreenRay Engine Direct3D
//-----------------------------------------------------------------------------
void ReleaseDirect3D()
{
  //Free GreenRay Engine Interface
  if (m_pGREngine != NULL) 
  {
    ReleaseClass(m_pGREngine); 
	DeleteClass(m_pGREngine);
  }
} //EndReleaseDirect3DProcedure

//-----------------------------------------------------------------------------
// Name: SetWindowedMode()
// Desc: 
//-----------------------------------------------------------------------------
void SetWindowedMode(BOOL Windowed)
{
  if (m_pGREngine != NULL) m_pGREngine->SetWindowed(Windowed);
} //EndSetWindowedModeProcedure

//-----------------------------------------------------------------------------
// Name: SetScreenResolution()
// Desc: 
//-----------------------------------------------------------------------------
void SetScreenResolution(int Width,int Height)
{
  if (m_pGREngine != NULL) m_pGREngine->SetScreenResolution(Width,Height);
} //EndSetScreenResolutionProcedure

//-----------------------------------------------------------------------------
// Name: SetSkyBoxMaterials()
// Desc: 
//-----------------------------------------------------------------------------
void SetSkyBoxMaterials(LPCSTR ZNeg,
			            LPCSTR XNeg,
                        LPCSTR ZPos,
				        LPCSTR XPos,
						LPCSTR YNeg,
						LPCSTR YPos)
{  
  if (m_pGREngine != NULL) m_pGREngine->SetSkyBoxTextures(ZNeg,XNeg,ZPos,XPos,YNeg,YPos);
} //EndSetSkyBoxMaterialsProcedure

//-----------------------------------------------------------------------------
// Name: UseCamera()
// Desc: 
//-----------------------------------------------------------------------------
void UseCamera(bool UseConnect)
{  
  if (m_pGREngine != NULL) m_pGREngine->SetUseCamera(UseConnect);
} //EndUseCameraProcedure



///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For C3DEngine Class
///////////////////////////////////////////////////////////////////////////////
C3DEngine::C3DEngine(HWND WndLayer)
{
  //Set Default Parameters
  m_pHWnd = WndLayer;          //Apropritate Window Layer Handle To Class
  m_pDevice = NULL;            //Direct3D Based Interface
  gr_Camera = NULL;            //Camera
  gr_SkyBox = NULL;            //SkyBox
  gr_FPS = NULL;               //FPS Counter
  gr_TestObj = NULL;           //Test Geometry Object 
  gr_TestObjShader = NULL;     //Test Geometry Light Shader
  gr_TestBarrel2 = NULL;       //Test Barrel2 Object
  gr_TestBarrelShader2 = NULL; //Test Barrel2 Light Shader
  gr_TestPipe1 = NULL;         //Test Pipe1 Object
  gr_TestPipeShader1 = NULL;   //Test Pipe1 Light Shader
  gr_TestPipe2 = NULL;         //Test Pipe2 Object
  gr_TestPipeShader2 = NULL;   //Test Pipe2 Light Shader



  gr_PhysX = NULL;         //PhysX Engine
  gr_WeaponSystem = NULL;  //Weapon System
  gr_Light = NULL;         //Deferred Lightning Shader

  //Set Default D3D Parameters And Presets
  WndHeight =              480;
  WndWidth =               640;
  bWindowed =              TRUE;
  SwapEffect =             D3DSWAPEFFECT_DISCARD; //D3DSWAPEFFECT_COPY; //D3DSWAPEFFECT_FLIP;
  bAutoDepthStencil =      TRUE;
  AutoDepthStencilFormat = D3DFMT_D16; //D3DFMT_D16; //D3DFMT_D16 D3DFMT_D15S1 D3DFMT_D24X8
                                       //D3DFMT_D24S8 D3DFMT_D24X4S4 D3DFMT_D32
  BackBufferCount =      2;
  MultiSampleType =      D3DMULTISAMPLE_NONE; //D3DMULTISAMPLE_4_SAMPLES; //4_SAMPLES //For Antialiasing Mode On 4x
  MultiSampleQuality =   0;
  PresentationInterval = D3DPRESENT_INTERVAL_ONE; //D3DPRESENT_INTERVAL_DEFAULT; //D3DPRESENT_INTERVAL_IMMEDIATE;

} //EndConstructionDirectives

C3DEngine::~C3DEngine()
{  
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: C3DEngine::CreateDirect3D()
// Desc: Create Direct3D Interface
//-----------------------------------------------------------------------------
void C3DEngine::CreateDirect3D(HWND WndLayer)
{   
  //Set Default Parameters
  HRESULT m_pResult = S_OK; //Set Result By Default
  m_pDirect3D = NULL;       //Direct3D Interface

  //Create Direct3D Object (DirectX 9)
  m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

  //Check If Error Then Release D3D Interface
  if (!m_pDirect3D)
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
	Log("GR_Engine.cpp: Failed To Create Direct3D Interface.. \n");
    m_pResult = E_FAIL;
  }
  
  //Display Parameters Automatic Searching. 
  //Recieved Data Will Be Converted Into The m_pDisplay Structure.
  D3DDISPLAYMODE m_pDisplay; 
  if (FAILED(m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&m_pDisplay)))
  {
    if (m_pDirect3D != NULL) m_pDirect3D->Release();
	Log("GR_Engine.cpp: Failed Get Adapter Display Mode.. \n");
    m_pResult = E_FAIL;
  }

  //All System Checks Passed, Create The Direct3D Device
  D3DPRESENT_PARAMETERS m_pD3DParam;
  memset(&m_pD3DParam,0,sizeof(m_pD3DParam));

  //Set Parameters On Direct3D Device 
  m_pD3DParam.SwapEffect                 = SwapEffect;
  
  //Set Screen Resolution
  m_pD3DParam.BackBufferWidth            = WndWidth; 
  m_pD3DParam.BackBufferHeight           = WndHeight;

  //Set Windowed Or Fullscreen Mode State
  m_pD3DParam.Windowed                   = bWindowed;

  //Set Back Buffer Format 
  m_pD3DParam.BackBufferFormat           = m_pDisplay.Format; //D3DFMT_A8R8G8B8;

  //Uncomment This Parameter Only If:
  //The Application Should Run In Full Screen Mode (Display Frequency Must Be Defined Manualy) 
  //It's Not Recommended To Switch This Parameter To Manual, Display Frequency Should Be 
  //Defined Automatically (m_pDisplay.RefreshRate)
  if (!bWindowed) m_pD3DParam.FullScreen_RefreshRateInHz = m_pDisplay.RefreshRate; //D3DPRESENT_RATE_DEFAULT; 
  
  //Enable Stencli Buffer, Stencil Buffer Is Needed For Shadows And Mirrors Rendering, 
  //It Repeats Contours Of A Source Mesh
  m_pD3DParam.EnableAutoDepthStencil     = bAutoDepthStencil;

  //Set Depth Stencil Format
  m_pD3DParam.AutoDepthStencilFormat     = AutoDepthStencilFormat;
  
  //Set Back Buffer Count
  m_pD3DParam.BackBufferCount            = BackBufferCount;

  //Set Number Of Antialiasing Sample Levels
  m_pD3DParam.MultiSampleType            = MultiSampleType;
  
  //Set Antialiasing Quality Level
  m_pD3DParam.MultiSampleQuality         = MultiSampleQuality;
  
  //Deep Buffer Usually Activated For Creation Shadow Voume Effect 
  //m_pD3DParam.Flags                    = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL; //D3DPRESENTFLAG_DEVICECLIP; //D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL; //|D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
 
  //Set Presentation Interval (VSync)
  m_pD3DParam.PresentationInterval       = PresentationInterval;

  //Create Direct3D Device, "скармливаем" ранее установленные параметры нашему устройству
  if (FAILED(m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,WndLayer,
  D3DCREATE_HARDWARE_VERTEXPROCESSING,&m_pD3DParam,&m_pDevice)))
  {
	//Errror Creating Direct3D Device Release It
    if (m_pDirect3D != NULL) m_pDirect3D->Release(); 	
	Log("GR_Engine.cpp: Error Creating Direct3D Device.. \n");
    m_pResult = E_FAIL;
  }

  //Ending Direct3D Presets Parameters
  if (m_pDevice != NULL)
  {
	//Set Scene Render States:
    //Весьма интересный и нужный режим состояния рендера
    //при этом режиме происходит "отсечение" невидимых взору граней (полигонов), т.е
    //граней находящихся с задней стороны модели или 3D объекта, они попросту не ренерятся
    //и не отображаюся, это позволяет очень сильно разгрузить работу нашей видеокарты и всей
    //системы в целом.
    //Режим D3DCULL_CW - устанавливает отсечение невидимый граней
    //Режим D3DCULL_CСW - устанавливает тоже отсечение только уже видимых граней (инверсия)
	//Режим D3DCULL_NONE - отключает оба этих режима рендерится вся модель как есть даже 
    //её внутренние части, это уже слишком тяжело для видеокарты, сразу же ощущается падение fps
    //после установки этого режима.
	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
    
	//Enable Z-Buffer
	m_pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

    //Set Scene Lighiting To FALSE
    m_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE); 
   
	//Режим рендера отображающий полигоны не цельными SOLID, а в режиме сетки WIREFRAME
	//m_pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
    
	//Установка цвета амбиентного (общего) освещения
	//m_pDevice->SetRenderState(D3DRS_AMBIENT,0xFFFFFFFF);
    
	//Данная установка включает режим работы антиалиасинга (прменения эффекта сглаживания 
	//рёбер или неровности краёв полигона называется антиалиасингом), нам пока не нужен 
	//поэтому закомментирован
	//m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE);
	
	//Установка режима закраски по Гуро, обычно она устанавливается по умолчанию 
	//(закраска по Гуро обычно нужна для сглаживания одутлых или круглых поверхностей)
    //m_pDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
  } //End if (m_pDevice != NULL)

  //Creating The Direct3D Device Is Complete Now Release It
  //if (m_pDirect3D != NULL) m_pDirect3D->Release(); 

  //Log The Direct3D Device Create Completion 
  if (m_pResult == S_OK) 
  {
    Log("GR_Engine.cpp: D3D Interface Was Created Sucessfully Ok! \n");
  }
  else
  {
    Log("GR_Engine.cpp: Fatal Error! D3D Interface Create Fail! \n");
  }
} //EndCreateDirect3DProcedure

//-----------------------------------------------------------------------------
// Name: C3DEngine::CreateObjects()
// Desc: Create Engine Scene Objects
//-----------------------------------------------------------------------------
void C3DEngine::CreateObjects()
{
  if (m_pDevice != NULL) 
  {
////////////////////////////////////////////////////////////////////////////////
    //Create Camera Class
    gr_Camera = new CGRCamera(m_pDevice,m_pHWnd); //Construct CGRCamera Class
	gr_Camera->SetCameraPos(D3DXVECTOR3(0.0f,25.0f,0.0f));
	//gr_Camera->SetCameraPos(D3DXVECTOR3(5.0f,50.78f,5.0f)); //Set Player Camera Position In Space
	gr_Camera->SetCameraSpeed(70.0f); //Set Player Camera Speed
    gr_Camera->SetMouseSpeed(0.3f);   //Set Mouse Speed

////////////////////////////////////////////////////////////////////////////////
	//Create SkyBox Class
	gr_SkyBox = new CGRSkyBox(m_pDevice); //Construct CGRSkyBox Class  
    gr_SkyBox->CreateBox(100.0f,100.0f,100.0f); //Create Our SkyBox Model With Size WHD

////////////////////////////////////////////////////////////////////////////////
    //Create FPS Output Information
    gr_FPS = new CGRFPS(m_pDevice);    //Create FPS Counter Class
    gr_FPS->SetFPSPos(WndWidth-65,12); //Set Position On Screen

////////////////////////////////////////////////////////////////////////////////
    //Creation And Loading Simple Map Model Class
    gr_TestObj = new CGRObject();
    gr_TestObj->SetTexturePath("\Models\\Locations\\Room_02\\");
    gr_TestObj->Create(m_pDevice,L"\Models\\Locations\\Room_02\\Basement.x",20);
	//\Models\\Locations\\Room_02\\
	//L"\Models\\Locations\\Room_02\\Basement.x"
	//L"\Models\\Locations\\Room_01\\Test_Room.x"
	//L"\Models\\Locations\\Terrain\\Terrain.x"
    //L"\Models\\StarWars\\Chu_Untor\\Chu_Untor.x"
////////////////////////////////////////////////////////////////////////////////

	//Create Physics Class
    gr_PhysX = new CGRPhysX(m_pDevice,m_pHWnd); //Construct CGRPhysX Class

    //Generate Triangles Adjacency For Physics Collision
	gr_PhysX->TrianglesFromDXMesh(gr_TestObj->GetLocalMeshGeometry()); 
	gr_PhysX->Init(GRPHYSX_ROOM);
////////////////////////////////////////////////////////////////////////////////   

	//Use And Create Deffered Lighting System
	gr_Light = new CGRLightSystem(m_pDirect3D,m_pDevice,WndWidth,WndHeight); //Construct CGRLightSystem Class
////////////////////////////////////////////////////////////////////////////////

    UINT MatNum;
    D3DXVECTOR4 MatVec(0,0,0,0);
    D3DXVECTOR4 DpfVec(0,0,0,0);

    //Attach Light Shader To Model 
    gr_TestObjShader = new CGRMeshXShader();
    gr_TestObjShader->Create(m_pDevice,gr_TestObj->GetCGRModel(),true);

	D3DXMATRIX m1,m2,MatRot,MatTrans;

   /* D3DXMATRIX m1,m2,MatRot,MatTrans;
	D3DXMatrixScaling( &m1, 30, 30, 30);
	D3DXMatrixRotationY(&MatRot, 0.0f);

    D3DXMatrixMultiply(&MatTrans,&m1,&MatRot);

	D3DXMatrixTranslation(&m2, 10, 30, 0);
	D3DXMatrixMultiply(& gr_TestObjShader->m_WorldMatrix, &MatTrans,&m2);*/


    MatNum = RegisterMaterial(Material(0.5f, 0.5f, 0.5f)); //MATERIAL_FULL_AMBIENT; //Used An Already Created Material
    MatVec = D3DXVECTOR4(1.3f,0.3f, MaterialIndexToV(MatNum),0);
    gr_TestObjShader->m_pShader->SetVector("f3Material",&MatVec);
    DpfVec = D3DXVECTOR4(GetDotProductFunctionU(DPF_FRONT_POW1),GetDotProductFunctionU(DPF_FRONT_POW512),0,0);
    gr_TestObjShader->m_pShader->SetVector("f2DotProductFuncs",&DpfVec);
////////////////////////////////////////////////////////////////////////////////

    //Creation And Loading Test Barrel Model Class
    gr_TestBarrel2 = new CGRObject();
    gr_TestBarrel2->SetTexturePath("\Models\\Weapons\\Beretta\\Shell\\");
    gr_TestBarrel2->Create(m_pDevice,L"\Models\\Weapons\\Beretta\\Shell\\Shell.x",1);
    //\Models\\Weapons\\Beretta\\Shell\\Shell.x

    //Attach Light Shader To Model 
    gr_TestBarrelShader2 = new CGRMeshXShader();
	gr_TestBarrelShader2->Create(m_pDevice,gr_TestBarrel2->GetCGRModel(),true);

	D3DXMATRIX MatRotX,MatRotY;
	D3DXMatrixScaling(&m1, 33, 33, 36);
	D3DXMatrixRotationX(&MatRotX, 1.6f);
	D3DXMatrixRotationY(&MatRotY, 1.8f);

    D3DXMatrixMultiply(&MatRot,&MatRotX,&MatRotY);
    D3DXMatrixMultiply(&MatTrans,&m1,&MatRot);

	D3DXMatrixTranslation(&m2, 10, 8.7f, -120);
	D3DXMatrixMultiply(&gr_TestBarrelShader2->m_WorldMatrix, &MatTrans,&m2);

    MatNum = RegisterMaterial(Material(0.8f, 0.1f, 0.0f)); //Used An Already Created Material
    MatVec = D3DXVECTOR4(0.5f,0.2f, MaterialIndexToV(MatNum),0);
    gr_TestBarrelShader2->m_pShader->SetVector("f3Material",&MatVec);
    DpfVec = D3DXVECTOR4(GetDotProductFunctionU(DPF_FRONT_POW4),GetDotProductFunctionU(DPF_FRONT_POW32),0,0);
    gr_TestBarrelShader2->m_pShader->SetVector("f2DotProductFuncs",&DpfVec);

	/*D3DXCreateTextureFromFile(m_pDevice, "Brick_Wall.tga", &difTex );
	gr_TestBarrelShader->m_pShader->SetTexture( "DiffuseMapTexture", difTex );


	D3DXCreateTextureFromFile(m_pDevice, "Brick_Wall_Spec.tga", &specTex );
	gr_TestBarrelShader2->m_pShader->SetTexture( "SpecularMapTexture", specTex );


	// load it twice so the second one is set same format, size etc
	D3DXCreateTextureFromFile(m_pDevice, "Brick_Wall_Norm.tga", &normTex0 );
	D3DXCreateTextureFromFile(m_pDevice, "Brick_Wall_Norm.tga", &normTex1 );

	//D3DXComputeNormalMap( normTex0, normTex1, 0, 0, D3DX_CHANNEL_LUMINANCE, 100 );
	gr_TestBarrelShader2->m_pShader->SetTexture( "NormalMapTexture", normTex0 );*/


////////////////////////////////////////////////////////////////////////////////

    //Creation And Loading Test Pipe1 Model Class
    gr_TestPipe1 = new CGRObject();
    gr_TestPipe1->SetTexturePath("\Models\\Pipes\\");
    gr_TestPipe1->Create(m_pDevice,L"\Models\\Pipes\\Pipe_01.x",70);

    //Attach Light Shader To Model 
    gr_TestPipeShader1 = new CGRMeshXShader();
	gr_TestPipeShader1->Create(m_pDevice,gr_TestPipe1->GetCGRModel(),true);

	D3DXMatrixScaling(&m1, 0.2f, 0.2f, 0.2f);
	D3DXMatrixRotationX(&MatRotX, 1.6f);
	D3DXMatrixRotationY(&MatRotY, 1.55f);

    D3DXMatrixMultiply(&MatRot,&MatRotX,&MatRotY);
    D3DXMatrixMultiply(&MatTrans,&m1,&MatRot);

	D3DXMatrixTranslation(&m2, 102.5f, 0.0f, -10.0f);
	D3DXMatrixMultiply(&gr_TestPipeShader1->m_WorldMatrix, &MatTrans, &m2);

    MatNum = RegisterMaterial(Material(1.4f, 0.f, 0.6f)); //Used An Already Created Material
    MatVec = D3DXVECTOR4(1.3f,0.45f, MaterialIndexToV(MatNum),0);
    gr_TestPipeShader1->m_pShader->SetVector("f3Material",&MatVec);
    DpfVec = D3DXVECTOR4(GetDotProductFunctionU(DPF_FRONT_POW1),GetDotProductFunctionU(DPF_FRONT_POW32),0,0);
    gr_TestPipeShader1->m_pShader->SetVector("f2DotProductFuncs",&DpfVec);

////////////////////////////////////////////////////////////////////////////////

    //Creation And Loading Test Pipe2 Model Class
    gr_TestPipe2 = new CGRObject();
    gr_TestPipe2->SetTexturePath("\Models\\Pipes\\");
    gr_TestPipe2->Create(m_pDevice,L"\Models\\Pipes\\Pipe_01.x",100);

    //Attach Light Shader To Model 
    gr_TestPipeShader2 = new CGRMeshXShader();
	gr_TestPipeShader2->Create(m_pDevice,gr_TestPipe2->GetCGRModel(),true);

	D3DXMatrixScaling(&m1, 0.2f, 0.2f, 0.2f);
	D3DXMatrixRotationX(&MatRotX, 1.6f);
	D3DXMatrixRotationY(&MatRotY, 1.55f);

	D3DXMatrixMultiply(&MatRot,&MatRotX,&MatRotY);
    D3DXMatrixMultiply(&MatTrans,&m1,&MatRot);

	D3DXMatrixTranslation(&m2, 102.5f, 0.0f, -20.0f);
	D3DXMatrixMultiply(&gr_TestPipeShader2->m_WorldMatrix, &MatTrans,&m2);

    MatNum = RegisterMaterial(Material(0.4f, 0.f, 0.6f)); //Used An Already Created Material
    MatVec = D3DXVECTOR4(1.3f,0.45f, MaterialIndexToV(MatNum),0);
    gr_TestPipeShader2->m_pShader->SetVector("f3Material",&MatVec);
    DpfVec = D3DXVECTOR4(GetDotProductFunctionU(DPF_FRONT_POW1),GetDotProductFunctionU(DPF_FRONT_POW32),0,0);
    gr_TestPipeShader2->m_pShader->SetVector("f2DotProductFuncs",&DpfVec);

////////////////////////////////////////////////////////////////////////////////

    //Crete Weapon System Class
    gr_WeaponSystem = new CGRWeaponSystem(m_pDevice,m_pHWnd,WndWidth,WndHeight); //Construct CGRWeaponSystem Class


  } //End if (m_pDevice != NULL)
} //EndCreateObjectsProcedure

//-----------------------------------------------------------------------------
// Name: C3DEngine::SetSkyBoxTextures()
// Desc: Change Dynamically SkyBox Textures
//-----------------------------------------------------------------------------
void C3DEngine::SetSkyBoxTextures(LPCSTR ZNeg,
								  LPCSTR XNeg,
                                  LPCSTR ZPos,
								  LPCSTR XPos,
								  LPCSTR YNeg,
								  LPCSTR YPos)
{
  //Set Textures On SkyBox
  if (gr_SkyBox != NULL) gr_SkyBox->SetTextures(ZNeg,XNeg,ZPos,XPos,YNeg,YPos);
} //EndSetSkyBoxTexturesProcedure

//-----------------------------------------------------------------------------
// Name: C3DEngine::SetUseCamera()
// Desc: Connect To Camera
//-----------------------------------------------------------------------------
void C3DEngine::SetUseCamera(bool UseConnect)
{  
  if (m_pGREngine != NULL) gr_Camera->UseCamera(UseConnect);
} //EndSetUseCameraProcedure

//-----------------------------------------------------------------------------
// Name: C3DEngine::Render()
// Desc: Render Scene And Objects
//-----------------------------------------------------------------------------
void C3DEngine::Render()
{   
  if (m_pDevice != NULL && gr_PhysX != NULL && gr_Camera != NULL && 
	  gr_WeaponSystem != NULL && gr_Light != NULL) 
  {
	//Update GreenRay Camera
    gr_Camera->UpdateCamera(); 
	gr_PhysX->JumpPlayer(gr_Camera->SpaceDown()); //Update Player Jump
	gr_PhysX->UpdatePlayer(gr_Camera);            //Update Player
	gr_PhysX->BeginUpdate(); //Begin Update PhysX
	bool bShoot = gr_PhysX->UpdateRC(gr_Camera->CamPosX(),
		                             gr_Camera->CamPosY(),
						             gr_Camera->CamPosZ(),
						             gr_Camera->MbLeftDown()); //Update PhysX RayCast

	//Update Light Shader
	gr_Light->UpdateLight(gr_Camera->View,gr_Camera->Position); 

	//Update Weapon System
    gr_WeaponSystem->Update(gr_Camera->View);

    //Clear Z-Buffer By Every Scene Frame Update Process
    m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER/*|D3DCLEAR_STENCIL*/,D3DCOLOR_XRGB(0,0,0),1.0f,0);
    
	//Begin Scene
    if (SUCCEEDED(m_pDevice->BeginScene()))
    {
      //Render All Scene Objects Here:

      //Render Light System
	  gr_Light->BeginShadow(0, D3DXVECTOR3(102.5f, 0.0f, -10.0f), 3.0f, 0.2f, 0.0f); //Begin Rendering Shadows
	  { 
        
		gr_TestPipeShader1->RenderShadow(0);
        gr_TestPipeShader2->RenderShadow(0);

	  } gr_Light->EndShadow(); //End Rendering Shadows

      
	  
	  gr_Light->BeginShadow(1, D3DXVECTOR3(10, 8.7f, -120), 11.6f,11.8f, 0.0f); //Begin Rendering Shadows
	  {
        
        gr_TestBarrelShader2->RenderShadow(1);
       	  
	  } gr_Light->EndShadow(); //End Rendering Shadows




      gr_Light->BeginLight(); //Begin Rendering Lighting
	  {
	    gr_TestObjShader->Render(0);
		gr_TestBarrelShader2->Render(0);
		gr_TestPipeShader1->Render(0);
		gr_TestPipeShader2->Render(0);
	    gr_WeaponSystem->RenderLighting();
	  
	  } gr_Light->EndLight(); //End Rendering Lighting


      //Render The SkyBox! 
	  //if (gr_SkyBox != NULL) 
	  //{
	    //gr_SkyBox->SetTransformCoord(gr_Camera->Pos());
	    //gr_SkyBox->Render();
	  //}
    
      //Display FPS Information 
      if (gr_FPS != NULL) gr_FPS->UpdateFPS();
      
	  //Processing Weapon Shoot
	  if (bShoot) 
	  {
	    //Update Weapon Shoot Flash Animation
		gr_WeaponSystem->UpdateFlash(gr_Camera->Position,gr_Camera->View);
        gr_Light->UseFlash(bShoot);

	    //Make Bullet Hole On The Walls
	    gr_WeaponSystem->UpdateBallistic(gr_PhysX->RayPos(),
			                             gr_PhysX->RayNormal(),
										 gr_PhysX->RayVec1(),
										 gr_PhysX->RayVec2());

	    //Make Weapon Shell Animation
		gr_WeaponSystem->UpdateShellTube(gr_PhysX->GetPhysXScene(),
			                             gr_PhysX->GetPhysXPlayer(),
										 gr_PhysX->GetPhysXWorld(),
										 gr_Camera->View);

		//Playback Sounds
        gr_WeaponSystem->PlaybackRicochet();
	  }

      //Render Weapon System
	  gr_WeaponSystem->SetProjection(gr_Camera->Projection);
	  gr_WeaponSystem->Render();
  
	  m_pDevice->EndScene(); //End Scene
    }
    m_pDevice->Present(NULL,NULL,NULL,NULL);
	gr_PhysX->EndUpdate();
  }
} //EndRenderingDirect3DProcedure

//------------------------------------------------------------------------------
// Name: C3DEngine::Release()
// Desc: Release Direct3D Devices And Free All Engine Resources
//------------------------------------------------------------------------------
void C3DEngine::Release()
{
  //Release Test Light Shader Mesh 
  if (gr_Light != NULL) 
  {
	ReleaseClass(gr_Light); 
	DeleteClass(gr_Light); 
  }

  //Release SkyBox
  if (gr_SkyBox != NULL) 
  { 
    ReleaseClass(gr_SkyBox); 
	DeleteClass(gr_SkyBox); 
  }

  //Release Camera
  if (gr_Camera != NULL) 
  { 
    ReleaseClass(gr_Camera); 
	DeleteClass(gr_Camera); 
  }
   
  //Release FPS Counter
  if (gr_FPS != NULL) 
  { 
    ReleaseClass(gr_FPS); 
	DeleteClass(gr_FPS); 
  }

  //Release PhysX Engine
  if (gr_PhysX != NULL) 
  { 
    ReleaseClass(gr_PhysX); 
	DeleteClass(gr_PhysX); 
  }

  //Release Test Barrel2 Object
  if (gr_TestBarrel2 != NULL) 
  { 
    ReleaseClass(gr_TestBarrel2); 
	DeleteClass(gr_TestBarrel2); 
  }

  //Release Test Barrel2 Object Light Shader
  if (gr_TestBarrelShader2 != NULL) DeleteClass(gr_TestBarrelShader2); 

  //Release Test Pipe1 Object
  if (gr_TestPipe1 != NULL) 
  { 
    ReleaseClass(gr_TestPipe1); 
	DeleteClass(gr_TestPipe1); 
  }

  //Release Test Pipe1 Object Light Shader
  if (gr_TestPipeShader1 != NULL) DeleteClass(gr_TestPipeShader1); 

   //Release Test Pipe2 Object
  if (gr_TestPipe2 != NULL) 
  { 
    ReleaseClass(gr_TestPipe2); 
	DeleteClass(gr_TestPipe2); 
  }

  //Release Test Pipe2 Object Light Shader
  if (gr_TestPipeShader2 != NULL) DeleteClass(gr_TestPipeShader2); 




  //Release Test Model Object
  if (gr_TestObj != NULL) 
  { 
    ReleaseClass(gr_TestObj); 
	DeleteClass(gr_TestObj); 
  }

  //Release Test Model Object Light Shader
  if (gr_TestObjShader != NULL) DeleteClass(gr_TestObjShader); 

  //Release Weapon System
  if (gr_WeaponSystem != NULL) 
  { 
    ReleaseClass(gr_WeaponSystem); 
	DeleteClass(gr_WeaponSystem); 
  }

  //Release The Direct3D Device
  if (m_pDevice != NULL) 
  {
    ReleaseClass(m_pDevice); 
	DeleteClass(m_pDevice);
    Log("GR_Engine.cpp: D3D Interface Was Released Ok! \n");
  }

  //Release The Direct3D Device Interface
  if (m_pDirect3D != NULL) 
  {
	Log("GR_Engine.cpp: Direct3D Interface Was Released Ok! \n");
	m_pDirect3D->Release();
  } 
} //EndReleaseProcedure

//------------------------------------------------------------------------------