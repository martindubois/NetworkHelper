
; Author    KMS - Martin Dubois, P.Eng.
; Copyright (C) 2020 KMS. All rights reserved.
; Product   NetworkHelper
; File      NetworkHelper.iss

[Setup]
AllowNetworkDrive=no
AllowUNCPath=no
AppCopyright=Copyright (C) 2020 KMS.
AppName=NetworkHelper
AppPublisher=KMS
AppPublisherURL=http://www.kms-quebec.com
AppSupportURL=http://www.kms-quebec.com
AppVersion=1.0.0
ArchitecturesAllowed=x64 x86
ArchitecturesInstallIn64BitMode=x64
DefaultDirName={pf}\NetworkHelper
LicenseFile=License.txt
MinVersion=10.0
OutputBaseFilename=NetworkHelper_1.0.0
OutputDir=Installer

[Files]
Source: "_DocUser\NetworkHelper.ReadMe.txt"                            ; DestDir: "{app}"; Flags: isreadme
Source: "NetworkHelper\_DocUser\NetworkHelper.NetworkHelper.ReadMe.txt"; DestDir: "{app}"
Source: "Import\KmsBase.KmsLib.ReadMe.txt"                             ; DestDir: "{app}"
Source: "Import\KmsBase.ReadMe.txt"                                    ; DestDir: "{app}"
Source: "Release\NetworkHelper.exe"                                    ; DestDir: "{app}"

[Registry]
Root: HKCR; SubKey: "Directory\shell\network_helper"        ; Flags: uninsdeletekey
Root: HKCR; SubKey: "Directory\shell\network_helper"        ; ValueType: string; ValueData: "NetworkHelper Here"
Root: HKCR; SubKey: "Directory\shell\network_helper\command";
Root: HKCR; SubKey: "Directory\shell\network_helper\command"; ValueType: string; ValueData: """{app}\NetworkHelper.exe"" ""Execute=ChangeDir %1""";
