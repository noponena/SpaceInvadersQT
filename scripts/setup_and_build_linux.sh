#!/bin/bash

# === Help Message ===
if [[ "$1" == "-h" || "$1" == "--help" || "$1" == "/?" ]]; then
    echo
    echo "Usage: $0 [--clean] [core_count]"
    echo
    echo "  --clean       Optional: Clean all build directories before building."
    echo "  core_count    Optional: Number of CPU cores to use for parallel builds."
    echo "                If omitted, core count is auto-detected."
    echo
    echo "Examples:"
    echo "  $0"
    echo "  $0 8"
    echo "  $0 --clean"
    echo "  $0 --clean 12"
    exit 0
fi

# === Move to project root if in scripts directory ===
cd "$(dirname "$0")/.."

# === Argument Parsing and Core Detection ===
CLEAN_BUILD=0
CORES=""

if [[ "$1" == "--clean" ]]; then
    CLEAN_BUILD=1
    if [[ -n "$2" ]]; then
        CORES="$2"
    fi
elif [[ -n "$1" ]]; then
    CORES="$1"
fi

if [[ -z "$CORES" ]]; then
    if command -v nproc >/dev/null 2>&1; then
        CORES=$(nproc)
    else
        CORES=2
    fi
fi

if ! ldconfig -p | grep -q openal; then
    echo "ERROR: OpenAL development package not found."
    echo "Please install it with: sudo apt install libopenal-dev"
    read -p "Press Enter to exit..."
    exit 1
fi

echo "Using $CORES parallel jobs for builds."

RESULT_DIR="script_build_result"

# ==== Clean build result if requested ====
if [[ "$CLEAN_BUILD" == "1" ]]; then
    echo "Cleaning previous build result: $RESULT_DIR"
    rm -rf "$RESULT_DIR"
fi

# ==== Build yaml-cpp (if present as submodule/thirdparty) ====
if [[ -d Thirdparty/yaml-cpp ]]; then
    pushd Thirdparty/yaml-cpp > /dev/null
    if [[ "$CLEAN_BUILD" == "1" ]]; then
        rm -rf build
    fi
    mkdir -p build
    cd build
    cmake .. 
    make -j"$CORES"
    popd > /dev/null
fi

# ==== Prepare Result/Output Directory ====
mkdir -p "$RESULT_DIR"

# ==== Build Space Invaders ====
qmake -o "$RESULT_DIR/Makefile" SpaceInvaders.pro
if [[ $? -ne 0 ]]; then
    echo "ERROR: qmake failed!"
    read -p "Press Enter to exit..."
    exit 1
fi

cd "$RESULT_DIR"
make -j"$CORES"
if [[ $? -ne 0 ]]; then
    echo "ERROR: Build failed!"
    read -p "Press Enter to exit..."
    exit 1
fi
cd ..

# ==== Copy levels directory to actual executable folder ====
TARGET_DIR="$RESULT_DIR/release"
if [[ ! -d "$TARGET_DIR" ]]; then
    TARGET_DIR="$RESULT_DIR"
fi

echo "Copying levels directory to $TARGET_DIR..."
rm -rf "$TARGET_DIR/levels"
if [[ -d levels ]]; then
    cp -r levels "$TARGET_DIR/"
    echo "Levels directory copied successfully."
else
    echo "WARNING: levels directory not found in project root!"
fi

# ==== Always remove the debug folder from build result ====
DEBUG_DIR="$RESULT_DIR/debug"
if [[ -d "$DEBUG_DIR" ]]; then
    rm -rf "$DEBUG_DIR"
    echo "Removed debug folder: $DEBUG_DIR"
fi

echo "Build succeeded!"
read -p "Press Enter to exit..."

