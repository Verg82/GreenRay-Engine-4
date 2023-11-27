/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Label.h: Interface For The CGRLabel Class.
// It's The GreenRay Label Realise Method Engine.
// Made Specially For Implementing To Output Text Information On The Screen.
// Date Creation: 29 August 2005
// Last Changing: 08 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>

///////////////////////////////////////////////////////////////////////////////
// External Definitions Or Prototypes
///////////////////////////////////////////////////////////////////////////////
//Font Style Definitions
#define FS_NORMAL 0
#define FS_BOLD   1
#define FS_ITALIC 2
#define FS_BETA   3

//Font Color Definitions
#define clBlack  D3DCOLOR_XRGB(0,0,0)       //0x000000
#define clWhite  D3DCOLOR_XRGB(255,255,255) //0xFFFFFF
#define clRed    D3DCOLOR_XRGB(255,0,0)     //0xFF0000
#define clGreen  D3DCOLOR_XRGB(0,255,0)     //0x00FF00
#define clBlue   D3DCOLOR_XRGB(0,0,255)     //0x0000FF
#define clYellow D3DCOLOR_XRGB(255,255,50)  //5

///////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
///////////////////////////////////////////////////////////////////////////////
//class CGRLabel;

//----------------------------------------------------------------------------
// Name: Struct CGRLabel
// Desc: Implementation Of The Text Out System Object
//----------------------------------------------------------------------------
#if !defined(AFX_LABEL_H__)
#define AFX_LABEL_H__

class CGRLabel
{ 
public:
  //Public Directives
  CGRLabel(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRLabel();

  void Release();
  void Invalidate();
  void Caption(LPCSTR Str);
public:
  //Label Properties  
  int FontSize;
  
  LPCSTR FontName;
  DWORD FontStyle;
  int Left;
  int Top;

  void SetColor(DWORD Color) { m_Color = Color; } //Set Font Color
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice; //Pointer On Direct3D Device Interface
  ID3DXFont*        m_pFont;   //General Direct3D Font Interface
  DWORD             m_Color;   //Text Color
};
#endif //!defined(AFX_LABEL_H__)

//----------------------------------------------------------------------------