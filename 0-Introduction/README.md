# Raylib Introduction

This is the first raylib example for Club. The goal is simple: get a small window running, draw a few shapes, and respond to mouse input.

The project includes raylib binaries for Windows. On macOS and Linux, CMake uses the raylib installation provided by the operating system or package manager.

## Windows

You can build the project with either Visual Studio (MSVC) or MinGW-w64. You do not need both.

### Option 1: Visual Studio / MSVC

Install Visual Studio with the **Desktop development with C++** workload, then run these commands from a developer PowerShell:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Debug
```

The executable will be in:

```text
build/Debug/RaylibTemplate.exe
```

### Option 2: MinGW-w64

You need a 64-bit MinGW-w64 toolchain and CMake. If you use MSYS2, open an **UCRT64** terminal and install:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake
```

Build and run the project with:

```bash
cmake -S . -B build-mingw -G "MinGW Makefiles"
cmake --build build-mingw
./build-mingw/RaylibTemplate.exe
```

The bundled MinGW raylib library is 64-bit, so the compiler must also target 64-bit Windows.

## macOS

If you do not already have Homebrew, open Terminal and run the official installer:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

The installer may ask for your administrator password and may print one extra command to add `brew` to your shell path. Follow that final instruction, then check that Homebrew is available:

```bash
brew --version
```

If macOS asks for Apple's command-line tools first, install them with:

```bash
xcode-select --install
```

Once Homebrew is ready, install CMake and raylib:

```bash
brew update
brew install cmake raylib
```

The project automatically checks the usual Homebrew locations on both Apple Silicon (`/opt/homebrew`) and Intel Macs (`/usr/local`).

Build and run:

```bash
cmake -S . -B build
cmake --build build
./build/RaylibTemplate
```

## Linux

On Ubuntu or Debian, install the compiler, CMake, and raylib development package:

```bash
sudo apt update
sudo apt install build-essential cmake pkg-config libraylib-dev
```

Then build and run:

```bash
cmake -S . -B build
cmake --build build
./build/RaylibTemplate
```

CMake first tries `pkg-config` to find raylib. If that is not available, it falls back to the standard system include and library paths. If your distribution does not provide `libraylib-dev`, install raylib using that distribution's package instructions.

## What should happen?

When the program starts, you should see a raylib window with a line, a circle, a rectangle. The circle follows the mouse, and clicking the left mouse button prints the mouse position to the terminal.


## resource

https://github.com/raysan5/raylib
https://www.raylib.com/cheatsheet/cheatsheet.html
https://www.raylib.com/examples.html