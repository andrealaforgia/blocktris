# Tetris Game

A complete Tetris implementation in C99 using SDL2, following modular game engine architecture patterns.

## Features

- **Classic Tetris Gameplay**: All 7 standard tetromino pieces (I, O, T, S, Z, J, L)
- **Accurate Physics**: Proper piece rotation, movement, and collision detection
- **Line Clearing**: Full line detection and clearing with cascade mechanics
- **Visual Design**: Clean line-based rendering with colored pieces and blue border
- **Responsive Controls**: Smooth movement with key repeat handling
- **Performance Optimized**: Object pool patterns and efficient memory management

## Game Specifications

- **Grid Size**: 20 width × 30 height
- **Window Size**: 90% of screen width with proper aspect ratio
- **Piece Colors**: Each tetromino type has its distinct color
- **Fall Speed**: Configurable timing system (500ms default)

## Controls

| Key | Action |
|-----|--------|
| ← → | Move piece left/right |
| ↑ | Rotate piece clockwise |
| ↓ | Rotate piece counter-clockwise |
| Space | Hard drop piece |
| ESC | Quit game |

## Requirements

- **C Compiler**: GCC with C99 support
- **SDL2**: Development libraries and headers
- **Make**: Build system

### Installing Dependencies

#### macOS (Homebrew)
```bash
brew install sdl2
```

#### Ubuntu/Debian
```bash
sudo apt-get install libsdl2-dev
```

#### Arch Linux
```bash
sudo pacman -S sdl2
```

## Building

### Quick Start
```bash
# Install dependencies (macOS)
make install-deps

# Build and run
make run
```

### Build Targets

```bash
# Build release version
make

# Build debug version
make debug

# Clean build artifacts
make clean

# Run release version
make run

# Run debug version
make run-debug

# Memory leak checking (requires valgrind)
make memcheck

# Show help
make help
```

## Project Structure

```
tetris/
├── include/
│   └── tetris.h           # Main header with all declarations
├── src/
│   ├── main.c             # Entry point and SDL initialization
│   ├── engine.c           # Core game engine and loop
│   ├── tetromino.c        # Piece definitions and logic
│   ├── board.c            # Game board management
│   ├── input.c            # Input handling system
│   └── utils.c            # Utility functions
├── build/                 # Build output directory
│   ├── obj/              # Object files
│   └── tetris            # Executable
├── Makefile              # Build configuration
└── README.md             # This file
```

## Architecture

The game follows C99 SDL2 engine architecture patterns:

- **Modular Design**: Separated concerns for rendering, input, game logic
- **Component-Based Entities**: Tetromino pieces as simple structs
- **State Management**: Clean separation of game states
- **Memory Management**: Stack allocation and efficient data structures
- **Event-Driven Input**: SDL2 event handling with key repeat logic

### Key Components

1. **Game Engine** (`engine.c`): Main loop, state management, timing
2. **Tetromino System** (`tetromino.c`): Piece definitions, rotation, movement
3. **Board Management** (`board.c`): Grid logic, collision detection, line clearing
4. **Input System** (`input.c`): Key handling with repeat support
5. **Rendering**: Direct SDL2 calls for line-based graphics

## Code Quality

- **Standards Compliance**: Strict C99 with `-Wall -Wextra -pedantic-errors`
- **Documentation**: Doxygen-style comments for all public functions
- **Error Handling**: Defensive programming with null pointer checks
- **Performance**: Optimized for smooth 60 FPS gameplay

## Gameplay Features

### Tetromino Pieces

All 7 standard pieces with proper colors:
- **I-piece**: Cyan (4-block line)
- **O-piece**: Yellow (2×2 square)  
- **T-piece**: Purple (T-shaped)
- **S-piece**: Green (S-shaped)
- **Z-piece**: Red (Z-shaped)
- **J-piece**: Blue (L-shaped, left)
- **L-piece**: Orange (L-shaped, right)

### Game Mechanics

- **Piece Movement**: Smooth left/right movement with collision detection
- **Rotation**: Clockwise and counter-clockwise with wall kick validation
- **Line Clearing**: Automatic detection and clearing of completed lines
- **Gravity**: Configurable piece falling with timing control
- **Hard Drop**: Instant piece placement with space bar

## Troubleshooting

### Build Issues

**SDL2 not found**:
```bash
# Verify SDL2 installation
sdl2-config --version
sdl2-config --cflags
sdl2-config --libs
```

**Compilation errors**:
```bash
# Clean and rebuild
make clean
make debug
```

### Runtime Issues

**No display**:
- Ensure display server is running
- Check SDL2 video driver support
- Try different display modes

**Performance issues**:
- Use release build: `make run`
- Check system resources
- Verify hardware acceleration

## Development

### Adding Features

The modular architecture makes it easy to extend:

1. **New Game States**: Add to `game_state_t` enum and handle in engine
2. **Enhanced Graphics**: Extend rendering functions in respective modules  
3. **Audio**: Add SDL2_mixer integration
4. **Menu System**: Implement in separate module following existing patterns

### Code Style

- **Functions**: `snake_case` naming
- **Constants**: `UPPER_CASE` naming  
- **Structs**: `typedef struct { ... } name_t` format
- **Headers**: Include guards, forward declarations
- **Documentation**: Doxygen-style comments

## License

This project is provided as a learning example and game template. Feel free to use, modify, and distribute according to your needs.