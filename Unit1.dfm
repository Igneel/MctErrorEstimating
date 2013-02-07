object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 747
  ClientWidth = 1076
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PG: TPageControl
    Left = -1
    Top = 1
    Width = 1074
    Height = 744
    ActivePage = TabSheet1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1043#1083#1072#1074#1085#1072#1103
      object Label1: TLabel
        Left = 16
        Top = 3
        Width = 132
        Height = 13
        Caption = #1064#1072#1075' '#1087#1086' '#1084#1072#1075#1085#1080#1090#1085#1086#1084#1091' '#1087#1086#1083#1102':'
      end
      object Label8: TLabel
        Left = 496
        Top = 515
        Width = 82
        Height = 13
        Caption = #1050#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090#1099':'
      end
      object Label9: TLabel
        Left = 469
        Top = 528
        Width = 21
        Height = 13
        Caption = 'sxx:'
      end
      object Label10: TLabel
        Left = 469
        Top = 559
        Width = 21
        Height = 13
        Caption = 'sxy:'
      end
      object Label2: TLabel
        Left = 752
        Top = 512
        Width = 26
        Height = 13
        Caption = #1057#1050#1054':'
      end
      object Label4: TLabel
        Left = 729
        Top = 530
        Width = 21
        Height = 13
        Caption = 'sxx:'
      end
      object Label7: TLabel
        Left = 729
        Top = 561
        Width = 21
        Height = 13
        Caption = 'sxy:'
      end
      object Label3: TLabel
        Left = 830
        Top = 515
        Width = 215
        Height = 13
        Caption = #1057#1050#1054'(%) '#1089#1095#1080#1090#1072#1077#1090#1089#1103' '#1086#1090' '#1089#1088#1077#1076#1085#1077#1075#1086' '#1079#1085#1072#1095#1077#1085#1080#1103':'
      end
      object Label5: TLabel
        Left = 897
        Top = 530
        Width = 21
        Height = 13
        Caption = 'sxx:'
      end
      object Label6: TLabel
        Left = 897
        Top = 561
        Width = 21
        Height = 13
        Caption = 'sxy:'
      end
      object Label11: TLabel
        Left = 752
        Top = 632
        Width = 109
        Height = 13
        Caption = #1064#1072#1075' '#1087#1086' '#1090#1077#1084#1087#1077#1088#1072#1090#1091#1088#1077':'
      end
      object LabeledEdit7: TLabeledEdit
        Left = 456
        Top = 635
        Width = 121
        Height = 21
        EditLabel.Width = 52
        EditLabel.Height = 13
        EditLabel.Caption = 'k (1.3-1.5)'
        TabOrder = 0
        Text = '1,3'
        OnKeyPress = g_Nz_parKeyPress
      end
      object LabeledEdit8: TLabeledEdit
        Left = 457
        Top = 675
        Width = 121
        Height = 21
        EditLabel.Width = 162
        EditLabel.Height = 13
        EditLabel.Caption = #1050#1086#1085#1094#1077#1085#1090#1088#1072#1094#1080#1103' '#1090#1103#1078#1077#1083#1099#1093' '#1076#1099#1088#1086#1082':'
        TabOrder = 1
        Text = '1e22'
        OnKeyPress = g_Nz_parKeyPress
      end
      object LabeledEdit6: TLabeledEdit
        Left = 320
        Top = 675
        Width = 121
        Height = 21
        EditLabel.Width = 35
        EditLabel.Height = 13
        EditLabel.Caption = #1040'(5-8):'
        TabOrder = 2
        Text = '5'
        OnKeyPress = g_Nz_parKeyPress
      end
      object LabeledEdit1: TLabeledEdit
        Left = 32
        Top = 635
        Width = 121
        Height = 21
        EditLabel.Width = 93
        EditLabel.Height = 13
        EditLabel.Caption = #1058#1086#1083#1097#1080#1085#1072' '#1086#1073#1088#1072#1079#1094#1072':'
        TabOrder = 3
        Text = '1e-5'
        OnKeyPress = g_Nz_parKeyPress
      end
      object LabeledEdit3: TLabeledEdit
        Left = 176
        Top = 635
        Width = 121
        Height = 21
        EditLabel.Width = 74
        EditLabel.Height = 13
        EditLabel.Caption = #1057#1080#1083#1072' '#1090#1086#1082#1072' ('#1040'):'
        TabOrder = 4
        Text = '1e-3'
        OnKeyPress = g_Nz_parKeyPress
      end
      object LabeledEdit5: TLabeledEdit
        Left = 320
        Top = 635
        Width = 121
        Height = 21
        EditLabel.Width = 88
        EditLabel.Height = 13
        EditLabel.Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' ('#1050'):'
        TabOrder = 5
        Text = '77'
        OnKeyPress = g_Nz_parKeyPress
      end
      object LabeledEdit4: TLabeledEdit
        Left = 176
        Top = 675
        Width = 121
        Height = 21
        EditLabel.Width = 127
        EditLabel.Height = 13
        EditLabel.Caption = #1052#1086#1083#1100#1085#1099#1081' '#1089#1086#1089#1090#1072#1074' '#1082#1072#1076#1084#1080#1103':'
        TabOrder = 6
        Text = '0,22'
        OnKeyPress = g_Nz_parKeyPress
      end
      object LabeledEdit2: TLabeledEdit
        Left = 32
        Top = 675
        Width = 121
        Height = 21
        EditLabel.Width = 15
        EditLabel.Height = 13
        EditLabel.Caption = 'c/b'
        TabOrder = 7
        Text = '3'
        OnKeyPress = g_Nz_parKeyPress
      end
      object Button5: TButton
        Left = 16
        Top = 587
        Width = 169
        Height = 25
        Caption = #1043#1077#1085#1077#1088#1072#1090#1086#1088' '#1073#1077#1083#1086#1075#1086' '#1096#1091#1084#1072
        TabOrder = 8
        OnClick = Button5Click
      end
      object Button3: TButton
        Left = 16
        Top = 554
        Width = 169
        Height = 25
        Caption = #1043#1077#1085#1077#1088#1072#1090#1086#1088' '#1085#1086#1088#1084#1072#1083#1100#1085#1086#1075#1086' '#1096#1091#1084#1072
        Enabled = False
        TabOrder = 9
        OnClick = Button3Click
      end
      object Button1: TButton
        Left = 16
        Top = 523
        Width = 75
        Height = 25
        Caption = #1056#1072#1089#1089#1095#1080#1090#1072#1090#1100
        Enabled = False
        TabOrder = 10
        OnClick = Button1Click
      end
      object Memo1: TMemo
        Left = 16
        Top = 259
        Width = 209
        Height = 209
        TabOrder = 11
      end
      object Button6: TButton
        Left = 16
        Top = 211
        Width = 75
        Height = 25
        Caption = #1047#1072#1087#1086#1083#1085#1080#1090#1100
        TabOrder = 12
        OnClick = Button6Click
      end
      object Button7: TButton
        Left = 150
        Top = 211
        Width = 75
        Height = 25
        Caption = #1043#1088#1072#1092#1080#1082#1080
        Enabled = False
        TabOrder = 13
        OnClick = Button7Click
      end
      object g_Nz_par: TStringGrid
        Left = 16
        Top = 59
        Width = 282
        Height = 137
        ColCount = 3
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        TabOrder = 14
        OnKeyPress = g_Nz_parKeyPress
        ColWidths = (
          64
          132
          76)
        RowHeights = (
          24
          24
          25
          24
          24)
      end
      object g_hsize: TEdit
        Left = 16
        Top = 19
        Width = 121
        Height = 21
        TabOrder = 15
        Text = '0,2'
      end
      object Chart1: TChart
        Left = 304
        Top = 3
        Width = 335
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'xx')
        View3D = False
        TabOrder = 16
        ColorPaletteIndex = 13
        object Series1: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart2: TChart
        Left = 304
        Top = 259
        Width = 335
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'xy')
        View3D = False
        TabOrder = 17
        ColorPaletteIndex = 13
        object Series2: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart4: TChart
        Left = 645
        Top = 259
        Width = 400
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'xy')
        View3D = False
        TabOrder = 18
        ColorPaletteIndex = 13
        object Series4: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart3: TChart
        Left = 645
        Top = 3
        Width = 400
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'xx')
        View3D = False
        TabOrder = 19
        ColorPaletteIndex = 13
        object Series3: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Button4: TButton
        Left = 645
        Top = 523
        Width = 75
        Height = 25
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        TabOrder = 20
        OnClick = Button4Click
      end
      object Edit5: TEdit
        Left = 496
        Top = 525
        Width = 121
        Height = 21
        TabOrder = 21
        Text = '50'
        OnKeyPress = g_Nz_parKeyPress
      end
      object Edit6: TEdit
        Left = 496
        Top = 561
        Width = 121
        Height = 21
        TabOrder = 22
        Text = '50'
        OnKeyPress = g_Nz_parKeyPress
      end
      object Button2: TButton
        Left = 239
        Top = 523
        Width = 75
        Height = 25
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        TabOrder = 23
        OnClick = Button2Click
      end
      object Edit1: TEdit
        Left = 752
        Top = 531
        Width = 121
        Height = 21
        TabOrder = 24
      end
      object Edit2: TEdit
        Left = 752
        Top = 558
        Width = 121
        Height = 21
        TabOrder = 25
      end
      object Edit4: TEdit
        Left = 924
        Top = 561
        Width = 121
        Height = 21
        TabOrder = 26
      end
      object Edit3: TEdit
        Left = 924
        Top = 534
        Width = 121
        Height = 21
        TabOrder = 27
      end
      object Button8: TButton
        Left = 88
        Top = 523
        Width = 97
        Height = 25
        Caption = #1054#1073#1088#1072#1090#1085#1099#1081' '#1088#1072#1089#1095#1077#1090
        Enabled = False
        TabOrder = 28
        OnClick = Button8Click
      end
      object Button9: TButton
        Left = 752
        Top = 600
        Width = 75
        Height = 25
        Caption = #1056#1072#1089#1095#1077#1090
        TabOrder = 29
        OnClick = Button9Click
      end
      object Edit7: TEdit
        Left = 752
        Top = 648
        Width = 121
        Height = 21
        TabOrder = 30
        Text = '10'
      end
    end
    object Графики: TTabSheet
      Caption = #1043#1088#1072#1092#1080#1082#1080
      ImageIndex = 1
      object Chart5: TChart
        Left = 3
        Top = 3
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'sxx')
        View3D = False
        TabOrder = 0
        ColorPaletteIndex = 13
        object Series5: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series6: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart6: TChart
        Left = 346
        Top = 3
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          's_eff')
        View3D = False
        TabOrder = 1
        ColorPaletteIndex = 13
        object LineSeries1: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object LineSeries2: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart7: TChart
        Left = 689
        Top = 3
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'Us')
        View3D = False
        TabOrder = 2
        ColorPaletteIndex = 13
        object LineSeries3: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object LineSeries4: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart8: TChart
        Left = 3
        Top = 259
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'sxy')
        View3D = False
        TabOrder = 3
        ColorPaletteIndex = 13
        object LineSeries5: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object LineSeries6: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart9: TChart
        Left = 346
        Top = 259
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'Rh_eff')
        View3D = False
        TabOrder = 4
        ColorPaletteIndex = 13
        object LineSeries7: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object LineSeries8: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Chart10: TChart
        Left = 689
        Top = 259
        Width = 337
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'Uy')
        View3D = False
        TabOrder = 5
        ColorPaletteIndex = 13
        object LineSeries9: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 10708548
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object LineSeries10: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Visible = False
          LinePen.Color = 3513587
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object sg1: TSaveDialog
    Left = 696
    Top = 632
  end
end
