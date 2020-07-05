@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem Produit   NetworkHelper
rem Fichier   Export.cmd

echo Executing  Export.cmd %1 %s  ...

rem ===== Configuration =====================================================

set PRODUCT=NetworkHelper

rem ===== Initialisation ====================================================

if ""=="%2" (
    set DST="K:\Export\%PRODUCT%\%1_Windows"
) else (
    set DST="K:\Export\%PRODUCT%\%1_%2_Windows"
)

set EXPORT_CMD_TXT="Export.cmd.txt"

set KMS_COPY="Import\Binaries\Release_32\KmsCopy.exe"

rem ===== Verification ======================================================

if ""=="%1" (
    echo FATAL ERROR  Invalid command line
    echo Usage  .\Export.cmd {Ma.Mi.Bu} [Type]
    pause
    exit /B 10
)

if exist %DST% (
    echo FATAL ERROR  %DST% already existe
    pause
    exit /B 20   
)

if not exist %EXPORT_CMD_TXT% (
	echo FATAL ERROR  %EXPORT_CMD_TXT%  does not exist
	pause
	exit /B 30
)

if not exist %KMS_COPY% (
	echo FATAL ERROR  %KMS_COPY% does not exist
	echo Execute .\Import.cmd
	pause
	exit /B 40
)

rem ===== Execution =========================================================

%KMS_COPY% . %DST% %EXPORT_CMD_TXT%
if ERRORLEVEL 1 (
	echo  ERROR  %KMS_COPY% . %DST% %EXPORT_CMD_TXT%  failed - %ERRORLEVEL%
	pause
	exit /B 60
)

rem ===== End ===============================================================

echo OK
