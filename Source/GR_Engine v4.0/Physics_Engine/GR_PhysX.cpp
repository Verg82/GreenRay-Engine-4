/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_PhysX.cpp
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection.
// Date Creation: 29 August 2005
// Last Changing: 30 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_PhysX.h>

///////////////////////////////////////////////////////////////////////////////
//Actor Globals 
///////////////////////////////////////////////////////////////////////////////
const NxCollisionGroup GroupA = 1;
const NxU32 GroupFlagA = 1 << GroupA;
const NxCollisionGroup GroupB = 2;
const NxU32 GroupFlagB = 1 << GroupB;

///////////////////////////////////////////////////////////////////////////////
//Global Variables Or Constants 
///////////////////////////////////////////////////////////////////////////////
bool ObjColliding = false;               //Object Colliding State
CGRWave* m_pSndShellDown = NULL; //Wave Directive For Shell Down Sound

//////////////////////////////////////////////////////////////////////////////
// Contact Report Virtual Class
//////////////////////////////////////////////////////////////////////////////
class ContactReport: public NxUserContactReport
{
  public:
    virtual void onContactNotify(NxContactPair& pair,NxU32 events)
	{
	  //This Is Where The Code To Respond To Collision Goes.
	  //You May Want To Set The PhysX Actor's Variable 'Name' 
	  //Using pair.actors[0]->setName("SomeName") To A Material Name 
	  //Such As 'Wood' Or 'Metal'. Then In This Callback, Use
	  //pair.actors[0]->getName() To Retrieve The Name And Play
	  //A Corresponding Sound.
   
	  if (pair.actors[0]->getName() == "ShellTube")
	  {
        //Play Shell Down Sound
        if (m_pSndShellDown != NULL) m_pSndShellDown->PlaybackReset();

		pair.actors[0]->setName(""); //Null Pair Name
	  }
      
	  ObjColliding = true;
    }
} gContactReport;

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRPhysX Class
//////////////////////////////////////////////////////////////////////////////
CGRPhysX::CGRPhysX(LPDIRECT3DDEVICE9 pDevice,HWND WndHndl)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pPhysicsSDK = NULL; 
  m_pScene = NULL; 
  m_pStatGeometryActor = NULL;
  Player = NULL;
  RayCast = NULL;

  //Initialize PhysicsSDK
  m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,NULL,NULL);
  //m_pPhysicsSDK->setParameter(NX_CCD_EPSILON,0.01f);
  //m_pPhysicsSDK->setParameter(NX_CONTINUOUS_CD,1);
  //m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.025f);
  m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.001f);

  //Collide 
  m_pPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
  m_pPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
  m_pPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
  m_pPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_POINT, 1);
  m_pPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1); 

  //Create A PhysX Scene
  NxSceneDesc Scene_Desc;
  Scene_Desc.gravity = NxVec3(0.0f,-80.0f,0.0f);
  Scene_Desc.userContactReport = &gContactReport;

  m_pScene = m_pPhysicsSDK->createScene(Scene_Desc);

  //Set Default Material
  NxMaterialDesc Material;
  Material.restitution	   = 0.0f;
  Material.staticFriction  = 0.5f;
  Material.dynamicFriction = 0.5f;
  DefaultMaterial = 0;
  //We Redefine The Default Material In Slot 0
  m_pScene->getMaterialFromIndex(DefaultMaterial)->loadFromDesc(Material); 
	
  //Set Frictionless Material For Player And Walls
  Material.restitution	   = 0.0f;
  Material.staticFriction  = 0.0f;
  Material.dynamicFriction = 0.0f;
  FrictionlessMaterial = m_pScene->createMaterial(Material)->getMaterialIndex();
//////////////////////////////////////////////////////////////////////////////

  //Create PhysX Player
  Player = new CGRPhysXPlayer(d_pDevice,m_pScene);      //Construct CGRPhysXPlayer Class
  Player->SetMaterial(FrictionlessMaterial);            //Set Player Collidable Box Material
  Player->SetSize(2.5f);                                //Set Player Collidable Box Size
  Player->SetHeight(20.0f);                             //Set Player Height 
  Player->SetPosition(D3DXVECTOR3(0.0f, 50.0f, 10.0f)); //Set Player Position In Space
  Player->SetDensity(10.0f);                            //Set Player Density
  Player->SetJumpSpeed(40.0f);                          //Set Player Jump Speed
  Player->CreatePlayer();                               //All Parameters Done Then Create Player
//////////////////////////////////////////////////////////////////////////////

  //Create RayCast
  RayCast = new CGRRayCast(d_pDevice,m_pScene); //Construct CGRRayCast Class
  RayCast->SetPlayerHeight(Player->Height());   //Set Player Height To RayCast
//////////////////////////////////////////////////////////////////////////////

  //Create Shell Down Sound
  m_pSndShellDown = new CGRWave();
  m_pSndShellDown->CreateSoundInterface(WndHndl,"\Sounds\\Weapon\\Misc\\ShellDownOne.wav",2,44100,32);

} //EndConstructionDirectives

CGRPhysX::~CGRPhysX()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRPhysX::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::Release()
{
  //Release PhysX Player
  if (Player != NULL) 
  {
	ReleaseClass(Player); 
	DeleteClass(Player); 
  }

  //Release PhysX RayCast
  if (RayCast != NULL)
  {
	ReleaseClass(RayCast); 
	DeleteClass(RayCast); 
  }

  //Release Shell Down Sound
  if (m_pSndShellDown != NULL)
  {
	m_pSndShellDown->ReleaseSound(); 
	m_pSndShellDown = NULL; 
  }

  //Destroy The Physics
  if (m_pPhysicsSDK != NULL)
  {
    if (m_pScene != NULL) m_pPhysicsSDK->releaseScene(*m_pScene);
    m_pPhysicsSDK->release();
  }
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::BeginUpdate()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::BeginUpdate()
{
  //Update The Physics Before Rendering
  m_pScene->simulate(1.0f/60.0f);
} //EndBeginUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::EndUpdate()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::EndUpdate()
{
  //Reset To False
  ObjColliding = false;

  //Fetch Simulation Results
  m_pScene->flushStream();
  m_pScene->fetchResults(NX_RIGID_BODY_FINISHED,true);
} //EndEndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::UpdatePlayer()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::UpdatePlayer(CGRCamera* &Camera)
{
  if (Player != NULL) Player->Update(Camera);
} //EndUpdatePlayerProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::JumpPlayer()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::JumpPlayer(bool JumpUp)
{
  if (Player != NULL && JumpUp) Player->Jump();
} //EndJumpPlayerProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::GenerateTrianglesFromDXMesh()
// Desc: Generate A Convex Physics Object From A DX Mesh
//-----------------------------------------------------------------------------
void CGRPhysX::GenerateTrianglesFromDXMesh(ID3DXMesh* &m_pMesh,
NxActor* &PhysXActor,NxVec3 &Mesh_Pos)
{
  //Used To Retrieve Flexible Vertex Format Information From Mesh File
  typedef struct 
  {
    D3DXVECTOR3 VertexPos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexCoord;
  } MESH_FVF;

  //Used To Copy Indices
  typedef struct 
  {
    short IBNumber[3];
  } IndexBufferStruct;

  int Num_Vertices = m_pMesh->GetNumVertices();
  int Num_Triangles = m_pMesh->GetNumFaces();

  DWORD FVF_Size = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

  //Create Pointer For Vertices
  Vertices = new NxVec3[Num_Vertices];

  //Get Vertex Buffer Of Mesh
  char* DXMeshPtr;
  m_pMesh->LockVertexBuffer(D3DLOCK_READONLY,(void**)&DXMeshPtr);
  for (int I = 0; I < Num_Vertices; I++)
  {
    MESH_FVF* DXMeshFVF = (MESH_FVF*)DXMeshPtr;
	Vertices[I] = NxVec3(DXMeshFVF->VertexPos.x,DXMeshFVF->VertexPos.y,DXMeshFVF->VertexPos.z);
	DXMeshPtr+= FVF_Size;
  }
  m_pMesh->UnlockVertexBuffer();

  //Get Index Buffer Of Mesh
  DWORD* Ind_Buff = 0;
  Triangles = 0;
  Triangles = new DWORD[Num_Triangles*3]; 
  m_pMesh->LockIndexBuffer(D3DLOCK_READONLY,(void**)&Ind_Buff); 
  memcpy(Triangles,Ind_Buff,sizeof(DWORD)*Num_Triangles*3); 
  m_pMesh->UnlockIndexBuffer();

  //Create Pointer For Indices
  /*IndexBufferStruct* Triangles = new IndexBufferStruct[Num_Triangles];

  //Get Index Buffer Of Mesh
  IndexBufferStruct* DXMeshIBPtr;
  m_pMesh->LockIndexBuffer(D3DLOCK_READONLY,(void**)&DXMeshIBPtr);
  for (int I = 0; I < Num_Triangles; I++)
  {
	Triangles[I].IBNumber[0] = DXMeshIBPtr[I].IBNumber[0];
	Triangles[I].IBNumber[1] = DXMeshIBPtr[I].IBNumber[1];
	Triangles[I].IBNumber[2] = DXMeshIBPtr[I].IBNumber[2];
  }
  m_pMesh->UnlockIndexBuffer();*/

  //Material Description For This Mesh
  NxMaterialDesc X_Material;

  //Frictionless Material For Player And Walls
  X_Material.restitution	 = 0.0f;
  X_Material.staticFriction	 = 0.0f;
  X_Material.dynamicFriction = 0.0f;
  int FrictionlessMaterial = m_pScene->createMaterial(X_Material)->getMaterialIndex();

  //Create Descriptor For Mesh
  bool BigMesh = false;
  if (Num_Triangles >= 5000) BigMesh = true;
  NxTriangleMeshDesc Mesh_Desc;
  Mesh_Desc.numVertices		    = Num_Vertices;
  Mesh_Desc.numTriangles		= Num_Triangles;
  Mesh_Desc.pointStrideBytes	= sizeof(NxVec3);
  if (BigMesh) Mesh_Desc.triangleStrideBytes = 3*sizeof(NxU32); //3*sizeof(NxU32); //Use This For Big Terrain 
  else Mesh_Desc.triangleStrideBytes = 3*sizeof(NxU16); //3*sizeof(NxU32); //Use This For Big Terrain
  Mesh_Desc.points			    = Vertices;
  Mesh_Desc.triangles		    = Triangles;
  if (!BigMesh) Mesh_Desc.flags	= NX_MF_16_BIT_INDICES; //Comment This If You Want To Use Big Terrain
  //Mesh_Desc.heightFieldVerticalAxis = NX_Y; 
  //Mesh_Desc.heightFieldVerticalExtent = -100.0f; 

  //Initialize Cooking
  bool Be_Cook = InitCooking();
  if (!Be_Cook) Log("GR_PhysX.cpp: Unable To Initialize The Cooking Library. \n");

  //Cooking From Memory
  MemoryWriteBuffer Mem_Buf;
  if (CookTriangleMesh(Mesh_Desc,Mem_Buf))
  { 
    //Mesh Shape Description
    NxTriangleMeshShapeDesc Mesh_ShapeDesc;
	Mesh_ShapeDesc.meshData = m_pPhysicsSDK->createTriangleMesh(MemoryReadBuffer(Mem_Buf.data));

	//For Load From File
    //bool status = NxCookTriangleMesh(TriMeshDesc,UserStream("c:\\tmp.bin",false)); 
    //Mesh_ShapeDesc.meshData = m_pPhysicsSDK->createTriangleMesh(UserStream("c:\\tmp.bin",true)); 

	//Mesh_ShapeDesc.shapeFlags = NX_SF_FEATURE_INDICES;
	Mesh_ShapeDesc.materialIndex = FrictionlessMaterial;
    //Mesh_ShapeDesc.group = 1;
  
    if (Mesh_ShapeDesc.meshData)
	{
	  //Create Body
	  NxBodyDesc Body_Desc;
	  Body_Desc.angularDamping = 0.0f;
	  Body_Desc.linearVelocity = NxVec3(0.0f,0.0f,0.0f);
  
      //Body Description And Parameters
	  NxActorDesc Actor_Desc;
	  Actor_Desc.shapes.pushBack(&Mesh_ShapeDesc);
	  Actor_Desc.body		  = &Body_Desc;
	  Actor_Desc.density	  = 10.0f;      //Mass Of Object
	  Actor_Desc.globalPose.t = Mesh_Pos;   //Position In Space

	  PhysXActor = m_pScene->createActor(Actor_Desc);  
	  PhysXActor->userData = (void*)1;
    }
  }

  CloseCooking(); //Close Cooking Param
  delete[] Vertices; //Free Vertices
  delete[] Triangles; //Free Indices
} //EndGenerateTrianglesFromDXMeshProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::TrianglesFromDXMesh()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::TrianglesFromDXMesh(ID3DXMesh* m_pMesh)
{
  //LPD3DXMESH _Mesh;
  //LoadXFile(d_pDevice,"\Models\\Locations\\Terrain\\Terrain.x",_Mesh);

  //Generate Physics Object
  GenerateTrianglesFromDXMesh(m_pMesh,m_pStatGeometryActor,NxVec3(0,0,0));
  //m_Mesh = m_pMesh;
} //EndTrianglesFromDXMeshProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::TrianglesFromDXMesh()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::TrianglesFromDXMeshVerb(ID3DXMesh* m_pMesh,NxActor* Actor,NxVec3 Pos)
{
  //LPD3DXMESH _Mesh;
  //LoadXFile("\Models\\Maps\\Terrain\\Terrain.x",_Mesh);

  //Generate Physics Object
  GenerateTrianglesFromDXMesh(m_pMesh,Actor,Pos);
  //m_Mesh = m_pMesh;
} //EndTrianglesFromDXMeshProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::SetActorGroup()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::SetActorGroup(NxActor* Actor,NxCollisionGroup Group)
{
  NxU32 nbShapes = Actor->getNbShapes();
  NxShape* const* Shapes = Actor->getShapes();

  while (nbShapes--)
  {
    Shapes[nbShapes]->setGroup(Group);
  }
} //EndSetActorGroupProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::Init()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::Init(DWORD Param)
{
  //Let The Scene Know To Check For Collision Between
  if (Param == 1 && Player != NULL)
  {
    m_pScene->setActorPairFlags(*Player->PhysXPlayer(),*m_pStatGeometryActor,NX_NOTIFY_ON_TOUCH);
    SetActorGroup(m_pStatGeometryActor,GroupA); //For Ray Cast
	m_pStatGeometryActor->raiseBodyFlag(NX_BF_KINEMATIC); //Static Object Parameter
  }
} //EndInitProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::SetActorPair()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::SetActorPair(NxActor* &ActorA,NxActor* ActorB,DWORD Param)
{
  m_pScene->setActorPairFlags(*ActorA,*ActorB,Param); //For PhysX Report And Collision Between
} //EndSetActorPairProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::SetRaycastGroup()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::SetRaycastGroup(NxActor* &Actor)
{
  SetActorGroup(Actor,GroupA); //Set For Raycast Contact Group 
} //EndSetRaycastGroupProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::SetBodyFlag()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPhysX::SetBodyFlag(NxActor* &Actor,NxBodyFlag Flag)
{
  Actor->raiseBodyFlag(Flag); //Static Object Parameter
} //EndSetBodyFlagProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::UpdateRC()
// Desc: 
//-----------------------------------------------------------------------------
bool CGRPhysX::UpdateRC(float X,float Y,float Z,bool BeShoot)
{
  bool Result = false;
  if (RayCast != NULL)
  {
    RayCast->SetCameraPos(X,Y,Z);
    Result = RayCast->Update(Player->PhysXPlayer(),GroupFlagA|GroupFlagB,false,BeShoot);
  }

  return Result;
} //EndTraceUpdateRCFunction

//-----------------------------------------------------------------------------