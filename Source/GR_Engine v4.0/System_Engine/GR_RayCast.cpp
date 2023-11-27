/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_RayCast.cpp
// It's The GreenRay Physics Engine.
// Made Specially For Implementing RayCast.
// Date Creation: 29 August 2005
// Last Changing: 05 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_RayCast.h>

//////////////////////////////////////////////////////////////////////////////
// Raycast Report Virtual Class
//////////////////////////////////////////////////////////////////////////////
class RaycastReport: public NxUserRaycastReport
{
  public:
    virtual bool onHit(const NxRaycastHit& hit)
    {
      int userData = (int)hit.shape->getActor().userData;
      userData |= 1; //Mark As Hit
      hit.shape->getActor().userData = (void*)userData;

      const NxVec3 &worldImpact = hit.worldImpact;
	
	  return true;
    }
} gRaycastReport;

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRRayCast Class
//////////////////////////////////////////////////////////////////////////////
CGRRayCast::CGRRayCast(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene)
{
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  d_pScene = pScene;
  
} //EndConstructionDirectives

CGRRayCast::~CGRRayCast()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRRayCast::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRRayCast::Release()
{
  d_pScene = NULL;  //NULL PhysX Scene Pointer
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRPhysX::Update()
// Desc: 
//-----------------------------------------------------------------------------
bool CGRRayCast::Update(NxActor* Actor,NxU32 GroupFlag,bool RaycastAll,bool BeShoot)
{
  bool Result = false;
  if (!RaycastAll) //Raycast Just Closest Shape
  {
    //Get Ray Origin
    NxVec3 Orig = Actor->getCMassGlobalPosition();

    //Set Ray Direction
    NxRay Ray;
    Ray.orig = Orig;
	Ray.orig.y = Ray.orig.y+PlayerHeight+1.0f;

    Ray.dir = NxVec3(PosX,PosY,PosZ);
    Ray.dir.normalize();

    NxRaycastHit Hit;
    //NxReal dist;

    //Get The Closest Shape
    NxShape* ClosestShape = d_pScene->raycastClosestShape(Ray,NX_ALL_SHAPES,Hit,GroupFlag);
    
    //Make RayCast With Closest Shape Geometry
    if (ClosestShape)
	{
      const NxVec3& WorldImpact = Hit.worldImpact; //Get Raycast Point
	  const NxVec3& WorldNormal = Hit.worldNormal; //Get Polygon Normal

      //dist = Hit.distance;
      Vec1 = D3DXVECTOR3(1.0f,1.0f,1.0f); //NxVec3_To_DXVec3(pos[0]);	
	  Vec2 = D3DXVECTOR3(1.0f,1.0f,1.0f); //NxVec3_To_DXVec3(pos[1]);
	  Vec3 = D3DXVECTOR3(1.0f,1.0f,1.0f); //NxVec3_To_DXVec3(pos[2]);


      //Create Decal On The Wall 
      if (BeShoot) //Update Weapon Shoot
      { 
		//if (Hit.shape->getGlobalPosition() == NxVec3(0.0f,0.001f,0.0f))
		if (Hit.shape->isCapsule())
		{
          
		}
        else
		{
		  //Playback Ricochet From Sound Container	 
          //gr_SoundsEff->PlaybackReset("Ricochet"); 
	
	      RCPos = NxVec3ToDXVec3(WorldImpact);
          RCNormal = NxVec3ToDXVec3(WorldNormal);
          RCVec1 = Vec1;
          RCVec2 = Vec2;
		  Result = true;
		}
      }
   

  

    } //End If (ClosestShape)
   





  }
  else //Raycast All Shapes Into The Scene
  {
    //Get Ray Origin
    NxVec3 Orig = Actor->getCMassGlobalPosition();

    //Get Ray Direction
    NxVec3 dir;
    NxMat33 m = Actor->getGlobalOrientation();
    m.getColumn(0,dir);
    dir = -dir;
    NxRay Ray(Orig,dir);

    //Get All Shapes
    NxU32 nbShapes = d_pScene->raycastAllShapes(Ray,gRaycastReport,NX_ALL_SHAPES,GroupFlag);
  }

  //Return Value
  return Result;
} //EndUpdateFunction

//----------------------------------------------------------------------------