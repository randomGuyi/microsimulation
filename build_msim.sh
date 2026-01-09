#!/usr/bin/env bash
# build_msim.sh - configure and build microsimulation (Release)
# Usage: ./build_msim.sh [--qt-dir /path/to/qt] [--qscintilla-dir /path/to/qscintilla] [--generator "Ninja"]
set -euo pipefail
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BUILD_DIR="$SCRIPT_DIR/cmake-build-release"
QT_DIR="${QT_DIR:-}"    # can be set in env or passed via --qt-dir
QSCINTILLA_DIR="${QSCINTILLA_DIR:-}"
GENERATOR="${GENERATOR:-Ninja}"  # default to Ninja if available

# parse args
while [[ $# -gt 0 ]]; do
  key="$1"
  case $key in
    --qt-dir)
      QT_DIR="$2"; shift; shift;;
    --qscintilla-dir)
      QSCINTILLA_DIR="$2"; shift; shift;;
    --generator)
      GENERATOR="$2"; shift; shift;;
    --clean)
      rm -rf "$BUILD_DIR"; shift;;
    *)
      echo "Unknown arg: $1"; exit 1;;
  esac
done

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

CMAKE_ARGS=(
  -DCMAKE_BUILD_TYPE=Release
  -G "$GENERATOR"
)
if [[ -n "$QT_DIR" ]]; then
  CMAKE_ARGS+=( -DQTDIR="$QT_DIR" )
fi
if [[ -n "$QSCINTILLA_DIR" ]]; then
  CMAKE_ARGS+=( -DQSCINTILLA_DIR="$QSCINTILLA_DIR" )
fi

# run cmake configure and build
cmake ${CMAKE_ARGS[*]} "$SCRIPT_DIR"
cmake --build . --config Release -- -j$(sysctl -n hw.ncpu)

echo "Build complete: $BUILD_DIR"
