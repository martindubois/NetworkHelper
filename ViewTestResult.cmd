
rem Author    KMS - Martin Dubois, P.Eng.
rem Copyright (C) 2020 KMS. All rights reserved.
rem Product   NetworkHelper
rem File      ViewTestResult.cmd
rem Usage     .\ViewTestResult.cmd

echo Executing  ViewTestResult.cmd  ...

rem ===== Initialisation ====================================================

set CHROME_EXE="C:\Program Files (x86)\Google\Chrome\Application\chrome.exe"

rem ===== Verification ======================================================

if not exist %CHROME_EXE% (
	echo FATAL ERROR  %CHROME_EXE%  does not exist
	echo Install Chrome
	pause
	exit /B 10
)

rem ===== Execution =========================================================

for %%f in (*.html) do %CHROME_EXE% --disable-background-mode --disable-plugins --start-maximized %CD%\%%f

rem ===== End ===============================================================

echo OK
