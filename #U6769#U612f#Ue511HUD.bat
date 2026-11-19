@echo off
cd /d "%~dp0"
if not exist "build\Release\VehicleHUD.exe" (
  echo VehicleHUD.exe not found. Please run 一键编译.bat first.
  pause
  exit /b 1
)
start "" "%~dp0build\Release\VehicleHUD.exe"
