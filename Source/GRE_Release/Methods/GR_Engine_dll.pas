{*******************************************************************************
// It's Unofficial Version Of The GreenRay Engine v.4.0.0.1
// GR_Engine_dll.pas
// It's A GreenRay Engine Based Interface Library.
// Programming By: Max Tantser (Verg)
// Date Creation: 29 August 2005
// Last Changing: 02 January 2012
// Author Rights By: Native Soft Technology And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2012
*******************************************************************************}
unit GR_Engine_dll;
interface uses WinTypes,Variants;

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
const GREngineLib = 'GREngine.dll';

//------------------------------------------------------------------------------
//GreenRay DLL Engine Directives
function CreateGR(WndLayer: HWND): HResult; cdecl; external GREngineLib;
procedure CreateDirect3DInterface(WndLayer: HWND); cdecl; external GREngineLib;
procedure RenderDirect3D(); cdecl; external GREngineLib;
procedure ReleaseDirect3D(); cdecl; external GREngineLib;
procedure SetWindowedMode(Windowed: Bool); cdecl; external GREngineLib;
procedure SetScreenResolution(Width,Height: Integer); cdecl; external GREngineLib;
procedure SetSkyBoxMaterials(ZNeg,XNeg,ZPos,XPos,YNeg,YPos: PChar); cdecl; external GREngineLib;
procedure UseCamera(UseConnect: Bool); cdecl; external GREngineLib;
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// Exporting Procedures Or Functions
////////////////////////////////////////////////////////////////////////////////
function GR_CreateDirect3D(FormLayer,Canvas: HWND): Bool;
procedure GR_SetSkyBoxMaterials(SkyName: string);

implementation uses Tsa_FileRoutines;

//------------------------------------------------------------------------------
// Name: GR_CreateDirect3D()
// Desc:
//------------------------------------------------------------------------------
function GR_CreateDirect3D(FormLayer,Canvas: HWND): Bool;
begin
  //Set Default Parameters
  Result:= False;

  //Before Release Direct3D If It's Startup
  ReleaseDirect3D();

  //Create GreenRay Engine Interface
  if (CreateGR(FormLayer) <> NULL) then begin

  //Set Direct3D Parameters
  SetScreenResolution(640,480);
  SetWindowedMode(True);

  //Create Direct3D Interface
  CreateDirect3DInterface(Canvas);

  //UseCamera(True);

  Result:= True; end;
end; //EndGR_CreateDirect3DFunction

//------------------------------------------------------------------------------
// Name: GR_SetSkyBoxMaterials()
// Desc:
//------------------------------------------------------------------------------
procedure GR_SetSkyBoxMaterials(SkyName: string);
var TexPath: PChar;
begin
  //Set Valid Texture Pathes
  TexPath:= PChar(ExtractFilePath(ParamStr(0))+'Textures\SkyBox\');

  //Here It's Parameters Setting Up To Engine
  SetSkyBoxMaterials(PChar(TexPath+SkyName+'\zneg.png'),
                     PChar(TexPath+SkyName+'\xneg.png'),
                     PChar(TexPath+SkyName+'\zpos.png'),
                     PChar(TexPath+SkyName+'\xpos.png'),
                     PChar(TexPath+SkyName+'\yneg.png'),
                     PChar(TexPath+SkyName+'\ypos.png'));
end; //EndGR_SetSkyBoxMaterialsProcedure

//------------------------------------------------------------------------------

end.
 