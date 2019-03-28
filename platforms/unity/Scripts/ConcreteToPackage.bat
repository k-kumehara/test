@echo off
:: =========================================
:: 
:: Concrete assets to unitypackage
:: 
:: (C) 2018 Yamaha Corporation
::        Confidential
:: 
:: =========================================

set SCRIPT_PATH=%~dp0
set PRJECT_PATH=%SCRIPT_PATH%\..\
set SRC=Assets\SoundXR
set DST=%SCRIPT_PATH%\soundxr_sdk.unitypackage

@echo on
"C:\Program Files\Unity\Hub\Editor\2018.2.18f1\Editor\Unity.exe" ^
-quit -batchmode -nographics ^
-logfile ConcreteToPackage.log ^
-projectPath %PRJECT_PATH% ^
-exportPackage %SRC% %DST%

cd %PRJECT_PATH%
rd /s /q Library
rd /s /q ProjectSettings
rd /s /q Packages

:: EOF