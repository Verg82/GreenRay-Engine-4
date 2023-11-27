/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_SkyBox.h: Interface For The SkyBox Geometry Implementation Class.
// It's The GreenRay SkyBox Geometry Transform Engine.
// Made Specially For Implementing Only SkyBox Geometry And Effects.
// Programming By: Verg
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Author Rights By: Zie Technology And Lucky's Flash Studio Inc.,
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
//class CGRSkyBox;

//-----------------------------------------------------------------------------
// Name: CGRSkyBox
// Desc: The Box Class
//-----------------------------------------------------------------------------
#if !defined(AFX_SKYBOX_H__)
#define AFX_SKYBOX_H__

//FVF (Flexible Vertex Format) Definition Keep Vertex Format And Texture Coordinates
#define D3DFVF_SKYBOXVERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(0)) //Vertex Format

class CGRSkyBox
{
public:
  //Public Directives
  //SkyBox Vertex And Texures Positions Structure
  struct SKYBOX_VERTEX
  {
    FLOAT x,y,z; //Coordinates
    FLOAT tu,tv; //Textures
  }; //End SkyBox Vertex Formats

  CGRSkyBox(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRSkyBox();
  void Release();

  void SetTextures(LPCSTR ZNeg,LPCSTR XNeg,LPCSTR ZPos,LPCSTR XPos,LPCSTR YNeg,LPCSTR YPos);
  HRESULT CreateBox(float fWidth,float fHeight,float fDepth);
  void SetTransformCoord(D3DXVECTOR3 CamPos) { VecPos = CamPos; }
  void Render();
protected:
  //Protected Directives
  LPDIRECT3DDEVICE9       d_pDevice;                   //Pointer On Direct3D Device Interface
  IDirect3DVertexBuffer9* m_pVertexBuffer;             //The Vertex Buffer
  IDirect3DIndexBuffer9*  m_pIndexBuffer;              //The Index Buffer
  IDirect3DTexture9*      m_pTex[6];                   //Variable Keep Same Textures
  LPCSTR                  m_TexPath[6];                //Variable Keep Texture Path
  float                   m_pWidth,m_pHeight,m_pDepth; //Variables Keep SkyBox WHD Size
  D3DXVECTOR3             VecPos;                      //Camera Position Vector In Space
};
#endif //!defined(AFX_SKYBOX_H__)

//-----------------------------------------------------------------------------