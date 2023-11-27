/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Camera.h
// It's An GreenRay GR_Camera Header File.
// Date Creation: 29 August 2005
// Last Changing: 06 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_System.h>
#include <GR_Methods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CGRCamera;

//-----------------------------------------------------------------------------
// Name: Struct CGRCamera
// Desc: 
//-----------------------------------------------------------------------------
#if !defined(AFX_CAMERA_H__)
#define AFX_CAMERA_H__
class CGRCamera
{	
public:
  //Public Directives
  CGRCamera(LPDIRECT3DDEVICE9 pDevice,HWND WndLayer);
  virtual ~CGRCamera();
  void Release();
  
  HRESULT CreateInputDevices(HWND WndLayer);
  void UpdateCamera();
  void UseCamera(bool UseConnect);
  
  //Установка начального расположения камеры в игровом мире или
  //вообще установка "глаз" в нужное местоположение.
  void SetCameraPos(D3DXVECTOR3 Pos_XYZ) { Position = Pos_XYZ; }

  //Скорость передвижения камеры, т.е фактически здесь
  //мы задаём скорость для движения камеры в стороны при управлении с клавиатуры.
  void SetCameraSpeed(float Cam_Speed) { m_fCameraSpeed = Cam_Speed; } 

  //Здесь задаётся скорость вращения и обзора мыши.
  void SetMouseSpeed(float Mouse_Speed) { m_fMouseSpeed = Mouse_Speed; }
 
  //Функция возвращающая местоположение камеры в игровом мире.
  D3DXVECTOR3 Pos() { return Position; }

  //Return Camera Dispose
  float CamPosX() { return PosX; }
  float CamPosY() { return PosY; }
  float CamPosZ() { return PosZ; }

  //Return Camera Button States
  bool SpaceDown() { return m_bSpaceDown; }
  bool MbLeftDown() { return m_bMbLeftDown; }
private:
  //Private Directives 
  LPDIRECT3DDEVICE9    d_pDevice;      //Pointer On Direct3D Device Interface
  IDirectInput8*       m_pDirectInput; //Base DirectInput Device
  IDirectInputDevice8* m_pMouse;       //Mouse Input Device
  IDirectInputDevice8* m_pKeyboard;    //Keyboard Input Device
  CGRDeltaTimer*       m_pDeltaTimer;  //Camera Delta Timer

  //Base Camera Directives
  bool m_nMbLDownOnce; //Use For Update Single Left Mouse Button Press Method
  bool m_bSpaceDown;   //Space Down State
  bool m_bMbLeftDown;  //Mouse Button Left Down State

  float Pitch,Yaw;
  float PosX,PosY,PosZ; //Camera Position
  float m_fCameraSpeed; //Camera Speed
  float m_fMouseSpeed;  //Mouse Speed
public:
  //Public Directives

  //Camera Vectors
  D3DXVECTOR3 Position;
  D3DXVECTOR3 Target;
  D3DXVECTOR3 Up;
  D3DXVECTOR3 Velocity;
  D3DXVECTOR3 Look_Vector; //Base Look Vector
  D3DXMATRIX View;
  D3DXMATRIX Projection;
};
#endif //!defined(AFX_CAMERA_H__)

//-----------------------------------------------------------------------------