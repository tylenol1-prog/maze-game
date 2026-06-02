#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

// ============================================================================
// CONSTANTS
// ============================================================================

constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 800;
constexpr int FPS = 60;
constexpr int MAZE_TILE_SIZE = 20;
constexpr float FRAME_TIME = 1.0f / FPS;

// ============================================================================
// COLOR DEFINITIONS
// ============================================================================

namespace Colors {
    constexpr SDL_Color BLACK = {0, 0, 0, 255};
    constexpr SDL_Color WHITE = {255, 255, 255, 255};
    constexpr SDL_Color GRAY = {100, 100, 100, 255};
    constexpr SDL_Color DARK_GRAY = {50, 50, 50, 255};
    constexpr SDL_Color RED = {200, 0, 0, 255};
    constexpr SDL_Color GREEN = {0, 200, 0, 255};
    constexpr SDL_Color BLUE = {0, 100, 255, 255};
    constexpr SDL_Color PALE = {220, 220, 200, 255};
    constexpr SDL_Color BLOOD_RED = {139, 0, 0, 255};
    constexpr SDL_Color YELLOW = {255, 255, 0, 255};
}

// ============================================================================
// ENUMS
// ============================================================================

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    WIN
};

// ============================================================================
// VECTOR2 STRUCT
// ============================================================================

struct Vector2 {
    float x, y;

    Vector2(float x = 0, float y = 0) : x(x), y(y) {}

    float distance(const Vector2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalize() const {
        float mag = magnitude();
        if (mag == 0) return Vector2(0, 0);
        return Vector2(x / mag, y / mag);
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
};

// ============================================================================
// MAZE CLASS
// ============================================================================

class Maze {
public:
    std::vector<std::vector<int>> grid;
    int width, height;

    Maze(int w, int h);
    void generateMaze();
    bool isWalkable(float x, float y, float radius = 15) const;
    void draw(SDL_Renderer* renderer) const;

private:
    static constexpr int WALKABLE = 0;
    static constexpr int WALL = 1;
};

// ============================================================================
// PLAYER CLASS
// ============================================================================

class Player {
public:
    Vector2 pos;
    float radius;
    float speed;
    float stamina;
    float maxStamina;
    bool sprinting;
    float noiseLevel;

    Player(float x, float y);
    void update(const Uint8* keys, const Maze& maze);
    void draw(SDL_Renderer* renderer) const;
    void reset(float x, float y);

private:
    static constexpr float BASE_SPEED = 4.0f;
    static constexpr float SPRINT_MULTIPLIER = 1.5f;
    static constexpr float SPRINT_DRAIN = 1.0f;
    static constexpr float STAMINA_REGEN = 0.5f;
    static constexpr float SPRINT_NOISE = 100.0f;
    static constexpr float WALK_NOISE = 50.0f;
    static constexpr float NOISE_DECAY = 2.0f;

    void drawFilledCircle(SDL_Renderer* r, int x, int y, int rad, const SDL_Color& c) const;
    void drawStaminaBar(SDL_Renderer* renderer) const;
};

// ============================================================================
// MONSTER CLASS
// ============================================================================

class Monster {
public:
    Vector2 pos;
    Vector2 target;
    float radius;
    float speed;
    float currentSpeed;
    float detectionRange;
    float hearingRange;
    float alertLevel;

    Monster(float x, float y);
    void update(const Player& player, const Maze& maze);
    void draw(SDL_Renderer* renderer) const;
    void reset(float x, float y);

private:
    static constexpr float ALERT_INCREASE_SIGHT = 5.0f;
    static constexpr float ALERT_INCREASE_SOUND = 2.0f;
    static constexpr float ALERT_DECREASE = 0.5f;
    static constexpr float ALERT_CHASE_THRESHOLD = 70.0f;
    static constexpr float ALERT_INVESTIGATE_THRESHOLD = 30.0f;
    static constexpr float CHASE_SPEED = 3.5f;
    static constexpr float INVESTIGATE_SPEED = 2.5f;
    static constexpr float PATROL_SPEED = 1.5f;
    static constexpr float ACCELERATION = 0.05f;
    static constexpr float DECELERATION = 0.05f;

    void drawFilledCircle(SDL_Renderer* r, int x, int y, int rad, const SDL_Color& c) const;
    void updateAI(const Player& player);
    void moveTowardTarget(const Maze& maze);
};

// ============================================================================
// GAME CLASS
// ============================================================================

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    GameState state;
    GameState previousState;
    bool running;

    std::unique_ptr<Player> player;
    std::unique_ptr<Monster> monster;
    std::unique_ptr<Maze> maze;

    float survivalTime;
    float targetSurvivalTime;

    void handleEvents();
    void update();
    void draw();
    void resetGame();

    void drawMenu() const;
    void drawPauseMenu() const;
    void drawGameOver() const;
    void drawWin() const;
    void drawHUD() const;
    void drawText(int x, int y, const std::string& text, const SDL_Color& color) const;

    Vector2 findValidSpawnPoint(const Vector2& avoidPos = Vector2(-1, -1), float minDistance = 0);
};

#endif // GAME_HPP
