/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Methods.cpp
// It's The GreenRay Direct3D Useful Control Interfaces Engine Source File.
// Made Specially For Implementing Useful Engine System Manipulations Parameters.
// Date Creation: 29 August 2005
// Last Changing: 02 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Methods.h>

//-----------------------------------------------------------------------------
// Name: Log()
// Desc: Log Event Procedure
//-----------------------------------------------------------------------------
void Log(LPCSTR Str)
{ 
  FILE *LogFile = fopen("GREngine.log","a"); //Open File For Append Mode
  if (LogFile == NULL) return;
  fprintf(LogFile,Str);
  //fputs(Str,LogFile);
  fclose(LogFile); 
} //EndLogProcedure

//-----------------------------------------------------------------------------
// Name: LoadXFile()
// Desc: Load A Mesh X-File Format
//-----------------------------------------------------------------------------
bool LoadXFile(LPDIRECT3DDEVICE9 Device,char* MeshFile,ID3DXMesh* &Mesh)
{
  //Zero Mesh And Create Buffer
  Mesh = 0;
  ID3DXBuffer* MeshBuffer = 0;

  //Load And Optimize The Mesh                 !!!
  if (FAILED(D3DXLoadMeshFromX(MeshFile,D3DXMESH_32BIT|D3DXMESH_MANAGED,Device,&MeshBuffer,0,0,0,&Mesh)))
  return false;

  Mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT|
  D3DXMESHOPT_VERTEXCACHE,(DWORD*)MeshBuffer->GetBufferPointer(),0,0,0);

  //Release And Zero The Buffer
  MeshBuffer->Release();
  
  return true;
} //EndLoadXFileFunction

//-----------------------------------------------------------------------------
// Name: FloatToDWord()
// Desc: Helper Function To Stuff A FLOAT Into A DWORD Argument
//-----------------------------------------------------------------------------
DWORD FloatToDWord(FLOAT F) 
{ 
  return *((DWORD*)&F); 
} //EndFloatToDWordFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRDeltaTimer Class
///////////////////////////////////////////////////////////////////////////////
CGRDeltaTimer::CGRDeltaTimer()
{
} //EndConstructionDirectives

CGRDeltaTimer::~CGRDeltaTimer()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRDeltaTimer::UpdateTimer()
// Desc: Execute Each Frame To Establish Time Base
//-----------------------------------------------------------------------------
void CGRDeltaTimer::UpdateTimer()
{
  CurrentTime = timeGetTime();
  DeltaTime = (float)((CurrentTime-LastTime)*0.001f);
  LastTime = CurrentTime;
} //EndUpdateTimerProcedure

//-----------------------------------------------------------------------------
// Name: CGRDeltaTimer::GetTime()
// Desc:
//-----------------------------------------------------------------------------
float CGRDeltaTimer::GetTime()
{
  return DeltaTime;
} //EndGetTimeFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRTimer Class
///////////////////////////////////////////////////////////////////////////////
CGRTimer::CGRTimer()
{
  m_bUsingQPF         = false;
  m_bTimerStopped     = true;
  m_llQPFTicksPerSec  = 0;

  m_llStopTime        = 0;
  m_llLastElapsedTime = 0;
  m_llBaseTime        = 0;

  //Use QueryPerformanceFrequency() To Get Frequency Of Timer.  
  LARGE_INTEGER qwTicksPerSec;
  m_bUsingQPF = (bool)(QueryPerformanceFrequency(&qwTicksPerSec) != 0);
  m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
} //EndConstructionDirectives

CGRTimer::~CGRTimer()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRTimer::Reset()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTimer::Reset()
{
  if (!m_bUsingQPF) return;

  //Get Either The Current Time Or The Stop Time
  LARGE_INTEGER qwTime;
  if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
  else QueryPerformanceCounter(&qwTime);

  m_llBaseTime = qwTime.QuadPart;
  m_llLastElapsedTime = qwTime.QuadPart;
  m_llStopTime = 0;
  m_bTimerStopped = FALSE;
} //EndResetProcedure

//-----------------------------------------------------------------------------
// Name: CGRTimer::Start()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTimer::Start()
{
  if (!m_bUsingQPF) return;

  //Get The Current Time
  LARGE_INTEGER qwTime;
  QueryPerformanceCounter(&qwTime);

  if (m_bTimerStopped) m_llBaseTime+= qwTime.QuadPart-m_llStopTime;
  m_llStopTime = 0;
  m_llLastElapsedTime = qwTime.QuadPart;
  m_bTimerStopped = FALSE;
} //EndStartProcedure

//-----------------------------------------------------------------------------
// Name: CGRTimer::Stop()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTimer::Stop()
{
  if (!m_bUsingQPF) return;

  if (!m_bTimerStopped)
  {
    //Get Either The Current Time Or The Stop Time
    LARGE_INTEGER qwTime;
    if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
    else QueryPerformanceCounter(&qwTime);

    m_llStopTime = qwTime.QuadPart;
    m_llLastElapsedTime = qwTime.QuadPart;
    m_bTimerStopped = TRUE;
  }
} //EndStopProcedure

//-----------------------------------------------------------------------------
// Name: CGRTimer::Advance()
// Desc: 
//-----------------------------------------------------------------------------
void CGRTimer::Advance()
{
  if (!m_bUsingQPF) return;
  m_llStopTime+= m_llQPFTicksPerSec/10;
} //EndAdvanceProcedure

//-----------------------------------------------------------------------------
// Name: CGRTimer::GetAbsoluteTime()
// Desc: 
//-----------------------------------------------------------------------------
double CGRTimer::GetAbsoluteTime()
{
  if (!m_bUsingQPF) return -1.0;

  //Get Either The Current Time Or The Stop Time
  LARGE_INTEGER qwTime;
  if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
  else QueryPerformanceCounter(&qwTime);

  double fTime = qwTime.QuadPart/(double) m_llQPFTicksPerSec;
  return fTime;
} //EndGetAbsoluteTimeFunction

//-----------------------------------------------------------------------------
// Name: CGRTimer::GetTime()
// Desc: 
//-----------------------------------------------------------------------------
double CGRTimer::GetTime()
{
  if (!m_bUsingQPF) return -1.0;

  //Get Either The Current Time Or The Stop Time
  LARGE_INTEGER qwTime;
  if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
  else QueryPerformanceCounter(&qwTime);

  double fAppTime = (double)(qwTime.QuadPart-m_llBaseTime)/(double)m_llQPFTicksPerSec;
  return fAppTime;
} //EndGetTimeFunction

//-----------------------------------------------------------------------------
// Name: CGRTimer::GetElapsedTime()
// Desc: 
//-----------------------------------------------------------------------------
double CGRTimer::GetElapsedTime()
{
  if (!m_bUsingQPF) return -1.0;

  //Get Either The Current Time Or The Stop Time
  LARGE_INTEGER qwTime;
  if (m_llStopTime != 0) qwTime.QuadPart = m_llStopTime;
  else QueryPerformanceCounter(&qwTime);

  double fElapsedTime = (double)(qwTime.QuadPart-m_llLastElapsedTime)/(double)m_llQPFTicksPerSec;
  m_llLastElapsedTime = qwTime.QuadPart;

  return fElapsedTime;
} //EndGetElapsedTimeFunction

//-----------------------------------------------------------------------------
// Name: CGRTimer::IsStopped()
// Desc: 
//-----------------------------------------------------------------------------
bool CGRTimer::IsStopped()
{
  return m_bTimerStopped;
} //EndIsStoppedFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRFPSTimer Class
///////////////////////////////////////////////////////////////////////////////
CGRFPSTimer::CGRFPSTimer()
{
  //Set Default Parameters
  QueryPerformanceFrequency((LARGE_INTEGER*)&m_TicksPerSecond);

  m_CurrentTime = m_LastTime = m_LastFPSUpdate = 0;
  m_NumFrames = 0;
  m_fRunningTime = m_fTimeElapsed = m_FPS = 0.0f;
  m_FPSUpdateInterval = m_TicksPerSecond >> 1;
  m_bTimerStopped = TRUE;
} //EndConstructionDirectives

CGRFPSTimer::~CGRFPSTimer()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRFPSTimer::Start()
// Desc: 
//-----------------------------------------------------------------------------
void CGRFPSTimer::Start()
{
  if (!m_bTimerStopped)
  {
    //Already Started
    return;
  }
  QueryPerformanceCounter((LARGE_INTEGER*)&m_LastTime);
  m_bTimerStopped = FALSE;
} //EndStartProcedure

//-----------------------------------------------------------------------------
// Name: CGRFPSTimer::Stop()
// Desc: 
//-----------------------------------------------------------------------------
void CGRFPSTimer::Stop()
{
  if (m_bTimerStopped)
  {
    //Already Stopped
    return;
  }
  INT64 StopTime = 0;
  QueryPerformanceCounter((LARGE_INTEGER*)&StopTime);
  m_fRunningTime+= (float)(StopTime-m_LastTime)/(float)m_TicksPerSecond;
  m_bTimerStopped = TRUE;
} //EndStopProcedure

//-----------------------------------------------------------------------------
// Name: CGRFPSTimer::Update()
// Desc: Updates The Timer. Calculates The Time Elapsed Since The Last Update Call.
// Updates The Frames Per Second And Updates The Total Running Time. 
//-----------------------------------------------------------------------------
void CGRFPSTimer::Update()
{
  if (m_bTimerStopped) { return; }

  //Get The Current Time
  QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTime);
    
  m_fTimeElapsed = (float)(m_CurrentTime-m_LastTime)/(float)m_TicksPerSecond;
  m_fRunningTime+= m_fTimeElapsed;

  //Update FPS
  m_NumFrames++;
  if (m_CurrentTime-m_LastFPSUpdate >= m_FPSUpdateInterval)
  {
	float CurrentTime = (float)m_CurrentTime/(float)m_TicksPerSecond;
    float LastTime = (float)m_LastFPSUpdate/(float)m_TicksPerSecond;
    m_FPS = (float)m_NumFrames/(CurrentTime-LastTime);

    m_LastFPSUpdate = m_CurrentTime;
    m_NumFrames = 0;
  }
  m_LastTime = m_CurrentTime;
} //EndUpdateProcedure

//-----------------------------------------------------------------------------