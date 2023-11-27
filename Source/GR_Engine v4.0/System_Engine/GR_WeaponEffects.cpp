/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_WeaponEffects.cpp
// It's The GreenRay Weapon Engine.
// Made Specially For Implementing Weapon Effects.
// Date Creation: 29 August 2005
// Last Changing: 08 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_WeaponEffects.h>

////////////////////////////////////////////////////////////////////////////////
// External Definitions And Prototypes 
////////////////////////////////////////////////////////////////////////////////
D3DVERTEXELEMENT9 m_pShellVertDecl[] =
{
  {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},	  
  D3DDECL_END()
};

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRWeaponBallistic Class
///////////////////////////////////////////////////////////////////////////////
CGRWeaponBallistic::CGRWeaponBallistic(LPDIRECT3DDEVICE9 pDevice)
{
  //Set Default Parameters
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pDecal = NULL;     //Bullet Hole Decal
  m_pWallSmoke = NULL; //Wall Smoke Particle
  m_pRock = NULL;      //Wall Rock Particle
  m_pSpark = NULL;     //Wall Spark Particle
  m_pScrap = NULL;     //Wall Scrap Particle

  //Create Bullet Hole Decal
  m_pDecal = new CGRDecal(d_pDevice,"\Textures\\Decals\\BulletHoles\\BerBHole_01.dds",0.7f,0.7f);
  
  //Create Wall Smoke Particle
  m_pWallSmoke = new CGRParticle(d_pDevice);
  m_pWallSmoke->SetTexture("\Textures\\Particles\\Smoke\\Smoke_06.png");
  m_pWallSmoke->SetMaxParticles(8);
  m_pWallSmoke->SetNumToRelease(1);
  m_pWallSmoke->SetReleaseInterval(0.019f);
  m_pWallSmoke->SetLifeCycle(0.6f);
  m_pWallSmoke->SetSize(5.5f);
  m_pWallSmoke->SetColor(D3DXCOLOR(0.09f,0.09f,0.09f,1.0f));
  m_pWallSmoke->SetVelocity(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
  m_pWallSmoke->SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pWallSmoke->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pWallSmoke->SetVelocityVar(3.5f);
  m_pWallSmoke->Init();
///////////////////////////////////////////////////////////////////////////////

  //Create Wall Particles
  m_pRock = new CGRParticle(d_pDevice);
  m_pRock->SetTexture("\Textures\\Particles\\Rocks\\Rock_01.png");
  m_pRock->SetMaxParticles(30);
  m_pRock->SetNumToRelease(6);
  m_pRock->SetReleaseInterval(0.05f);
  m_pRock->SetLifeCycle(2.6f);
  m_pRock->SetSize(0.38f);
  m_pRock->SetColor(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));
  m_pRock->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pRock->SetGravity(D3DXVECTOR3(0.0f, -85.0f, 0.0f));
  m_pRock->SetWind(D3DXVECTOR3(0.0f,0.0f,0.0f));
  m_pRock->SetVelocityVar(2.8f);
  m_pRock->SetCollisionPlane(D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,CR_STICK);
  m_pRock->Init();
///////////////////////////////////////////////////////////////////////////////

  //Create Spark Particle
  m_pSpark = new CGRParticle(d_pDevice);
  m_pSpark->SetTexture("\Textures\\Particles\\Sparks\\Spark_01.png");
  m_pSpark->SetMaxParticles(5);
  m_pSpark->SetNumToRelease(1);
  m_pSpark->SetReleaseInterval(0.01f);
  m_pSpark->SetLifeCycle(0.4f);
  m_pSpark->SetSize(1.3f);
  m_pSpark->SetColor(D3DCOLOR_XRGB(255,163,94));
  m_pSpark->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pSpark->SetGravity(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
  m_pSpark->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pSpark->SetVelocityVar(50.0f);
  m_pSpark->SetCollisionPlane(D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,CR_BOUNCE);
  m_pSpark->Init();
///////////////////////////////////////////////////////////////////////////////

  //Create Scrap Particle
  m_pScrap = new CGRParticle(d_pDevice);
  m_pScrap->SetTexture("\Textures\\Particles\\Sparks\\Spark_02Rot.png");
  m_pScrap->SetMaxParticles(5);
  m_pScrap->SetNumToRelease(5);
  m_pScrap->SetReleaseInterval(0.01f);
  m_pScrap->SetLifeCycle(0.4f);
  m_pScrap->SetSize(0.6f);
  m_pScrap->SetColor(D3DCOLOR_XRGB(255,163,100));
  m_pScrap->SetVelocity(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
  m_pScrap->SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pScrap->SetWind(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
  m_pScrap->SetVelocityVar(10.0f);
  //m_pScrap->SetCollisionPlane(D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),1.0f,CR_BOUNCE);
  m_pScrap->Init();

} //EndConstructionDirectives

CGRWeaponBallistic::~CGRWeaponBallistic()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRWeaponBallistic::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeaponBallistic::Release()
{
  //Release Bullet Hole Decal
  if (m_pDecal != NULL) 
  { 
    ReleaseClass(m_pDecal);
	DeleteClass(m_pDecal);
  }

  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponBallistic::SetCoord()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponBallistic::SetCoord(D3DXVECTOR3 Pos,
							      D3DXVECTOR3 Normal,
							      D3DXVECTOR3 Vec1,
							      D3DXVECTOR3 Vec2)
{
  //Set Decal Position
  if (m_pDecal != NULL) m_pDecal->SetCoord(Pos,Normal,Vec1,Vec2);

  //Set Smoke Particle Position
  if (m_pWallSmoke != NULL) m_pWallSmoke->SetPosition(D3DXVECTOR3(Pos.x,Pos.y+2.4f,Pos.z));
  if (m_pRock != NULL) m_pRock->SetPosition(D3DXVECTOR3(Pos.x,Pos.y,Pos.z));
  if (m_pSpark != NULL) m_pSpark->SetPosition(D3DXVECTOR3(Pos.x,Pos.y,Pos.z));
  if (m_pScrap != NULL) m_pScrap->SetPosition(D3DXVECTOR3(Pos.x,Pos.y,Pos.z)); 
} //EndSetCoordProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponBallistic::RenderParticles()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponBallistic::RenderParticles()
{
  //Update Wall Smoke Particles
  if (m_pWallSmoke != NULL)
  {
    m_pWallSmoke->Update(m_fUpdateTime);
 
    if (m_pWallSmoke->Is_Updated) 
	{
      m_pWallSmoke->Release(); 
	  ReleaseClass(m_pWallSmoke); 
	  DeleteClass(m_pWallSmoke);
	  m_pWallSmoke = NULL;
	}
  } //End Update Smoke Particle

  //Draw Some Smoke Particles
  if (m_pWallSmoke != NULL)
  {
    //Set Render Matrices
    D3DXMATRIX Mat_Part;
    D3DXMatrixIdentity(&Mat_Part);
    d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Part);

    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	
	// Render Particle System
    d_pDevice->SetTexture(0,m_pWallSmoke->GetTexture());
	m_pWallSmoke->Render();

    //Reset Render States...
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawWallSmokeParticles
///////////////////////////////////////////////////////////////////////////////

  //Update Wall Rock Particle System
  if (m_pRock != NULL)
  {
    m_pRock->Update(m_fUpdateTime);

    if (m_pRock->Is_Updated) 
	{
      m_pRock->Release(); 
	  ReleaseClass(m_pRock); 
	  DeleteClass(m_pRock);
      m_pRock = NULL;
	}
  } //End Update Rock Particles

  //Draw Some Wall Rock Particles
  if (m_pRock != NULL)
  {
    //Set Render Matrices
    D3DXMATRIX Mat_Part;
    D3DXMatrixIdentity(&Mat_Part);
    d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Part);

    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//Render Particle System
    d_pDevice->SetTexture(0,m_pRock->GetTexture());
	m_pRock->Render();

    //Reset Render States...
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawWallRockParticles
///////////////////////////////////////////////////////////////////////////////

  //Update Wall Spark Particle System
  if (m_pSpark != NULL)
  {
    m_pSpark->Update(m_fUpdateTime);

    if (m_pSpark->Is_Updated) 
	{
      m_pSpark->Release(); 
	  ReleaseClass(m_pSpark); 
	  DeleteClass(m_pSpark);
      m_pSpark = NULL;
	}
  } //End Update Spark Particles

  //Draw Some Wall Spark Particles
  if (m_pSpark != NULL)
  {
    //Set Render Matrices
    D3DXMATRIX Mat_Part;
    D3DXMatrixIdentity(&Mat_Part);
    d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Part);

    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//Render Particle System
    d_pDevice->SetTexture(0,m_pSpark->GetTexture());
	m_pSpark->Render();

    //Reset Render States...
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawWallSparkParticles
///////////////////////////////////////////////////////////////////////////////

  //Update Wall Scrap Particle System
  if (m_pScrap != NULL)
  {
    m_pScrap->Update(m_fUpdateTime);

    if (m_pScrap->Is_Updated) 
	{
      m_pScrap->Release(); 
	  ReleaseClass(m_pScrap); 
	  DeleteClass(m_pScrap);
      m_pScrap = NULL;
	}
  } //End Update Scrap Particles

  //Draw Some Wall Scrap Particles
  if (m_pScrap != NULL)
  {
    //Set Render Matrices
    D3DXMATRIX Mat_Part;
    D3DXMatrixIdentity(&Mat_Part);
    d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Part);

    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//Render Particle System
    d_pDevice->SetTexture(0,m_pScrap->GetTexture());
	m_pScrap->Render();

    //Reset Render States...
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawWallScrapParticles
} //EndRenderParticlesProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponBallistic::Render()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponBallistic::Render()
{
  //Render Bullet Hole Decal
  if (m_pDecal != NULL) m_pDecal->Render();

  //Render Particle Effects
  RenderParticles();
} //EndRenderProcedure

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRShell Class
//////////////////////////////////////////////////////////////////////////////
CGRShell::CGRShell(LPDIRECT3DDEVICE9 pDevice,
				   NxScene* pScene,
				   D3DXMATRIX MatView,
				   LPCWSTR ShellName,
				   LPCSTR ShellTexture)
{
  //Set Default Parameters
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  d_pScene = pScene;   //Apropritate Scene Pointer To Class
  m_pShell = NULL;
  m_pShellShader = NULL;
  m_pPhysXShell = NULL;
  m_pShellSmoke = NULL;

  ShellRadius = 0.095f; //Set Shell Radius
  ShellHeight = 0.36f;  //Set Shell Height
  Dropped = false;
  Rotation = 0.0f;

  //Create The Shell Cylinder Mesh
  //Create Shell Model
  m_pShell = new CGRMeshX(); //Construct Shell Model Class
  //if (m_pTexPath != NULL)
  m_pShell->SetTexturePath(ShellTexture); //Set Model Texture Path

  //Create Vertex Declaration
  d_pDevice->CreateVertexDeclaration(m_pShellVertDecl,&pVertDecl);
 
  //Load An Object
  if (FAILED(m_pShell->Create(d_pDevice,ShellName,100)))
  {
    Log("GR_WeaponEffects.cpp: Error Load Shell Model.. \n");
	return;
  }

  //Set A Reasonable Vertex Type
  m_pShell->SetVertexDecl(m_pShellVertDecl);
    
  //Initialize The Vertex Buffers For The File-Based Objects
  m_pShell->RestoreObjects();
///////////////////////////////////////////////////////////////////////////////

  UINT MatNum;
  D3DXVECTOR4 MatVec(0,0,0,0);
  D3DXVECTOR4 DpfVec(0,0,0,0);

  //Attach Light Shader To Model 
  m_pShellShader = new CGRMeshXShader();
  m_pShellShader->Create(d_pDevice,m_pShell,true);

  MatNum = RegisterMaterial(Material(0.8f, 0.1f, 0.0f)); //Used An Already Created Material
  MatVec = D3DXVECTOR4(0.8f,1.2f, MaterialIndexToV(MatNum),0);
  m_pShellShader->m_pShader->SetVector("f3Material",&MatVec);
  DpfVec = D3DXVECTOR4(GetDotProductFunctionU(DPF_FRONT_POW2),GetDotProductFunctionU(DPF_FRONT_POW32),0,0);
  m_pShellShader->m_pShader->SetVector("f2DotProductFuncs",&DpfVec);
///////////////////////////////////////////////////////////////////////////////

  //Create PhysX Object
  //Define Tube Physics Object
  NxBodyDesc CapsuleBodyDesc;
  CapsuleBodyDesc.angularDamping = 0.1f;
  CapsuleBodyDesc.linearVelocity = NxVec3(0.0f, 0.0f, 0.0f);
  
  NxCapsuleShapeDesc CapsuleDesc;
  CapsuleDesc.height = ShellHeight+0.3f;
  CapsuleDesc.radius = ShellRadius+0.3f;
  //CapsuleDesc.group = 2;
  CapsuleDesc.skinWidth = 0.01f;

  NxActorDesc CapsuleActorDesc;
  CapsuleActorDesc.shapes.pushBack(&CapsuleDesc);
  CapsuleActorDesc.body = &CapsuleBodyDesc;
  CapsuleActorDesc.density = 30.0f;
  CapsuleActorDesc.name = "ShellTube";

  //NxVec3 CylPos = m_pPhysXPlayer->getGlobalPosition();
  //CapsuleActorDesc.globalPose.t = NxVec3(CylPos.x,CylPos.y,CylPos.z+14.0f);
  
  //Add New Tube Object To Physics Scene
  m_pPhysXShell = d_pScene->createActor(CapsuleActorDesc);
  //SetActorGroup(m_pPhysXShell,groupB);
  


  //Set Startup Matrices And Tube Position
  D3DXMATRIXA16 mTransform;
  D3DXMATRIXA16 mInvView,mWorld;

  //Inverse Matrix Here
  D3DXMatrixIdentity(&mInvView);
  D3DXMatrixIdentity(&mWorld);
  D3DXMatrixInverse(&mInvView,NULL,&MatView);

  //Set Model Postition On Screen Space
  D3DXVECTOR4 Pos(0.47f, -0.15f, 2.5f, 1.0f);
  D3DXVec4Transform(&Pos,&Pos,&mInvView);
  
  mWorld._41 = Pos.x;
  mWorld._42 = Pos.y;
  mWorld._43 = Pos.z;
  //mWorld._44 = 1.5f;

  //D3DXMatrixTranslation(&mWorld,Pos.x,Pos.y,Pos.z);

  //Multiply Matrices
  mWorld*= mInvView*MatView; 

  D3DXMatrixIdentity(&mTransform);
  D3DXMatrixInverse(&mTransform,NULL,&MatView);

  mTransform._41 = mWorld._41;
  mTransform._42 = mWorld._42;
  mTransform._43 = mWorld._43;
 
  //Set Transform And Draw Model Here
  //d_pDevice->SetTransform(D3DTS_WORLD,&mTransform);
  //m_pTube->DrawSubset(0); //Draw Model

  //Set Matrices To PhysX
  NxMat34 Matr;
  Matr.setColumnMajor44(mTransform);
  m_pPhysXShell->setGlobalPose(Matr);

  //Add Force To Shell
  m_pPhysXShell->addForce(NxVec3(500.0f, 13000.0f, 0.0f));

  //Add Rotation Moment To Shell
  m_pPhysXShell->addTorque(NxVec3(1000.0f, 15000.0f, 65000.0f));

  Rotation = 1.57f;

  //Set All Members Of The World Matrix Back To Zero
  D3DXMatrixIdentity(&mInvView);
  d_pDevice->SetTransform(D3DTS_WORLD,&mInvView);
  D3DXMatrixIdentity(&mWorld);
  d_pDevice->SetTransform(D3DTS_WORLD,&mWorld);
  D3DXMatrixIdentity(&mTransform);
  d_pDevice->SetTransform(D3DTS_WORLD,&mTransform);
///////////////////////////////////////////////////////////////////////////////

  //Create Shell Smoke Particle System
  m_pShellSmoke = new CGRParticle(d_pDevice);
  m_pShellSmoke->SetTexture("\Textures\\Particles\\Smoke\\Smoke_07.png");
  m_pShellSmoke->SetMaxParticles(60);
  m_pShellSmoke->SetNumToRelease(1);
  m_pShellSmoke->SetReleaseInterval(0.39f);
  m_pShellSmoke->SetLifeCycle(4.5f);
  m_pShellSmoke->SetSize(2.8f);
  m_pShellSmoke->SetColor(D3DXCOLOR(0.04f,0.04f,0.04f,1.0f));
  m_pShellSmoke->SetVelocity(D3DXVECTOR3(0.0f, 2.6f, 0.0f));
  m_pShellSmoke->SetGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pShellSmoke->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pShellSmoke->SetVelocityVar(0.4f);
  m_pShellSmoke->Init();

} //EndConstructionDirectives

CGRShell::~CGRShell()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRShell::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRShell::Release()
{
  //Release Shell Object
  if (m_pShell != NULL) m_pShell = NULL; 

  d_pScene = NULL;  //NULL PhysX Scene Pointer
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRShell::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CGRShell::Render()
{
  if (m_pShell != NULL) 
  {
	//Set The World Matrix
	//D3DXMatrixIdentity(&mPos);
	//d_pDevice->SetTransform(D3DTS_WORLD,&mPos);

	//Cylinders Created With The D3DXCreateCylinder() Object Are Created Sideways,
	//So We Must Offset The Rotation By 45 Degrees To Match The PhysX Matrix
	
	//Retrieve Capsule Transformation, Offset It, Apply It, And Render The Cylinder
	D3DXMatrixIdentity(&mOffset);
	//D3DXMatrixRotationX(&mOffset,1.57f);
	D3DXMatrixRotationX(&mOffset,Rotation);
	if (m_pPhysXShell != NULL) m_pPhysXShell->getGlobalPose().getColumnMajor44(mPos);

	mOffset*= mPos;

  }
///////////////////////////////////////////////////////////////////////////////

  //Update Shell Smoke Particle
  if (m_pShellSmoke != NULL)
  {
    m_pShellSmoke->Update(m_fUpdateTime);
	m_pShellSmoke->SetPosition(D3DXVECTOR3(mOffset._41+0.24,mOffset._42+0.2f,mOffset._43));
    
	if (m_pShellSmoke->Is_Updated) 
	{
      m_pShellSmoke->Release(); 
	  ReleaseClass(m_pShellSmoke); 
	  DeleteClass(m_pShellSmoke);
	  m_pShellSmoke = NULL;

	  //Release Shell PhysX Animation Actors By Time:
      if (Dropped == true)
	  {
	    ReleaseActor(); //Release Actor
	  }
	  else
	  {
	    //Release PhysX Actor And Shell Mesh
		ReleaseActor(); //Release Actor
        if (m_pShell != NULL && m_pShellShader != NULL) 
		{ 
		  DeleteClass(m_pShellShader); 
	      m_pShellShader = NULL; 
		  DeleteClass(m_pShell); 
		  m_pShell = NULL; 
		} //Release Shell Object
	  }
	}
  } //End Update Shell Smoke Particle

  //Draw Some Shell Smoke Particles
  if (m_pShellSmoke != NULL)
  {
    //Set Render Matrices
    D3DXMATRIX Mat_Part;
    D3DXMatrixIdentity(&Mat_Part);
    d_pDevice->SetTransform(D3DTS_WORLD,&Mat_Part);

    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	//d_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    //d_pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    //d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	
	//Render Particle System
    d_pDevice->SetTexture(0,m_pShellSmoke->GetTexture());
	m_pShellSmoke->Render();

    //Reset Render States...
    //d_pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawParticles
} //EndRenderProcedure

//-----------------------------------------------------------------------------
// Name: CGRShell::RenderLighting()
// Desc: 
//-----------------------------------------------------------------------------
void CGRShell::RenderLighting()
{
  //Transform And Draw Shell Model Here:
  //d_pDevice->SetTransform(D3DTS_WORLD,&Offset);
  if (m_pShellShader != NULL) m_pShellShader->RenderShell(mOffset);
} //EndRenderLightingProcedure

//-----------------------------------------------------------------------------
// Name: CGRShell::ApplyForceToActor()
// Desc: 
//-----------------------------------------------------------------------------
NxVec3 CGRShell::ApplyForceToActor(NxActor* Actor,const NxVec3 &ForceDir,const NxReal ForceStrength)
{
  NxVec3 ForceVec = ForceStrength*ForceDir*m_fUpdateTime;
  Actor->addForce(ForceVec);
  return ForceVec;
} //EndApplyForceToActorFunction

//-----------------------------------------------------------------------------
// Name: CGRShell::GetActor()
// Desc: 
//-----------------------------------------------------------------------------
NxActor* CGRShell::GetActor()
{
  return m_pPhysXShell;
  //m_pPhysXShell->getName();
} //EndGetActorFunction

//-----------------------------------------------------------------------------
// Name: CGRShell::GetActorName()
// Desc: 
//-----------------------------------------------------------------------------
TCHAR* CGRShell::GetActorName()
{
  return (char*)m_pPhysXShell->getName();
} //EndGetActorNameFunction

//-----------------------------------------------------------------------------
// Name: CGRShell::ReleaseActor()
// Desc: 
//-----------------------------------------------------------------------------
void CGRShell::ReleaseActor()
{
  if (m_pPhysXShell != NULL)
  {
    //Set Last Object Position
	m_pPhysXShell->getGlobalPose().getColumnMajor44(mPos);

	//Release PhysX Actor
	//m_pPhysXShell->releaseShape;
    m_pPhysXShell = NULL;
	  
	//Stop And Release Timer
	//m_pTimer->Stop();
	//DeleteClass(m_pTimer);
    //m_pTimer = NULL;
  } 
} //EndReleaseActorProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRWeaponFlash Class
///////////////////////////////////////////////////////////////////////////////
CGRWeaponFlash::CGRWeaponFlash(LPDIRECT3DDEVICE9 pDevice)
{
  //Set Default Parameters
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pFlash = NULL; //Fire Particle
  m_pSmoke = NULL; //Smoke Particle
  NumShoots = 0; //Set Number Of Shoots To Null
  
} //EndConstructionDirectives

CGRWeaponFlash::~CGRWeaponFlash()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRWeaponFlash::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeaponFlash::Release()
{
  //Release Smoke If Exists
  if (m_pSmoke != NULL) 
  {
    ReleaseClass(m_pSmoke); 
	DeleteClass(m_pSmoke); 
  }

  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponFlash::Update()
// Desc: 
//-----------------------------------------------------------------------------
void CGRWeaponFlash::Update()
{
  //Before Release Flash If Exists
  if (m_pFlash != NULL) 
  {
    ReleaseClass(m_pFlash); 
	DeleteClass(m_pFlash); 
  }

  //Release Smoke If Exists
  if (m_pSmoke != NULL) 
  {
    ReleaseClass(m_pSmoke); 
	DeleteClass(m_pSmoke); 
  }

  //Create Flash Particle System
  m_pFlash = new CGRParticle(d_pDevice);
  m_pFlash->SetTexture("\Textures\\Particles\\Sparks\\Spark_01.png");
  m_pFlash->SetMaxParticles(40);
  m_pFlash->SetNumToRelease(1);
  m_pFlash->SetReleaseInterval(0.019f);
  m_pFlash->SetLifeCycle(0.11f);
  m_pFlash->SetSize(3.6f);
  m_pFlash->SetColor(D3DCOLOR_XRGB(255,163,94));
  m_pFlash->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 30.0f));
  m_pFlash->SetGravity(D3DXVECTOR3(0.0f, 0.0f, 30.0f));
  m_pFlash->SetWind(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
  m_pFlash->SetVelocityVar(8.5f);
  m_pFlash->Init();
///////////////////////////////////////////////////////////////////////////////

  //Create Smoke Particle System
  m_pSmoke = new CGRParticle(d_pDevice);
  m_pSmoke->SetTexture("\Textures\\Particles\\Smoke\\Smoke_01.png");
  m_pSmoke->SetMaxParticles(15);
  m_pSmoke->SetNumToRelease(3);
  m_pSmoke->SetReleaseInterval(0.019f);
  m_pSmoke->SetLifeCycle(0.6f);
  m_pSmoke->SetSize(4.5f);
  m_pSmoke->SetColor(D3DXCOLOR(0.03f,0.03f,0.03f,1.0f));
  m_pSmoke->SetVelocity(D3DXVECTOR3(0.0f, 0.0f, 3.0f));
  m_pSmoke->SetGravity(D3DXVECTOR3(0.0f, 0.0f, 3.0f));
  m_pSmoke->SetWind(D3DXVECTOR3(0.0f, 12.0f, 0.0f));
  m_pSmoke->SetVelocityVar(0.9f);
  m_pSmoke->Init();


} //EndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponFlash::RenderParticles()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponFlash::RenderParticles()
{
  //Update Weapon Flash Particles
  if (m_pFlash != NULL)
  {
    m_pFlash->Update(m_fUpdateTime);
 
    if (m_pFlash->Is_Updated) 
	{
      m_pFlash->Release(); 
	  ReleaseClass(m_pFlash); 
	  DeleteClass(m_pFlash);
	  m_pFlash = NULL;
	}
  } //End Update Flash Particles

  //Draw Some Flash Particles
  if (m_pFlash != NULL)
  {
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	
	// Render Particle System
    d_pDevice->SetTexture(0,m_pFlash->GetTexture());
	m_pFlash->Render();

    // Reset Render States...
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //EndDrawFlashParticles
///////////////////////////////////////////////////////////////////////////////

  //Update Smoke Particle
  if (m_pSmoke != NULL)
  {
    m_pSmoke->Update(m_fUpdateTime);
    m_pSmoke->SetPosition(D3DXVECTOR3(-0.35f,0.85f,1.0f));

    if (m_pSmoke->Is_Updated) 
	{
      m_pSmoke->Release(); 
	  ReleaseClass(m_pSmoke); 
	  DeleteClass(m_pSmoke);
	  m_pSmoke = NULL;
	}
  } //End Update Smoke Particle

  //Draw Some Smoke Particles
  if (m_pSmoke != NULL)
  {
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
 	d_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    d_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	
	// Render Particle System
    d_pDevice->SetTexture(0,m_pSmoke->GetTexture());
	m_pSmoke->Render();

    // Reset Render States...
    d_pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    d_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
  } //End Draw Smoke Particles
} //EndRenderParticlesProcedure

//-----------------------------------------------------------------------------
// Name: CGRWeaponFlash::Render()
// Desc:
//-----------------------------------------------------------------------------
void CGRWeaponFlash::Render(D3DXMATRIX MatView)
{
  //Set Startup Matrices And Flash Position
  D3DXMATRIXA16 mTransform;
  D3DXMATRIXA16 mInvView,mWorld;

  //Inverse Matrix Here
  D3DXMatrixIdentity(&mInvView);
  D3DXMatrixIdentity(&mWorld);
  D3DXMatrixInverse(&mInvView,NULL,&MatView);

  //Set Model Postition On Screen Space
  D3DXVECTOR4 Pos(2.5f, -2.4f, 9.85f, 1.0f);
  D3DXVec4Transform(&Pos,&Pos,&mInvView);
  
  mWorld._41 = Pos.x;
  mWorld._42 = Pos.y;
  mWorld._43 = Pos.z;
  //mWorld._44 = 1.5f;

  //Multiply Matrices
  mWorld*= mInvView*MatView; 

  D3DXMatrixIdentity(&mTransform);
  D3DXMatrixInverse(&mTransform,NULL,&MatView);

  mTransform._41 = mWorld._41;
  mTransform._42 = mWorld._42;
  mTransform._43 = mWorld._43;
 
  //Set Transform And Draw Flash Here
  d_pDevice->SetTransform(D3DTS_WORLD,&mTransform);

  //Draw Flash In ViewPort 
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

  //Render Particle Effects
  RenderParticles();

  //When Render Passed Then Release Viewports 
  ReleaseClass(pBackBuffer);
  d_pDevice->SetViewport(&OriginalVP);
} //EndRenderProcedure

//-----------------------------------------------------------------------------