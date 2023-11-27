/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_WeaponEffects.h
// It's The GreenRay Weapon Engine.
// Made Specially For Implementing Weapon Effects.
// Date Creation: 29 August 2005
// Last Changing: 08 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_MeshX.h>
#include <GR_Decals.h>
#include <GR_Particles.h>
#include <GR_LightSystem.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRWeaponBallistic;
//class CGRShell;
//class CGRWeaponFlash;

//-----------------------------------------------------------------------------
// Name: Struct CGRWeaponBallistic
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRWeaponBallistic_H__)
#define AFX_CGRWeaponBallistic_H__

class CGRWeaponBallistic
{
public:
  //Public Directives
  CGRWeaponBallistic(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRWeaponBallistic();
  void Release();
  
  void Render();
  void RenderParticles();
  void SetCoord(D3DXVECTOR3 Pos,D3DXVECTOR3 Normal,D3DXVECTOR3 Vec1,D3DXVECTOR3 Vec2);
  void SetUpdateTime(float UpdateTime) { m_fUpdateTime = UpdateTime; }
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  CGRDecal* m_pDecal;          //Bullet Hole Decal
  CGRParticle* m_pWallSmoke;   //Wall Smoke Particle System
  CGRParticle* m_pRock;        //Wall Rock Particle System
  CGRParticle* m_pSpark;       //Wall Spark Particle System
  CGRParticle* m_pScrap;       //Wall Scrap Particle System
  float m_fUpdateTime;         //Particles Update Time
};
#endif //!defined(AFX_CGRWeaponBallistic_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRShell
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRShell_H__)
#define AFX_CGRShell_H__
class CGRShell
{	
public:
  //Public Directives
  CGRShell(LPDIRECT3DDEVICE9 pDevice,NxScene* pScene,D3DXMATRIX MatView,
	       LPCWSTR ShellName,LPCSTR ShellTexture);
  virtual ~CGRShell();
  void Release();

  void Render();
  void RenderLighting();
  void SetUpdateTime(float UpdateTime) { m_fUpdateTime = UpdateTime; }
  NxVec3 ApplyForceToActor(NxActor* Actor,const NxVec3 &ForceDir,const NxReal ForceStrength);
  NxActor* GetActor();
  TCHAR* GetActorName();
  void ReleaseActor();
public:
  //Public Directives
  bool Dropped;   //Drop Down State
  float Rotation; //Rotation Space
private:
  //Private Directives  
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  LPDIRECT3DVERTEXDECLARATION9 pVertDecl; //Vertex Declaration For Loadable Model
  NxScene* d_pScene; //Pointer On Scene PhysX Interface

  LPCSTR m_pTexPath;              //This Value Keep Texture Path
  CGRMeshX* m_pShell;             //Shell Model Class
  CGRMeshXShader* m_pShellShader; //Shell Mesh Light Shader
  CGRParticle* m_pShellSmoke;     //Smoke Particle For Shell
  float m_fUpdateTime;            //Particles Update Time
  
  NxActor* m_pPhysXShell; //PhysX Actor Shell
  D3DXMATRIX mPos;        //Global Object Position Matrix
  D3DXMATRIX mOffset;     //Global Object Offset Matrix

  float ShellRadius;
  float ShellHeight;
};
#endif //!defined(AFX_CGRShell_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRWeaponFlash
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRWeaponFlash_H__)
#define AFX_CGRWeaponFlash_H__

class CGRWeaponFlash
{
public:
  //Public Directives
  CGRWeaponFlash(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRWeaponFlash();
  void Release();
  
  void Update();
  void Render(D3DXMATRIX MatView);
  void RenderParticles();
  void SetUpdateTime(float UpdateTime) { m_fUpdateTime = UpdateTime; }
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  CGRParticle* m_pFlash;       //Weapon Flash Particle System
  CGRParticle* m_pSmoke;       //Smoke Particle System
  float m_fUpdateTime;         //Particles Update Time
  int NumShoots;               //Number Of Shoots
};
#endif //!defined(AFX_CGRWeaponFlash_H__)

//-----------------------------------------------------------------------------