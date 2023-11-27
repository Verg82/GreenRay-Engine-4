/****************************************************************************/ 
// It's An Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Engine.h Visual Studio 8 Version.
// It's An GreenRay GREngine Header File.
// Contain General 3D Engine Initialization Parameters And Instructions.
// Date Creation: 29 August 2005
// Last Changing: 03 January 2011
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
//#pragma once

#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_Camera.h>
#include <GR_SkyBox.h>
#include <GR_Label.h>
#include <GR_Fps.h>
#include <GR_Object.h>
#include <GR_PhysX.h>
#include <GR_WeaponSystem.h>
#include <GR_LightSystem.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class C3DEngine;

//-----------------------------------------------------------------------------
// Name: C3DEngine
// Desc: The GreenRay 3D Engine Class
//-----------------------------------------------------------------------------
#if !defined(AFX_3DENGINE_H__)
#define AFX_3DENGINE_H__

class C3DEngine
{
public:
  //Public Directives
  C3DEngine(HWND WndLayer);
  virtual ~C3DEngine();
  void Release();
  void CreateDirect3D(HWND WndLayer);
  void CreateObjects();
  void SetSkyBoxTextures(LPCSTR ZNeg,LPCSTR XNeg,LPCSTR ZPos,LPCSTR XPos,LPCSTR YNeg,LPCSTR YPos);
  void SetUseCamera(bool UseConnect);
  
  void InitTime();
  void UpdateTime();
  
  void UpdateFPS();
  void Render();

  void SetWindowed(BOOL State) { bWindowed = State; }
  void SetScreenResolution(int Width,int Height) { WndWidth = Width; WndHeight = Height; }
  void SetSwapEffect(D3DSWAPEFFECT Value) { SwapEffect = Value; }
  void SetAutoDepthStencil(BOOL State) { bAutoDepthStencil = State; }
  void SetAutoDepthStencilFormat(D3DFORMAT Value) { AutoDepthStencilFormat = Value; }
  void SetBackBufferCount(UINT Value) { BackBufferCount = Value; }
  void SetMultiSampleType(D3DMULTISAMPLE_TYPE Value) { MultiSampleType = Value; }
  void SetMultiSampleQuality(DWORD Value) { MultiSampleQuality = Value; }
  void SetPresentationInterval(UINT Value) { PresentationInterval = Value; } 
private:
  //Private Directives
  IDirect3D9* m_pDirect3D;     //Direct3D Interface
  IDirect3DDevice9* m_pDevice; //Direct3D Device Interface (DirectX 9)
  HWND m_pHWnd; //Window Handle

  //D3D Param Directives
  BOOL                bWindowed;
  int                 WndHeight;
  int                 WndWidth;
  D3DSWAPEFFECT       SwapEffect;
  BOOL                bAutoDepthStencil;
  D3DFORMAT           AutoDepthStencilFormat;
  UINT                BackBufferCount;
  D3DMULTISAMPLE_TYPE MultiSampleType;
  DWORD               MultiSampleQuality;
  UINT                PresentationInterval;

  //GreenRay Engine Objects Directives
  CGRCamera*   gr_Camera;   //GreenRay Camera
  CGRSkyBox*   gr_SkyBox;   //GreenRay SkyBox
  CGRFPS*      gr_FPS;      //GreenRay FPS Counter

  CGRObject*       gr_TestObj;           //Simple Object Class
  CGRMeshXShader*  gr_TestObjShader;     //Simple Object Light Shader
  CGRObject*       gr_TestBarrel2;       //Simple Object Class
  CGRMeshXShader*  gr_TestBarrelShader2; //Simple Object Light Shader
  CGRObject*       gr_TestPipe1;         //Simple Object Class
  CGRMeshXShader*  gr_TestPipeShader1;   //Simple Object Light Shader
  CGRObject*       gr_TestPipe2;         //Simple Object Class
  CGRMeshXShader*  gr_TestPipeShader2;   //Simple Object Light Shader

  CGRPhysX*        gr_PhysX;         //Global PhysX Interface Class
  CGRWeaponSystem* gr_WeaponSystem;  //GreenRay Weapon System Class
  CGRLightSystem*  gr_Light;         //Lighting Shader Test
protected:
  //Protected Directives
};
#endif //!defined(AFX_3DENGINE_H__)