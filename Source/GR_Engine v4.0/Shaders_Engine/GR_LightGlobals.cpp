/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_LightGlobals.cpp
// It's The GreenRay Lighting Shader Engine.
// Made Specially For Objects That Are Shared All Over The Place
// Date Creation: 29 August 2005
// Last Changing: 21 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_LightGlobals.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRLightGlobals Class
///////////////////////////////////////////////////////////////////////////////
CGRLightGlobals::CGRLightGlobals()
{
  //Set Default Parameters
  m_EffectPool = NULL;
  m_LightEffectPool = NULL;
  m_pD3D = NULL;
  m_pD3DDevice = NULL; 
  m_Framebuffer = NULL;
} //EndConstructionDirectives

CGRLightGlobals::~CGRLightGlobals()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRLightGlobals::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRLightGlobals::Release()
{
  m_EffectPool = NULL;
  m_LightEffectPool = NULL;
  m_Framebuffer = NULL;
  m_pD3DDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------