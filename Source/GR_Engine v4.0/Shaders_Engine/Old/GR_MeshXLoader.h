/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_MeshXLoader.h
// It's The GreenRay Lighting Shader Engine.
// Mesh Handler Class, Sample That Render Lots Of Different Meshes.
// Date Creation: 29 August 2005
// Last Changing: 21 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_MeshX.h>
#include <GR_GBuffer.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRModMeshX;

//-----------------------------------------------------------------------------
// Name: Struct CGRModMeshX
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRModMeshX_H__)
#define AFX_CGRModMeshX_H__

class CGRModMeshX
{
public:
  //Public Directives
  CGRModMeshX();
  virtual ~CGRModMeshX();

  HRESULT Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR MeshName,const LPCSTR FxFile);
  virtual void Reset();
  virtual void Render(UINT RenderPass);
  virtual void RenderWeapon(UINT RenderPass);
  virtual void RenderShell(UINT RenderPass);

  virtual void RenderShadow();

  void SetTexturePath(LPCSTR Tex_Path);
  LPD3DXMESH GetLocalMeshGeometry() { return m_pObject->GetLocalMesh(); }

  D3DXMATRIX   m_WorldMatrix; //World Position/Rotation/Scale
  ID3DXEffect* m_pShader;	  //Shader That Output This Mesh Into Fat Framebuffer
  //ID3DXMesh*   m_pMesh;		  //D3DX Mesh To Store Object
  bool		   m_bCastShadow; //Does This Mesh Cast A Shadow
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice;            //Pointer On Direct3D Device Interface
  LPDIRECT3DVERTEXDECLARATION9 pVertDecl; //Vertex Declaration For Loadable Model

  CGRMeshX* m_pObject;        //Model Mesh Class
  D3DXMATERIAL* g_pMaterials; //Pointer To Material Info In m_pMaterials
  LPCSTR m_pTexPath;          //This Value Keep Texture Path
};
#endif //!defined(AFX_CGRModMeshX_H__)

//-----------------------------------------------------------------------------