@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem Produit   NetworkHelper
rem Fichier	  Test.cmd
rem Usage     .\Test.cmd

echo Executing  Test.cmd  ...

rem ===== Configuration ====================================================

set PRODUCT=NetworkHelper

rem ===== Initialisation ===================================================

set NETWORK_HELPER_EXE=%PRODUCT%.exe

set NH_LIB_TEST_EXE=NHLib_Test.exe

set DEBUG_32=Debug

set BASE_TXT="NetworkHelper\Tests\Base.txt"

set RELEASE_32=Release

rem ===== Validation =======================================================

if not exist "%DEBUG_32%" (
    echo FATAL ERROR  %DEBUG_32%  does not exist
	pause
	exit /B 10
)

if not exist "%RELEASE_32%" (
    echo FATAL ERROR  %RELEASE_32% does not exist
	pause
	exit /B 20
)

if not exist "%DEBUG_32%\%NH_LIB_TEST_EXE%" (
    echo  FATAL ERROR  %DEBUG_32%\%NH_LIB_TEST_EXE%  does not exist
	pause
	exit /B 30
)

if not exist "%RELEASE_32%\%NH_LIB_TEST_EXE%" (
    echo  FATAL ERROR  %RELEASE_32%\%NH_LIB_TEST_EXE%  does not exist
	pause
	exit /B 40
)

if not exist "%DEBUG_32%\%NETWORK_HELPER_EXE%" (
    echo  FATAL ERROR  %DEBUG_32%\%NETWORK_HELPER_EXE%  does not exist
	pause
	exit /B 50
)

if not exist "%RELEASE_32%\%NETWORK_HELPER_EXE%" (
    echo  FATAL ERROR  %RELEASE_32%\%NETWORK_HELPER_EXE%  does not exist
	pause
	exit /B 60
)
rem ===== Execution =========================================================

echo Testing  %DEBUG_32%\%NH_LIB_TEST_EXE%  ...

%DEBUG_32%\%NH_LIB_TEST_EXE%
if ERRORLEVEL 1 (
    echo ERROR  %DEBUG_32%\%NH_LIB_TEST_EXE%  reported an error - %ERRORLEVEL%
	pause
	exit /B 70
)

echo Testing  %RELEASE_32%\%NH_LIB_TEST_EXE%  ...

%RELEASE_32%\%NH_LIB_TEST_EXE%
if ERRORLEVEL 1 (
    echo ERROR  %RELEASE_32%\%NH_LIB_TEST_EXE%  reported an error - %ERRORLEVEL%
	pause
	exit /B 80
)

echo Testing  %DEBUG_32%\%NETWORK_HELPER_EXE%  ...

%DEBUG_32%\%NETWORK_HELPER_EXE% "Execute=ExecuteScript %BASE_TXT%"
if ERRORLEVEL 1 (
    echo ERROR  %DEBUG_32%\%NETWORK_HELPER_EXE% "Execute=ExecuteScript %BASE_TXT%"  reported an error - %ERRORLEVEL%
	pause
	exit /B 90
)

echo Testing  %RELEASE_32%\%CISCO_HELPER_EXE%  ...

%RELEASE_32%\%NETWORK_HELPER_EXE% "Execute=ExecuteScript %BASE_TXT%"
if ERRORLEVEL 1 (
    echo ERROR  %RELEASE_32%\%NETWORK_HELPER_EXE% "Execute=ExecuteScript %BASE_TXT%"  reported an error - %ERRORLEVEL%
	pause
	exit /B 100
)

rem ===== End ===============================================================

echo OK
