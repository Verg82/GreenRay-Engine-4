/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Interface.h
// It's The GreenRay User Interface Engine.
// Made Specially For Implementing User Interface.
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_Shaders.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRInterface;

//-----------------------------------------------------------------------------
// Name: Struct CGRInterface
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRInterface_H__)
#define AFX_CGRInterface_H__

//Define A FVF For Our Interface Layer
#define LAYER_D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CGRInterface
{ 
public:
  //Public Directives
  
  //Define A Custom Vertex For Our UI Layer
  struct LAYER_VERTEX
  {
    FLOAT x,y,z; //Position Of Vertex In Space
	FLOAT tu,tv; //Texture Coordinates
  };

  CGRInterface(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRInterface();
  void Release();

  void SetTop(int Top) { m_nTop = Top; }
  void SetLeft(int Left) { m_nLeft = Left; }
  void SetWidth(int Width) { m_nWidth = Width; }
  void SetHeight(int Height) { m_nHeight = Height; }
  void SetOpacityLevel(int Opacity);
  void CreateTexture(LPCSTR FileName,DWORD dwKeyColor);
  bool GetInplace(int x,int y); 
  void Render(D3DXMATRIX Projection);
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  LPDIRECT3DVERTEXDECLARATION9 m_pUIVertexDecl; //Vertex Declaration Interface
  LPD3DXEFFECT m_pUIEffect; //Effect For Visualize User Interface 
  D3DXHANDLE m_pUITechHandle; //Effect Technique Handle
  LPDIRECT3DVERTEXBUFFER9 m_pUIVertexBuffer; //Vertex Buffer For UI 
  LPDIRECT3DTEXTURE9 m_pUITexture; //Texture UI Interface

  float m_fOpacity; //Set Opacity Level
  int m_nWidth;     //Width Of Interface Panel
  int m_nHeight;    //Height Of Interface Panel
  int m_nTop;       //Top Position Of Interface Panel
  int m_nLeft;      //Left Position Of Interface Panel

  HRESULT CreateVertexBuffer(); //It's For Create UI Panel Vertex Buffer
};
#endif //!defined(AFX_CGRInterface_H__)

//-----------------------------------------------------------------------------