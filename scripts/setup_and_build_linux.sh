#!/bin/bash
set -e
# Install dependencies for Ubuntu/Debian
sudo apt update
sudo apt install -y build-essential qtbase5-dev qt5-qmake libopenal-dev libsndfile1-dev libyaml-cpp-dev

# Build Space Invaders
qmake SpaceInvaders.pro
make -j$(nproc)
