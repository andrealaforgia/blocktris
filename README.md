# Blocktris

A modern block-falling puzzle game written in C99 with SDL2, featuring pentomino pieces and enhanced gameplay mechanics inspired by classic Tetris.

## Features

- **Pentomino Pieces**: 18 unique pieces including classic tetrominoes and challenging pentomino shapes
- **Enhanced Graphics**: Smooth countdown sequences, visual effects, and polished UI
- **Advanced Scoring**: Level progression, line clear bonuses, and combo multipliers
- **Professional Animation**: Animated countdowns (3-2-1-GO!) with fade effects
- **Responsive Controls**: Precise movement with key repeat handling and hard drop
- **Modern Architecture**: Clean C99 code with modular SDL2 engine design

## Gameplay

### Piece Types

The game features an expanded set of puzzle pieces:

**Classic Tetrominoes:**
- **I-piece**: Cyan (4-block line)
- **O-piece**: Yellow (2×2 square)
- **T-piece**: Purple (T-shaped)
- **S-piece**: Green (S-shaped)
- **Z-piece**: Red (Z-shaped)
- **J-piece**: Blue (L-shaped)
- **L-piece**: Orange (L-shaped)

**Pentomino Pieces:**
- **X-piece**: Cross shape
- **U-piece**: U-shaped
- **V-piece**: V-shaped
- **W-piece**: W-shaped
- **N-piece**: Zigzag
- **P-piece**: P-shaped
- **F-piece**: F-shaped
- **Y-piece**: Y-shaped
- And more unique 5-block configurations

### Game Mechanics

- **Precise Physics**: Accurate collision detection and rotation system
- **Line Clearing**: Multi-line clears with cascading effects
- **Level Progression**: Increasing difficulty with faster fall speeds
- **Scoring System**: Points for placement, line clears, and soft/hard drops
- **Smooth Animations**: Professional countdown and transition effects

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
- **SDL2**: Version 2.0+ with development headers
- **SDL2_mixer**: For audio support
- **Make**: Build system

### Installing Dependencies

Use the automatic installation target in the Makefile:

```bash
# Install dependencies automatically (detects your platform)
make install
```

**Manual Installation:**

#### macOS (Homebrew)
```bash
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

## Building & Running

### Quick Start
```bash
# Clone and build
git clone https://github.com/andrealaforgia/blocktris.git
cd blocktris
make install  # Install dependencies
make run      # Build and run the game
```

### Available Targets
```bash
# Build the game
make

# Install system dependencies
make install

# Run the game (builds automatically if needed)
make run

# Run test suite
make test

# Clean build artifacts
make clean

# Code quality tools
make lint     # Run cppcheck linter
make format   # Format code with clang-format
```

## Project Architecture

```
blocktris/
├── engine/                    # Core game engine
│   └── core/                 # Engine components
│       ├── graphics/         # Rendering system
│       ├── audio/           # Audio management
│       ├── input/           # Input handling
│       ├── math/            # Math utilities
│       ├── memory/          # Memory management
│       └── utils/           # Utility functions
├── game/                     # Game-specific code
│   ├── assets/              # Game assets
│   └── src/                 # Game source code
│       ├── entities/        # Game entities
│       ├── managers/        # Game managers
│       ├── rendering/       # Game rendering
│       ├── scoring/         # Scoring system
│       └── stages/          # Game stages
├── build/                   # Build output
└── Makefile                 # Build configuration
```

## Technical Highlights

### Engine Architecture
- **Modular Design**: Clean separation between engine and game code
- **Component System**: Entity-component architecture for game objects
- **State Management**: Robust game state and stage management
- **Resource Management**: Efficient texture and audio loading
- **Object Pooling**: Memory-efficient piece management

### Graphics System
- **SDL2 Rendering**: Hardware-accelerated 2D graphics
- **Bitmap Fonts**: Custom arcade-style font rendering
- **Animation System**: Smooth transitions and effects
- **Responsive Design**: Automatic screen scaling and centering

### Performance Features
- **60 FPS Target**: Consistent frame rate with delta timing
- **Memory Efficiency**: Stack allocation and object pooling
- **Optimized Rendering**: Minimal draw calls and state changes
- **Profiling Ready**: Debug builds with performance metrics

## Code Quality

- **Standards**: Strict C99 compliance with `-Wall -Wextra -pedantic-errors`
- **Documentation**: Comprehensive inline documentation
- **Testing**: Defensive programming with extensive error checking
- **Modularity**: Clean interfaces and minimal dependencies

## Development

### Adding New Features

1. **New Piece Types**: Add to `piece_type_t` enum and define in piece data
2. **Game Modes**: Implement new stages following existing patterns
3. **Visual Effects**: Extend rendering system in graphics modules
4. **Audio**: Add sound effects using SDL2_mixer integration

### Code Style Guide

- **Naming**: `snake_case` for functions, `UPPER_CASE` for constants
- **Structures**: `typedef struct { ... } name_t` format
- **Headers**: Include guards and forward declarations
- **Comments**: Doxygen-style documentation

## Contributing

This project serves as both a playable game and a learning resource for:
- C99 game development
- SDL2 engine architecture  
- Memory-efficient game design
- Cross-platform development

## License

Open source project provided as educational material. Feel free to use, modify, and learn from the codebase.

---

**Blocktris** - Where classic puzzle gaming meets modern code architecture.