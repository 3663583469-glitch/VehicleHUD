# VehicleHUD — Windows 11 simulation prototype

A standalone desktop HUD prototype written in C++17 with SDL2, OpenGL and Dear ImGui. It displays locally generated demo vehicles, distance, bearing and vehicle heading.

> **Simulation only.** This program does not attach to War Thunder, inspect game memory, reveal hidden player positions, or bypass anti-cheat. Displayed coordinates and distances are generated demo values, not live game information.

## Create a Windows portable executable (.exe)

The included GitHub Actions workflow builds the Windows x64 executable on a Windows runner. Push this folder to a GitHub repository, open **Actions**, run **Build portable Windows x64 HUD**, then download the `VehicleHUD-Windows-x64-portable` artifact. It contains `VehicleHUD.exe` and any required SDL runtime DLL.

## Create a Windows installer (.exe)

1. On Windows 11, install **Visual Studio 2022** and select **Desktop development with C++**.
2. Install **CMake**, **Git**, and **Inno Setup 6**.
3. Extract this ZIP to a normal folder, for example `C:\Projects\VehicleHUD`.
4. Double-click **一键生成安装包.bat**.
5. The script downloads SDL2 and Dear ImGui on the first build, compiles the x64 Release app, then invokes Inno Setup.
6. The installer is created at `installer-output\VehicleHUD-Setup-Windows11-x64.exe`.

The installer provides an installation wizard, Start Menu shortcut, optional desktop shortcut, and normal Windows uninstall entry. It can offer to launch the HUD after installation.

## Build the app only

If you only want the executable, install Visual Studio 2022 C++ tools, CMake and Git, then double-click `一键编译.bat`. The executable is written to `build\Release\VehicleHUD.exe`.

## Controls and display

- Drag the title bar to move the window.
- Use **Keep window on top** to toggle always-on-top behavior.
- Press **Esc** to exit.

## Data fields

`src/Vehicle.h` defines each vehicle's name, map coordinates, heading, calculated distance and bearing. `src/Simulator.cpp` generates demo vehicles and calculates distance and bearing relative to a simulated observer at `(0, 0)`. Distances are demo coordinate units displayed as metres.

## Troubleshooting

- **`cmake` or `git` not found:** install them and reopen the terminal.
- **No C++ compiler:** install Visual Studio 2022's Desktop development with C++ workload.
- **Inno Setup not found:** install Inno Setup 6 and rerun `一键生成安装包.bat`.
- **Dependency download fails:** check network access to GitHub, then rerun the batch file.
- **OpenGL initialization fails:** update the graphics driver.
