/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Object.h
// It's The GreenRay Object Loader Interface.
// Made Specially For Implementing Loading And Rendering 3D Objects.
// Date Creation: 29 August 2005
// Last Changing: 29 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_MeshX.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRObject;

//-----------------------------------------------------------------------------
// Name: CGRObject
// Desc: The Map Environment Class
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRObject_H__)
#define AFX_CGRObject_H__

class CGRObject
{
public:
  //Public Directives
  CGRObject();
  virtual ~CGRObject();

  void Create(LPDIRECT3DDEVICE9 pDevice,LPCWSTR File_Name,UINT MapAmplitude);
  void Release();
  void SetTexturePath(LPCSTR Tex_Path);

  void Render();
  LPD3DXMESH GetLocalMeshGeometry() { return m_pObject->GetLocalMesh(); }
  CGRMeshX* GetCGRModel() { return m_pObject; }
protected:
  //Protected Directives
  LPDIRECT3DDEVICE9 d_pDevice;            //Pointer On Direct3D Device Interface
  LPDIRECT3DVERTEXDECLARATION9 pVertDecl; //Vertex Declaration For Loadable Model
  LPCSTR m_pTexPath;                      //This Value Keep Texture Path

  CGRMeshX* m_pObject;        //Model Mesh Class
  D3DXMATERIAL* g_pMaterials; //Pointer To Material Info In m_pMaterials
};
#endif //!defined(AFX_CGRObject_H__)

//----------------------------------------------------------------------------