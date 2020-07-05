@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem Product   NetworkHelper
rem File      Import.cmd
rem Usage     .\Import.cmd

echo Executing  Import.cmd  ...

rem ===== Configuration =====================================================

set EXPORT_FOLDER=K:\Export

rem ===== Dependencies ======================================================

set KMS_BASE_VERSION=3.0.20
set KMS_BASE="%EXPORT_FOLDER%\KmsBase\%KMS_BASE_VERSION%_Windows"

set HTML_INTERFACE_VERSION=1.0.0
set HTML_INTERFACE="%EXPORT_FOLDER%\HTML_Interface\%HTML_INTERFACE_VERSION%_Windows"

if not exist %EXPORT_FOLDER% (
    set KMS_BASE="C:\Program Files\KmsBase"
    set HTML_INTERFACE="C:\Program Files\HTML_Interface"
)

rem ===== Constants =========================================================

set DST_FOLDER="%CD%\Import"

rem ===== Verification ======================================================

if not exist %KMS_BASE% (
    echo FATAL ERROR  %KMS_BASE%  does not exist
    echo Install KmsBase %KMS_BASE_VERSION%
    pause
    exit /B 10
)

if not exist %HTML_INTERFACE% (
    echo FATAL ERROR  %HTML_INTERFACE%  does not exist
    echo Install HTML_Interface %HTML_INTERFACE_VERSION%
    pause
    exit /B 20
)

rem ===== Execution =========================================================

if not exist %DST_FOLDER%  mkdir %DST_FOLDER%

pushd  %KMS_BASE%
    call Import.cmd %DST_FOLDER%
popd

if ERRORLEVEL 1 (
    echo ERROR  call Import.cmd %DST_FOLDER%  failed - %ERRORLEVEL%
    pause
    exit /B 30
)

pushd %HTML_INTERFACE%
    call Import.cmd %DST_FOLDER%
popd

if ERRORLEVEL 1 (
    echo ERROR  call Import.cmd %DST_FOLDER%  failed - %ERRORLEVEL%
    pause
    exit /B 40
)

rem ===== End ===============================================================

echo OK
