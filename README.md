# CHIP-8 Emulator

A CHIP-8 emulator implemented in C++ with ncurses terminal-based graphics and keyboard input support.

## Description

CHIP-8 is an interpreted programming language developed by Joseph Weisbecker in the mid-1970s. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers to make game programming easier. This emulator faithfully recreates the CHIP-8 system, allowing you to run classic games and programs in your terminal.

## Features

- Complete CHIP-8 instruction set implementation
- Terminal-based graphics using ncurses with Unicode box drawing
- Real-time keyboard input handling
- Sound support (basic beep)
- 60Hz display refresh rate with 30Hz timer updates
- Debug output for unknown instructions
- Cross-platform compatibility (macOS, Linux)

## Requirements

- C++17 compatible compiler (GCC, Clang, etc.)
- CMake 3.10 or higher
- ncurses library
- A terminal that supports Unicode characters

**OR**

- Nix package manager (recommended for reproducible builds)

### Installing Dependencies

**macOS:**
```bash
brew install cmake ncurses
```

**Ubuntu/Debian:**
```bash
sudo apt-get install cmake libncurses5-dev libncursesw5-dev
```

**Arch Linux:**
```bash
sudo pacman -S cmake ncurses
```

## Building

### Using Nix (Recommended)

**With Nix flakes:**
```bash
# Clone the repository
git clone https://github.com/AdekSycamore/chip-8.git
cd chip-8

# Build the project
nix build

# Run directly without building locally
nix run . -- path/to/rom.ch8

# Enter development environment
nix develop
```

**With legacy Nix:**
```bash
# Enter development environment
nix-shell

# Then build normally
mkdir build && cd build
cmake .. && make
```

### Using traditional tools

```bash
# Clone the repository
git clone https://github.com/AdekSycamore/chip-8.git
cd chip-8

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make
```

## Usage

### With Nix
```bash
# Run directly from Nix store
nix run . -- path/to/rom.ch8

# Or after building
./result/bin/chip8-emulator path/to/rom.ch8
```

### Traditional build
```bash
# Run the emulator with a ROM file
./chip8 path/to/rom.ch8
```

### Keyboard Layout

The CHIP-8 keypad is mapped to your keyboard as follows:

```
CHIP-8 Keypad    Keyboard
1 2 3 C          1 2 3 4
4 5 6 D    →     Q W E R
7 8 9 E          A S D F
A 0 B F          Z X C V
```

### Controls

- **Q**: Quit the emulator
- **1-4, Q-R, A-F, Z-V**: CHIP-8 keypad input

## Project Structure

```
CHIP-8/
├── src/
│   ├── main.cpp        # Main entry point
│   ├── chip8.cpp       # CHIP-8 CPU implementation
│   ├── chip8.hpp       # CHIP-8 class definition
│   ├── display.cpp     # Display rendering
│   ├── display.hpp     # Display class definition
│   ├── utils.cpp       # Utility functions
│   └── utils.hpp       # Utility function declarations
├── CMakeLists.txt      # Build configuration
└── README.md           # This file
```

## Technical Details

### Architecture
- **CPU**: 8-bit with 16 general-purpose registers (V0-VF)
- **Memory**: 4KB RAM (0x000-0xFFF)
- **Display**: 64x32 monochrome pixels
- **Stack**: 16 levels for subroutine calls
- **Timers**: 60Hz delay and sound timers

### Implementation Features
- Function pointer dispatch tables for efficient instruction decoding
- Separate timing for CPU cycles, display refresh, and timer updates
- Unicode characters for crisp terminal graphics
- Minimal input lag with optimized keyboard handling

## ROM Compatibility

This emulator supports standard CHIP-8 ROMs (.ch8 files). You can find ROMs at:
- [CHIP-8 Archive](https://github.com/kripod/chip8-roms)
- [Zophar's Domain](https://www.zophar.net/pdroms/chip8.html)

## Known Issues

- Sound is limited to terminal bell character
- Some ROMs may have timing-sensitive behavior
- Terminal size must accommodate the display (minimum 66x18 characters)

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is open source. Feel free to use, modify, and distribute.