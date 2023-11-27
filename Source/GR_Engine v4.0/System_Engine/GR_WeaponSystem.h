/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_WeaponSystem.h
// It's The GreenRay Weapon System Engine.
// Made Specially For Implementing All Weapon Management Functions.
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Interface.h>
#include <GR_Object.h>
#include <GR_LightSystem.h>
#include <GR_WeaponEffects.h>
#include <GR_Sound.h>

////////////////////////////////////////////////////////////////////////////////
// Global Definitions
////////////////////////////////////////////////////////////////////////////////
#define MAX_SHELLS 200

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRWeaponSystem;

//-----------------------------------------------------------------------------
// Name: Struct CGRWeaponSystem
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRWeaponSystem_H__)
#define AFX_CGRWeaponSystem_H__

class CGRWeaponSystem
{ 
public:
  //Public Directives
  CGRWeaponSystem(LPDIRECT3DDEVICE9 pDevice,HWND WndHndl,int ScreenHeight,int ScreenWidth);
  virtual ~CGRWeaponSystem();
  void Release();

  void Render();
  void RenderLighting();
  void SetProjection(D3DXMATRIX CamProjection) { m_pCamProjection = CamProjection; }
  void UpdateBallistic(D3DXVECTOR3 Pos,D3DXVECTOR3 Normal,D3DXVECTOR3 Vec1,D3DXVECTOR3 Vec2);
  void Update(D3DXMATRIX CamView) { m_pCamView = CamView; };
  void UpdateShellTube(NxScene* pScene,NxActor* pActor,NxActor* pEnvironment,D3DXMATRIX MatView);
  void UpdateFlash(D3DXVECTOR3 CamPos,D3DXMATRIX MatView);
  void PlaybackRicochet();
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
 
  int ScrWidth;  //Screen Width
  int ScrHeight; //Screen Height

  CGRInterface*   m_pCrosshair;     //Weapon Crosshair
  CGRObject*      m_pBeretta;       //Beretta Weapon Model Class
  CGRMeshXShader* m_pBerettaShader; //Beretta Weapon Model Light Shader
  CGRDeltaTimer*  m_pDeltaTimer;    //Delta Timer

  //Sound System Directives
  CGRWave* m_pSndRicochet; //Wave Directive For Ricochet Sound

  //Bullet Hole And Other Ballistic Effects Directives
  CGRWeaponBallistic* m_pBallistic[MAX_SHELLS]; //Weapon Ballistic Effects Array
  int Num_Ballistic;                            //Num Of Ballistics

  //Weapon Shell Ballistic Directives
  CGRShell* m_pShell[MAX_SHELLS]; //Weapon Shell Array
  int Num_Shells;                 //Num Of Shells

  //Weapon Flash Directives
  CGRWeaponFlash* m_pFlash;    //Flash System
  D3DXMATRIX m_pCamView;       //Camera View
  D3DXMATRIX m_pCamProjection; //Camera Projection Matrix
};
#endif //!defined(AFX_CGRWeaponSystem_H__)

//-----------------------------------------------------------------------------