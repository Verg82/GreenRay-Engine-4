/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Fps.cpp
// It's The GreenRay FPS Count Realise Method Engine.
// Made Specially For Implementing To Output FPS Information On The Screen.
// Date Creation: 29 August 2005
// Last Changing: 08 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Fps.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRFPS Class
///////////////////////////////////////////////////////////////////////////////
CGRFPS::CGRFPS(LPDIRECT3DDEVICE9 pDevice)
{
  //Set Sefault Parameters
  d_pDevice = pDevice;
  m_pFPSTimer = NULL; //FPS Timer
  m_lblFPS = NULL;    //FPS Label

  //Create FPS Timer Class
  m_pFPSTimer = new CGRFPSTimer(); //Construct CGRFPSTimer Class
  m_pFPSTimer->Start();

  //Create FPS Label Class
  m_lblFPS = new CGRLabel(d_pDevice); //Construct CGRLabel Class  
  m_lblFPS->SetColor(clYellow);
  m_lblFPS->Invalidate();
} //EndConstructionDirectives

CGRFPS::~CGRFPS()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRFPS::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRFPS::Release()
{
  //Release FPS Timer
  if (m_pFPSTimer != NULL) DeleteClass(m_pFPSTimer); 
 
  //Release FPS Label
  if (m_lblFPS != NULL) 
  { 
    ReleaseClass(m_lblFPS); 
	DeleteClass(m_lblFPS); 
  }
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRFPS::SetFPSPos()
// Desc: 
//-----------------------------------------------------------------------------
void CGRFPS::SetFPSPos(int X,int Y)
{
  if (m_lblFPS != NULL) 
  {
    m_lblFPS->Left = X;
    m_lblFPS->Top = Y;
  }
} //EndSetFPSPosProcedure

//-----------------------------------------------------------------------------
// Name: CGRFPS::UpdateFPS()
// Desc: 
//-----------------------------------------------------------------------------
void CGRFPS::UpdateFPS()
{
  if (m_pFPSTimer != NULL)
  {
    m_pFPSTimer->Update();
    sprintf(m_FPS,"FPS %.2f",m_pFPSTimer->GetFPS());
	if (m_lblFPS != NULL) m_lblFPS->Caption(m_FPS);
  }
} //EndUpdateFPSProcedure

//----------------------------------------------------------------------------