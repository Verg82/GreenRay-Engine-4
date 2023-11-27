/*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GRSound.h: 
// DirectSound Framework Classes For Reading And Writing Wav Files And
// Playing Them In DirectSound Buffers. Feel Free To Use This Class 
// As A Starting Point For Adding Extra Functionality.
// Date Creation: 29 August 2005
// Last Changing: 07 February 2012
// Author Rights By: Native Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2012
*******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
class CGRWave;
class CGRMusic;
class CGRSoundManager;
class CGRWaveFile;
class CGRSound;
class CGRStreamingSound;

////////////////////////////////////////////////////////////////////////////////
// Global GRSound Constants 
////////////////////////////////////////////////////////////////////////////////
#define WAVEFILE_READ 1
#define WAVEFILE_WRITE 2

//-----------------------------------------------------------------------------
// Name: Class CGRWave
// Desc: Base Class Of The GreenRay Wave Interface For Initialize 
// DirectSound Functions.
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRWave_H__)
#define AFX_CGRWave_H__
class CGRWave
{
private:
	//Private Directives
	CGRSoundManager* gr_SoundManager; //GreenRay DirectSound Manager Class
    CGRSound* gr_Sound; //The GreenRay Sound Base Interface
public:
	//Public Directives
	HRESULT CreateSoundInterface(HWND WndHndl,LPTSTR FileName,int Channels,int Frequency,int Bitrate);
    void PlaybackSound();
	void PlaybackReset();
	void PlaybackLooping();
	void StopSound();
	void ReleaseSound();

	CGRWave();
    virtual ~CGRWave();
protected:
   //Protected Directives
}; 
#endif //!defined(AFX_CGRWave_H__)

//-----------------------------------------------------------------------------
// Name: Class CGRMusic
// Desc: Base Class Of The GreenRay Music Mp3 Interface For Initialize 
// DirectSound Functions.
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRMusic_H__)
#define AFX_CGRMusic_H__
class CGRMusic
{
private:
  //Private Directives
  //IBaseFilter* pBaseFilter;
  IGraphBuilder* pGraphBuilder;
  IMediaControl* pMediaControl;
  IMediaEventEx* pMediaEventEx;
public:
  //Public Directives
  BOOL bUnplay;

  CGRMusic();
  ~CGRMusic();  
  void Release(); 

  HRESULT CreateSound(LPCWSTR FileName);
  void PlaySound();
  void PauseSound();
  void StopSound();
  BOOL IsUnplaying();
protected:
  //Protected Directives
};
#endif //!defined(AFX_CGRMusic_H__)

//-----------------------------------------------------------------------------
// Name: Class CGRSoundManager
// Desc: Initialize The GreenRay Wave Interface For Playback Of Wave Files.
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRSoundManager_H__)
#define AFX_CGRSoundManager_H__
class CGRSoundManager
{
private:
	//Private Directives
public:
	//Public Directives
	CGRSoundManager();
    virtual ~CGRSoundManager();
    
    //inline LPDIRECTSOUND8 GetDirectSound() { return m_pDS; }
	HRESULT Initialize(HWND WndHndl,DWORD dwCoopLevel);
	HRESULT SetPrimaryBufferFormat(DWORD dwPrimaryChannels,DWORD dwPrimaryFreq,DWORD dwPrimaryBitRate);
	HRESULT Create(CGRSound** ppSound,LPTSTR strWaveFileName, 
	DWORD dwCreationFlags = 0,GUID _GUID3DAlgorithm = GUID_NULL,DWORD dwNumBuffers = 1);
	HRESULT CreateFromMemory(CGRSound** ppSound,BYTE* pbData,ULONG ulDataSize, 
	LPWAVEFORMATEX pwfx,DWORD dwCreationFlags = 0,GUID _GUID3DAlgorithm = GUID_NULL,DWORD dwNumBuffers = 1);
	HRESULT CreateStreaming(CGRStreamingSound** ppStreamingSound,LPTSTR strWaveFileName, 
	DWORD dwCreationFlags,GUID _GUID3DAlgorithm,DWORD dwNotifyCount,DWORD dwNotifySize,HANDLE hNotifyEvent);
	HRESULT Get3DListenerInterface(LPDIRECTSOUND3DLISTENER* ppDSListener);
protected:
   //Protected Directives 
   IDirectSound8* m_pDirectSound; //Base DirectSound Interface
}; 
#endif //!defined(AFX_CGRSoundManager_H__)

//-----------------------------------------------------------------------------
// Name: Class CGRWaveFile
// Desc: Encapsulates Reading Or Writing Sound Data To Or From A Wave Files
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRWaveFile_H__)
#define AFX_CGRWaveFile_H__
class CGRWaveFile
{
private:
	//Private Directives
public:
	//Public Directives
	WAVEFORMATEX* m_pwfx;        // Pointer To WAVEFORMATEX Structure
    HMMIO         m_hmmio;       // MM I/O Handle For The WAVE
    MMCKINFO      m_ck;          // Multimedia RIFF Chunk
    MMCKINFO      m_ckRiff;      // Use In Opening A WAVE File
    DWORD         m_dwSize;      // The Size Of The wave File
    MMIOINFO      m_mmioinfoOut;
    DWORD         m_dwFlags;
    BOOL          m_bIsReadingFromMemory;
    BYTE*         m_pbData;
    BYTE*         m_pbDataCur;
    ULONG         m_ulDataSize;
    CHAR*         m_pResourceBuffer;

	CGRWaveFile();
    ~CGRWaveFile();

	HRESULT Open(LPTSTR strFileName,WAVEFORMATEX* pwfx,DWORD dwFlags);
	HRESULT OpenFromMemory(BYTE* pbData,ULONG ulDataSize,WAVEFORMATEX* pwfx,DWORD dwFlags);
	HRESULT ResetFile();
	DWORD GetSize();
	HRESULT Read(BYTE* pBuffer,DWORD dwSizeToRead,DWORD* pdwSizeRead);
    HRESULT Write(UINT nSizeToWrite,BYTE* pbData,UINT* pnSizeWrote);
    HRESULT Close();

    WAVEFORMATEX* GetFormat() { return m_pwfx; };
protected:
   //Protected Directives 
   HRESULT ReadMMIO();
   HRESULT WriteMMIO(WAVEFORMATEX *pwfxDest);
}; 
#endif //!defined(AFX_CGRWaveFile_H__)

//-----------------------------------------------------------------------------
// Name: Class CGRSound
// Desc: Encapsulates Functionality Of A DirectSound Buffer.
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRSound_H__)
#define AFX_CGRSound_H__
class CGRSound
{
private:
	//Private Directives
public:
	//Public Directives
	CGRSound(LPDIRECTSOUNDBUFFER* apDSBuffer,DWORD dwDSBufferSize,
    DWORD dwNumBuffers,CGRWaveFile* pWaveFile,DWORD dwCreationFlags);
    virtual ~CGRSound();
  
	HRESULT Get3DBufferInterface(DWORD dwIndex,LPDIRECTSOUND3DBUFFER* ppDS3DBuffer);
	HRESULT FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB,BOOL bRepeatWavIfBufferLarger);
	LPDIRECTSOUNDBUFFER GetFreeBuffer();
    LPDIRECTSOUNDBUFFER GetBuffer(DWORD dwIndex);

    HRESULT Play(DWORD dwPriority = 0,DWORD dwFlags = 0,LONG lVolume = 0,LONG lFrequency = -1,LONG lPan = 0);
    HRESULT Play3D(LPDS3DBUFFER p3DBuffer,DWORD dwPriority = 0,DWORD dwFlags = 0,LONG lFrequency = 0);
    HRESULT Stop();
    HRESULT Reset();
    BOOL IsSoundPlaying();
protected:
    /*Protected Directives*/ 
	LPDIRECTSOUNDBUFFER* m_apDSBuffer;
    DWORD                m_dwDSBufferSize;
    CGRWaveFile*         m_pWaveFile;
    DWORD                m_dwNumBuffers;
    DWORD                m_dwCreationFlags;

	HRESULT RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB,BOOL* pbWasRestored);
}; 
#endif //!defined(AFX_CGRSound_H__)

//-----------------------------------------------------------------------------
// Name: Class CGRStreamingSound
// Desc: Encapsulates Functionality To Play A Wave File With DirectSound.  
//       The Create() Method Loads A Chunk Of Wave File Into The Buffer, 
//       And As Sound Plays More Is Written To The Buffer By Calling 
//       HandleWaveStreamNotification() Whenever hNotifyEvent Is Signaled.
//-----------------------------------------------------------------------------
#if !defined(AFX_CGRStreamingSound_H__)
#define AFX_CGRStreamingSound_H__
class CGRStreamingSound: public CGRSound
{
private:
	//Private Directives
public:
	//Public Directives
    CGRStreamingSound(LPDIRECTSOUNDBUFFER pDSBuffer,DWORD dwDSBufferSize, 
	CGRWaveFile* pWaveFile,DWORD dwNotifySize);
    ~CGRStreamingSound();

    HRESULT HandleWaveStreamNotification(BOOL bLoopedPlay);
    HRESULT Reset();
protected:
	//Protected Directives
    DWORD m_dwLastPlayPos;
    DWORD m_dwPlayProgress;
    DWORD m_dwNotifySize;
    DWORD m_dwNextWriteOffset;
    BOOL m_bFillNextNotificationWithSilence;
};
#endif //!defined(AFX_CGRStreamingSound_H__)

//----------------------------------------------------------------------------