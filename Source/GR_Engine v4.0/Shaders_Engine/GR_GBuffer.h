/******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_GBuffer.h
// It's The GreenRay Lighting Shader Engine.
// GBuffer Base Class.
// Date Creation: 29 August 2005
// Last Changing: 17 February 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <GR_System.h>
#include <GR_Methods.h>
#include <GR_LightGlobals.h>

////////////////////////////////////////////////////////////////////////////////
// Global Definitions Or Prototypes
////////////////////////////////////////////////////////////////////////////////
//16 Seperate Scalars (In 4 Sets Of 4)
#define NUMBER_PARAMETERS_CHANNELS 16
#define NUMBER_CHANNELS_PER_SURFACE 4
#define NUMBER_PARAMETER_SURFACES (NUMBER_PARAMETERS_CHANNELS/NUMBER_CHANNELS_PER_SURFACE)

////////////////////////////////////////////////////////////////////////////////
// Global Definitions Or Prototypes
////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: Enum MRT_TYPE
// Desc: There Are Several Ways Of Accessing Multple Render Targets That Make 
//       Up The Fat Framebuffer
//-----------------------------------------------------------------------------
enum MRT_TYPE
{
  MT_SINGLE, //Single Render Target Only
  MT_MRT,	 //Use Multiple Render Targets
  MT_MET,	 //Use Multiple Element Textures (ToDo)
};

//-----------------------------------------------------------------------------
// Name: Enum FFB_BUFFER_NAME
// Desc: 
//-----------------------------------------------------------------------------
enum FFB_BUFFER_NAME
{
  FFB_BUFFER0 = (1 << 0),
  FFB_BUFFER1 = (1 << 1),
  FFB_BUFFER2 = (1 << 2),
  FFB_BUFFER3 = (1 << 3),
  FFB_DEPTH	  = (1 << 4), //Depth And Stencil Are Special In Lots Of Ways...
  FFB_STENCIL = (1 << 5),

  //Buffer Renaming
  //Its Easier To Keep Track Of The Buffers If There Given Meaningful Name, 
  //So Just Alias The Buffer Numbers
  FFB_POSITION = FFB_BUFFER0,
  FFB_NORMAL   = FFB_BUFFER1,
  FFB_DIFFUSE  = FFB_BUFFER2,
  FFB_SPECULAR = FFB_BUFFER3,
  MAX_FFV      = 0xFFFFFFFF
};

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRGBuffer;
//class CGRGBufferSingle;
//class CGRGBufferMRT;

//-----------------------------------------------------------------------------
// Name: Struct CGRGBuffer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRGBuffer_H__)
#define AFX_CGRGBuffer_H__

class CGRGBuffer
{
public:
  virtual ~CGRGBuffer() {};

  //Method The Derived Class Is Using
  virtual MRT_TYPE GetType() = 0;
  virtual bool CreateBuffers(unsigned int Width,unsigned int Height) = 0;

  //Number Of Render Requires For All Parameter Buffers To Be Filled
  virtual unsigned int GetRenderPassCount() = 0;

  virtual unsigned int GetWidth() = 0;
  virtual unsigned int GetHeight() = 0;

  //Sets The Specified Buffers As The Current Render Target
  virtual void SelectBuffersAsRenderTarget(unsigned int Buffers) = 0;

  //Sets The Specified Buffer Into The Specified Sampler For Access In Pixel Shader
  virtual void SelectBufferAsTexture(FFB_BUFFER_NAME Buffer,unsigned int SamplerNum) = 0;

  //Get The Texture Of The Specified Buffer (Not DEPTH Or STENCIL)
  virtual LPDIRECT3DTEXTURE9 GetBufferTexture(FFB_BUFFER_NAME Buffer) = 0;
  
  //Get The Surface With The Specified Buffer 
  virtual LPDIRECT3DSURFACE9 GetBufferSurface(FFB_BUFFER_NAME Buffer) = 0;

  //Clears The Specified Buffers (Default To Usual Settings)
  virtual void Clear(unsigned int Buffers,unsigned int Color = 0,float ZVal = 1.0f,unsigned int StencilVal = 0) = 0;
};
#endif //!defined(AFX_CGRGBuffer_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRGBufferSingle
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRGBufferSingle_H__)
#define AFX_CGRGBufferSingle_H__

class CGRGBufferSingle: public CGRGBuffer
{
public:
  //Public Directives
  CGRGBufferSingle(LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pD3DDevice);
  ~CGRGBufferSingle();

  //Method The Derived Class Is Using
  MRT_TYPE GetType() { return MT_SINGLE; }
  bool CreateBuffers(unsigned int Width,unsigned int Height);

  //Number Of Render Requires For All Parameter Buffers To Be Filled
  unsigned int GetRenderPassCount();
  unsigned int GetWidth() { return m_Width; };
  unsigned int GetHeight() { return m_Height; };

  void SelectBuffersAsRenderTarget(unsigned int Name);
  void SelectBufferAsTexture(FFB_BUFFER_NAME name,unsigned int SamplerNum);
  LPDIRECT3DTEXTURE9 GetBufferTexture(FFB_BUFFER_NAME Buffer);
  LPDIRECT3DSURFACE9 GetBufferSurface(FFB_BUFFER_NAME Buffer);

  void Clear(unsigned int Buffers,unsigned int Color = 0,float ZVal = 1.f,unsigned int StencilVal = 0);
  static D3DFORMAT m_FatFormat; //Format (For Simplicity And Compatiblity All RT's Are The Same)
protected:
  //Protected Directives
  unsigned int		m_numSimulRT;	  //How Many Render Target We Output To A The Same Time
  unsigned int		m_Width,m_Height;
  LPDIRECT3D9		m_pD3D;			  //D3D Object
  LPDIRECT3DDEVICE9	m_pD3DDevice;	  //D3D Device Object

  LPDIRECT3DTEXTURE9 m_pFatTexture[NUMBER_PARAMETER_SURFACES]; //The Textures That Make Up The Fat Framebuffer
  LPDIRECT3DSURFACE9 m_pFatSurface[NUMBER_PARAMETER_SURFACES]; //The Surface Interface Of The Textures Above
  LPDIRECT3DSURFACE9 m_pFatDepth;							   //The Depth Stencil Surface
};
#endif //!defined(AFX_CGRGBufferSingle_H__)

//-----------------------------------------------------------------------------
// Name: Struct CGRGBufferMRT
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRGBufferMRT_H__)
#define AFX_CGRGBufferMRT_H__

class CGRGBufferMRT: public CGRGBuffer
{
public:
  //Public Directives
  CGRGBufferMRT(LPDIRECT3D9 pD3D,LPDIRECT3DDEVICE9 pD3DDevice);
  ~CGRGBufferMRT();

  //Method The Derived Class Is Using
  MRT_TYPE GetType() { return MT_MRT; }

  bool CreateBuffers(unsigned int Width,unsigned int Height);

  //Number Of Render Requires For All Parameter Buffers To Be Filled
  unsigned int GetRenderPassCount();
  unsigned int GetWidth() { return m_Width; };
  unsigned int GetHeight() { return m_Height; };

  void SelectBuffersAsRenderTarget(unsigned int Name);
  void SelectBufferAsTexture(FFB_BUFFER_NAME Name,unsigned int SamplerNum);
  LPDIRECT3DTEXTURE9 GetBufferTexture(FFB_BUFFER_NAME Buffer);
  LPDIRECT3DSURFACE9 GetBufferSurface(FFB_BUFFER_NAME Buffer);

  void Clear(unsigned int Buffers,unsigned int Color = 0,float ZVal = 1.f,unsigned int StencilVal = 0);
  static D3DFORMAT m_FatFormat;	//Format (For Simplicity And Compatiblity All RT's Are The Same)
protected:
  //Protected Directives
  unsigned int		m_numSimulRT;	   //How Many Render Target We Output To A The Same Time
  unsigned int		m_Width,m_Height;
  LPDIRECT3D9		m_pD3D;			   //D3D Object
  LPDIRECT3DDEVICE9	m_pD3DDevice;	   //D3D Device object

  LPDIRECT3DTEXTURE9 m_pFatTexture[NUMBER_PARAMETER_SURFACES]; //The Textures That Make Up The Fat Framebuffer
  LPDIRECT3DSURFACE9 m_pFatSurface[NUMBER_PARAMETER_SURFACES]; //The Surface Interface Of The Textures Above
  LPDIRECT3DSURFACE9 m_pFatDepth;							   //The Depth Stencil Surface
}; 
#endif //!defined(AFX_CGRGBufferMRT_H__)

//-----------------------------------------------------------------------------