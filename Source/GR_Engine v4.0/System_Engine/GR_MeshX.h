/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_MeshX.h
// It's The GreenRay Model Loader Interface.
// Made Specially For Implementing Loading And Rendering DirectX Meshes.
// Date Creation: 29 August 2005
// Last Changing: 28 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRMeshX;

//-----------------------------------------------------------------------------
// Name: Struct CGRMeshX
// Desc: Class For Loading And Rendering DirectX Meshes
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRMeshX_H__)
#define AFX_CGRMeshX_H__

class CGRMeshX
{
private:
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
public:
  WCHAR m_strName[512];
  LPD3DXMESH m_pSysMemMesh; //SysMem Mesh, Lives Through Resize
  LPD3DXMESH m_pLocalMesh;  //Local Mesh, Rebuilt On Resize
    
  DWORD m_dwNumMaterials;          //Materials For The Mesh
  D3DMATERIAL9* m_pMaterials;
  //IDirect3DBaseTexture9** m_pTextures;
  LPDIRECT3DTEXTURE9* m_pTextures;       //Keep All Loadable Textures From Mesh Data
  LPDIRECT3DTEXTURE9* m_pSpecTextures;   //Keep All Loadable Specular Textures From Mesh Data
  LPDIRECT3DTEXTURE9* m_pNormTextures;   //Keep All Loadable Normal Map Textures From Mesh Data
  LPDIRECT3DTEXTURE9* m_pNormSrc;        //Keep All Loadable Source Normal Map Textures From Mesh Data
  LPDIRECT3DTEXTURE9* m_pHeightTextures; //Keep All Loadable Height Map Textures From Mesh Data
  LPCSTR m_pTexPath;                     //This Value Keep Texture Path
  bool m_bUseMaterials;
public:
  //Rendering The Mesh
  HRESULT Render(bool bDrawOpaqueSubsets = true,bool bDrawAlphaSubsets = true);
  HRESULT Render(ID3DXEffect* pEffect,
  D3DXHANDLE hTexture = NULL,D3DXHANDLE hDiffuse = NULL,
  D3DXHANDLE hAmbient = NULL,D3DXHANDLE hSpecular = NULL,
  D3DXHANDLE hEmissive = NULL,D3DXHANDLE hPower = NULL,
  bool bDrawOpaqueSubsets = true,bool bDrawAlphaSubsets = true);

  //Mesh Access
  LPD3DXMESH GetSysMemMesh() { return m_pSysMemMesh; }
  LPD3DXMESH GetLocalMesh() { return m_pLocalMesh; }

  //Rendering Options
  void UseMeshMaterials(bool bFlag) { m_bUseMaterials = bFlag; }
  HRESULT SetFVF(DWORD dwFVF);
  HRESULT SetVertexDecl(const D3DVERTEXELEMENT9* pDecl);

  //Initializing
  HRESULT RestoreObjects();
  HRESULT InvalidateObjects();
    
  //Set Texture Disposition Folder
  void SetTexturePath(LPCSTR Tex_Path);

  //Creation/Destruction
  HRESULT Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR File_Name,UINT MapAmplitude);
  HRESULT CreateMaterials(LPCWSTR Str_Path,ID3DXBuffer* pAdjacencyBuffer,
  ID3DXBuffer* pMtrlBuffer,UINT MapAmplitude);
    
  CGRMeshX();
  virtual ~CGRMeshX();
  HRESULT Destroy();
};
#endif //!defined(AFX_CGRMeshX_H__)

//----------------------------------------------------------------------------