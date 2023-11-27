/****************************************************************************/ 
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Camera.cpp
// It's An GreenRay GR_Camera Source File.
// Date Creation: 29 August 2005
// Last Changing: 06 January 2012
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2012
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <GR_Camera.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CGRCamera Class
//////////////////////////////////////////////////////////////////////////////
CGRCamera::CGRCamera(LPDIRECT3DDEVICE9 pDevice,HWND WndLayer)
{
  //Set Default Parameters
  d_pDevice = pDevice; //Apropritate Device Pointer To Class
  m_pDirectInput = NULL;
  m_pMouse = NULL;
  m_pKeyboard = NULL;
  m_pDeltaTimer = NULL;
  m_bSpaceDown = false;
  m_bMbLeftDown = false;
  m_nMbLDownOnce = false;

  //Create Input Devices
  if (SUCCEEDED(CreateInputDevices(WndLayer)))
  {
    //m_pMouse->Acquire();
	//m_pKeyboard->Acquire();
  }
  
  m_fCameraSpeed = 0.01f;
  m_fMouseSpeed = 0.01f;

  Pitch = 0.0f;
  Yaw = 0.0f;
  Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
  Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
  D3DXVec3Normalize(&Look_Vector,&(Target-Position));

  //Set The Projection Matrix
  float Aspect_Ratio = 1.0f;
  D3DXMatrixPerspectiveFovLH(&Projection,D3DX_PI/4,Aspect_Ratio,0.1f,1000.0f);
  d_pDevice->SetTransform(D3DTS_PROJECTION,&Projection);

  //Create Camera Delta Timer Class
  m_pDeltaTimer = new CGRDeltaTimer(); //Construct CGRDeltaTimer Class
} //EndConstructionDirectives

CGRCamera::~CGRCamera()
{
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CGRCamera::Release()
// Desc: Освобождение занятых ресурсов
//-----------------------------------------------------------------------------
void CGRCamera::Release()
{
  m_pMouse->Unacquire();    //UnControl Mouse
  m_pKeyboard->Unacquire(); //UnControl Keyboard

  ReleaseClass(m_pMouse);       //Release Mouse
  ReleaseClass(m_pKeyboard);    //Release Keyboard
  ReleaseClass(m_pDirectInput); //Release DirectInput

  //Release Camera Delta Timer
  if (m_pDeltaTimer != NULL) DeleteClass(m_pDeltaTimer); 
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::CreateInputDevices()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CGRCamera::CreateInputDevices(HWND WndLayer)
{
  HRESULT Result = S_OK;
  //Create InputDevice (DirectInput 8)
  if (FAILED(DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,
  IID_IDirectInput8,(void**)&m_pDirectInput,NULL))) 
  {
    //Интерфейс DirectInput вернул ошибку, чтож продолжать далее не имеет смысла
	//Выводим сообщение об ошибке:
    MessageBoxA(NULL,"Can't Create DirectInputContext","GreenRay Engine Error",
	MB_SYSTEMMODAL|MB_ICONEXCLAMATION); 
	Result = E_FAIL;
  }
  else //Всё в порядке продолжаем:
  {
    //Create Mouse Input Device
    //Создаём виртуальное устройство ввода для мыши
    if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse,&m_pMouse,NULL)))
    {
      MessageBoxA(NULL,"Can't Create MouseInputDevice","GreenRay Engine Error",
	  MB_SYSTEMMODAL|MB_ICONEXCLAMATION); 
	  m_pDirectInput->Release(); 
	  Result = E_FAIL;
    }

	//Создаем формат данных для мыши и передаём его в структуру 
	//DirectInput Data Format (DIDATAFORMAT)
    if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse2))) 
    {
      MessageBoxA(NULL,"Can't Set MouseDataFormat","GreenRay Engine Error",
	  MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
      m_pMouse->Release();
      m_pDirectInput->Release();
      m_pMouse = NULL;
      m_pDirectInput = NULL;
	  Result = E_FAIL;
    }
 
    if (FAILED(m_pMouse->SetCooperativeLevel(WndLayer,DISCL_FOREGROUND|DISCL_EXCLUSIVE)))
    {
      MessageBoxA(NULL,"Can't Set Mouse CooperativeLevel","GreenRay Engine Error",
	  MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
      m_pMouse->Release();
	  m_pDirectInput->Release();
	  m_pMouse = NULL;
      m_pDirectInput = NULL;
	  Result = E_FAIL;
    }

    //Create Keyboard Input Device
	//Создаём виртуальное устройство ввода для клавиатуры
    if FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard,&m_pKeyboard,NULL))
    {
      MessageBoxA(NULL,"Can't Create KeyboardInputDevice","GreenRay Engine Error",
	  MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
      m_pKeyboard->Release();
	  m_pKeyboard = NULL;
	  Result = E_FAIL;
    }
    
	//Создаем формат данных для клавиатуры и передаём его в структуру 
	//DirectInput Data Format (DIDATAFORMAT)
    if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard))) 
    {
      MessageBoxA(NULL,"Can't Set KeyboardDataFormat","GreenRay Engine Error",
	  MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
      m_pMouse->Release();
	  m_pKeyboard->Release();
	  m_pDirectInput->Release();
      m_pDirectInput = NULL;
      Result = E_FAIL;
    }

    if (FAILED(m_pKeyboard->SetCooperativeLevel(WndLayer,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
    {
      MessageBoxA(NULL,"Can't Set Keyboard CooperativeLevel","GreenRay Engine Error",
	  MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
      m_pMouse->Release();
	  m_pKeyboard->Release();
	  m_pDirectInput->Release();
      m_pDirectInput = NULL;
	  Result = E_FAIL;
    }

    /*DIPROPDWORD dipdw = {
	{
      sizeof(DIPROPDWORD),  //diph.dwSize
	  sizeof(DIPROPHEADER), //diph.dwHeaderSize
	  0,                    //diph.dwObj
	  DIPH_DEVICE,},        //diph.dwHow
	  DINPUT_BUFFERSIZE256, //dwData
    };
           
    if (FAILED(m_pKeyboard->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph)))
    {
      MessageBoxA(NULL,"Can't Set Property For KeyboardDevice","GreenRay Engine Error",
	  MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
      m_pMouse->Release();
	  m_pKeyboard->Release();
	  m_pDirectInput->Release();
      m_pDirectInput = NULL;
	  return;
    } */
  }
  return Result;
} //EndCreateInputDevicesFunction

//-----------------------------------------------------------------------------
// Name: CGRCamera::UpdateCamera()
// Desc: Poll The DirectInput Devices
//-----------------------------------------------------------------------------
void CGRCamera::UpdateCamera()
{ 
  DIMOUSESTATE2 Mouse_Data; //Value Keep Mouse States
  char Key_Buffer[256]; //Buffer To Hold Prev. Keyboard States

  //d_pDevice->TestCooperativeLevel(); 
  float m_fForward = 0;
  float m_fSideward = 0;

  //Set Camera States By Default
  m_bSpaceDown = false;
  m_bMbLeftDown = false;

  //Update Camera Delta Timer Every Frame
  if (m_pDeltaTimer != NULL) m_pDeltaTimer->UpdateTimer();

  //Donation: GetMouseState 
  if (m_pMouse != NULL && m_pDeltaTimer != NULL) //Check On Ready Mouse
  {         
    //Get Mouse Input Data
    //Перехватываем и обрабатываем входные данные поступающие от "мыши". 
    ZeroMemory(&Mouse_Data,sizeof(Mouse_Data));
    m_pMouse->GetDeviceState(sizeof(Mouse_Data),&Mouse_Data);
    //m_pMouse->Acquire();

    //Here Getting Mouse States:
	//Здесь получаем непосредственно координаты поступающие при движении
	//мышкой.
	Pitch-= (double)Mouse_Data.lY*m_pDeltaTimer->GetTime()*m_fMouseSpeed;
	Yaw-= (double)Mouse_Data.lX*m_pDeltaTimer->GetTime()*m_fMouseSpeed;
   
	//Mouse Button States (0- It's Button Up)
	//Обрабатываем нажатия кнопок мыши:
    
	//Здесь происходит обработка события когда нажата левая кнопка мыши
	//Mb Left Down
	if ((Mouse_Data.rgbButtons[0x00] & 0x80) != 0) 
	{
	  //Update Weapon Shoot By Timer For Automatic Mode:
	  /*static float fLastTime = 0.0f; //Timing Variality Directives
	  double fTime = m_pTimer->GetAbsoluteTime();

	  //Update The Scene Stats Once Per Second
      if (fTime-fLastTime > 0.097f)
      {
        fLastTime = fTime;
        gr_Weapon->WeaponShoot(); 
      }*/
		
      //Here Update Left Mouse Button Down For Single Mode:
	  if (!m_nMbLDownOnce) m_bMbLeftDown = true; 
	  m_nMbLDownOnce = true;
	}
    
	//Mb Left UP
	if ((Mouse_Data.rgbButtons[0x00] & 0x80) == 0) 
	{ 
	  m_nMbLDownOnce = false; 
	}  
    

  



  }

  //Donation: GetKeyboardState
  if (m_pKeyboard != NULL) //Check On Ready Keyboard
  {
    //Keyboard Input Data
	//Перехватываем и обрабатываем входные данные поступающие от клавиатуры. 
    ZeroMemory(&Key_Buffer,sizeof(Key_Buffer));
    m_pKeyboard->GetDeviceState(sizeof(Key_Buffer),Key_Buffer);
    //m_pKeyboard->Acquire();

    //Keyboard Contol Input Mode
	//Здесь реализовано WASD-овское управление игроком т.е вперёд, назад, вбок 
	//и Strafe соответственно, не долго думая при желании можно переделать его 
	//и в "тракторное" управление, кому как нравится и привычнее, тогда
	//в клавиатурном буфере нужно поменять на: Key_Buffer[DIK_UP], Key_Buffer[DIK_DOWN],
	//Key_Buffer[DIK_LEFT], Key_Buffer[DIK_RIGHT] соответственно..  
    if (Key_Buffer[DIK_W] & 0x80) m_fForward = m_fCameraSpeed*m_pDeltaTimer->GetTime();
    if (Key_Buffer[DIK_S] & 0x80) m_fForward = -m_fCameraSpeed*m_pDeltaTimer->GetTime();
    if (Key_Buffer[DIK_A] & 0x80) m_fSideward = -m_fCameraSpeed*m_pDeltaTimer->GetTime();
    if (Key_Buffer[DIK_D] & 0x80) m_fSideward = m_fCameraSpeed*m_pDeltaTimer->GetTime();
	
	//Эта строка понадобится, когда к движку будет подключен PhysX :))
	//и здесь будет обрабатываться "прыжок" игрока..
	if (Key_Buffer[DIK_SPACE] & 0x80) m_bSpaceDown = true;    

	//Release Camera Control
    if (Key_Buffer[DIK_ESCAPE] & 0x80) UseCamera(false);
  }

  //Restrict The Ability To Look Too High Or Too Low
  //Здесь происходит ограничение взгляда камеры мышью в 
  //самой верхней точке, т.е когда мы смотрим в потолок, и 
  //в самой нижней когда смотрим в пол..
  //Если не применять этот "ограничитель" то при движении мышкой
  //камера начнет прокручиваться дальше и получится совсем ненужный
  //эффект вставания игрока в "мостик" или на голову при взгляде вниз.. :)))) 
  if (Pitch < -1.56f) Pitch = -1.56f; //Y
  if (Pitch > 1.56f) Pitch = 1.56f;
  
  if (Yaw >= 6.28f) Yaw = 0.0f;  //X
  if (Yaw <= -6.28f) Yaw = 0.0f;

  //Get Camera X,Y,Z Position Coordinates
  //Получение позиции камеры игрока в пространстве..
  //Это пригодится к примеру когда мы будем делать привязку SkyBox к
  //нашей камере и много еще для чего в дальнейшем..
  PosX = (cosf(Pitch)*cosf(Yaw)*10.0f);
  PosY = (sinf(Pitch)*10.0f);
  PosZ = (sinf(Yaw)*cosf(Pitch)*10.0f);

  //Здесь идёт расчёт и преобразоание по формуле - позиций, координат, векторов и
  //передача их в матрицы:
  //Set The Target Of The Camera
  Target = D3DXVECTOR3(PosX,PosY,PosZ)+Position;

  //Update The Look Vector
  D3DXVec3Normalize(&Look_Vector,&(Target-Position));
  D3DXVECTOR3 XZLook_Vector = Look_Vector;
  XZLook_Vector.y = 0;
  D3DXVec3Normalize(&XZLook_Vector,&XZLook_Vector);
  D3DXVECTOR3 Side_Vector(XZLook_Vector.z,0.0f,-XZLook_Vector.x);
  Velocity = (XZLook_Vector*m_fForward)+(Side_Vector*m_fSideward);
  

  //Временные преобразования, после подключения PhysX их не станет..
  //Apply Transform
  /*Position+= (XZLook_Vector*m_fForward)+(Side_Vector*m_fSideward);
  Target+= (XZLook_Vector*m_fForward)+(Side_Vector*m_fSideward);

  //Update The View Matix
  D3DXMatrixLookAtLH(&View,&Position,&Target,&Up);
  //Update The Active View
  d_pDevice->SetTransform(D3DTS_VIEW,&View);*/
} //EndUpdateCameraProcedure

//-----------------------------------------------------------------------------
// Name: CGRCamera::UseCamera()
// Desc: Use Camera
//-----------------------------------------------------------------------------
void CGRCamera::UseCamera(bool UseConnect)
{  
  if (UseConnect) 
  {  
    m_pMouse->Acquire();    //Control Mouse
	m_pKeyboard->Acquire(); //Control Keyboard
  }
  else
  {
    m_pMouse->Unacquire();    //UnControl Mouse
    m_pKeyboard->Unacquire(); //UnControl Keyboard
  }
} //EndUseCameraProcedure

//-----------------------------------------------------------------------------