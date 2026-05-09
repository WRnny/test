@echo off
set SYSCFG_PATH="D:\ti\sysconfig_1.23.1\sysconfig_cli.bat"
set SDK_ROOT="D:\ti\mspm0_sdk_2_04_00_06"
set PROJ_DIR="D:\ti\mspm0_sdk_2_04_00_06\循迹小车-模板\keil"

if not exist %SYSCFG_PATH% (
    echo Couldn't find Sysconfig Tool %SYSCFG_PATH%
    echo "Update the path to sysconfig_cli.bat"
    pause
    exit
)

echo Using Sysconfig Tool from %SYSCFG_PATH%
echo Running SysConfig for MSPM0G3507...

%SYSCFG_PATH% --device MSPM0G3507 -o %PROJ_DIR% -s %SDK_ROOT%\.metadata\product.json --compiler keil %PROJ_DIR%\empty.syscfg

echo SysConfig completed.
pause