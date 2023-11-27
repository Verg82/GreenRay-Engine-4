/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_PhysX.h
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection.
// Date Creation: 29 August 2005
// Last Changing: 30 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_PhysXPlayer.h>
#include <GR_RayCast.h>
#include <GR_Sound.h>
#include <Nx_Cooking.h>
#include <Nx_Stream.h>

////////////////////////////////////////////////////////////////////////////////
// GreenRay PhysX Constants
////////////////////////////////////////////////////////////////////////////////
#define GRPHYSX_ROOM 1
#define GRPHYSX_BARREL 2

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRPhysX;

//-----------------------------------------------------------------------------
// Name: Struct CGRPhysX
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRPhysX_H__)
#define AFX_CGRPhysX_H__
class CGRPhysX
{	
public:
  //Public Directives
  CGRPhysX(LPDIRECT3DDEVICE9 pDevice,HWND WndHndl);
  virtual ~CGRPhysX();
  void Release();
  
  NxScene* GetPhysXScene() { return m_pScene; }
  NxActor* GetPhysXWorld() { return m_pStatGeometryActor; }
  NxActor* GetPhysXPlayer() { if (Player != NULL) return Player->PhysXPlayer(); }

  void BeginUpdate();
  void EndUpdate();
  void UpdatePlayer(CGRCamera* &Camera);
  void JumpPlayer(bool JumpUp);
  void TrianglesFromDXMesh(ID3DXMesh* m_pMesh);
  void TrianglesFromDXMeshVerb(ID3DXMesh* m_pMesh,NxActor* Actor,NxVec3 Pos);
  void GenerateTrianglesFromDXMesh(ID3DXMesh* &m_pMesh,NxActor* &PhysXActor,NxVec3 &Mesh_Pos);
  void SetActorPair(NxActor* &ActorA,NxActor* ActorB,DWORD Param);
  void SetRaycastGroup(NxActor* &Actor);
  void SetBodyFlag(NxActor* &Actor,NxBodyFlag Flag);
  void SetActorGroup(NxActor* Actor,NxCollisionGroup Group);
  void Init(DWORD Param);
  bool UpdateRC(float X,float Y,float Z,bool BeShoot);
  D3DXVECTOR3 RayPos()    { if (RayCast != NULL) return RayCast->RayPos();    }
  D3DXVECTOR3 RayNormal() { if (RayCast != NULL) return RayCast->RayNormal(); }
  D3DXVECTOR3 RayVec1()   { if (RayCast != NULL) return RayCast->RayVec1();   }
  D3DXVECTOR3 RayVec2()   { if (RayCast != NULL) return RayCast->RayVec2();   }
public:
  //Public Directives
private:
  //Private Directives 
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  NxPhysicsSDK* m_pPhysicsSDK; //General PhysX Interface
  NxScene* m_pScene;           //General Scene PhysX Interface
  
  NxVec3* Vertices; //Keep Vertices Structure
  DWORD* Triangles; //Keep Indices Structure

  NxActor* m_pStatGeometryActor; //Static Mesh Geometry Actor
  CGRPhysXPlayer* Player;        //PhysX Player
  CGRRayCast* RayCast;           //Ray Cast

  //Keep An Index For Each Material
  int DefaultMaterial;
  int FrictionlessMaterial; //Set Material For Player
};
#endif //!defined(AFX_CGRPhysX_H__)

//----------------------------------------------------------------------------