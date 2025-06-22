# Space Invaders QT

Space Invaders QT is a simple shooter game implemented in C++ using the Qt framework. The project demonstrates sprite based rendering, basic physics and audio playback.

## Dependencies

- **Qt** (Widgets, OpenGL, Multimedia)
- **OpenAL** – audio playback
- **libsndfile** – loading sound assets
- **yaml-cpp** – parsing level data

All third party libraries are provided in the `Thirdparty/` directory.

## Building

Run the following commands from the project root:

```bash
qmake SpaceInvaders.pro
make
```

The built binary will be placed in the current directory.

To enable the optional performance benchmark mode, define `PERFORMANCE_BENCHMARK` before building. This can be done by uncommenting the macro in `Game/Core/GameRunnerView.h` (and `Game/Core/GameRunner.h`) or by passing `DEFINES+=PERFORMANCE_BENCHMARK` to `qmake`.

## Gameplay

Use the arrow keys to move your ship and press <kbd>Space</kbd> to fire the primary weapon. Hold <kbd>Shift</kbd> to trigger the equipped secondary weapon. Additional debug actions exist (such as toggling auto‑fire with <kbd>C</kbd>) but the basic gameplay is to destroy enemy waves while avoiding hits.

Have fun!

