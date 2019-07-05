object SCAF: TSCAF
  Left = 0
  Top = 0
  Caption = 'SCAF'
  ClientHeight = 470
  ClientWidth = 680
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 680
    Height = 470
    ActivePage = Aluno
    Align = alClient
    TabOrder = 0
    object Aluno: TTabSheet
      Caption = 'Aluno'
      object DBNavigator1: TDBNavigator
        Left = 0
        Top = 417
        Width = 672
        Height = 25
        DataSource = DataSource1
        Align = alBottom
        TabOrder = 0
      end
      object DBGrid1: TDBGrid
        Left = 0
        Top = 0
        Width = 672
        Height = 417
        Align = alClient
        DataSource = DataSource1
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Expanded = False
            FieldName = 'tag_aluno'
            Title.Caption = 'TAG'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'nome'
            Title.Caption = 'Nome'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'matricula'
            Title.Caption = 'Matricula'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'cargo_aluno'
            Title.Caption = 'Matricula'
            Visible = True
          end>
      end
    end
    object Servidor: TTabSheet
      Caption = 'Servidor'
      ImageIndex = 1
      object DBNavigator2: TDBNavigator
        Left = 0
        Top = 417
        Width = 672
        Height = 25
        DataSource = DataSource2
        Align = alBottom
        TabOrder = 0
      end
      object DBGrid2: TDBGrid
        Left = 0
        Top = 0
        Width = 672
        Height = 417
        Align = alClient
        DataSource = DataSource2
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Expanded = False
            FieldName = 'tag_id'
            Title.Caption = 'TAG'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'Nome'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'matr'
            Title.Caption = 'Matricula'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'nome_facul'
            Title.Caption = 'Universidade'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'Cargo'
            Visible = True
          end>
      end
    end
    object Aula: TTabSheet
      Caption = 'Aula'
      ImageIndex = 2
      object DBNavigator3: TDBNavigator
        Left = 0
        Top = 417
        Width = 672
        Height = 25
        DataSource = DataSource3
        Align = alBottom
        TabOrder = 0
      end
      object DBGrid3: TDBGrid
        Left = 0
        Top = 0
        Width = 672
        Height = 417
        Align = alClient
        DataSource = DataSource3
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Expanded = False
            FieldName = 'id_materia'
            Title.Caption = 'ID'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'nome_materia'
            Title.Caption = 'Nome Materia'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'n_sala'
            Title.Caption = 'Sala'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'hora_inicio'
            Title.Caption = 'Hora Inicio'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'hora_fim'
            Title.Caption = 'Hora Fim'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'week_day'
            Title.Caption = 'Dia Semana'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'tag_professor'
            Title.Caption = 'TAG Professor'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'nome_facul'
            Title.Caption = 'Universidade'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'Cargo'
            Visible = True
          end>
      end
    end
    object ESP: TTabSheet
      Caption = 'ESP'
      ImageIndex = 3
      object DBNavigator4: TDBNavigator
        Left = 0
        Top = 417
        Width = 672
        Height = 25
        DataSource = DataSource4
        Align = alBottom
        TabOrder = 0
      end
      object DBGrid4: TDBGrid
        Left = 0
        Top = 0
        Width = 672
        Height = 417
        Align = alClient
        DataSource = DataSource4
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Expanded = False
            FieldName = 'id'
            Title.Caption = 'ID'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'n_sala'
            Title.Caption = 'Sala'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'nome_facul'
            Title.Caption = 'Universidade'
            Visible = True
          end>
      end
    end
    object Relatorio: TTabSheet
      Caption = 'Relat'#243'rio'
      ImageIndex = 4
      object DBNavigator5: TDBNavigator
        Left = 0
        Top = 392
        Width = 672
        Height = 25
        DataSource = DataSource5
        VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbDelete, nbRefresh]
        Align = alBottom
        TabOrder = 0
        ExplicitTop = 417
      end
      object DBGrid5: TDBGrid
        Left = 0
        Top = 0
        Width = 672
        Height = 392
        Align = alClient
        DataSource = DataSource5
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'Tahoma'
        TitleFont.Style = []
        Columns = <
          item
            Expanded = False
            FieldName = 'tag_id'
            Title.Caption = 'TAG'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'nome'
            Title.Caption = 'Nome'
            Width = 120
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'matricula'
            Title.Caption = 'Matricula'
            Width = 90
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'esp_id'
            Title.Caption = 'ESP ID'
            Width = 100
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'date'
            Title.Caption = 'Data'
            Width = 150
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'status'
            Title.Caption = 'Status'
            Width = 60
            Visible = True
          end>
      end
      object Button1: TButton
        Left = 0
        Top = 417
        Width = 672
        Height = 25
        Align = alBottom
        Caption = 'Export'
        TabOrder = 2
        OnClick = Button1Click
        ExplicitLeft = -3
        ExplicitTop = 424
      end
    end
  end
  object FDConnection1: TFDConnection
    Params.Strings = (
      'Database=C:\Servidor\database.db'
      'LockingMode=Normal'
      'DriverID=SQLite')
    LoginPrompt = False
    Left = 616
    Top = 64
  end
  object FDGUIxWaitCursor1: TFDGUIxWaitCursor
    Provider = 'Forms'
    Left = 616
    Top = 184
  end
  object FDPhysSQLiteDriverLink1: TFDPhysSQLiteDriverLink
    Left = 616
    Top = 120
  end
  object FDTable1: TFDTable
    Connection = FDConnection1
    UpdateOptions.UpdateTableName = 'Aluno'
    TableName = 'Aluno'
    Left = 504
    Top = 72
  end
  object FDTable2: TFDTable
    IndexFieldNames = 'tag_id'
    Connection = FDConnection1
    UpdateOptions.UpdateTableName = 'Servidor'
    TableName = 'Servidor'
    Left = 504
    Top = 136
  end
  object FDTable3: TFDTable
    IndexFieldNames = 'id_materia'
    Connection = FDConnection1
    UpdateOptions.UpdateTableName = 'Aula'
    TableName = 'Aula'
    Left = 504
    Top = 192
  end
  object FDTable4: TFDTable
    IndexFieldNames = 'id'
    Connection = FDConnection1
    UpdateOptions.UpdateTableName = 'esp'
    TableName = 'esp'
    Left = 504
    Top = 256
  end
  object DataSource1: TDataSource
    DataSet = FDTable1
    Left = 452
    Top = 80
  end
  object DataSource2: TDataSource
    DataSet = FDTable2
    Left = 452
    Top = 136
  end
  object DataSource3: TDataSource
    DataSet = FDTable3
    Left = 452
    Top = 200
  end
  object DataSource4: TDataSource
    DataSet = FDTable4
    Left = 444
    Top = 264
  end
  object DataSource5: TDataSource
    DataSet = FDQuery1
    Left = 444
    Top = 336
  end
  object FDQuery1: TFDQuery
    Connection = FDConnection1
    SQL.Strings = (
      'select '
      '  a.tag_id'
      ' , b.nome'
      ' , b.matricula'
      ' , a.esp_id'
      
        ' , strftime('#39'%d-%m-%Y | %H:%M:%S'#39', datetime(a.timestamp, '#39'unixep' +
        'och'#39')) as date'
      ' , a.status'
      'from relatorio a '
      '  inner join Aluno b ON(a.tag_id = b.tag_aluno)'
      'union'
      ''
      'select '
      '  a.tag_id'
      ' , b.nome'
      ' , b.matr'
      ' , a.esp_id'
      
        ' , strftime('#39'%d-%m-%Y | %H:%M:%S'#39', datetime(a.timestamp, '#39'unixep' +
        'och'#39')) as date'
      ' , a.status'
      ''
      ''
      'from relatorio a '
      '  inner join Servidor b ON(a.tag_id = b.tag_id)'
      ''
      'order by date')
    Left = 612
    Top = 240
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'csv'
    Filter = 'CSV|*.csv'
    Left = 612
    Top = 296
  end
end
