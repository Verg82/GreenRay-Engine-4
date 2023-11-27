/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_LightGlobals.h
// It's The GreenRay Lighting Shader Engine.
// Made Specially For Objects That Are Shared All Over The Place
// Date Creation: 29 August 2005
// Last Changing: 21 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <GR_System.h>

////////////////////////////////////////////////////////////////////////////////
// Global Definitions
////////////////////////////////////////////////////////////////////////////////
#define MAX_OBJECTS      32
#define MAX_SHADOWS      2
#define MAX_POINT_LIGHTS 6
#define MAX_LIGHTS       32

////////////////////////////////////////////////////////////////////////////////
// Global Definitions Or Prototypes
////////////////////////////////////////////////////////////////////////////////
#define SHADOW_MAP_FORMAT D3DFMT_R32F
#define SHADOW_MAP_SIZE 512

//-----------------------------------------------------------------------------
// Name: Enum DOTP_FUNCTIONS
// Desc: The Dotproduct Functions Take A Single Dotproduct (Ranging From -1 To 1) 
// In Practise We Use 2D Textures So We Can Store A Series Of Functions In A 
// Single Texture And Then Pick The Function Via The Extra Coordinate.
//-----------------------------------------------------------------------------
enum DOTP_FUNCTIONS
{
  DPF_ZERO = 0,
  DPF_QUARTER,
  DPF_HALF,
  DPF_THREEQUARTER,
  DPF_ONE,

  DPF_FRONT_POW1,
  DPF_FRONT_POW2,
  DPF_FRONT_POW3,
  DPF_FRONT_POW4,

  DPF_FRONT_POW8,
  DPF_FRONT_POW12,
  DPF_FRONT_POW16,
  DPF_FRONT_POW20,
  DPF_FRONT_POW24,
  DPF_FRONT_POW28,
  DPF_FRONT_POW32,
  DPF_FRONT_POW36,
  DPF_FRONT_POW40,
  DPF_FRONT_POW44,
  DPF_FRONT_POW48,
  DPF_FRONT_POW52,
  DPF_FRONT_POW56,
  DPF_FRONT_POW60,
  DPF_FRONT_POW64,

  DPF_FRONT_POW128,
  DPF_FRONT_POW256,
  DPF_FRONT_POW512,
  DPF_FRONT_POW640,
  DPF_FRONT_POW768,
  DPF_FRONT_POW896,
  DPF_FRONT_POW1024,

  //Enough Of The Blinn/Phong Stuff Lets Get Creative

  //This Is A Slight Power Function Around 0 (90 Degrees Out Of Normal) Nice Back Lit Effect With N.E
  DPF_FRESNEL_POW4,
  //Similar To Above But Shifted To Around +0.3 And Clamped So Negative Results
  DPF_SHIFTED_FRESNEL_POW8,
};

//-----------------------------------------------------------------------------
// Name: Enum ATTENUATION_FUNCTION
// Desc: Some Attenuation Shapes
//-----------------------------------------------------------------------------
enum ATTENUATION_FUNCTION
{
  AF_ZERO =	0,
  AF_QUARTER,
  AF_HALF,
  AF_THREEQUARTER,
  AF_ONE,

  AF_LINEAR,
  AF_SQUARED,
  AF_SHAPE1,
  AF_SHAPE2,
};

//-----------------------------------------------------------------------------
// Name: Struct Material
// Desc: Material Parameters That Are NOT Evaluated Per-Pixel (Lookup'ed Per Pixel)
//-----------------------------------------------------------------------------
struct Material
{
  float KAmb;
  float KEmm;
  float KSpecCol; //A Blend Factor Between Specular Taking Surface Colour Only (Metals) Or Light Colour

  Material(float inKAmb,float inKEmm,float inKSpecCol)
  {
    KAmb = inKAmb;
	KEmm = inKEmm;
	KSpecCol = inKSpecCol;
  }				
};

//-----------------------------------------------------------------------------
// Name: Struct CULLINFO
// Desc: Stores Information That Will Be Used When Culling Objects. It Needs
//       To Be Recomputed Whenever The View Matrix Or Projection Matrix Changes.
//-----------------------------------------------------------------------------
struct CULLINFO
{
  D3DXVECTOR3 VecFrustum[8]; //Corners Of The View Frustum
  D3DXPLANE PlaneFrustum[6]; //Planes Of The View Frustum
};

//-----------------------------------------------------------------------------
// Name: Enum CULLSTATE
// Desc: 
//-----------------------------------------------------------------------------
enum CULLSTATE
{
  CS_UNKNOWN,  //Cull State Not Yet Computed
  CS_INSIDE,   //Object Bounding Box Is At Inside The Frustum
  CS_OUTSIDE,  //Object Bounding Box Is Outside The Frustum
  CS_CROSSING, //Is Possible In And Out
};

//-----------------------------------------------------------------------------
// Name: Enum CULLPLANES
// Desc: 
//-----------------------------------------------------------------------------
enum CULLPLANES
{
  CP_NEAR	= 0,
  CP_FAR	= 1,
  CP_LEFT	= 2,
  CP_RIGHT	= 3,
  CP_TOP	= 4,
  CP_BOTTOM	= 5,
};

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRLightGlobals;

//-----------------------------------------------------------------------------
// Name: Struct CGRLightGlobals
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRLightGlobals_H__)
#define AFX_CGRLightGlobals_H__

class CGRLightGlobals
{
public:
  //Public Directives
  CGRLightGlobals();
  virtual ~CGRLightGlobals();
  void Release();  
private:
  //Private Directives
public:
  //Public Directives
  ID3DXEffectPool* m_EffectPool;	  //Pool That All Mesh Shader Should Use
  ID3DXEffectPool* m_LightEffectPool; //Pool That All Lights Should Use

  IDirect3D9* m_pD3D;			   //The D3D Object
  IDirect3DDevice9*	m_pD3DDevice;  //The D3D Device Object
  class CGRGBuffer* m_Framebuffer; //The Frame Object
  CULLINFO m_CullInfo;			   //The Frustum Culling Info

  //Light Matrices
  D3DXMATRIXA16 m_Proj;
  D3DXMATRIXA16 m_View;
  D3DXMATRIXA16 m_ViewProj;

  D3DXMATRIXA16 m_ShadowView;
  D3DXMATRIXA16 m_ShadowViewProj;

  D3DXVECTOR3   m_pCamPos;        //Camera Position
};
#endif //!defined(AFX_CGRLightGlobals_H__)

extern CGRLightGlobals* gl_LObj;

//-----------------------------------------------------------------------------