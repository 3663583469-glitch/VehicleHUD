@echo off
cd /d "%~dp0"
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build_installer_windows.ps1"
if errorlevel 1 (
  echo.
  echo Installer build failed. Read the message above.
  pause
  exit /b 1
)
echo.
echo Installer created successfully.
pause
