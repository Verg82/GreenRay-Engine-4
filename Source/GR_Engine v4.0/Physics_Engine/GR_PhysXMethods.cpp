/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_PhysXMethods.cpp
// It's PhysX Useful Control Interfaces Engine Header File.
// Made Specially For Implementing Physics Engine Helpers And System Parameters.
// Date Creation: 29 August 2005
// Last Changing: 30 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_PhysXMethods.h>

//-----------------------------------------------------------------------------
// Name: NxVec3ToDXVec3()
// Desc: Convert A PhysX Vec3 To A DirectX Vec3
//-----------------------------------------------------------------------------
D3DXVECTOR3 NxVec3ToDXVec3(NxVec3 PhysXVec3)
{
  return D3DXVECTOR3(PhysXVec3.x,PhysXVec3.y,PhysXVec3.z);
} //EndNxVec3ToDXVec3Function

//-----------------------------------------------------------------------------