/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_RayCast.h
// It's The GreenRay Physics Engine.
// Made Specially For Implementing RayCast.
// Date Creation: 29 August 2005
// Last Changing: 05 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_PhysXMethods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRRayCast;

//-----------------------------------------------------------------------------
// Name: Struct CGRRayCast
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRRayCast_H__)
#define AFX_CGRRayCast_H__
class CGRRayCast
{	
public:
  //Public Directives
  CGRRayCast(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene);
  virtual ~CGRRayCast();
  void Release();
  
  void SetCameraPos(float X,float Y,float Z) { PosX = X; PosY = Y; PosZ = Z; }
  bool Update(NxActor* Actor,NxU32 GroupFlag,bool RaycastAll,bool BeShoot);
  D3DXVECTOR3 RayPos()    { return RCPos;    }
  D3DXVECTOR3 RayNormal() { return RCNormal; }
  D3DXVECTOR3 RayVec1()   { return RCVec1;   }
  D3DXVECTOR3 RayVec2()   { return RCVec2;   }
  void SetPlayerHeight(float PlHeight) { PlayerHeight = PlHeight; }
public:
  //Public Directives
private:
  //Private Directives 
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  NxScene* d_pScene;           //Pointer On PhysX Scene Interface
  D3DXVECTOR3 Vec1,Vec2,Vec3;  //Normalized Triangle Vectors !!!

  float PosX,PosY,PosZ;        //Camera Position
  
  //Polygon RayCast Parameters
  float PlayerHeight;
  D3DXVECTOR3 RCPos;
  D3DXVECTOR3 RCNormal;
  D3DXVECTOR3 RCVec1;
  D3DXVECTOR3 RCVec2;
};
#endif //!defined(AFX_CGRRayCast_H__)

//----------------------------------------------------------------------------