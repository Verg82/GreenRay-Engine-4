/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_TnLLighting.cpp
// It's The GreenRay Direct3D Lighting Methods.
// Made Specially For Implementing T&L Lighting Methods.
// Date Creation: 29 August 2005
// Last Changing: 31 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_TnLLighting.h>

//-----------------------------------------------------------------------------
// Name: InitMaterial()
// Desc: Initializes A D3DMATERIAL9 Structure, Setting The Diffuse And Ambient
//       Colors. It Does Not Set Emissive Or Specular Colors
//-----------------------------------------------------------------------------
void InitMaterial(D3DMATERIAL9& Mtrl,float R,float G,float B,float A)
{
  ZeroMemory(&Mtrl,sizeof(D3DMATERIAL9));
  Mtrl.Diffuse.r = Mtrl.Ambient.r = R;
  Mtrl.Diffuse.g = Mtrl.Ambient.g = G;
  Mtrl.Diffuse.b = Mtrl.Ambient.b = B;
  Mtrl.Diffuse.a = Mtrl.Ambient.a = A;
} //EndInitMaterialProcedure

//-----------------------------------------------------------------------------
// Name: RenderHardwareTnL()
// Desc: Render Hardware Transform And Lighting Effect (T&L)
//-----------------------------------------------------------------------------
void RenderHardwareTnL(LPDIRECT3DDEVICE9 pDevice,D3DXVECTOR3 VecNormal,int LightNumber)
{
  if (LightNumber == 0 || LightNumber == 1) 
  {
    D3DMATERIAL9 Material; //Material
    D3DLIGHT9 Light; //Light
  
    //Setting Up Material
    //                     R    G    B    A
	InitMaterial(Material,1.0f,1.0f,0.99f,1.0f);
    pDevice->SetMaterial(&Material);

    //Setup Light
    ZeroMemory(&Light,sizeof(D3DLIGHT9));
    Light.Type       = D3DLIGHT_DIRECTIONAL;
    Light.Diffuse.r  = 1.0f;
    Light.Diffuse.g  = 1.0f;
    Light.Diffuse.b  = 1.0f;
    Light.Range      = 10000.0f;
 
    //Light.Direction = D3DXVECTOR3(0.5f, -0.8f, 0.7f);
    Light.Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

    //Setup Normals
    D3DXVECTOR3 VectorDir;
    VectorDir = VecNormal,D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction,&VectorDir);

    //Render Light
    pDevice->SetLight(LightNumber,&Light);
    pDevice->LightEnable(LightNumber,TRUE);
    pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
    pDevice->SetRenderState(D3DRS_AMBIENT,0);
  }

  if (LightNumber == 2) 
  {
	pDevice->LightEnable(1,FALSE);
    pDevice->LightEnable(0,FALSE);
	//pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
  }
} //EndRenderHardwareTnLProcedure

//-----------------------------------------------------------------------------