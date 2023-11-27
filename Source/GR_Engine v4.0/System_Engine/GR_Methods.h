/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Methods.h
// It's The GreenRay Direct3D Useful Control Interfaces Engine Header File.
// Made Specially For Implementing Useful Engine System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 02 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>

////////////////////////////////////////////////////////////////////////////////
// Directives For Release Classes
////////////////////////////////////////////////////////////////////////////////
//Directive For Release Class
#define ReleaseClass(_Ptr) { if (_Ptr) { (_Ptr)->Release(); (_Ptr) = NULL; } }
//Directive For Delete Class
#define DeleteClass(_Ptr) { if (_Ptr) { delete(_Ptr); (_Ptr) = NULL; } }
//Directive For Delete Array
#define DeleteArray(_Ptr) { if (_Ptr) { delete[](_Ptr); (_Ptr) = NULL; } }

////////////////////////////////////////////////////////////////////////////////
// Conversions Definitions
////////////////////////////////////////////////////////////////////////////////
//#ifndef NOMINMAX
#ifndef Max
#define Max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef Min
#define Min(a,b) (((a) < (b)) ? (a) : (b))
#endif
//#endif  

///////////////////////////////////////////////////////////////////////////////
// Exporting Procedures And Functions
///////////////////////////////////////////////////////////////////////////////
void Log(LPCSTR Str); //Log Event Procedure
bool LoadXFile(LPDIRECT3DDEVICE9 Device,char* MeshFile,ID3DXMesh* &Mesh);

////////////////////////////////////////////////////////////////////////////////
// Directives Or Helper Functions Or Procedures
////////////////////////////////////////////////////////////////////////////////
DWORD FloatToDWord(FLOAT F);

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRDeltaTimer;
//class CGRTimer;
//class CGRFPSTimer;

//-----------------------------------------------------------------------------
// Name: Struct CGRDeltaTimer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_DELTATIMER_H__)
#define AFX_DELTATIMER_H__

class CGRDeltaTimer
{ 
public:
  //Public Directives
  CGRDeltaTimer();
  virtual ~CGRDeltaTimer();
  void UpdateTimer();
  float GetTime();
private:
  //Private Directives
  DWORD CurrentTime; //Current Timer Value
  DWORD LastTime;    //Previous Timer Value
  float DeltaTime;   //Time Elapsed Since Last Frame
};
#endif //!defined(AFX_DELTATIMER_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRTimer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_TIMER_H__)
#define AFX_TIMER_H__

class CGRTimer
{ 
public:
  //Public Directives  
  CGRTimer();
  virtual ~CGRTimer();
  
  void Reset();             //Resets The Timer
  void Start();             //Starts The Timer
  void Stop();              //Stop (Or Pause) The Timer
  void Advance();           //Advance The Timer By 0.1 Seconds
  double GetAbsoluteTime(); //Get The Absolute System Time
  double GetTime();         //Get The Current Time
  double GetElapsedTime();  //Get The Time That Elapsed Between GetElapsedTime() Calls
  bool IsStopped();         //Returns True If Timer Stopped
private:
  //Private Directives
protected:
  //Protected Directives
  bool m_bUsingQPF;
  bool m_bTimerStopped;
  LONGLONG m_llQPFTicksPerSec;
  LONGLONG m_llStopTime;
  LONGLONG m_llLastElapsedTime;
  LONGLONG m_llBaseTime;
};
#endif //!defined(AFX_TIMER_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRFPSTimer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_FPSTIMER_H__)
#define AFX_FPSTIMER_H__

class CGRFPSTimer
{ 
public:
  //Public Directives
  CGRFPSTimer();
  virtual ~CGRFPSTimer();

  void Start();
  void Stop();
  void Update();

  BOOL IsStopped() { return m_bTimerStopped; }
  float GetFPS() { return m_FPS; }
  float GetRunningTime() { return m_fRunningTime; }
  float GetElapsedTime() { return m_bTimerStopped ? 0.0f : m_fTimeElapsed; }
private:
  //Private Directives
  INT64 m_TicksPerSecond;
  INT64 m_CurrentTime;
  INT64 m_LastTime;
  INT64 m_LastFPSUpdate;
  INT64 m_FPSUpdateInterval;
  UINT m_NumFrames;
  float m_fRunningTime;
  float m_fTimeElapsed;
  float m_FPS;
  BOOL m_bTimerStopped;
};
#endif //!defined(AFX_FPSTIMER_H__)

//-----------------------------------------------------------------------------