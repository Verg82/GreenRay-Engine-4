/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// Nx_Cooking.h: Directives From The Novodex Class.
// It's The GreenRay Physics Engine.
// Made Specially For Implementing Physics And Collision Detection And Other.
// Date Creation: 29 August 2005
// Last Changing: 21 January 2007
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
*******************************************************************************/
#pragma once
#include <GR_System.h>

//-----------------------------------------------------------------------------
class NxPMap;
class NxTriangleMesh;
class NxUserOutputStream;

bool HasCookingLibrary(); //Check To See If The Cooking Library Is Available Or Not!
bool InitCooking(NxUserAllocator* allocator = NULL,NxUserOutputStream* outputStream = NULL);
void CloseCooking();
bool CookConvexMesh(const NxConvexMeshDesc &desc,NxStream &stream);
bool CookClothMesh(const NxClothMeshDesc &desc,NxStream &stream);
bool CookTriangleMesh(const NxTriangleMeshDesc &desc,NxStream &stream);
bool CreatePMap(NxPMap &pmap,const NxTriangleMesh &mesh,NxU32 density,NxUserOutputStream* outputStream = NULL);
bool ReleasePMap(NxPMap &pmap);

//-----------------------------------------------------------------------------