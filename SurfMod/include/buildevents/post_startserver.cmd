@echo off

cd C:\Users\mEl\Desktop\Surf-Mod-mm\Release

set "outputFile=SurfMod_mm.dll"

set "copyPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX\cstrike\addons\SurfMod\dlls"

if exist %copyPath% (copy %outputFile% %copyPath%) else (echo "something wrong")

set "hldsParam=-console -game cstrike -secure -pingboost 3 -master +port 27017 +map de_dust2 +maxplayers 16 +sys_ticrate 512"

set "hldsPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX"

if exist %hldsPath% (start /min /d "%hldsPath%" hlds.exe %hldsParam%)