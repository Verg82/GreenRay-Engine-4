/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Decals.h:
// It's The GreenRay Decals Engine.
// Made Specially For Implementing Decals Technology.
// Date Creation: 29 August 2005
// Last Changing: 06 February 2012
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
//class CGRDecal;

//-----------------------------------------------------------------------------
// Name: Struct CGRDecal
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRDecal_H__)
#define AFX_CGRDecal_H__

#define D3DFVF_DECALVERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(0)) //Decal Vertex Format

class CGRDecal
{	
public:
  //Public Directives
  //Decal Vertex, Texures Positions
  struct DECAL_VERTEX
  {
    FLOAT x,y,z; //Coordinates
    FLOAT tu,tv; //Textures
  }; //End Vertex Formats

  CGRDecal(LPDIRECT3DDEVICE9 pDevice,LPCSTR DecalName,float Height,float Width);
  virtual ~CGRDecal();
  void Release();

  HRESULT CreateDecalSurface(float fWidth,float fHeight);
  void SetCoord(D3DXVECTOR3 vPos,D3DXVECTOR3 vNormal,D3DXVECTOR3 vVec1,D3DXVECTOR3 vVec2);
  void Render();
public:
  //Public Directives
private:
  //Private Directives 
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  IDirect3DVertexBuffer9* m_pVertexBuffer; //The Vertex Buffer
  //ID3DXMesh* m_pDecal; //Decal Interface
  IDirect3DTexture9* m_pTexture; //Decal Texture
  DWORD dwColor; //Decal Texture Alpha Color

  D3DXVECTOR3 Pos,Normal,Vec_1,Vec_2; //Vectors And Normal For Orienting Decal
  D3DXMATRIX Mat_Trans; //Transform Matrices
};
#endif //!defined(AFX_CGRDecal_H__)

//-----------------------------------------------------------------------------