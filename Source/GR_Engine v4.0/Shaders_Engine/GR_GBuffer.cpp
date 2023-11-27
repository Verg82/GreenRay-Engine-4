/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_GBuffer.cpp
// It's The GreenRay Lighting Shader Engine.
// GBuffer Base Class.
// Date Creation: 29 August 2005
// Last Changing: 17 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_GBuffer.h>

// NOTES: 16Bit INT Format Is A Good Comprimise Between The Size Of FLOAT32 And The Precision 
//		  Needed To Store Positions Etc
//		  I Might Seem More Worthwhile To Have Different Buffers At Different Bitdepth But This Has
//		  Compitibity Issues That I Don't Want To Address Here (ATI Radeon 9700 MRT Requires Same Bitdepths)
D3DFORMAT CGRGBufferSingle::m_FatFormat = D3DFMT_A16B16G16R16; //A 16bit INTEGER Format
D3DFORMAT CGRGBufferMRT::m_FatFormat = D3DFMT_A16B16G16R16;    //A 16bit INTEGER Format
//D3DFORMAT CGRGBufferMRT::m_FatFormat = D3DFMT_A32B32G32R32F; //A 32bit FLOAT Format

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRGBufferSingle Class
///////////////////////////////////////////////////////////////////////////////
CGRGBufferSingle::CGRGBufferSingle(LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pD3DDevice) 
{
  //Set Default Parameters
  m_pD3D = pD3D;
  m_pD3DDevice = pD3DDevice;

  assert(m_pD3D != 0);
  assert(m_pD3DDevice != 0);

  m_pD3D->AddRef();
  m_pD3DDevice->AddRef();

  D3DCAPS9 d3dCaps;
  m_pD3DDevice->GetDeviceCaps(&d3dCaps);
	
  m_numSimulRT = 1; //d3dCaps.NumSimultaneousRTs;
} //EndConstructionDirectives

CGRGBufferSingle::~CGRGBufferSingle()
{
  ReleaseClass(m_pFatDepth);

  for (unsigned int I = 0; I < NUMBER_PARAMETER_SURFACES; ++I)
  {
	ReleaseClass(m_pFatSurface[I]);
	ReleaseClass(m_pFatTexture[I]);
  }

  ReleaseClass(m_pD3D);
  ReleaseClass(m_pD3DDevice);
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRGBufferSingle::CreateBuffers()
// Desc: Creates The Buffers Of The Size Requested
//-----------------------------------------------------------------------------
bool CGRGBufferSingle::CreateBuffers(unsigned int Width,unsigned int Height)
{
  HRESULT HRes;
	
  //Create The Fat Framebuffer
  for (unsigned int I = 0; I < NUMBER_PARAMETER_SURFACES; ++I)
  {
    HRes = m_pD3DDevice->CreateTexture(Width,				  //Size Should Be >= To Backbuffer
									   Height,					
									   1,					  //Mipmaps TODO?
									   D3DUSAGE_RENDERTARGET, //Render Target Obviously
									   m_FatFormat,			  //Format Our Fat Framebuffer Is Made Up Of
									   D3DPOOL_DEFAULT,		  //Render Target Cannot Be Managed
									   &m_pFatTexture[I],	  //Dest Ptr
									   0);					  //Ignored
	if (FAILED(HRes)) return false;
	m_pFatTexture[I]->GetSurfaceLevel(0,&m_pFatSurface[I]);
  }

  //Create The Depth/Stencil Buffer We Will Use When Rendering Into The Fat Framebuffer
  //Discard? Multisample?
  HRes = m_pD3DDevice->CreateDepthStencilSurface(Width,				  //Size Should Be >= To Backbuffer
												 Height, 
												 D3DFMT_D24S8,		  //Format (TODO Option For Float Depth?)
												 D3DMULTISAMPLE_NONE, //Multisample
												 0,				      //Multisample Quality
												 FALSE,			      //Discard TODO?
												 &m_pFatDepth,	      //Dest
												 0);				  //Ignored
  m_Width = Width;
  m_Height = Height;
  return true;
} //EndCreateBuffersFunction

//-----------------------------------------------------------------------------
// Name: CGRGBufferSingle::GetRenderPassCount()
// Desc: How Many Passes To Fill The GBuffer (4 Passes)
//-----------------------------------------------------------------------------
unsigned int CGRGBufferSingle::GetRenderPassCount()
{
  return NUMBER_PARAMETER_SURFACES;
} //EndGetRenderPassCountFunction

//-----------------------------------------------------------------------------
// Name: CGRGBufferSingle::SelectBuffersAsRenderTarget()
// Desc: Which Buffer To Render To, Only 1 Non Depth/Stencil At A Time 
//-----------------------------------------------------------------------------
void CGRGBufferSingle::SelectBuffersAsRenderTarget(unsigned int Buffers)
{
  if (Buffers == 0) return;

  if (Buffers & FFB_DEPTH || Buffers & FFB_STENCIL) m_pD3DDevice->SetDepthStencilSurface(m_pFatDepth);
  else m_pD3DDevice->SetDepthStencilSurface(0);

  //Remove Stencil And Depth Flag To Help Debugger
  Buffers &= ~(FFB_DEPTH|FFB_STENCIL);

  if (Buffers & FFB_BUFFER0)
  {
	m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[0]);
	Buffers &= ~FFB_BUFFER0;
	assert(Buffers == 0);
  }
  
  if (Buffers & FFB_BUFFER1)
  {
	m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[1]);
	Buffers &= ~FFB_BUFFER1;
	assert(Buffers == 0);
  }
  
  if (Buffers & FFB_BUFFER2)
  {
	m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[2]);
	Buffers &= ~FFB_BUFFER2;
	assert(Buffers == 0);
  }
	
  if (Buffers & FFB_BUFFER3)
  {
	m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[3]);
	Buffers &= ~FFB_BUFFER3;
	assert(Buffers == 0);
  }
} //EndSelectBuffersAsRenderTargetProcedure

//-----------------------------------------------------------------------------
// Name: CGRGBufferSingle::SelectBufferAsTexture()
// Desc: Buffer Is Selected In The Specified Sampler
//-----------------------------------------------------------------------------
void CGRGBufferSingle::SelectBufferAsTexture(FFB_BUFFER_NAME Name,unsigned int SamplerNum)
{
  m_pD3DDevice->SetTexture(SamplerNum,GetBufferTexture(Name));
} //EndSelectBufferAsTextureProcedure

//-----------------------------------------------------------------------------
// Name: CGRGBufferSingle::GetBufferTexture()
// Desc: Return Specified Buffer As A Texture
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CGRGBufferSingle::GetBufferTexture(FFB_BUFFER_NAME Name)
{
  //Depth Textures Etc Are Largely Not Supported So I Don't Use Them
  assert(!(Name & FFB_DEPTH || Name & FFB_STENCIL));

  if (Name & FFB_BUFFER0) return m_pFatTexture[0];
  else if (Name & FFB_BUFFER1) return m_pFatTexture[1];
  else if (Name & FFB_BUFFER2) return m_pFatTexture[2];
  else if (Name & FFB_BUFFER3) return m_pFatTexture[3];
  else return 0;
} //EndGetBufferTextureFunction

//-----------------------------------------------------------------------------
// Name: CGRGBufferSingle::GetBufferSurface()
// Desc: Return Specified Buffer As A Surface
//-----------------------------------------------------------------------------
LPDIRECT3DSURFACE9 CGRGBufferSingle::GetBufferSurface(FFB_BUFFER_NAME Name)
{
  if ((Name & FFB_DEPTH || Name & FFB_STENCIL)) return m_pFatDepth;
  else if (Name & FFB_BUFFER0) return m_pFatSurface[0];
  else if (Name & FFB_BUFFER1) return m_pFatSurface[1];
  else if (Name & FFB_BUFFER2) return m_pFatSurface[2];
  else if (Name & FFB_BUFFER3) return m_pFatSurface[3];
  else return 0;
} //EndGetBufferSurfaceFunction

//-----------------------------------------------------------------------------
// Name: CGRGBufferSingle::Clear()
// Desc: Clears The Specified Buffers (Reasonable Slow...)
//-----------------------------------------------------------------------------
void CGRGBufferSingle::Clear(unsigned int Buffers,unsigned int Color,float ZVal,unsigned int StencilVal)
{
  unsigned int Flags = 0;

  if (Buffers != 0)
  {
    if (Buffers & FFB_DEPTH || Buffers & FFB_STENCIL)
	{
	  m_pD3DDevice->SetDepthStencilSurface(m_pFatDepth);
	  if (Buffers & FFB_DEPTH) Flags |= D3DCLEAR_ZBUFFER;
	  if (Buffers & FFB_STENCIL) Flags |= D3DCLEAR_STENCIL;
	}

    if (Buffers & FFB_BUFFER0)
    {
	  Flags |= D3DCLEAR_TARGET;
	  m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[0]);
	  m_pD3DDevice->Clear(0L,NULL,Flags,Color,ZVal,StencilVal);
    }
	
    if (Buffers & FFB_BUFFER1)
    {
	  Flags |= D3DCLEAR_TARGET;
	  m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[1]);
	  m_pD3DDevice->Clear(0L,NULL,Flags,Color,ZVal,StencilVal);
    }
	
    if (Buffers & FFB_BUFFER2)
    {
	  Flags |= D3DCLEAR_TARGET;
	  m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[2]);
	  m_pD3DDevice->Clear(0L,NULL,Flags,Color,ZVal,StencilVal);
    }
	
    if (Buffers & FFB_BUFFER3)
    {
	  Flags |= D3DCLEAR_TARGET;
	  m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[3]);
	  m_pD3DDevice->Clear(0L,NULL,Flags,Color,ZVal,StencilVal);
    }
  }
} //EndClearProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRGBufferMRT Class
///////////////////////////////////////////////////////////////////////////////
CGRGBufferMRT::CGRGBufferMRT(LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pD3DDevice)
{
  //Set Default Parameters
  m_pD3D = pD3D;
  m_pD3DDevice = pD3DDevice;
  assert(m_pD3D != 0);
  assert(m_pD3DDevice != 0);

  m_pD3D->AddRef();
  m_pD3DDevice->AddRef();

  D3DCAPS9 d3dCaps;
  m_pD3DDevice->GetDeviceCaps(&d3dCaps);
  m_numSimulRT = Min(d3dCaps.NumSimultaneousRTs,NUMBER_PARAMETER_SURFACES);
} //EndConstructionDirectives

CGRGBufferMRT::~CGRGBufferMRT()
{
  ReleaseClass(m_pFatDepth);

  for (unsigned int I = 0; I < NUMBER_PARAMETER_SURFACES; ++I)
  {
    ReleaseClass(m_pFatSurface[I]);
	ReleaseClass(m_pFatTexture[I]);
  }

  ReleaseClass(m_pD3D);
  ReleaseClass(m_pD3DDevice);
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRGBufferMRT::CreateBuffers()
// Desc: Creates The Buffers Of The Size Requested
//-----------------------------------------------------------------------------
bool CGRGBufferMRT::CreateBuffers(unsigned int Width,unsigned int Height)
{
  HRESULT HRes;
	
  //Create The Fat Framebuffer
  for (unsigned int I = 0; I < NUMBER_PARAMETER_SURFACES; ++I)
  {
    HRes = m_pD3DDevice->CreateTexture(Width,				  //Size Should Be >= To Backbuffer
									   Height,					
									   1,					  //Mipmaps TODO?
									   D3DUSAGE_RENDERTARGET, //Render Target Obviously
									   m_FatFormat,			  //Format Our Fat Framebuffer Is Made Up Of
									   D3DPOOL_DEFAULT,		  //Render Target Cannot Be Managed
									   &m_pFatTexture[I],	  //Dest Ptr
									   0);					  //Ignored
	if (FAILED(HRes)) return false;
	m_pFatTexture[I]->GetSurfaceLevel(0,&m_pFatSurface[I]);
  }
  //Create The Depth/Stencil Buffer We Will Use When Rendering Into The Fat Framebuffer
  //Discard? Multisample?
  HRes = m_pD3DDevice->CreateDepthStencilSurface(Width,				  //Size Should Be >= To Backbuffer
												 Height, 
												 D3DFMT_D24S8,		  //Format (TODO Option For Float Depth?)
												 D3DMULTISAMPLE_NONE, //Multisample
												 0,				      //Multisample Quality
												 FALSE,			      //Discard TODO?
												 &m_pFatDepth,	      //Dest
												 0);				  //Ignored
													
  m_Width = Width;
  m_Height = Height;
  return true;
} //EndCreateBuffersFunction

//-----------------------------------------------------------------------------
// Name: CGRGBufferMRT::GetRenderPassCount()
// Desc: How Many Passes To Fill The GBuffer (For 4 Simul MRT, Only 1 Pass)
//-----------------------------------------------------------------------------
unsigned int CGRGBufferMRT::GetRenderPassCount()
{
  return NUMBER_PARAMETER_SURFACES/m_numSimulRT;
} //EndGetRenderPassCountFunction

//-----------------------------------------------------------------------------
// Name: CGRGBufferMRT::SelectBuffersAsRenderTarget()
// Desc: Which Buffer To Render To 
//       You MUST Select Render Targets Starting At 0 (No Gaps)
//-----------------------------------------------------------------------------
void CGRGBufferMRT::SelectBuffersAsRenderTarget(unsigned int Buffers)
{
  if (Buffers == 0) return;
  assert(Buffers & FFB_BUFFER0); //D3D Requires RT0 To Have A Target

  if (Buffers & FFB_DEPTH | Buffers & FFB_STENCIL)
  m_pD3DDevice->SetDepthStencilSurface(m_pFatDepth);
  else m_pD3DDevice->SetDepthStencilSurface(0);

  unsigned int NumTargets = 0;

  if (Buffers & FFB_BUFFER0)
  {
    m_pD3DDevice->SetRenderTarget(0,m_pFatSurface[0]);
	NumTargets++;

	if (Buffers & FFB_BUFFER1)
	{
	  m_pD3DDevice->SetRenderTarget(1,m_pFatSurface[1]);
	  NumTargets++;
		
	  if (Buffers & FFB_BUFFER2)
	  {
	    m_pD3DDevice->SetRenderTarget(2,m_pFatSurface[2]);
		NumTargets++;
		if (Buffers & FFB_BUFFER3)
		{
		  m_pD3DDevice->SetRenderTarget(3,m_pFatSurface[3]);
		  NumTargets++;
		}
	  }
	}
  }
  while (NumTargets < 4) m_pD3DDevice->SetRenderTarget(NumTargets++,0);
} //EndSelectBuffersAsRenderTargetProcedure

//-----------------------------------------------------------------------------
// Name: CGRGBufferMRT::SelectBufferAsTexture()
// Desc: Buffer Is Selected In The Specified Sampler
//-----------------------------------------------------------------------------
void CGRGBufferMRT::SelectBufferAsTexture(FFB_BUFFER_NAME Name,unsigned int SamplerNum)
{
  //Depth Textures Etc Are Largely Not Supported So I Don't Use Them
  assert(!(Name & FFB_DEPTH || Name & FFB_STENCIL));

  m_pD3DDevice->SetTexture(SamplerNum,GetBufferTexture(Name));
} //EndSelectBufferAsTextureProcedure

//-----------------------------------------------------------------------------
// Name: CGRGBufferMRT::GetBufferTexture()
// Desc: Return Specified Buffer As A Texture
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CGRGBufferMRT::GetBufferTexture(FFB_BUFFER_NAME Name)
{
  //Depth Textures Etc Are Largely Not Supported So I Don't Use Them
  assert(!(Name & FFB_DEPTH || Name & FFB_STENCIL));

  if (Name & FFB_BUFFER0) return m_pFatTexture[0];
  else if (Name & FFB_BUFFER1) return m_pFatTexture[1];
  else if (Name & FFB_BUFFER2) return m_pFatTexture[2];
  else if (Name & FFB_BUFFER3) return m_pFatTexture[3];
  else return 0;
} //EndGetBufferTextureFunction

//-----------------------------------------------------------------------------
// Name: CGRGBufferMRT::GetBufferSurface()
// Desc: Return Specified Buffer As A Surface
//-----------------------------------------------------------------------------
LPDIRECT3DSURFACE9 CGRGBufferMRT::GetBufferSurface(FFB_BUFFER_NAME Name)
{
  if ((Name & FFB_DEPTH || Name & FFB_STENCIL)) return m_pFatDepth;
  else if (Name & FFB_BUFFER0) return m_pFatSurface[0];
  else if (Name & FFB_BUFFER1) return m_pFatSurface[1];
  else if (Name & FFB_BUFFER2) return m_pFatSurface[2];
  else if (Name & FFB_BUFFER3) return m_pFatSurface[3];
  else return 0;
} //EndGetBufferSurfaceFunction

//-----------------------------------------------------------------------------
// Name: CGRGBufferMRT::Clear()
// Desc: Clears The Specified Buffers (Don't Be Too Clever...)
//-----------------------------------------------------------------------------
void CGRGBufferMRT::Clear(unsigned int Buffers,unsigned int Color, 
float ZVal,unsigned int StencilVal)
{
  unsigned int Flags = 0;

  if (Buffers != 0)
  {
    unsigned int curTarget = 0;
	
	//Debug Complains If We Select The Same Buffer Several Times...
	for (unsigned int I = 1; I < 4; I++) m_pD3DDevice->SetRenderTarget(I,0);

	if (Buffers & FFB_BUFFER0) m_pD3DDevice->SetRenderTarget(curTarget++,m_pFatSurface[0]);
	if (Buffers & FFB_BUFFER1) m_pD3DDevice->SetRenderTarget(curTarget++,m_pFatSurface[1]);
	if (Buffers & FFB_BUFFER2) m_pD3DDevice->SetRenderTarget(curTarget++,m_pFatSurface[2]);
	if (Buffers & FFB_BUFFER3) m_pD3DDevice->SetRenderTarget(curTarget++,m_pFatSurface[3]);

	if (curTarget != 0)
	{
	  Flags |= D3DCLEAR_TARGET;
	}
		
	if (Buffers & FFB_DEPTH || Buffers & FFB_STENCIL)
	{
	  m_pD3DDevice->SetDepthStencilSurface(m_pFatDepth);
	  if (Buffers & FFB_DEPTH) Flags |= D3DCLEAR_ZBUFFER;
	  if (Buffers & FFB_STENCIL) Flags |= D3DCLEAR_STENCIL;
	}
	m_pD3DDevice->Clear(0L,NULL,Flags,Color,ZVal,StencilVal);
  }
} //EndClearProcedure

//-----------------------------------------------------------------------------
