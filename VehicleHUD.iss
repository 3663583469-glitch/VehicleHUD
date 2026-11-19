#define AppName "VehicleHUD"
#define AppVersion "0.2.0"
#define AppPublisher "VehicleHUD Demo"
#define AppExeName "VehicleHUD.exe"

[Setup]
AppId={{A6F4B10D-6D8C-4A2A-9B61-2A0F4D3C8E20}
AppName={#AppName}
AppVersion={#AppVersion}
AppPublisher={#AppPublisher}
DefaultDirName={autopf}\VehicleHUD
DefaultGroupName={#AppName}
UninstallDisplayIcon={app}\{#AppExeName}
OutputDir=installer-output
OutputBaseFilename=VehicleHUD-Setup-Windows11-x64
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=admin
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
DisableProgramGroupPage=yes

[Languages]
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "创建桌面快捷方式"; GroupDescription: "附加选项："; Flags: unchecked

[Files]
Source: "build\Release\VehicleHUD.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\Release\SDL2.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

[Icons]
Name: "{autoprograms}\{#AppName}"; Filename: "{app}\{#AppExeName}"
Name: "{autodesktop}\{#AppName}"; Filename: "{app}\{#AppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#AppExeName}"; Description: "启动 VehicleHUD"; Flags: postinstall nowait skipifsilent
