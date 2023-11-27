object FRM_GRGui: TFRM_GRGui
  Left = 210
  Top = 133
  Width = 920
  Height = 587
  Caption = 'GreenRay Engine 4.0'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Pnl_Client: TPanel
    Left = 0
    Top = 41
    Width = 904
    Height = 469
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    object Rb_Splitter1: TRbSplitter
      Left = 652
      Top = 0
      Width = 4
      Height = 469
      Align = alRight
      Color = clGray
      ParentColor = False
      GradientType = gtHorizontal
      GripAlign = gaVertical
      FadeSpeed = fsMedium
      Colors.DefaultFrom = clBtnFace
      Colors.DefaultTo = clGray
      Colors.OverFrom = clWhite
      Colors.OverTo = clRed
      ShowGrip = True
      DrawAll = True
    end
    object Pnl_Tools: TPanel
      Left = 656
      Top = 0
      Width = 248
      Height = 469
      Align = alRight
      BevelOuter = bvNone
      Color = clMedGray
      TabOrder = 0
      object Pnl_EngineParam: TTSAPanel
        Left = 0
        Top = 0
        Width = 248
        Height = 469
        Animated = False
        Color = clBtnFace
        BorderColor = 4276545
        HeaderVisible = True
        HeaderColor = 13555414
        HeaderMinColor = 15395562
        HeaderHeight = 17
        HeaderFont.Charset = RUSSIAN_CHARSET
        HeaderFont.Color = clWhite
        HeaderFont.Height = -11
        HeaderFont.Name = 'Tahoma'
        HeaderFont.Style = []
        HeaderCaption = 'Engine Parameters:'
        HeaderIcon = '<>'
        Moveable = False
        UseGradient = True
        GradientFrom = 6974058
        GradientTo = 3158064
        GradientStyle = gsTop
        CloseButton = False
        CloTrasparent = False
        UseRounded = False
        UseMover = False
        BottomLine = False
        BottomLineTop = 0
        BottomLineColor = clBlack
        ActiveCanvas = False
        Align = alClient
        TabOrder = 0
      end
    end
    object Pnl_D3DOutput: TPanel
      Left = 0
      Top = 0
      Width = 652
      Height = 469
      Align = alClient
      BevelOuter = bvLowered
      Color = 5789784
      TabOrder = 1
    end
  end
  object Status_Bar: TStatusBar
    Left = 0
    Top = 510
    Width = 904
    Height = 19
    Panels = <
      item
        Text = '(C) Digital Dreams Development Inc., 2012'
        Width = 230
      end
      item
        Text = 'Debug mode..'
        Width = 50
      end>
  end
  object Pnl_Top: TPanel
    Left = 0
    Top = 0
    Width = 904
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object Img_Header: TImage
      Left = 0
      Top = 0
      Width = 904
      Height = 41
      Align = alClient
      Picture.Data = {
        07544269746D6170FA000000424DFA0000000000000036000000280000000100
        0000310000000100180000000000C40000000000000000000000000000000000
        00008484840084848400E5E5E500E5E5E500E0E0E000DEDEDE00DBDBDB00DADA
        DA00D8D8D800D6D6D600D5D5D500D0D0D000D0D0D000D0D0D000CDCDCD00CDCD
        CD00C9C9C900C9C9C900C4C4C400C1C1C100BFBFBF00BEBEBE00BDBDBD00B9B9
        B900B9B9B900B8B8B800B6B6B600B4B4B400B3B3B300B2B2B200AEAEAE00ABAB
        AB00ABABAB00A9A9A900A5A5A500A5A5A5009F9F9F009F9F9F009F9F9F009D9D
        9D009C9C9C0099999900969696009494940095959500909090008D8D8D008E8E
        8E008A8A8A00}
      Stretch = True
    end
    object SpeedButton1: TSpeedButton
      Left = 8
      Top = 3
      Width = 33
      Height = 33
      Flat = True
      Glyph.Data = {
        060E0000424D060E000000000000360000002800000022000000220000000100
        180000000000D00D000000000000000000000000000000000000F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        0000F3F3F3000000000000000000000000000000000000000000000000000000
        0000000000000101010101010101010101011E1E1E3D3D3D4C4C4C6262626363
        634F4F4F4444442828280A0A0A01010101010101010101010101010101010100
        0000000000F3F3F30000F3F3F300000000000000000000000000000001010100
        0000000000000000000000000000010101020202323232767676A2A2A2C0C0C0
        CACACACCCCCCCDCDCDC8C8C8C1C1C1ADADAD8F8F8F6666662626260303030101
        01010101010101010101000000F3F3F30000F3F3F30000000000000000000000
        00010101000000000000000000000000000000000000323232696969B6B6B6D1
        D1D1E0E0E0E8E8E8F0F0F0F5F5F5F6F6F6F5F5F5F2F2F2E8E8E8DBDBDBC5C5C5
        B0B0B07070701E1E1E010101010101010101000000F3F3F30000F3F3F3000000
        0000000000000000000101010101010000000000000000000000003232329494
        94C6C6C6E0E0E0E9E9E9EFEFEFF6F6F6FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFCFCFCF3F3F3DBDBDBC3C3C3959595272727010101010101000000F3F3F3
        0000F3F3F3000000000000000000000000010101010101000000000000000000
        3232329C9C9CC6C6C6E1E1E1E6E6E6EBEBEBEFEFEFF4F4F4F9F9F9FEFEFEFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEAEACCCCCC9A9A9A1D1D1D01
        0101000000F3F3F30000F3F3F300000000000000000000000000000001010100
        0000000000000000A0A0A0CBCBCBE1E1E1DFDFDFE5E5E5E9E9E9EEEEEEF4F4F4
        F9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE9E9
        E9C5C5C5727272040404000000F3F3F30000F3F3F30000000000000000000000
        00000000000000000000000000323232BEBEBEDFDFDFDFDFDFDFDFDFE0E0E0E1
        E1E1E2E2E2E5E5E5E8E8E8ECECECF2F2F2F6F6F6FDFDFDFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFDFDFDDBDBDBB0B0B02A2A2A000000F3F3F30000F3F3F3000000
        000000000000000000000000010101000000000000323232DCDCDCDEDEDEDFDF
        DFE1E1E1E2E2E2E1E1E1E1E1E1E0E0E0DFDFDFDEDEDEDEDEDEE0E0E0E2E2E2EC
        ECECFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFECECECC3C3C3515151000000F3F3F3
        0000F3F3F3000000000000000000000000000000000000000000000000999999
        E4E4E4E2E2E2E1E1E1E2E2E2E4E4E4E4E4E4E4E4E4E4E4E4E3E3E3E3E3E3E2E2
        E2E0E0E0DEDEDEDCDCDCDFDFDFF2F2F2FFFFFFFFFFFFFFFFFFF8F8F8CDCDCD77
        7777000000F3F3F30000F3F3F300000000000000000000000000000000000000
        0000000000999999EAEAEAFDFDFDE4E4E4E4E4E4E6E6E6E7E7E7E6E6E6E6E6E6
        E6E6E6E5E5E5E4E4E4E3E3E3E2E2E2E0E0E0DEDEDEDCDCDCF5F5F5FFFFFFFFFF
        FFFEFEFED2D2D2858585000000F3F3F30000F3F3F30000000000000000000000
        00000000000000000000010101999999EAEAEAFDFDFDFDFDFDEBEBEBE9E9E9EA
        EAEAE9E9E9E9E9E9E9E9E9E7E7E7E6E6E6E4E4E4E4E4E4E1E1E1E0E0E0DCDCDC
        E6E6E6FFFFFFFFFFFFFEFEFED3D3D3858585000000F3F3F30000F3F3F3000000
        000000000000000000000000000000000000101010A9A9A9EFEFEFFEFEFEFFFF
        FFF7F7F7EBEBEBECECECECECECEBEBEBEBEBEBEAEAEAE9E9E9E7E7E7E6E6E6E4
        E4E4E2E2E2E0E0E0E1E1E1FAFAFAFFFFFFFFFFFFCFCFCF727272000000F3F3F3
        0000F3F3F3000000000000000000000000000000000000000000181818B1B1B1
        F4F4F4FFFFFFFFFFFFFFFFFFF2F2F2ECECECEEEEEEEEEEEEEDEDEDEBEBEBEBEB
        EBE9E9E9E7E7E7E5E5E5E4E4E4E1E1E1DDDDDDF6F6F6FFFFFFF9F9F9C8C8C853
        5353000000F3F3F30000F3F3F300000000000000000000000000000000000000
        00002A2A2AC0C0C0F7F7F7FFFFFFFFFFFFFFFFFFFDFDFDF1F1F1F0F0F0F0F0F0
        F0F0F0EEEEEEEDEDEDEBEBEBE9E9E9E7E7E7E5E5E5E3E3E3DFDFDFF0F0F0FFFF
        FFF2F2F2BDBDBD2C2C2C000000F3F3F30000F3F3F30000000000000000000000
        000000000000000000002F2F2FC6C6C6F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFB
        FBFBF3F3F3F2F2F2F1F1F1F0F0F0EEEEEEECECECEBEBEBE8E8E8E6E6E6E4E4E4
        E0E0E0EDEDEDFFFFFFE4E4E48C8C8C0D0D0D000000F3F3F30000F3F3F3000000
        0000000000000000000000000000000000002E2E2EC8C8C8FBFBFBFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFAFAFAF4F4F4F2F2F2F1F1F1F0F0F0EEEEEEECECECEA
        EAEAE7E7E7E5E5E5E2E2E2EDEDEDFDFDFDCFCFCF565656010101000000F3F3F3
        0000F3F3F30000000000000000000000000000000000000000002E2E2EC3C3C3
        FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9F4F4F4F3F3F3F1F1
        F1EFEFEFEDEDEDEBEBEBE8E8E8E5E5E5E3E3E3E9E9E9F1F1F1A7A7A717171701
        0101000000F3F3F30000F3F3F300000000000000000000000000000000000000
        00002E2E2EA1A1A1EAEAEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        F9F9F9F5F5F5F2F2F2F0F0F0EEEEEEEBEBEBE9E9E9E6E6E6E4E4E4E6E6E6D2D2
        D2646464010101010101000000F3F3F30000F3F3F30000000000000000000000
        00000000000000000000303030BCBCBCB3B3B3D9D9D9FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFF8F8F8F3F3F3F1F1F1EEEEEEECECECEAEAEAE7E7E7
        E5E5E5E3E3E3A4A4A4181818010101010101000000F3F3F30000F3F3F3000000
        000000000000000000000000000000000000232323BEBEBEF3F3F3C4C4C4BBBB
        BBE4E4E4FDFDFDFFFFFFFFFFFFFFFFFFE7EBEBF3F5F5F7F7F7F1F1F1F0F0F0ED
        EDEDECECECEAEAEAE9E9E9CBCBCB545454010101010101000000000000F3F3F3
        0000F3F3F3000000000000000000000000000000000000000000151515B2B2B2
        FFFFFFFFFFFFE8E8E8C0C0C0BABABACECECEE1E2E2ABA9A98C65658D7E7EEDEE
        EEFBFBFBF0F0F0E9E9E9DFDFDFD3D3D3BFBFBF8B8B8B0A0A0A01010101010100
        0000000000F3F3F30000F3F3F300000000000000000000000000000000000000
        0000070707A5A5A5FEFEFEFFFFFFFFFFFFFFFFFFF2F2F2DADADAA3A6A6896161
        D665659E6767A0A3A3BDBDBDB9B9B9BCBCBCB9B9B9BDBDBDA3A3A33131310101
        01010101000000000000000000F3F3F30000F3F3F30000000000000000000101
        01010101010101010101010101828282F4F4F4FFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFACB1B1AF5B5BD46666926A6AD2D5D5EDEDEDE4E4E4E5E5E5E8E8E8CACACA
        585858010101010101000000000000000000000000F3F3F30000F3F3F3000000
        000000010101010101010101010101010101010101535353DADADAFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFACB2B2AF5A5AB85E5E8C8585FAFBFBF5F5F5ECECECEF
        EFEFD6D6D67676760A0A0A010101010101000000000000000000000000F3F3F3
        0000F3F3F30000000101010101011010103838384040402D2D2D0606060E0E0E
        7E7E7ED1D1D1F2F2F2FFFFFFFFFFFFFFFFFFE0E1E1867C7C958C8CE8EAEAFFFF
        FFEFEFEFEEEEEED3D3D37F7F7F10101001010101010100000000000000000000
        0000000000F3F3F30000F3F3F30000000101012828288C8C8CC1C1C1CBCBCBBC
        BCBC7373730C0C0C0B0B0B454545848484BEBEBED3D3D3E8E8E8C7C7C7CDCECE
        FDFEFEFCFCFCF4F4F4E5E5E5C2C2C26C6C6C1010100101010101010000000000
        00000000000000000000000000F3F3F30000F3F3F3000000181818929292BFBF
        BF919191787878BDBDBDD0D0D07E7E7E0A0A0A010101030303171717898989BD
        BDBDB2B2B2ACACACA2A2A29797978A8A8A727272363636020202010101000000
        000000000000000000000000000000000000000000F3F3F30000F3F3F3000000
        363636AEAEAE8F8F8F0E0E0E0101014B4B4BBEBEBED5D5D58080801F1F1F0D0D
        0D525252C9C9C9FFFFFFFAFAFA9A9A9A19191909090901010101010101010100
        0000000000000000000000000000000000000000000000000000000000F3F3F3
        0000F3F3F30000001E1E1EA1A1A18F8F8F0303030101010101014C4C4CBDBDBD
        DFDFDFB8B8B8A8A8A8D4D4D4FBFBFBF1F1F1ABABAB2727270101010101010000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000F3F3F30000F3F3F30000000404044A4A4AA0A0A064646406060601
        01010303034E4E4EB7B7B7E0E0E0F3F3F3EBEBEBCFCFCF929292262626010101
        0101010000000000000000000000000000000000000000000000000000000000
        00000000000000000000000000F3F3F30000F3F3F30000000000000000004949
        498C8C8C7C7C7C4C4C4C0F0F0F0101012929296565658989897272724040400C
        0C0C010101010101000000000000000000000000000000000000000000000000
        000000000000000000000000000000000000000000F3F3F30000F3F3F3000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000F3F3F3
        0000F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F30000}
      OnClick = SpeedButton1Click
    end
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 56
    object M_File: TMenuItem
      Caption = '&File'
      object M_Exit: TMenuItem
        Caption = '&Exit'
        ShortCut = 32883
        OnClick = M_ExitClick
      end
    end
    object M_Tools: TMenuItem
      Caption = '&Tools'
    end
    object M_Options: TMenuItem
      Caption = '&Options'
    end
    object M_Help: TMenuItem
      Caption = '&Help'
      object M_Doc: TMenuItem
        Caption = '&GreenRay Tecnical Documentation..'
      end
      object M_Split01: TMenuItem
        Caption = '-'
      end
      object M_About: TMenuItem
        Caption = '&About'
      end
    end
  end
end
