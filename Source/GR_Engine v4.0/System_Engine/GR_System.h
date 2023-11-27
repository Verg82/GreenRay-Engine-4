/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_D3DSystem.h: Interface Keep All Direct3D Definitions And Prototypes
// Used In GreenRay Engine System.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

//Windows Libraries
#pragma comment(lib,"winmm.lib")

//Direct3D Libraries (DirectX 9)
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//#pragma comment(lib,"d3dx9d.lib")
//#pragma comment(lib,"d3dxof.lib")

#pragma comment(lib,"dinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"PhysXLoader.lib")

//Windows And System Directives
#include <windows.h>
#include <stdio.h>

//Math Functions And Conversions Directives
#include <vector>
//#include <math.h>
//#include <atlstr.h>   
//#include <tchar.h>
//#include <wchar.h>
//#include <strsafe.h>

//Direct3D Directives (DirectX 9)
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>
#include <d3dx9mesh.h>
#include <dinput.h>

//Sound System Directives
#include <mmsystem.h>
#include <dshow.h>
#include <dsound.h>
#include <dmusici.h>

//PhysX Directives
#include <NxPhysics.h>
#include <NxCooking.h>
#include <NxStream.h>
#include <NxPhysicsSDK.h>
#include <NxPMap.h>
#include <PhysXLoader.h>
#include <NxUserRaycastReport.h>