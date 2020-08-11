@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem Product   NetworkHelper
rem File      Build.cmd
rem Usage     .\Build.cmd

rem CODE REVIEW 2020-06-12 KMS - Martin Dubois, P.Eng.

echo Executing  Build.cmd  ...

rem ===== Initialisation ====================================================

set CERT_SHA=D5B26E6B4E3159472C2C98307C45D40B129F2078

set EXPORT_CMD="Export.cmd"

set INNO_COMPIL32="C:\Program Files (x86)\Inno Setup 5\Compil32.exe"

set KMS_VERSION="Import\Binaries\Release_32\KmsVersion.exe"

set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\MSBuild\15.0\Bin\MSBuild.exe"

set PRODUCT=NetworkHelper

set SIGNTOOL_EXE="C:\Program Files (x86)\Windows Kits\10\Tools\bin\i386\signtool.exe"

set SOLUTION="%PRODUCT%.sln"

set TEST_CMD="Test.cmd"

rem ===== Verification ======================================================

if not exist %INNO_COMPIL32% (
	echo FATAL ERROR  %INNO_COMPIL32%  does not exist
	echo Install Inno 5.6.1
	pause
	exit /B 10
)

if not exist %KMS_VERSION% (
    echo FATAL ERROR  %KMS_VERSION%  does not exist
	echo Install KmsTools version 2.3.3 or higher
	pause
	exit /B 20
)

if not exist %MSBUILD% (
	echo FATAL ERROR  %MSBUILD%  does not exist
	echo Install Visual Studio 2017
	pause
	exit /B 30
)

if not exist %SIGNTOOL_EXE% (
	echo FATAL ERROR  %SIGNTOOL_EXE%  does not exist
	echo Install the WDK
	pause
	exit /B 35
)

rem ====== Execution ========================================================

set OPTIONS=%SOLUTIONS% /target:rebuild

%MSBUILD% %OPTIONS% /property:Configuration=Debug /property:Platform=x86
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Debug /property:Platform=x86  failed - %ERRORLEVEL%
	pause
	exit /B 40
)

%MSBUILD% %OPTIONS% /property:Configuration=Release /property:Platform=x86
if ERRORLEVEL 1 (
	echo ERROR  %MSBUILD% %OPTIONS% /property:Configuration=Release /property:Platform=x86  failed - %ERRORLEVEL%
	pause
	exit /B 50
)

call %TEST_CMD%
if ERRORLEVEL 1 (
    echo ERROR  %TEST_CMD% reported an error - %ERRORLEVEL%
	pause
	exit /B 60
)

%KMS_VERSION% Common\Version.h Export.cmd.txt %PRODUCT%.iss
if ERRORLEVEL 1 (
	echo ERROR  %KMS_VERSION% Common\Version.h Export.cmd.txt %PRODUCT%.iss  failed - %ERRORLEVEL%
	pause
	exit /B 70
)

%INNO_COMPIL32% /cc %PRODUCT%.iss
if ERRORLEVEL 1 (
	echo ERROR  %INNO_COMPIL32% /cc %PRODUCT%.iss  failed - %ERRORLEVEL%
	pause
	exit /B 80
)

%SIGNTOOL_EXE% sign /fd sha256 /sha1 %CERT_SHA% /td sha256 /tr http://timestamp.digicert.com Installer/NetworkHelper_*.exe
if ERRORLEVEL 1 (
	echo ERROR  %SIGNTOOL_EXE% sign /fd sha256 /sha1 %CERT_SHA% /td sha256 /tr http://timestamp.digicert.com Installer/NetworkHelper_*.exe  failed - %ERRORLEVEL%
	pause
	exit /B 85
)

%KMS_VERSION% -S Common\Version.h Export.cmd
if ERRORLEVEL 1 (
    echo ERROR  %KMS_VERSION% -S Common\Version.h Export.cmd  failed - %ERRORLEVEL%
	pause
	exit /B 90
)

rem ===== End ===============================================================

echo  OK
