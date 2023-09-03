@echo off

set "outputFile=..\Release\SurfMod_mm.dll"

set "copyPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX\cstrike\addons\SurfMod\dlls"

copy %outputFile% %copyPath%

cd C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX

start server_start.bat & exit