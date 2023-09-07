@echo off

tasklist /fi "imagename eq hlds.exe" |find ":" > nul
	if errorlevel 1 (taskkill /f /im "hlds.exe")

ping 127.0.0.1 -n 2 > nul

set "outputFile=..\Release\SurfMod_mm.dll"

set "copyPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX\cstrike\addons\SurfMod\dlls"

if exist %copyPath% (copy %outputFile% %copyPath%)

set "hldsParam=-console -game cstrike -secure -pingboost 3 -master +port 27017 +map de_dust2 +maxplayers 16 +sys_ticrate 512"

set "hldsPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX"

if exist %hldsPath% (start /min /d "%hldsPath%" hlds.exe %hldsParam%)