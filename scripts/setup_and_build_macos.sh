#!/bin/bash
set -e
# Install dependencies using Homebrew
brew install qt openal-soft libsndfile yaml-cpp
# Ensure qmake is on PATH
export PATH="$(brew --prefix qt)/bin:$PATH"

# Build Space Invaders
qmake SpaceInvaders.pro
make -j$(sysctl -n hw.ncpu)
