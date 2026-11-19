$ErrorActionPreference = 'Stop'

Write-Host "=======================================" -ForegroundColor Cyan
Write-Host " VehicleHUD - Windows 11 build" -ForegroundColor Cyan
Write-Host "=======================================" -ForegroundColor Cyan

foreach ($tool in @('cmake', 'git')) {
    if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) {
        throw "$tool was not found. Install Visual Studio 2022 (Desktop development with C++), CMake and Git, then retry."
    }
}

cmake -S . -B build -A x64
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed (exit code $LASTEXITCODE)." }

cmake --build build --config Release --parallel
if ($LASTEXITCODE -ne 0) { throw "Build failed (exit code $LASTEXITCODE)." }

$exe = Join-Path $PSScriptRoot 'build\Release\VehicleHUD.exe'
if (-not (Test-Path $exe)) { throw "Build finished but executable was not found at $exe" }

Write-Host "`nBuild successful: $exe" -ForegroundColor Green
Write-Host "Run VehicleHUD.exe to start the simulation HUD."
