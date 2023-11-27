/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Fps.h
// It's The GreenRay FPS Count Realise Method Engine.
// Made Specially For Implementing To Output FPS Information On The Screen.
// Date Creation: 29 August 2005
// Last Changing: 08 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_Label.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRFPS;

//-----------------------------------------------------------------------------
// Name: Struct CGRFPS
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRFPS_H__)
#define AFX_CGRFPS_H__

class CGRFPS
{ 
public:
  //Public Directives
  CGRFPS(LPDIRECT3DDEVICE9 pDevice);
  virtual ~CGRFPS();
  void Release();
  void UpdateFPS();
  void SetFPSPos(int X,int Y);
private:
  //Private Directives
  LPDIRECT3DDEVICE9 d_pDevice;   //Pointer On Direct3D Device Interface
  CGRFPSTimer*      m_pFPSTimer; //GreenRay FPS Timer Counter
  CGRLabel*         m_lblFPS;    //GreenRay FPS Output Label
  char              m_FPS[10];   //FPS Text
};
#endif //!defined(AFX_CGRFPS_H__)

//----------------------------------------------------------------------------