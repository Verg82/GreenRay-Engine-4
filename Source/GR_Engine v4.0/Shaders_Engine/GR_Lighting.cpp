/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Lighting.cpp
// It's The GreenRay Lighting Shader Engine.
// Implementation Of Base Lights Code Shared By Sub-Classes.
// Date Creation: 29 August 2005
// Last Changing: 17 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Lighting.h>

//-----------------------------------------------------------------------------
// Name: Bias()
// Desc: 
//-----------------------------------------------------------------------------
inline float Bias(float x)
{
  return (x*0.5f)+0.5f;
} //EndBiasFunction

//-----------------------------------------------------------------------------
// Name: UnBias()
// Desc: 
//-----------------------------------------------------------------------------
inline float UnBias(float x)
{
  return (x*2.f)-1.f;
} //EndUnBiasFunction

//-----------------------------------------------------------------------------
// Name: GetDotProductFunctionU()
// Desc: 
//-----------------------------------------------------------------------------
float GetDotProductFunctionU(enum DOTP_FUNCTIONS Func)
{
  float u = (float(Func)*float(BILERP_PROTECTION))+float(BILERP_PROTECTION)/2;

  const float spix = 1.f/(float(NUM_DOTP_FUNCTIONS)*BILERP_PROTECTION);
  
  u*= spix;
  return u;
} //EndGetDotProductFunctionUFunction

//-----------------------------------------------------------------------------
// Name: MaterialIndexToV()
// Desc: 
//-----------------------------------------------------------------------------
float MaterialIndexToV(unsigned int Material)
{
  return float(Material)/float(NUM_MATERIALS);
} //EndMaterialIndexToVFunction

//-----------------------------------------------------------------------------
// Name: AttenuationIndexToV()
// Desc: 
//-----------------------------------------------------------------------------
float AttenuationIndexToV(enum ATTENUATION_FUNCTION Func)
{
  float u = (float(Func)*float(BILERP_PROTECTION))+float(BILERP_PROTECTION)/2;
  const float spix = 1.f/(float(NUM_ATTENUATION_FUNCTIONS)*BILERP_PROTECTION);
  u*= spix;
  return u;
} //EndAttenuationIndexToVFunction

//-----------------------------------------------------------------------------
// Name: RegisterMaterial()
// Desc: 
//-----------------------------------------------------------------------------
std::vector< Material > MaterialList;
unsigned int RegisterMaterial(const Material& Mat)
{
  MaterialList.push_back(Mat);
  return MaterialList.size()-1;
} //EndRegisterMaterialFunction

//-----------------------------------------------------------------------------
// Name: EmptyMaterialList()
// Desc: 
//-----------------------------------------------------------------------------
void EmptyMaterialList()
{
  MaterialList.clear();

  //Add The Default Materials 
  MaterialList.push_back(Material(0,0,1));
  //Ambient Only
  MaterialList.push_back(Material(1,0,1));
  //Emmisive Only
  MaterialList.push_back(Material(0,1,1));
  //Specular (No Ambient Or Emmisive)
  MaterialList.push_back(Material(0,0,1));
  //Metal Specular (No Ambient Slight Emmsive)
  MaterialList.push_back(Material(0,0.1f,0));
} //EndEmptyMaterialListProcedure

//-----------------------------------------------------------------------------
// Name: FillMaterialTexture()
// Desc: The Material Texture Takes The Single Index Stored In The Fat Framebuffer 
// To The Parameters Used To Select The Various Components Of The Light Model
//-----------------------------------------------------------------------------
void WINAPI FillMaterialTexture(D3DXVECTOR4* pOut, 
const D3DXVECTOR2* pTexCoord,const D3DXVECTOR2* pTexelSize,LPVOID pData)
{
  UNREFERENCED_PARAMETER(pData);
  UNREFERENCED_PARAMETER(pTexelSize);

  //pTexCoord->x = u = Material Item (1 Of The 4 Items)
  //pTexCoord->y = u = Material Number
  const unsigned int Index = unsigned int(pTexCoord->x*NUM_MATERIALS);
  const unsigned int MatNum = Index;

  if (MatNum < MaterialList.size())
  {
    pOut->x = 0; //Unused
	pOut->y = MaterialList[MatNum].KSpecCol;
	pOut->z = MaterialList[MatNum].KAmb;
	pOut->w = MaterialList[MatNum].KEmm;		
  } 
  else
  {
	*pOut = D3DXVECTOR4(0,0,0,0);
  }
} //EndFillMaterialTextureProcedure

//-----------------------------------------------------------------------------
// Name: FillDotProductTexture()
// Desc: 
//-----------------------------------------------------------------------------
void WINAPI FillDotProductTexture(D3DXVECTOR4* pOut, 
const D3DXVECTOR2* pTexCoord,const D3DXVECTOR2* pTexelSize,LPVOID pData)
{
  UNREFERENCED_PARAMETER(pData);
  UNREFERENCED_PARAMETER(pTexelSize);
  //pTexCoord->x Need Unbiasing To -1 To 1

  //pTexCoord->x = u = N dot L
  //pTexCoord->y = v = Material Select
  //pOut->x = Shaped Dot Product Result
  const unsigned int Index = unsigned int(pTexCoord->y *float(NUM_DOTP_FUNCTIONS)*float(BILERP_PROTECTION));
  const unsigned int MatNum = Index/BILERP_PROTECTION;
  float dot = UnBias(pTexCoord->x);

  //Each Dot Product Function Take 2 Lines In v
  //If Both Lines Are The Same The Function Is Stepped, Else You Can Use Bilerp To Give
  //You A Peiceiwise Linear Function Between Functions.
  //Dot Product Funcion 0 Is Texel.y 1, Function 1 = texel.y = 3
  //Sampling Should Take Place In The Middle Of Lines
  if (MatNum <= DPF_ONE)
  {
    switch (MatNum)
	{
	  case DPF_ZERO:
		   pOut->x = 0.0f;
		   pOut->y = 1.0f;
		   break;
	  case DPF_QUARTER:
		   pOut->x = 0.25f;
		   pOut->y = 1.0f;
		   break;
	  case DPF_HALF:
		   pOut->x = 0.5f;
		   pOut->y = 1.0f;
		   break;
	  case DPF_THREEQUARTER:
		   pOut->x = 0.75f;
		   pOut->y = 1.0f;
		   break;
	  case DPF_ONE:
		   pOut->x = 1.f;
		   pOut->y = 1.0f;
		   break;
	}
  } 
  //4 Dot Product Functions Are Stepped
  //Nil                 - Return 0 
  //Front Linear pow1	- Standard Lambert Diffuse (Back Culled)
  //Front Linear pow2	- if (dot < 0) Return 0 else Return	dot^2
  //Front Linear pow3	- if (dot < 0) Return 0 else Return	dot^3
  //Front Linear pow4	- if (dot < 0) Return 0 else Return	dot^4
  else if (MatNum <= DPF_FRONT_POW4)
  {
	//Backface Light
	dot = Max(dot,0.f);
	pOut->y = (dot <= 0) ? 0.f : 1.0f;
    float Power = float(MatNum-DPF_FRONT_POW1);
	Power += 1;
	pOut->x = powf(dot,Power);
  }
  //Next Dot Product Functions Are Stepped
  //Each One Is Front Facing Only And Has The Dot Raised
  //Power From 8 To 64 Every 4 Powers
  else if (MatNum <= DPF_FRONT_POW64)
  {
    //Backface Light
	dot = Max(dot,0.f);
	pOut->y = (dot <= 0) ? 0.f : 1.0f;

	float Power = float(MatNum-DPF_FRONT_POW8);
	Power = (Power*4)+8;
	pOut->x = powf(dot,Power);
  }
  //Next Dot Product Functions Are Stepped
  //Each One Is Front Facing Only And Has The Dot Raised
  //Power From 128 To 1024 Every 218 Powers
  else if (MatNum <= DPF_FRONT_POW1024)
  {
	//Backface Light
	dot = Max(dot,0.f);
	pOut->y = (dot <= 0) ? 0.f : 1.0f;

	float Power = float(MatNum-DPF_FRONT_POW128);
	Power = (Power*128)+128;
	pOut->x = powf(dot,Power);
  } 	
  else if (MatNum <= DPF_FRESNEL_POW4)
  {
    float Power = float(MatNum-DPF_FRESNEL_POW4);
	Power = (Power*4)+4;

	//Make The Dot Go 0 To 1 To 0 Rather Than -1 To 0 To 1
	if (dot < 0) dot= fabsf(dot+1);
	else dot= fabsf(dot-1);

	pOut->x = powf(dot,Power);
	pOut->y = 1.0f;

  } 	
  else if (MatNum <= DPF_SHIFTED_FRESNEL_POW8)
  {
	float Power = float(MatNum-DPF_SHIFTED_FRESNEL_POW8);
	Power = (Power*4)+8;

	dot = dot-0.4f;
	if (dot < -1) dot = -1;

	//Make The Dot Go 0 To 1 To 0 Rather Than -1 To 0 To 1
	if (dot < 0) dot= fabsf(dot+1);
	else dot= fabsf(dot-1);

	pOut->x = powf(dot,Power);
	pOut->y = 1.0f;
  }
} //EndFillDotProductTextureProcedure

//-----------------------------------------------------------------------------
// Name: FillAttenuationTexture()
// Desc: 
//-----------------------------------------------------------------------------
void WINAPI FillAttenuationTexture(D3DXVECTOR4* pOut, 
const D3DXVECTOR2* pTexCoord,const D3DXVECTOR2* pTexelSize,LPVOID pData)
{
  UNREFERENCED_PARAMETER(pData);
  UNREFERENCED_PARAMETER(pTexelSize);

  const unsigned int Index = unsigned int(pTexCoord->y*float(NUM_ATTENUATION_FUNCTIONS)*float(BILERP_PROTECTION));
  const unsigned int MatNum = Index/BILERP_PROTECTION;

  if (MatNum <= AF_ONE)
  {
    switch (MatNum)
	{
	  case AF_ZERO:
		   pOut->x = 0.0f;
		   break;
	  case AF_QUARTER:
		   pOut->x = 0.25f;
		   break;
	  case AF_HALF:
		   pOut->x = 0.5f;
		   break;
	  case AF_THREEQUARTER:
		   pOut->x = 0.75f;
		   break;
	  case AF_ONE:
		   pOut->x = 1.f;
		   break;
	}
  }  
  else if (MatNum <= AF_LINEAR)
  {
    //Just Flip Things Over So At Distance 0 Attenuation = 1 
	float Atten = 1.f-pTexCoord->x;
	pOut->x = Atten;
  } 
  else if (MatNum <= AF_SQUARED)
  {
    //Square The Flipped Input
	float Atten = 1.f-pTexCoord->x;
	Atten = Atten*Atten;
	pOut->x = Atten;
  } 
  else if (MatNum <= AF_SHAPE1)
  {
	//A Constant Start (For 0.5f) Then Linear To 0 At in->x = 1
	float Atten = 1.f-pTexCoord->x;
	Atten = (Atten*1.5f);
	if (Atten > 1) Atten = 1;
	pOut->x = Atten;
  } 
  else if (MatNum <= AF_SHAPE2)
  {
	//A Constant Start (For 0.3f) Then Squared To 0 At in->x = 1
	float Atten = 1.f-pTexCoord->x;
	Atten = (Atten*1.3f);
	if (Atten > 1) Atten = 1;
	Atten /= 1.3f;
	Atten = Atten*Atten;
	pOut->x = Atten;
  }

  if (pTexCoord->x > 0.9999f) pOut->x = 0;
} //EndFillAttenuationTextureProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRBaseLight Class
///////////////////////////////////////////////////////////////////////////////
CGRBaseLight::CGRBaseLight() 
{
  //Set Default Parameters
  m_pShader = NULL;
  m_pSphMesh = NULL;
  m_LightColor = D3DXVECTOR4(1,1,1,0);
  m_LightIntensity = 1.0f;
  D3DXMatrixIdentity(&m_WorldMatrix);
} //EndConstructionDirectives

CGRBaseLight::~CGRBaseLight()
{
  ReleaseClass(m_pShader);
  ReleaseClass(m_pSphMesh);
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRBaseLight::Reset()
// Desc: Called When The Device Has Changed In Some Way
//-----------------------------------------------------------------------------
void CGRBaseLight::Reset()
{
  if (m_pShader) m_pShader->OnLostDevice();
} //EndResetProcedure

//-----------------------------------------------------------------------------
// Name: CGRBaseLight::Update()
// Desc: Make Sure Matrices Etc Are Up To Date
//-----------------------------------------------------------------------------
void CGRBaseLight::Update()
{
  //Calculate Our Local Matrix In View Space
  const D3DXMATRIXA16& MatView = gl_LObj->m_View;
  D3DXMatrixMultiply(&m_WorldViewMatrix,&m_WorldMatrix,&MatView);

  //Calculate Actual Colour
  m_ActualColor = m_LightColor*m_LightIntensity;
} //EndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CGRBaseLight::ApplyFullScreenPixelShader
// Desc: Render A Full Screen Quad With Whatever Settings Are Currently Set
//-----------------------------------------------------------------------------
void CGRBaseLight::ApplyFullScreenPixelShader(const UINT Width,const UINT Height)
{
  //Render Pixel Shader Quad To Combine The Render Target
  struct
  {
    float x,y,z,w;
	float u0,v0;
  } 
  quad[4] = 
  { 
    { (float)0,		(float)0,	   0.1f, 1,	0, 0, }, 
	{ (float)Width, (float)0,	   0.1f, 1,	1, 0, }, 
	{ (float)0,		(float)Height, 0.1f, 1,	0, 1, }, 
	{ (float)Width, (float)Height, 0.1f, 1,	1, 1, } 
  };

  gl_LObj->m_pD3DDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_TEX1);
  gl_LObj->m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,quad,sizeof(quad[0]));
} //EndApplyFullScreenPixelShaderProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRDirectionalLight Class
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: CGRDirectionalLight::Create()
// Desc: Loads The FX For Processing Directional Lights
//-----------------------------------------------------------------------------
HRESULT CGRDirectionalLight::Create()
{
  HRESULT HRes;

  //A Pixel Shader That Does Directional Lighting On The Fat Framebuffer
  if (FAILED(HRes = D3DXCreateEffectFromFile(gl_LObj->m_pD3DDevice, 
											 TEXT("\Shaders\\LightDirectional.fx"),
											 0,                          //Macros
											 0,                          //Include
											 0,                          //Flags
											 gl_LObj->m_LightEffectPool, //Effect Pool
											 &m_pShader,0))) return HRes;
  return S_OK;
} //EndCreateFunction

//-----------------------------------------------------------------------------
// Name: CGRDirectionalLight::Update()
// Desc: Updates Directional Lights Parameters
//-----------------------------------------------------------------------------
void CGRDirectionalLight::Update()
{
  //Call Our Super Update
  CGRBaseLight::Update();

  D3DXVECTOR4 Vec;
  const D3DXVECTOR3 zAxis(0,0,1);
  //Extract The View Direction (Faces Down The Z)
  D3DXVec3TransformNormal((D3DXVECTOR3*)&Vec,&zAxis,&m_WorldViewMatrix);
  D3DXVec3Normalize((D3DXVECTOR3*)&Vec,(D3DXVECTOR3*)&Vec);
  m_pShader->SetVector("f3LightDirection",&Vec);

  //Update Color
  m_pShader->SetVector("f3LightColour",&m_ActualColor);
} //EndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CGRDirectionalLight::Render()
// Desc: Directional Lights Are Full Screen Lights
//-----------------------------------------------------------------------------
void CGRDirectionalLight::Render()
{
  //Directional Lights Don't Use The Mesh Object They Render To The Entire Screen
  UINT iPass,iPasses;
  m_pShader->Begin(&iPasses,0);
    
  for (iPass = 0; iPass < iPasses; ++iPass)
  {
    m_pShader->BeginPass(iPass);
    m_pShader->CommitChanges();
    ApplyFullScreenPixelShader(gl_LObj->m_Framebuffer->GetWidth(),gl_LObj->m_Framebuffer->GetHeight());
    m_pShader->EndPass();
  }
  m_pShader->End();
} //EndRenderProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRHemisphereLight Class
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: CGRHemisphereLight::Create()
// Desc: Loads The FX For Processing Hemisphere Lights
//-----------------------------------------------------------------------------
HRESULT CGRHemisphereLight::Create()
{
  HRESULT HRes;
  
  //A Pixel Shader That Does Directional Lighting On The Fat Framebuffer
  if (FAILED(HRes = D3DXCreateEffectFromFile(gl_LObj->m_pD3DDevice, 
											 TEXT("\Shaders\\LightHemisphere.fx"),
											 0,                          //Macros
											 0,                          //Include
											 0,                          //Flags
											 gl_LObj->m_LightEffectPool, //Effect Pool
											 &m_pShader,0))) return HRes;

  m_LightColor = D3DXVECTOR4(0.1f, 0.1, 0.1f, 0);
  m_SkyColor = D3DXVECTOR4(0.075f, 0.075f, 0.075f, 0);
  return S_OK;
} //EndCreateFunction

//-----------------------------------------------------------------------------
// Name: CGRHemisphereLight::Update()
// Desc: Updates Hemisphere Lights Parameters
//-----------------------------------------------------------------------------
void CGRHemisphereLight::Update()
{
  //Call Our Super Update
  CGRBaseLight::Update();

  D3DXVECTOR4 Vec;
  const D3DXVECTOR3 yAxis(0,1,0);
  //Extract The World Up Direction (Up The Y)
  D3DXVec3TransformNormal((D3DXVECTOR3*)&Vec,&yAxis,&m_WorldViewMatrix);
  D3DXVec3Normalize((D3DXVECTOR3*)&Vec,(D3DXVECTOR3*)&Vec);
  m_pShader->SetVector("f3WorldUpInViewSpace",&Vec);

  D3DXVECTOR4 ActualSkyColor = m_SkyColor*m_LightIntensity;

  //Ground Colour = Actual Colour, Sky Colour = SkyColour*Intensity
  m_pShader->SetVector("f3GroundColour",&m_ActualColor);
  m_pShader->SetVector("f3SkyColour",&ActualSkyColor);
} //EndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CGRHemisphereLight::Render()
// Desc: Hemisphere Lights Are Full Screen Lights
//-----------------------------------------------------------------------------
void CGRHemisphereLight::Render()
{
  //Directional Lights Don't Use The Mesh Object They Render To The Entire Screen
  UINT iPass,iPasses;
  m_pShader->Begin(&iPasses,0);
  
  for (iPass = 0; iPass < iPasses; ++iPass)
  {
    m_pShader->BeginPass(iPass);
    m_pShader->CommitChanges();
    ApplyFullScreenPixelShader(gl_LObj->m_Framebuffer->GetWidth(),gl_LObj->m_Framebuffer->GetHeight());
    m_pShader->EndPass();
  }
  m_pShader->End();
} //EndRenderProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRPointLight Class
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: CGRPointLight::Create()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CGRPointLight::Create()
{
  HRESULT HRes;

  //A Pixel Shader That Does Directional Lighting On The Fat Framebuffer
  if (FAILED(HRes = D3DXCreateEffectFromFile(gl_LObj->m_pD3DDevice, 
                                             TEXT("\Shaders\\LightPoint.fx"),
                                             0,                   //Macros
											 0,                   //Include
											 0,                   //Flags
											 gl_LObj->m_LightEffectPool, //Effect Pool
											 &m_pShader,0))) return HRes;

  //A Sphere Mesh Of Radius 0.5f, It Well Tessleated (We Are Not Vertex Limited At This Stage)
  if (FAILED(HRes = D3DXCreateSphere(gl_LObj->m_pD3DDevice,0.5f,20,10,&m_pSphMesh,NULL))) return HRes;
  m_ShowVolume = false;

  return S_OK;
} //EndCreateFunction

//-----------------------------------------------------------------------------
// Name: CGRPointLight::Update()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPointLight::Update()
{
  if (m_pShader == 0) return;
  
  //Call Our Super Update
  CGRBaseLight::Update();

  D3DXVECTOR4 Vec;
  const D3DXVECTOR3 Pt(0,0,0);

  //Extract The Position
  D3DXVec3Transform(&Vec,&Pt,&m_WorldViewMatrix);

  m_pShader->SetVector("f3LightPosition",&Vec);

  //Update Colour
  m_pShader->SetVector("f3LightColour",&m_ActualColor);

  //Update Distanace
  m_pShader->SetFloat("fRcpRadius",1.f/m_Radius);

  //Update Attenuation Function
  m_pShader->SetFloat("fAttenuationFunction",AttenuationIndexToV(AF_SHAPE1));
} //EndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CGRPointLight::CullLight()
// Desc: Determine The Cullstate For An Object Bounding Box (OBB).  
//       The Algorithm Is:
//       1) If Any OBB Corner Pt Is Inside The Frustum, Return CS_INSIDE
//       2) Else If All OBB Corner Pts Are Outside A Single Frustum Plane, 
//          Return CS_OUTSIDE
//       3) Else If Any Frustum Edge Penetrates A Face Of The OBB, Return 
//          CS_INSIDE_SLOW
//       4) Else If Any OBB Edge Penetrates A Face Of The Frustum, Return
//          CS_INSIDE_SLOW
//       5) Else If Any Point In The Frustum Is Outside Any Plane Of The 
//          OBB, Return CS_OUTSIDE_SLOW
//       6) Else Return CS_INSIDE_SLOW
//-----------------------------------------------------------------------------
CULLSTATE CGRPointLight::CullLight(CULLINFO* pCullInfo)
{
  ZeroMemory(&m_bOutside,sizeof(m_bOutside));

  D3DXVECTOR3 LocalVecBounds[8];
  D3DXVECTOR4 WorldVecBounds[8];

  LocalVecBounds[0] = D3DXVECTOR3(-1, 1, 1);
  LocalVecBounds[1] = D3DXVECTOR3( 1, 1, 1);
  LocalVecBounds[2] = D3DXVECTOR3(-1,-1, 1);
  LocalVecBounds[3] = D3DXVECTOR3( 1,-1, 1);
  LocalVecBounds[4] = D3DXVECTOR3(-1, 1,-1);
  LocalVecBounds[5] = D3DXVECTOR3( 1, 1,-1);
  LocalVecBounds[6] = D3DXVECTOR3(-1,-1,-1);
  LocalVecBounds[7] = D3DXVECTOR3( 1,-1,-1);

  D3DXVec3TransformArray(WorldVecBounds,sizeof(D3DXVECTOR4),LocalVecBounds,sizeof(D3DXVECTOR3),&m_WorldMatrix,8);
  D3DXVECTOR4* pVecBounds = WorldVecBounds;

  //Check Boundary Vertices Against All 6 Frustum Planes, 
  //And Store Result (1 If Outside) In A Bitfield
  for (int iPoint = 0; iPoint < 8; iPoint++)
  {
    for (int iPlane = 0; iPlane < 6; iPlane++)
    {
      if (pCullInfo->PlaneFrustum[iPlane].a*pVecBounds[iPoint].x+
          pCullInfo->PlaneFrustum[iPlane].b*pVecBounds[iPoint].y+
          pCullInfo->PlaneFrustum[iPlane].c*pVecBounds[iPoint].z+
          pCullInfo->PlaneFrustum[iPlane].d < 0)
      {
        m_bOutside[iPoint] |= (1 << iPlane);
      }
    }
  }

  m_bOut = m_bOutside[0] & m_bOutside[1] & m_bOutside[2] & m_bOutside[3] & 
		   m_bOutside[4] & m_bOutside[5] & m_bOutside[6] & m_bOutside[7];
  m_bIn =  m_bOutside[0] | m_bOutside[1] | m_bOutside[2] | m_bOutside[3] | 
		   m_bOutside[4] | m_bOutside[5] | m_bOutside[6] | m_bOutside[7];

  //If All Points Are Outside Any Single Frustum Plane, The Object Is
  //Outside The Frustum
  if (m_bOut != 0)
  {
    return CS_OUTSIDE;
  } 
  else if (m_bIn == 0)
  {
    return CS_INSIDE;
  } else return CS_CROSSING;
} //EndCullLightFunction

//-----------------------------------------------------------------------------
// Name: CGRPointLight::CrossesNearPlane()
// Desc: 
//-----------------------------------------------------------------------------
bool CGRPointLight::CrossesNearPlane()
{
  //Behind Near Plane
  if ((m_bOut & (1 << CP_NEAR)) != 0)
  {
    return false;
  }
  //In Front Of Near Plane
  if ((m_bIn & (1 << CP_NEAR)) == 0)
  {
    return false;
  }
  return true;
} //EndCrossesNearPlaneFunction

//-----------------------------------------------------------------------------
// Name: CGRPointLight::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CGRPointLight::Render()
{
  if (m_pShader == 0) return;

  CULLSTATE CullState = CullLight(&gl_LObj->m_CullInfo);
  if (CullState == CS_OUTSIDE) return;

  //Debugger Or Proper Code
  if (m_ShowVolume)
  {
    if (CrossesNearPlane() == true) m_pShader->SetTechnique("Debug0"); 
	else m_pShader->SetTechnique("Debug1");
  } 
  else
  {
    //Normal If The Sphere Cross The Near Plane Use The More Expensive Version
	if (CrossesNearPlane() == true) m_pShader->SetTechnique("T0"); 
	else m_pShader->SetTechnique("T1");
  }

  const D3DXMATRIXA16& MatViewProjection = gl_LObj->m_ViewProj;
  D3DXMATRIXA16 MatWorldViewProjection;
  D3DXMatrixMultiply(&MatWorldViewProjection,&m_WorldMatrix,&MatViewProjection);
  m_pShader->SetMatrix("matWorldView",&m_WorldViewMatrix);
  m_pShader->SetMatrix("matWorldViewProjection",&MatWorldViewProjection);
  m_pShader->CommitChanges();

  //Directional Lights Don't Use The Mesh Object They Render To The Entire Screen
  UINT iPass,iPasses;
  m_pShader->Begin(&iPasses,0); //D3DXFX_DONOTSAVESTATE
  for (iPass = 0; iPass < iPasses; ++iPass)
  {
    m_pShader->BeginPass(iPass);
    m_pSphMesh->DrawSubset(0);
    m_pShader->EndPass();
  }
  m_pShader->End();
} //EndRenderProcedure

//-----------------------------------------------------------------------------