/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Label.cpp
// It's The GreenRay Label Realise Method Engine.
// Made Specially For Implementing To Output Text Information On The Screen.
// Date Creation: 29 August 2005
// Last Changing: 08 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Label.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRLabel Class
///////////////////////////////////////////////////////////////////////////////
CGRLabel::CGRLabel(LPDIRECT3DDEVICE9 pDevice)
{
  //Set Default Parameters
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pFont = NULL;

  //Set Default Font Parameters
  m_Color =   clBlack;         //Set Font Color
  FontName =  "MS Sans Serif"; //Set Font Name
  FontSize =  8;               //Set Font Size
  FontStyle = FS_NORMAL;       //Set Font Style
  Left =      10;
  Top =       10; 

} //EndConstructionDirectives

CGRLabel::~CGRLabel()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRLabel::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLabel::Release()
{
  if (m_pFont != NULL) 
  {
    ReleaseClass(m_pFont);
	DeleteClass(m_pFont);
  }
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRLabel::Invalidate()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLabel::Invalidate()
{
  //If Founded Early Created Font Then Release Him And Prepare New 
  Release();

  HDC DC = GetDC(0);
  int nLogPixelsY = GetDeviceCaps(DC,LOGPIXELSY);
  ReleaseDC(0,DC);
 
  int Cap_Size = -FontSize*nLogPixelsY/72; //Set Font Size
 
  //Create Direct3D Font
  if (FAILED(D3DXCreateFont(d_pDevice,Cap_Size,0,FontStyle,0,FALSE,DEFAULT_CHARSET,
  OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,FontName,&m_pFont))) 
  {
	Log("GR_Label.cpp: Can't Create D3DXFont \n");
    return;
  }
} //EndInvalidateProcedure

//-----------------------------------------------------------------------------
// Name: CGRLabel::Caption()
// Desc: Draw And Display Text
//-----------------------------------------------------------------------------
void CGRLabel::Caption(LPCSTR Str)
{
  RECT OutRect;
  
  //Set Text Position
  OutRect.left = Left;
  OutRect.top = Top;
  OutRect.right = 0;
  OutRect.bottom = 0;

  //Output Text
  if (m_pFont != NULL) m_pFont->DrawText(NULL,Str,-1,&OutRect,DT_NOCLIP,m_Color);
} //EndCaptionProcedure

//-----------------------------------------------------------------------------