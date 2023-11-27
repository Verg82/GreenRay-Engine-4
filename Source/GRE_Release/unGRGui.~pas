{*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// unGRGui.pas
// It's A GreenRay Engine GUI Form.
// Programming By: Max Tantser (Verg)
// Date Creation: 29 August 2005
// Last Changing: 02 January 2012
// Author Rights By: Native Soft Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2012
*******************************************************************************}
unit unGRGui;
interface uses
  Windows,Messages,SysUtils,Classes,Graphics,Controls,Forms,ComCtrls,ExtCtrls,
  Tsa_Helper,RbSplitter,Menus,TSAPanel2,StdCtrls,Buttons;

type
  TFRM_GRGui = class(TForm)
    Status_Bar: TStatusBar;
    MainMenu1: TMainMenu;
    M_File: TMenuItem;
    M_Tools: TMenuItem;
    M_Options: TMenuItem;
    M_Help: TMenuItem;
    M_Exit: TMenuItem;
    M_Doc: TMenuItem;
    M_Split01: TMenuItem;
    M_About: TMenuItem;
    Pnl_Top: TPanel;
    Img_Header: TImage;
    Pnl_Client: TPanel;
    Pnl_Tools: TPanel;
    Pnl_EngineParam: TTSAPanel;
    Rb_Splitter1: TRbSplitter;
    Pnl_D3DOutput: TPanel;
    SpeedButton1: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject;var Action: TCloseAction);
    procedure M_ExitClick(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
  private
    {Private Declarations}
    Tmr_Render: TTsaTimer;
    procedure OnRenderUpdate(Sender: TObject);
    procedure WMGetMinMaxInfo(var Msg: TWMGetMinMaxInfo); message WM_GETMINMAXINFO;
  public
    {Public Declarations}
  end;

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////  
var FRM_GRGui: TFRM_GRGui;
implementation uses GR_Engine_dll;
{$R *.dfm}

//------------------------------------------------------------------------------
// Name: TFRM_GRGui.WMGetMinMaxInfo()
// Desc:
//------------------------------------------------------------------------------
procedure TFRM_GRGui.WMGetMinMaxInfo(var Msg: TWMGetMinMaxInfo);
begin
  inherited;
  Msg.MinMaxInfo^.ptMinTrackSize:= Point(640,480); //MinForm Size
end; //EndWMGetMinMaxInfoProcedure

//------------------------------------------------------------------------------
// Name: TFRM_GRGui.M_ExitClick()
// Desc:
//------------------------------------------------------------------------------
procedure TFRM_GRGui.M_ExitClick(Sender: TObject);
begin
  Close();
end; //EndM_ExitClickProcedure

//------------------------------------------------------------------------------
// Name: TFRM_GRGui.FormCreate()
// Desc:
//------------------------------------------------------------------------------
procedure TFRM_GRGui.FormCreate(Sender: TObject);
begin
  //Set Default Parameters
  Tmr_Render:= nil;
  //Pnl_D3DOutput.DoubleBuffered:= True;

  //Create GreenRay Direct3D Interface
  if (GR_CreateDirect3D(Handle,Pnl_D3DOutput.Handle)) then begin

  //Set Simple SkyBox Materils
  GR_SetSkyBoxMaterials('GrandCanyon');

  Tmr_Render:= TTsaTimer.Create(nil);
  Tmr_Render.Interval:= 1;
  Tmr_Render.OnTimer:= OnRenderUpdate;
  Tmr_Render.Enabled:= True; end;
end; //EndFormCreateProcedure

//------------------------------------------------------------------------------
// Name: TFRM_GRGui.FormClose()
// Desc:
//------------------------------------------------------------------------------
procedure TFRM_GRGui.FormClose(Sender: TObject;var Action: TCloseAction);
begin
  //Release GreenRay Direct3D Interface
  ReleaseDirect3D();
end; //EndFormCloseProcedure

//------------------------------------------------------------------------------
// Name: TFRM_GRGui.OnRenderUpdate()
// Desc:
//------------------------------------------------------------------------------
procedure TFRM_GRGui.OnRenderUpdate(Sender: TObject);
begin
  //Pool Render Scene Update
  RenderDirect3D();
end; //EndOnRenderUpdateProcedure


procedure TFRM_GRGui.SpeedButton1Click(Sender: TObject);
begin
  UseCamera(True);
end;

end.
