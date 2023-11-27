/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_PhysXPlayer.cpp
// It's The GreenRay Physics Engine.
// Made Specially For Implementing PhysX Based Engine Player.
// Date Creation: 29 August 2005
// Last Changing: 01 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_PhysXPlayer.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRPhysXPlayer Class
///////////////////////////////////////////////////////////////////////////////
CGRPhysXPlayer::CGRPhysXPlayer(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene)
{
  //Set Sefault Parameters
  d_pDevice = pDevice;
  d_pScene = pScene;
  m_pPhysXPlayer = NULL;


} //EndConstructionDirectives

CGRPhysXPlayer::~CGRPhysXPlayer()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRPhysXPlayer::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysXPlayer::Release()
{
  m_pPhysXPlayer = NULL; //NULL PhysX Player
  d_pScene = NULL;       //NULL PhysX Scene Pointer
  d_pDevice = NULL;      //NULL Device Pointer
  PlayerPos = D3DXVECTOR3(0.0f, 50.0f, 10.0f);       
  PlayerSize = 2.0f;  
  PlayerDensity = 10.0f; 
  PlayerHeight = 1.8f;
  JumpSpeed = 20.0f;
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysXPlayer::CreatePlayer()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysXPlayer::CreatePlayer()
{
  //Create The Player Object
  //We Use A Box To Represent The Player
  //Define Physics Object Before
  NxBodyDesc BodyDesc;
  BodyDesc.angularDamping = 0.5f;
  BodyDesc.linearVelocity = NxVec3(0.0f, 0.0f, 0.0f);

  NxCapsuleShapeDesc CapsuleDesc;
  CapsuleDesc.height = PlayerHeight;
  CapsuleDesc.radius = PlayerSize;
        
  //NxBoxShapeDesc BoxDesc; 
  //BoxDesc.dimensions = NxVec3(PlayerSize,PlayerHeight,PlayerSize); //Player Collidable Box Size

  //Make Sure The Walls And Player All Use A Frictionless Material
  //BoxDesc.materialIndex = iMaterial;
  //BoxDesc.skinWidth = 0.5f;
  CapsuleDesc.materialIndex = iMaterial;
  CapsuleDesc.skinWidth = 0.5f; //0.0001f;

  //Set Parameters For PhysX Actor
  NxActorDesc ActorDesc;
  ActorDesc.shapes.pushBack(&CapsuleDesc);
  ActorDesc.body = &BodyDesc;
  ActorDesc.density = PlayerDensity;
  ActorDesc.globalPose.t = NxVec3(PlayerPos);

  //Add New Capsule Object To Physics Scene
  m_pPhysXPlayer = d_pScene->createActor(ActorDesc);
} //EndCreatePlayerProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysXPlayer::Update()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysXPlayer::Update(CGRCamera* &Camera)
{
  //Create Matrix To Represent Rotation And Set To Identity
  NxMat33 NewPlayerMat;
  NewPlayerMat.id();

  //Zero The Rotation Of The Player
  m_pPhysXPlayer->setGlobalOrientation(NewPlayerMat);

  //Set The X/Z Velocity And Let The Physics Handle The Y Velocity
  NxVec3 PlayerVel;
  PlayerVel = m_pPhysXPlayer->getLinearVelocity();
  PlayerVel.x = Camera->Velocity.x*50.0f;
  PlayerVel.z = Camera->Velocity.z*50.0f;
  m_pPhysXPlayer->setLinearVelocity(PlayerVel);

  //Set The Camera Position Based On The Player Physics Object
  Camera->Position = NxVec3ToDXVec3(m_pPhysXPlayer->getGlobalPosition());
  Camera->Target = Camera->Position+Camera->Look_Vector*20.0f;

  //Set The Eye Position To Be Even With The Top Of The Physics Object And Player Height
  Camera->Position.y+= PlayerHeight;
  Camera->Target.y+= PlayerHeight;

  D3DXMatrixLookAtLH(&Camera->View,&Camera->Position,&Camera->Target,&Camera->Up);

  //Update The Active View
  d_pDevice->SetTransform(D3DTS_VIEW,&Camera->View);
} //EndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysXPlayer::Jump()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysXPlayer::Jump()
{
  //Make The Player Jump
  m_pPhysXPlayer->setLinearVelocity(NxVec3(0.0f,JumpSpeed,0.0f));
} //EndJumpProcedure

//-----------------------------------------------------------------------------