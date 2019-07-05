program Project1;

uses
  Vcl.Forms,
  Unit1 in 'Unit1.pas' {SCAF},
  Vcl.Themes,
  Vcl.Styles;

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.Title := 'SCAF';
  TStyleManager.TrySetStyle('Metropolis UI Green');
  Application.CreateForm(TSCAF, SCAF);
  Application.Run;
end.
