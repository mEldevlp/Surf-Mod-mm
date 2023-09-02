@echo off

taskkill /IM hlds.exe /F

set "outputFile=..\Release\SurfMod_mm.dll"

set "copyPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX\cstrike\addons\SurfMod\dlls"

copy %outputFile% %copyPath%

cd C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX\

start /wait /high hlds.exe -console -game cstrike +port 27017 +map de_dust2 +maxplayers 16 +sys_ticrate 512