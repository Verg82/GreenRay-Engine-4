/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Shaders.h
// It's The GreenRay Shader Engine.
// Made Specially For Implementing Shader Effects.
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>

////////////////////////////////////////////////////////////////////////////////
// Exporting Procedures And Functions
////////////////////////////////////////////////////////////////////////////////
HRESULT CreateVertDeclaration(LPDIRECT3DDEVICE9 pDevice,IDirect3DVertexDeclaration9** Declaration,D3DVERTEXELEMENT9* Decl);
HRESULT LoadShaderFile(LPDIRECT3DDEVICE9 pDevice,LPCSTR FileName,LPD3DXEFFECT* Effect);

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRShaders;

//-----------------------------------------------------------------------------
// Name: Struct CGRShaders
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRShaders_H__)
#define AFX_CGRShaders_H__

class CGRShaders
{
public:
  //Public Directives
  CGRShaders();
  virtual ~CGRShaders();
  void Release();
  
private:
  //Private Directives


};
#endif //!defined(AFX_CGRShaders_H__)

//-----------------------------------------------------------------------------