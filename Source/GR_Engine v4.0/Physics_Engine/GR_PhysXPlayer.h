/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_PhysXPlayer.h
// It's The GreenRay Physics Engine.
// Made Specially For Implementing PhysX Based Engine Player.
// Date Creation: 29 August 2005
// Last Changing: 01 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_PhysXMethods.h>
#include <GR_Camera.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRPhysXPlayer;

//-----------------------------------------------------------------------------
// Name: Struct CGRPhysXPlayer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRPhysXPlayer_H__)
#define AFX_CGRPhysXPlayer_H__

class CGRPhysXPlayer
{ 
public:
  //Public Directives
  CGRPhysXPlayer(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene);
  virtual ~CGRPhysXPlayer();
  void Release();

  void CreatePlayer();
  void Update(CGRCamera* &Camera);
  void Jump();
  void SetMaterial(int Material) { iMaterial = Material; }
  void SetPosition(D3DXVECTOR3 Pos) { PlayerPos = Pos; }
  void SetSize(float Size) { PlayerSize = Size; }
  void SetDensity(float Density) { PlayerDensity = Density; }
  void SetHeight(float Height) { PlayerHeight = Height; }
  void SetJumpSpeed(float Speed) { JumpSpeed = Speed; }
  NxActor* PhysXPlayer() { return m_pPhysXPlayer; }
  float Height() { return PlayerHeight; }
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  NxScene* d_pScene;           //Pointer On PhysX Scene Interface
  NxActor* m_pPhysXPlayer;     //Actor Player PhysX Interface
  int iMaterial;               //Material
  D3DXVECTOR3 PlayerPos;       //Player Global Position
  float PlayerSize;            //Player Collidable BoxSize
  float PlayerDensity;         //Player Density
  float PlayerHeight;          //Player Height
  float JumpSpeed;             //Player Jump Speed
};
#endif //!defined(AFX_CGRPhysXPlayer_H__)

//-----------------------------------------------------------------------------