/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_LightSystem.h
// It's The GreenRay Lighting Shader Engine.
// Made Specially For Shader Deferred Lighting Technology Implementation.
// Date Creation: 29 August 2005
// Last Changing: 21 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Object.h>
#include <GR_GBuffer.h>
#include <GR_Lighting.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRMeshXShader;
//class CGRLightSystem;

//-----------------------------------------------------------------------------
// Name: Struct CGRMeshXShader
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRMeshXShader_H__)
#define AFX_CGRMeshXShader_H__

class CGRMeshXShader
{
public:
  //Public Directives
  CGRMeshXShader();
  virtual ~CGRMeshXShader();

  HRESULT Create(LPDIRECT3DDEVICE9 pDevice,CGRMeshX* pGRMesh,bool Textured);
  virtual void Reset();

  //Mesh Rendering 
  virtual void Render(UINT RenderPass);
  virtual void RenderWeapon(UINT RenderPass);
  virtual void RenderShell(D3DXMATRIX MatWorld);
  virtual void RenderShadow(UINT ShNum);

  D3DXMATRIX   m_WorldMatrix; //World Position/Rotation/Scale
  ID3DXEffect* m_pShader;	  //Shader That Output This Mesh Into Fat Framebuffer
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  D3DXMATERIAL* g_pMaterials;  //Pointer To Material Info In m_pMaterials
  CGRMeshX* m_pGRMesh;         //Pointer To GreenRay Mesh Object

  //LPDIRECT3DTEXTURE9 LightTexture;
};
#endif //!defined(AFX_CGRMeshXShader_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRLightSystem
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRLightSystem_H__)
#define AFX_CGRLightSystem_H__

class CGRLightSystem
{
public:
  //Public Directives
  CGRLightSystem(LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pDevice,int WndWidth,int WndHeight);
  virtual ~CGRLightSystem();
  void Release();
  
  void BeginShadow(UINT ShadowNum,D3DXVECTOR3 ShadowPos,float LightPosX,float LightPosY,float LightPosZ);
  void EndShadow();

  void BeginLight();
  void EndLight();

  void UpdateLight(D3DXMATRIX CamView,D3DXVECTOR3 CamPos);
  void UpdateCullInfo(CULLINFO* pCullInfo);
  void SetPointLightParam(UINT LightNum,D3DXVECTOR3 LightPos,float LightVal,D3DXVECTOR4 LightColor);
  void SetFlashLightParam(UINT LightNum,D3DXVECTOR4 LightPos,float LightVal,D3DXVECTOR4 LightColor);
  void LightGBuffer();
  void UseFlash(bool bFlash) { m_bFlash = bFlash;}
protected:
  //Protected Directives
  //GBuffer
  class CGRGBuffer*	 m_FrameBuffer;
  LPDIRECT3DSURFACE9 m_pBackBuffer;
  LPD3DXEFFECT		 m_ShowFatFx;

  //Lights
  CGRBaseLight*		   m_ActiveLights[MAX_LIGHTS];
  CGRHemisphereLight*  m_LightHemisphere;
  CGRDirectionalLight* m_LightDirectional[MAX_SHADOWS];
  CGRPointLight*	   m_LightPoint[MAX_POINT_LIGHTS];

  //Textures
  LPDIRECT3DTEXTURE9 m_pShadowMap[MAX_SHADOWS];
  LPDIRECT3DSURFACE9 m_pShadowMapSurf[MAX_SHADOWS];
  LPDIRECT3DSURFACE9 m_pShadowMapZ;

  LPDIRECT3DTEXTURE9 m_pDotProductFuncs;
  LPDIRECT3DTEXTURE9 m_pMaterialFuncs;
  LPDIRECT3DTEXTURE9 m_pAttenuationFuncs;

  enum
  {
	DFT_DISPLAY_BUFFER0,
	DFT_DISPLAY_BUFFER1,
	DFT_DISPLAY_BUFFER2,
	DFT_DISPLAY_BUFFER3,

	DFT_COMBINE_COLOR,
	DFT_COMBINE_MONO,

	DFT_COMBINE_WIERD,
  } m_DisplayFatType;

private:
  //Private Directives
  LPDIRECT3D9		d_pD3D;	           //D3D Object
  LPDIRECT3DDEVICE9 d_pDevice;         //Pointer On Direct3D Device Interface
  D3DVIEWPORT9      m_pShadowViewport; //Shadow Map Viewport
  bool m_bFlash;                       //Weapon Flash Light Render State
};
#endif //!defined(AFX_CGRLightSystem_H__)

//-----------------------------------------------------------------------------