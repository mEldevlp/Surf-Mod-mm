@echo off

taskkill /IM hlds.exe /F

set "outputFile=..\Release\SurfMod_mm.dll"

set "copyPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX\cstrike\addons\SurfMod\dlls"

copy %outputFile% %copyPath%
