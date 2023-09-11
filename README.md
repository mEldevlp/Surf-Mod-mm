<div align="center">

<h3>Surf Mod</h3>
<p>A metamod plugin for CS 1.6 &nbsp;|&nbsp; ReGameDLL & ReHLDS</p> <br>
  
[![Windows CI][build-windows-badge]][build-link]&nbsp;
[![Linux CI][build-linux-badge]][build-link]&nbsp;
[![License][license-badge]][license-link]&nbsp;
![Visitor Badge][visitor-badge]&nbsp;
</div>

## Overview
This plugin in development but in plans to add the next features:

<details>
  <summary>Remove Surf Stack</summary>
<br>
- Remove STUCK on the ramp (when you surf with high fps you are stucked)
</details>

<details>
  <summary>Duel system in the Balkan rules</summary>
<br>
Previously, any duels on the surf were without any plugins

This plugin will have a judge will can do start the duel, stop and give the round to duelists as well

- The system of returning to the round (returns the score of the game in need round)

- The system of waiting for the player (in case the Internet turned off or for anything)

- The "resign" system, the player can surrender at any time
</details>

<details>
  <summary>Box zones</summary>
<br>
  
I will try to do about the same [click](https://forums.alliedmods.net/showthread.php?t=176678). Author R3X

</details>

<details>
  <summary>Surf Speedrun Timer</summary>
<br>
	
Speedrun Timer. Instead of a big [kz-arg](https://forums.alliedmods.net/showthread.php?t=81692) plugin, there will be a simple timer with saving the results and outputting it to MOTD<br><br>
  
Will works with box-zones
</details>

## Want to help?
Just clone this repo

```sh
git clone https://github.com/mEldevlp/Surf-Mod-mm.git
```

In Visual Studio 2022 i have pre and post building events. Change these for yourself

Path: **".\SurfMod\include\buildevents\"**

<details>
  <summary>Event <b>pre_shutdown_server</b> is shutdown server (hlds.exe process kill)</summary>
  
```bat
tasklist /fi "imagename eq hlds.exe" |find ":" > nul
	if errorlevel 1 (taskkill /f /im "hlds.exe")
```
</details>

<details>
  <summary>Event <b>post_startserver</b> you need change path everthere because ur path 100% diff</summary>
  
```bat
cd C:\Users\mEl\Desktop\Surf-Mod-mm\Release

set "outputFile=SurfMod_mm.dll"

set "copyPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX\cstrike\addons\SurfMod\dlls"

if exist %copyPath% (copy %outputFile% %copyPath%) else (echo "something wrong")

set "hldsParam=-console -game cstrike -secure -pingboost 3 -master +port 27017 +map de_dust2 +maxplayers 16 +sys_ticrate 512"

set "hldsPath=C:\Users\mEl\Desktop\CS_SERVER_1_9_AMX"

if exist %hldsPath% (start /min /d "%hldsPath%" hlds.exe %hldsParam%)
```
</details>


Also you can delete events.

<details>
  <summary>In <b>SurfMod.vcxproj</b> file tags <<em>PostBuildEvent</em>> and <<em>PreBuildEvent</em>> </summary>

```xml
    <PostBuildEvent>
      <Command>./include/buildevents/post_startserver.cmd</Command>
    </PostBuildEvent>
    <PreBuildEvent>
      <Command>./include/buildevents/pre_shutdown_server.cmd</Command>
    </PreBuildEvent>
```
</details>

Or use path `Project -> Properties -> Build Events`


## Todo list
- [x] Remove Surf stuck
- [ ] Surf Duel System (in progress)
- [ ] Box zones (anti camp, start zone instead of a button, etc) (didn't start)
- [ ] Speedrun Timer for sr maps (special maplist) (didn't start)

[build-windows-badge]:        https://img.shields.io/github/actions/workflow/status/mEldevlp/Surf-Mod-mm/msbuild.yml?branch=master&style=for-the-badge&label=Windows%20CI&logo=windows
[build-linux-badge]:          https://img.shields.io/github/actions/workflow/status/mEldevlp/Surf-Mod-mm/makefile.yml?branch=master&style=for-the-badge&label=Linux%20CI&logo=linux&logoColor=white
[license-badge]:              https://img.shields.io/github/license/mEldevlp/Surf-Mod-mm?style=for-the-badge&label=license&color=success
[visitor-badge]:              https://visitor-badge.feriirawann.repl.co?username=mElDevlp&repo=Surf-Mod-mm&logo=Opsgenie&style=for-the-badge&label=Visitors

[license-link]:               https://github.com/mEldevlp/Surf-Mod-mm/blob/master/LICENSE
[build-link]:                 https://github.com/mEldevlp/Surf-Mod-mm/actions
