/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_WeaponSystem.cpp
// It's The GreenRay Weapon System Engine.
// Made Specially For Implementing All Weapon Management Functions.
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_WeaponSystem.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRWeaponSystem Class
///////////////////////////////////////////////////////////////////////////////
CGRWeaponSystem::CGRWeaponSystem(LPDIRECT3DDEVICE9 pDevice,HWND WndHndl,int ScreenHeight,int ScreenWidth)
{
  //Set Sefault Parameters
  d_pDevice = pDevice;
  m_pDeltaTimer = NULL;    //Delta Timer
  m_pCrosshair = NULL;     //Weapon Crosshair
  m_pBeretta = NULL;       //Beretta Model
  m_pBerettaShader = NULL; //Beretta Shader
  m_pSndRicochet = NULL;   //Ricochet Sound
  m_pFlash = NULL;         //Weapon Flash

  Num_Ballistic = 0;
  Num_Shells = 0;

  ScrWidth = ScreenWidth;   //Set Screen Width
  ScrHeight = ScreenHeight; //Set Screen Height

  //Create Delta Timer Class
  m_pDeltaTimer = new CGRDeltaTimer(); //Construct CGRDeltaTimer Class

////////////////////////////////////////////////////////////////////////////////
  //Create Crosshair Class
  m_pCrosshair = new CGRInterface(d_pDevice); //Create Crosshair
  m_pCrosshair->SetTop((ScrHeight-32)*0.362);
  m_pCrosshair->SetLeft((ScrWidth-32)*0.68);
  m_pCrosshair->SetWidth(32);
  m_pCrosshair->SetHeight(32);
  m_pCrosshair->SetOpacityLevel(100); //Set Opacity Level

  //Create Beretta Texture
  m_pCrosshair->CreateTexture("\Textures\\Crosshairs\\Beretta\\CrossSmall_.png",D3DCOLOR_XRGB(0,0,0)); //Create Crosshair Texture
////////////////////////////////////////////////////////////////////////////////

  //Create Beretta Weapon
  m_pBeretta = new CGRObject();
  m_pBeretta->SetTexturePath("\Textures\\Weapons\\Beretta\\");
  m_pBeretta->Create(d_pDevice,L"\Models\\Weapons\\Beretta\\Beretta.x",1);
////////////////////////////////////////////////////////////////////////////////  

  UINT MatNum;
  D3DXVECTOR4 MatVec(0,0,0,0);
  D3DXVECTOR4 DpfVec(0,0,0,0);
  D3DXVECTOR4 Color;

  //Attach Light Shader To Beretta Weapon Model 
  m_pBerettaShader = new CGRMeshXShader();
  m_pBerettaShader->Create(d_pDevice,m_pBeretta->GetCGRModel(),false);

  Color = D3DXVECTOR4(1,1,1,1);
  MatNum = RegisterMaterial(Material(0.3f, 0.1f, 0.2f)); //Used An Already Created Material
  MatVec = D3DXVECTOR4(2.3f,0.7f, MaterialIndexToV(MatNum),0);
  m_pBerettaShader->m_pShader->SetVector("f3Material",&MatVec);
  DpfVec = D3DXVECTOR4(GetDotProductFunctionU(DPF_FRONT_POW3),GetDotProductFunctionU(DPF_FRONT_POW512),0,0);
  m_pBerettaShader->m_pShader->SetVector("f2DotProductFuncs",&DpfVec);
  m_pBerettaShader->m_pShader->SetVector("f3Colour0",&Color);
//////////////////////////////////////////////////////////////////////////////// 

  //Create Ricochet Sound
  m_pSndRicochet = new CGRWave();
  m_pSndRicochet->CreateSoundInterface(WndHndl,"\Sounds\\Weapon\\Beretta\\Ricochet.wav",2,44100,32);
//////////////////////////////////////////////////////////////////////////////// 

  //Create Flash Update System
  m_pFlash = new CGRWeaponFlash(d_pDevice);

} //EndConstructionDirectives

CGRWeaponSystem::~CGRWeaponSystem()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRWeaponSystem::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeaponSystem::Release()
{
  //Release Flash System
  if (m_pFlash != NULL) 
  { 
    ReleaseClass(m_pFlash);
	DeleteClass(m_pFlash);
  }

  //Release Ricochet Sound
  if (m_pSndRicochet != NULL) 
  { 
    m_pSndRicochet->ReleaseSound(); 
	m_pSndRicochet = NULL; 
  }
  
  //Release Crosshair	 
  if (m_pCrosshair != NULL) 
  {
    ReleaseClass(m_pCrosshair);
	DeleteClass(m_pCrosshair);
  }

  //Release Ballistic System 
  for (int I = 0; I < Num_Ballistic; ++I)
  {
    if (m_pBallistic[I] != NULL) 
	{ 
	  ReleaseClass(m_pBallistic[I]);
	  DeleteClass(m_pBallistic[I]);
    }
  } //End For

  //Release Shell System 
  for (int I = 0; I < Num_Shells; ++I)
  {
    if (m_pShell[I] != NULL) 
	{ 
	  ReleaseClass(m_pShell[I]);
	  DeleteClass(m_pShell[I]);
    }
  } //End For

  //Release Beretta Weapon Model	 
  if (m_pBeretta != NULL) 
  {	  
    ReleaseClass(m_pBeretta);
	DeleteClass(m_pBeretta);
  }

  //Release Beretta Light Shader 	 
  if (m_pBerettaShader != NULL) DeleteClass(m_pBerettaShader);
  
  //Release Delta Timer
  if (m_pDeltaTimer != NULL) DeleteClass(m_pDeltaTimer); 
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponSystem::UpdateBallistic()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponSystem::UpdateBallistic(D3DXVECTOR3 Pos,
									  D3DXVECTOR3 Normal,
									  D3DXVECTOR3 Vec1,
									  D3DXVECTOR3 Vec2)
{
  m_pBallistic[Num_Ballistic] = new CGRWeaponBallistic(d_pDevice);
  m_pBallistic[Num_Ballistic]->SetCoord(Pos,Normal,Vec1,Vec2);
  Num_Ballistic++;
} //EndUpdateBallisticProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponSystem::UpdateShellTube()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponSystem::UpdateShellTube(NxScene* pScene,
									  NxActor* pActor,
									  NxActor* pEnvironment,
									  D3DXMATRIX MatView)
{
  //Create Weapon Tube
  m_pShell[Num_Shells] = new CGRShell(d_pDevice,pScene,MatView, 
  L"\Models\\Weapons\\Beretta\\Shell\\Shell.x",
  "\Models\\Weapons\\Beretta\\Shell\\");

  //Ignore Contact Between Player Cube And Gun Tube
  pScene->setActorPairFlags(*pActor,*m_pShell[Num_Shells]->GetActor(),NX_IGNORE_PAIR);
  //pScene->setActorPairFlags(*m_pShell[Num_Shells]->GetActor(),*m_pShell[Num_Shells]->GetActor(),NX_IGNORE_PAIR);
  
  //Set Contact Respond Between Gun Tube And Level Geometry
  pScene->setActorPairFlags(*pEnvironment,*m_pShell[Num_Shells]->GetActor(),NX_NOTIFY_ON_TOUCH);

  Num_Shells++;
} //EndUpdateShellTubeProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponSystem::UpdateFlash()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponSystem::UpdateFlash(D3DXVECTOR3 CamPos,D3DXMATRIX MatView)
{
  //Update Flash Animation
  if (m_pFlash != NULL)	m_pFlash->Update();
} //EndUpdateFlashProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponSystem::PlaybackRicochet()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponSystem::PlaybackRicochet()
{
  //Play Ricochet Sound
  if (m_pSndRicochet != NULL) m_pSndRicochet->PlaybackReset();
} //EndPlaybackRicochetProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponSystem::Render()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponSystem::Render()
{
  //Update Delta Timer Every Frame
  if (m_pDeltaTimer != NULL) m_pDeltaTimer->UpdateTimer();

  //Render Weapon Ballistic 
  for (int I = 0; I < Num_Ballistic; ++I)
  {
    if (m_pBallistic[I] != NULL) 
	{
	  m_pBallistic[I]->SetUpdateTime(m_pDeltaTimer->GetTime()); 
	  m_pBallistic[I]->Render();
	}
  } //End For Ballistic
  
  //Render Weapon Shells
  for (int I = 0; I < Num_Shells; ++I)
  {
    if (m_pShell[I] != NULL) 
	{ 
	  m_pShell[I]->SetUpdateTime(m_pDeltaTimer->GetTime()); 
	  m_pShell[I]->Render();
    }
  } //End For Shells

  //Render Weapon Flash
  if (m_pFlash != NULL) 
  { 
	m_pFlash->SetUpdateTime(m_pDeltaTimer->GetTime()); 
	m_pFlash->Render(m_pCamView);
  }

  //Render Weapon Crosshair
  if (m_pCrosshair != NULL) m_pCrosshair->Render(m_pCamProjection);
} //EndRenderProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponSystem::RenderLighting()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponSystem::RenderLighting()
{
  //Render Beretta Model Weapon
  if (m_pBerettaShader != NULL) m_pBerettaShader->RenderWeapon(0);

  //Render Weapon Shells Lighting
  for (int I = 0; I < Num_Shells; ++I)
  {
    if (m_pShell[I] != NULL) m_pShell[I]->RenderLighting();
  }
} //EndRenderLightingProcedure

//-----------------------------------------------------------------------------