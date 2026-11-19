$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot

Write-Host "=======================================" -ForegroundColor Cyan
Write-Host " VehicleHUD - Windows 11 installer" -ForegroundColor Cyan
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
if (-not (Test-Path $exe)) { throw "Executable was not found at $exe" }

# If SDL2 was built as a shared library, place its runtime DLL beside the app.
$sdlDll = Join-Path $PSScriptRoot 'build\Release\SDL2.dll'
if (-not (Test-Path $sdlDll)) {
    $foundSdl = Get-ChildItem (Join-Path $PSScriptRoot 'build') -Filter 'SDL2.dll' -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($foundSdl) { Copy-Item $foundSdl.FullName $sdlDll -Force }
}

# Locate the Inno Setup compiler.
$iscc = (Get-Command 'ISCC.exe' -ErrorAction SilentlyContinue).Source
if (-not $iscc) {
    $candidates = @(
        "$env:ProgramFiles\Inno Setup 6\ISCC.exe",
        "${env:ProgramFiles(x86)}\Inno Setup 6\ISCC.exe"
    )
    $iscc = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
}
if (-not $iscc) {
    throw "VehicleHUD.exe compiled successfully, but Inno Setup 6 was not found. Install Inno Setup 6, then run this script again to create the setup EXE."
}

& $iscc (Join-Path $PSScriptRoot 'VehicleHUD.iss')
if ($LASTEXITCODE -ne 0) { throw "Inno Setup failed (exit code $LASTEXITCODE)." }

$installer = Join-Path $PSScriptRoot 'installer-output\VehicleHUD-Setup-Windows11-x64.exe'
if (-not (Test-Path $installer)) { throw "Installer output was not found at $installer" }
Write-Host "`nInstaller created: $installer" -ForegroundColor Green
Start-Process explorer.exe -ArgumentList (Join-Path $PSScriptRoot 'installer-output')
