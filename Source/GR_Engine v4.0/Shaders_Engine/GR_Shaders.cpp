/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Shaders.cpp
// It's The GreenRay Shader Engine.
// Made Specially For Implementing Shader Effects.
// Date Creation: 29 August 2005
// Last Changing: 03 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Shaders.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRShaders Class
///////////////////////////////////////////////////////////////////////////////
CGRShaders::CGRShaders()
{

} //EndConstructionDirectives

CGRShaders::~CGRShaders()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRShaders::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CGRShaders::Release()
{

} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CreateVertDeclaration()
// Desc: Universal Function For Create And Prepare Vertex Declarations
//-----------------------------------------------------------------------------
HRESULT CreateVertDeclaration(LPDIRECT3DDEVICE9 pDevice,IDirect3DVertexDeclaration9** Declaration,D3DVERTEXELEMENT9* Decl)
{
  HRESULT Result;

  //Create Vertex Declaration
  if (Declaration != NULL)
  if (FAILED(Result = pDevice->CreateVertexDeclaration(Decl,Declaration)))
  {
	Log("GR_Shaders.cpp: Can't Create Vertex Declaration.. \n");
    return Result;
  }

  return Result;  
} //EndCreateVertDeclarationFunction

//-----------------------------------------------------------------------------
// Name: LoadShaderFile()
// Desc: Loads Pixel Shaders
//-----------------------------------------------------------------------------
HRESULT LoadShaderFile(LPDIRECT3DDEVICE9 pDevice,LPCSTR FileName,LPD3DXEFFECT* Effect)
{
  LPD3DXBUFFER ErrorMessages;
  LPD3DXEFFECT TempEffect;
  HRESULT Result;
  
  //Check On PixelShader 2.0 Present Parameters
  D3DCAPS9 _Caps;
  pDevice->GetDeviceCaps(&_Caps);
  if (FAILED(_Caps.PixelShaderVersion < D3DPS_VERSION(2,0)))
  {
    Log("GR_Shaders.cpp: Your Video Card Not Support Pixel Shader Version 2.0..\n");
  }
  else 
  {
    if(FAILED(Result = D3DXCreateEffectFromFile(pDevice,FileName,NULL,NULL,0,NULL,&TempEffect,&ErrorMessages)))
    {
      Log("GR_Shaders.cpp: Could Not Create An HLSL Pixel Shader File.. \n");      
      return Result;
    }
  }

  *Effect = TempEffect;
  return Result;  
} //EndLoadShaderFileFunction

//-----------------------------------------------------------------------------