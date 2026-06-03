# SCP 096 - Survive Him in  the Maze 🎮
i may have added duplicate files.
A fast-paced survival game where you must navigate through a procedurally generated maze while evading an intelligent monster. Can you survive for 3 minutes?
originally by scp company.
## 🎯 Gameplay

- **Objective**: Survive in the maze for 180 seconds without being caught by the monster
- **Controls**:
  - `W/A/S/D` - Move around the maze
  - `LSHIFT` - Sprint (depletes stamina, makes more noise)
  - `ESC` - Pause/Resume
  - `SPACE` - Start game or return to menu

## ✨ Features

### Core Mechanics
- **Procedurally Generated Mazes**: Every game generates a unique maze layout
- **Intelligent AI Monster**: 
  - Patrol mode when unaware
  - Investigation mode when suspicious
  - Chase mode when the player is detected
  - Advanced AI with sight and hearing mechanics

### Player Systems
- **Stamina Management**: Sprint for speed boosts but manage your stamina bar
- **Noise Generation**: 
  - Sprinting creates loud noise (100%)
  - Walking creates moderate noise (50%)
  - Quiet noise decays over time
- **Collision Detection**: Navigate maze walls with proper physics

### Monster AI
- **Three Alert States**:
  - **Patrol** (Alert < 30%): Random patrol, low speed
  - **Investigate** (Alert 30-70%): Searches suspicious areas, medium speed
  - **Chase** (Alert > 70%): Direct pursuit with maximum speed
- **Detection Systems**:
  - Visual range: 300 pixels
  - Audio range: 250 pixels + noise sensitivity
  - Real-time alert level adjustment

### Visual Polish
- Dynamic monster eye color (changes with alert level)
- Real-time stamina bar with color coding (green/red)
- Alert level indicator
- Survival timer display

## 🛠️ Building

### Prerequisites
- C++17 compatible compiler
- CMake 3.12+
- SDL2 development libraries

### Linux/macOS
```bash
# Install SDL2
sudo apt-get install libsdl2-dev  # Ubuntu/Debian
brew install sdl2                  # macOS

# Build
mkdir build
cd build
cmake ..
make
```

### Windows (MSVC)
```bash
# Install SDL2 (pre-built or via vcpkg)
# Using vcpkg: vcpkg install sdl2:x64-windows

mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

## 📁 Project Structure

```
maze-game/
├── src/
│   ├── game.hpp          # Header file with class definitions
│   └── game.cpp          # Implementation file
├── CMakeLists.txt        # Build configuration
└── README.md             # This file
```

## 🏗️ Architecture

### Class Hierarchy

**Vector2**
- 2D vector operations for positions and directions
- Methods: `distance()`, `magnitude()`, `normalize()`, operator overloads

**Maze**
- Procedural maze generation using recursive backtracking
- Collision detection for player and monster
- Grid-based rendering system

**Player**
- Position, stamina, and noise management
- Input handling and movement validation
- Visual representation with stamina bar

**Monster**
- AI-driven pathfinding and behavior
- Alert level system
- Adaptive speed based on awareness state

**Game**
- Main game loop and state management
- Event handling and frame timing
- Rendering and HUD display

## 🔧 Key Improvements from Original

1. **Code Organization**
   - Separated headers and implementation
   - Proper namespacing for colors
   - Clear class responsibilities

2. **Memory Management**
   - `std::unique_ptr` for automatic cleanup
   - RAII principles throughout
   - No memory leaks

3. **Constants & Magic Numbers**
   - All constants defined at top of headers
   - Configurable game parameters
   - Documented value meanings

4. **Error Handling**
   - SDL initialization error checks
   - Graceful failure modes
   - Console error reporting

5. **AI Improvements**
   - Separated AI logic into dedicated methods
   - Better state transitions
   - More realistic monster behavior

6. **Vector Math**
   - Proper vector operations
   - Normalized direction calculations
   - Cleaner movement code

7. **Game States**
   - Added PAUSED state
   - Better state transitions
   - Configurable game duration

8. **Rendering**
   - Organized draw functions
   - Stamina bar with color feedback
   - HUD elements foundation

## 🎮 Gameplay Tips

- **Use the maze**: The AI can't see through walls, use them to your advantage
- **Manage stamina**: Don't sprint constantly; you'll need it when the monster spots you
- **Listen**: Keep noise levels down when possible to avoid detection
- **Find patterns**: The maze generation creates natural choke points; use them strategically

## 🚀 Future Enhancements

- [ ] Text rendering for UI (requires SDL_ttf)
- [ ] Multiple difficulty levels
- [ ] Sound effects and music
- [ ] Collectible power-ups
- [ ] Multiple monsters
- [ ] Score system and high scores
- [ ] Mobile touch controls
- [ ] Particles and visual effects

## 📝 Code Quality

- **C++17 Features**: Modern C++ practices
- **Memory Safe**: No manual memory management
- **Well-Documented**: Clear comments and structure
- **Extensible**: Easy to add new features

## 🎓 Learning Points

This codebase demonstrates:
- Object-oriented game design
- 2D game loop architecture
- Procedural generation techniques
- AI pathfinding and behavior trees
- SDL2 graphics programming
- CMake project configuration
- Modern C++ best practices

---

**Created**: 2026-06-02  
**Version**: 1.0 (Enhanced)
