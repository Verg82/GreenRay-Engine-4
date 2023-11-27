/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_TnLLighting.h
// It's The GreenRay Direct3D Lighting Methods Header File.
// Made Specially For Implementing T&L Lighting Methods.
// Date Creation: 29 August 2005
// Last Changing: 31 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>

///////////////////////////////////////////////////////////////////////////////
// Exporting Procedures And Functions
///////////////////////////////////////////////////////////////////////////////
void InitMaterial(D3DMATERIAL9& Mtrl,float R,float G,float B,float A); //Setting Up Matherial Directives
void RenderHardwareTnL(LPDIRECT3DDEVICE9 pDevice,D3DXVECTOR3 VecNormal,int LightNumber);

//-----------------------------------------------------------------------------