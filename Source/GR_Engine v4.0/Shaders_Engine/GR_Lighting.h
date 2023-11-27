/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Lighting.h
// It's The GreenRay Lighting Shader Engine.
// Implementation Of Base Lights Code Shared By Sub-Classes.
// Date Creation: 29 August 2005
// Last Changing: 17 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_GBuffer.h>

///////////////////////////////////////////////////////////////////////////////
// Global Definitions Or Prototypes
///////////////////////////////////////////////////////////////////////////////
//Round Up Sigh
#define BILERP_PROTECTION  2

//Space For 512 Different Materials
#define NUM_MATERIALS	   512
//Dot Ranges From -1 To 1 With Linear Interpolation Between Samples
#define DOTP_WIDTH		   2048
//Space For 512 Different Dot Product Functions (Need Bilerp Protection In y)
#define NUM_DOTP_FUNCTIONS 512

//Attenuation Texture Function Width Ranges From 0-1 With Linear Interpolation Between Sample
#define ATTENUATION_WIDTH		  512
//Space For 512 Different Attuentation Functions
#define NUM_ATTENUATION_FUNCTIONS 512

///////////////////////////////////////////////////////////////////////////////
// External Definitions Or Prototypes
///////////////////////////////////////////////////////////////////////////////
//Default Materials
#define MATERIAL_NIL			0
#define MATERIAL_FULL_AMBIENT	1
#define MATERIAL_FULL_EMMISIVE	2
#define MATERIAL_STD_SPECULAR	3
#define MATERIAL_METAL_SPECULAR	4

//D3DX Fill Texture Callbacks
extern void WINAPI FillDotProductTexture(D3DXVECTOR4* pOut, 
const D3DXVECTOR2* pTexCoord,const D3DXVECTOR2* pTexelSize,LPVOID pData);
extern void WINAPI FillMaterialTexture(D3DXVECTOR4* pOut, 
const D3DXVECTOR2* pTexCoord,const D3DXVECTOR2* pTexelSize,LPVOID pData);
extern void WINAPI FillAttenuationTexture(D3DXVECTOR4* pOut, 
const D3DXVECTOR2* pTexCoord,const D3DXVECTOR2* pTexelSize,LPVOID pData);

//Add A New Texture To The Material List (Should Be All Set Before FillMaterialTexture Is Called)
extern unsigned int RegisterMaterial(const Material& mat);
//Clear The Material List
extern void EmptyMaterialList();
//Get The Texture Coordinate Of The Specified Material
extern float MaterialIndexToV(unsigned int Material);
//Get The Texture Coordinate Of The Attenuation Function
extern float AttenuationIndexToV(enum ATTENUATION_FUNCTION Func);
//Get The Texture Coordinate Of The Dot Product Function
extern float GetDotProductFunctionU(enum DOTP_FUNCTIONS Func);

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRBaseLight;
//class CGRDirectionalLight;
//class CGRHemisphereLight;
//class CGRPointLight;

//-----------------------------------------------------------------------------
// Name: Struct CGRBaseLight
// Desc: Simple Base Class For Light Objects
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRBaseLight_H__)
#define AFX_CGRBaseLight_H__

class CGRBaseLight
{
public:
  //Piblic Directives
  CGRBaseLight();
  virtual ~CGRBaseLight();

  //Handle After A Reset
  virtual void Reset();

  //Do Any Update Code Necessary
  virtual void Update();
	
  //Render The Light Geometry Reading Parameters From The G-Buffer
  virtual void Render() = 0;

  D3DXMATRIX   m_WorldMatrix;  //World Position/Rotation/Scale 
  ID3DXEffect* m_pShader;	   //Light Shader
  ID3DXMesh*   m_pSphMesh;	   //D3DX That Hits Every Pixel Used By This Light

  D3DXVECTOR4 m_LightColor;	    //Color Of The Light
  float		  m_LightIntensity;	//Intensity Of The Light
protected:
  //Protected Directives
  D3DXMATRIX  m_WorldViewMatrix; //After Update Has Factoring In The View Space
  D3DXVECTOR4 m_ActualColor;	 //Color*Intensity (Calculated In Update)

  //Runs The Pixel Shader To The Entire Screen
  void ApplyFullScreenPixelShader(const UINT Width,const UINT Height);
};
#endif //!defined(AFX_CGRBaseLight_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRDirectionalLight
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRDirectionalLight_H__)
#define AFX_CGRDirectionalLight_H__

class CGRDirectionalLight: public CGRBaseLight
{
public:
  //Public Directives
  HRESULT Create();
  void Update();
  void Render();
protected:
  //Protected Directives
};
#endif //!defined(AFX_CGRDirectionalLight_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRHemisphereLight
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRHemisphereLight_H__)
#define AFX_CGRHemisphereLight_H__

class CGRHemisphereLight: public CGRBaseLight
{
public:
  //Public Directives
  HRESULT Create();
  void Update();
  void Render();
  D3DXVECTOR4 m_SkyColor;
protected:
  //Protected Directives
};
#endif //!defined(AFX_CGRHemisphereLight_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRPointLight
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRPointLight_H__)
#define AFX_CGRPointLight_H__

class CGRPointLight: public CGRBaseLight
{
public:
  //Public Directives
  HRESULT Create();
  void Update();
  void Render();

  //Radius Should Be Half The World Scale Of The Object Else Special Things Will Happen
  float m_Radius;

  //Each Light Should Have A Unique ID For The Stencil Buffer
  //You Could Either Allocate Them Every Frame (And If Nessecary Clear The Stencil 
  //Buffer N Times To Support > 255 Lights)
  //Or As The Demos Do Just Allocate A Static ID.
  //Used To Guarentee Each Light Effects Each Pixel Once And Once Only
  unsigned char m_LightID;

  //Use Debugger Pixel Shader Or The Real Thing (True For Debugger)
  bool m_ShowVolume;
protected:
  //Protected Directives
  CULLSTATE CullLight(CULLINFO* pCullInfo);
  bool CrossesNearPlane();
  BYTE m_bOutside[8];
  BYTE m_bOut;
  BYTE m_bIn;
};
#endif //!defined(AFX_CGRPointLight_H__)

//-----------------------------------------------------------------------------