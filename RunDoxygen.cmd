@echo off

rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem Product   NetworkHelper
rem File      RunDoxygen.cmd

echo Executing  RunDoxygen.cmd  ...

rem ===== Initialisation ====================================================

set DOXYFILE_EN="DoxyFile_en.txt"

set DOXYGEN="C:\Program Files\doxygen\bin\doxygen.exe"

rem ===== Validation ========================================================

if not exist %DOXYFILE_EN% (
    echo FATAL ERROR  %DOXYFILE_EN%  does not exist
	pause
	exit /B 10
)

if not exist %DOXYGEN% (
    echo FATAL ERROR  %DOXYGEN% does not exist
	echo Install doxygen
	pause
	exit /B 20
)

rem ===== Execution =========================================================

%DOXYGEN% %DOXYFILE_EN%
if ERRORLEVEL 1 (
    echo ERROR  %DOXYGEN% %DOXYFILE_EN%  reported an error - %ERRORLEVEL%
	pause
	exit /B 30
)

rem ===== End ===============================================================

echo  OK!
