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
1. Download and run the [Qt Online Installer](https://www.qt.io/download).
2. Install the desktop Qt components which include `qmake`.
3. Use the pre-built OpenAL and libsndfile libraries in `Thirdparty/`.
   Build `yaml-cpp` from `Thirdparty/yaml-cpp` with CMake if needed.

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

