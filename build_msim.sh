#!/bin/bash
# build_msim.sh - configure and build microsimulation (Release)
# Edit the DEFAULT_* values below to match your environment, or pass --qt-dir / --qscintilla-dir on the CLI.
set -euo pipefail
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BUILD_DIR="$SCRIPT_DIR/cmake-build-release"

# ===================== USER CONFIG (edit these defaults) =====================
# Examples:
# macOS Homebrew Qt: /opt/homebrew/opt/qt
# Intel macOS Homebrew Qt: /usr/local/opt/qt
# Windows MinGW Qt: C:/Qt/6.9.1/mingw_64
DEFAULT_QT_DIR="/opt/homebrew/opt/qt"
# QScintilla default (Homebrew example)
DEFAULT_QSCINTILLA_DIR="/opt/homebrew/opt/qscintilla2"
# =============================================================================

QT_DIR="${QT_DIR:-$DEFAULT_QT_DIR}"
QSCINTILLA_DIR="${QSCINTILLA_DIR:-$DEFAULT_QSCINTILLA_DIR}"
QSCINTILLA_INCLUDE_DIR="${QSCINTILLA_INCLUDE_DIR:-}"   # optional explicit override
QSCINTILLA_LIBRARY="${QSCINTILLA_LIBRARY:-}"           # optional explicit override
GENERATOR="${GENERATOR:-Ninja}"

# parse args (CLI overrides env/defaults)
while [[ $# -gt 0 ]]; do
  key="$1"
  case $key in
    --qt-dir)
      QT_DIR="$2"; shift; shift;;
    --qscintilla-dir)
      QSCINTILLA_DIR="$2"; shift; shift;;
    --qscintilla-include)
      QSCINTILLA_INCLUDE_DIR="$2"; shift; shift;;
    --qscintilla-lib)
      QSCINTILLA_LIBRARY="$2"; shift; shift;;
    --generator)
      GENERATOR="$2"; shift; shift;;
    --clean)
      rm -rf "$BUILD_DIR"; shift;;
    *)
      echo "Unknown arg: $1"; exit 1;;
  esac
done

# Helper: try to locate QScintilla include and library if a QSCINTILLA_DIR is given and explicit values are not supplied.
if [[ -n "$QSCINTILLA_DIR" && -z "$QSCINTILLA_INCLUDE_DIR" ]]; then
  if [[ -d "$QSCINTILLA_DIR/include/Qsci" ]]; then
    QSCINTILLA_INCLUDE_DIR="$QSCINTILLA_DIR/include"
  elif [[ -d "$QSCINTILLA_DIR/include" ]]; then
    QSCINTILLA_INCLUDE_DIR="$QSCINTILLA_DIR/include"
  fi
fi

if [[ -n "$QSCINTILLA_DIR" && -z "$QSCINTILLA_LIBRARY" ]]; then
  # look for likely library file names
  candidates=(
    "$QSCINTILLA_DIR/lib/libqscintilla2_qt6.dylib"
    "$QSCINTILLA_DIR/lib/libqscintilla2_qt6.so"
    "$QSCINTILLA_DIR/lib/libqscintilla2_qt6.a"
    "$QSCINTILLA_DIR/lib/qscintilla2_qt6.dll"
    "$QSCINTILLA_DIR/lib/qscintilla2_qt6.lib"
  )
  for f in "${candidates[@]}"; do
    if [[ -f "$f" ]]; then
      QSCINTILLA_LIBRARY="$f"
      break
    fi
  done
  # fallback: pick any file matching *qscintilla* in lib
  if [[ -z "$QSCINTILLA_LIBRARY" && -d "$QSCINTILLA_DIR/lib" ]]; then
    found=$(ls "$QSCINTILLA_DIR/lib"/*qscintilla* 2>/dev/null | head -n1 || true)
    if [[ -n "$found" ]]; then
      QSCINTILLA_LIBRARY="$found"
    fi
  fi
fi

# Configure and build
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

CMAKE_ARGS=(
  -DCMAKE_BUILD_TYPE=Release
  -G "$GENERATOR"
)

# Pass Qt prefix to CMake as CMAKE_PREFIX_PATH (preferred way to locate Qt via find_package)
if [[ -n "$QT_DIR" ]]; then
  CMAKE_ARGS+=( -DCMAKE_PREFIX_PATH="$QT_DIR" )
fi

# Pass explicit QScintilla include and library locations if available
if [[ -n "$QSCINTILLA_INCLUDE_DIR" ]]; then
  CMAKE_ARGS+=( -DQSCINTILLA_INCLUDE_DIR="$QSCINTILLA_INCLUDE_DIR" )
fi
if [[ -n "$QSCINTILLA_LIBRARY" ]]; then
  CMAKE_ARGS+=( -DQSCINTILLA_LIBRARY="$QSCINTILLA_LIBRARY" )
fi

echo "Running cmake with: ${CMAKE_ARGS[*]}"
cmake ${CMAKE_ARGS[*]} "$SCRIPT_DIR"

# build using parallel jobs
if command -v nproc >/dev/null 2>&1; then
  JOBS=$(nproc)
elif [[ "$(uname)" == "Darwin" ]]; then
  JOBS=$(sysctl -n hw.ncpu)
else
  JOBS=2
fi
cmake --build . --config Release -- -j${JOBS}

echo "Build complete: $BUILD_DIR"
