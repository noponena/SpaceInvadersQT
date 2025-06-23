# Space Invaders QT

Space Invaders QT is a simple 2D shooter game implemented in C++ using the Qt framework. The project demonstrates sprite-based rendering, basic physics, and audio playback.

## Prerequisites

- **C++ Compiler** supporting C++17 or newer (e.g. MinGW, MSVC, GCC, Clang)
- **CMake** (for building on all platforms)
- **Git** (to clone the repo, optional)
- **Windows, Linux (Ubuntu/Debian), or macOS**

## Dependencies

- **Qt** (widgets and graphics)
- **OpenAL** – audio playback
- **libsndfile** – loading sound assets
- **yaml-cpp** – parsing level data
- **pkg-config** – required for finding system packages (sndfile, etc.) on Linux/macOS

On Windows, all required third-party libraries are included in the `Thirdparty/` directory (for MinGW). On Linux and macOS, dependencies can be installed via your system package manager.

---

## Installing Dependencies

### Windows

#### 1. Install Qt Framework (with MinGW Toolchain)

1. Download and run the [Qt Online Installer (Open Source)](https://www.qt.io/download-qt-installer).
2. Log in to your Qt account (or create one if needed).
3. Select **Developer and Designer Tools → Qt for desktop development**.
    - For a minimal install, choose **Custom Installation** and select:
        - **Qt > 6.9.1** (or newer) → `MinGW 13.1.0 64-bit`
        - **Build Tools** → `MinGW 13.1.0 64-bit`
    - *You do not need Qt Creator, documentation, or examples unless you want them.*

#### 2. Install OpenAL Runtime

- Go to `Thirdparty/OpenAL/redist/`.
- Double-click `oalinst.exe` and follow the prompts.

#### 3. Set Up Environment Variables

1. Open the **Start Menu** → search for **Environment Variables** → **Edit the system environment variables**.
2. In the dialog, click **Environment Variables…**.
3. Under **User variables** or **System variables**, find `Path` and click **Edit**.
4. Add the following (adjust for your installed versions):

    ```
    C:\Qt\<qt_version>\mingw_64\bin
    C:\Qt\Tools\mingw<mingw_version>_64\bin
    C:\Qt\Tools\CMake_64\bin
    ```

    *Example:* For Qt 6.9.1 and MinGW 13.1.0, use `<qt_version>=6.9.1`, `<mingw_version>=1310`.

5. Click **OK** on all dialogs.
6. Open a new Command Prompt or PowerShell and run:

    ```cmd
    cmake --version
    mingw32-make --version
    ```

    *Each command should display a version number.*

### Linux (Ubuntu/Debian)

Install all dependencies via apt:
```bash
sudo apt update
sudo apt install build-essential qt6-base-dev libopenal-dev libsndfile1-dev libyaml-cpp-dev pkg-config
```

### macOS
Install dependencies using Homebrew:
```bash
brew install qt openal-soft libsndfile yaml-cpp pkg-config
```

## Building

1. Run the build script for your OS (from the ```scripts``` folder):
    - **Windows**: ```setup_and_build_windows.bat```
    - **Linux**: ```setup_and_build_linux.sh```
    - **macOS**: ```setup_and_build_macos.sh```
2. Check the build log to verify a successful build.

**Note**: On Linux/macOS make sure the scripts are executable:

- Linux:
    ```bash
    chmod +x scripts/setup_and_build_linux.sh
    ```
- macOS:
    ```bash
    chmod +x scripts/setup_and_build_macos.sh
    ```

## Running the game

1. Go to the `script_build_result\` folder (created by the build script).
2. Launch the game:
    - **Windows**: Double-click `SpaceInvadersQT.exe`
    - **Linux/macOS**: run 
        ```cmd
        chmod +x ./SpaceInvaders
        ./SpaceInvaders
         ```
3. Select level using the **Level Selector**

**Note**: The ```levels/``` folder must be present in the project root; the build script copies it to the output folder automatically.

## Gameplay

- Move your ship with the arrow keys.
- Fire your primary weapon with <kbd>Space</kbd>.
- Fire equipped secondary weapons with keys <kbd>1</kbd> – <kbd>4</kbd>.
- Destroy all enemies before they reach the bottom of the screen and avoid their projectiles.
- Collect in-game currency and health dropped by enemies (currency has no use yet; future updates will add an in-game shop).
- Have fun! (optional)

## Notes
- **Build system migrated from qmake to CMake** as of June 2025.
- The old .pro and .pri files have been removed; only CMake is supported for building.
- Resource files are handled automatically by CMake (no need for manual .qrc or .ui compilation).
