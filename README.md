# Space Invaders QT

Space Invaders QT is a simple shooter game implemented in C++ using the Qt framework. The project demonstrates sprite based rendering, basic physics and audio playback.

## Dependencies

- **Qt** (Widgets, OpenGL, Multimedia)
- **OpenAL** – audio playback
- **libsndfile** – loading sound assets
- **yaml-cpp** – parsing level data

All third party libraries are provided in the `Thirdparty/` directory.


## Installing Dependencies

### Linux (Ubuntu/Debian)
Install the required packages:
```bash
sudo apt update
sudo apt install build-essential qtbase5-dev qt5-qmake libopenal-dev libsndfile1-dev libyaml-cpp-dev
```

### macOS
Install via Homebrew:
```bash
brew install qt openal-soft libsndfile yaml-cpp
```
Ensure `qmake` is on your `PATH`, e.g. `export PATH="$(brew --prefix qt)/bin:$PATH"`.

### Windows

1. **Install Qt Framework**
   - Download and run the [Qt Online Installer](https://www.qt.io/download).
   - During installation, make sure to select the **Desktop Qt components**, which include `qmake`.

2. **Use Pre-Built Libraries**
   - This project uses pre-built third-party libraries located in the `Thirdparty/` directory.

3. **Install OpenAL Runtime**
   - Navigate to `Thirdparty/OpenAL/redist/`
   - Run `oalinst.exe` to install the OpenAL runtime required for sound playback.

4. **Install libsndfile (for sound file decoding)**
   - **Option A (Recommended):**  
     Download and install the official Windows installer from the [libsndfile GitHub Releases](https://github.com/libsndfile/libsndfile/releases).  
     This will register `libsndfile-1.dll` in your system so the game can find it automatically.
   - **Option B (Manual):**  
     Extract `libsndfile-1.dll` from a release package and **copy it to the same directory as your executable** (`.exe`) after build.

5. **Build `yaml-cpp` (YAML parser)**
   - Open a terminal and run the following commands:
     ```bash
     cd Thirdparty/yaml-cpp
     mkdir build
     cd build
     cmake ..
     cmake --build . --config Release
     ```
   - After build, make sure the resulting `.lib` and `.dll` files are available to your linker/runtime.


## Building

Run the following commands from the project root:

```bash
qmake SpaceInvaders.pro
make
```

The built binary will be placed in the current directory.

To enable the optional performance benchmark mode, build using the
`performance_benchmark` configuration:

```bash
qmake CONFIG+=performance_benchmark SpaceInvaders.pro
make
```

## Gameplay

Use the arrow keys to move your ship and press <kbd>Space</kbd> to fire the primary weapon. Hold <kbd>Shift</kbd> to trigger the equipped secondary weapon. Additional debug actions exist (such as toggling auto‑fire with <kbd>C</kbd>) but the basic gameplay is to destroy enemy waves while avoiding hits.

Have fun!

