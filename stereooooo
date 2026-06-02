#include "game.hpp"
#include <sstream>
#include <iomanip>

// ============================================================================
// MAZE CLASS IMPLEMENTATION
// ============================================================================

Maze::Maze(int w, int h) : width(w), height(h) {
    grid.assign(h, std::vector<int>(w, WALL));
    generateMaze();
}

void Maze::generateMaze() {
    for (int y = 1; y < height - 1; y += 2) {
        for (int x = 1; x < width - 1; x += 2) {
            grid[y][x] = WALKABLE;
            if (rand() % 100 < 60 && x + 1 < width) grid[y][x + 1] = WALKABLE;
            if (rand() % 100 < 60 && y + 1 < height) grid[y + 1][x] = WALKABLE;
        }
    }
}

bool Maze::isWalkable(float x, float y, float radius) const {
    int gridX = static_cast<int>(x / MAZE_TILE_SIZE);
    int gridY = static_cast<int>(y / MAZE_TILE_SIZE);

    std::vector<std::pair<int, int>> checkPoints = {
        {gridX, gridY}, {gridX + 1, gridY}, {gridX - 1, gridY},
        {gridX, gridY + 1}, {gridX, gridY - 1}
    };

    for (const auto& p : checkPoints) {
        if (p.first >= 0 && p.first < width && p.second >= 0 && p.second < height) {
            if (grid[p.second][p.first] == WALL) return false;
        }
    }

    if (x - radius < 0 || x + radius > SCREEN_WIDTH) return false;
    if (y - radius < 0 || y + radius > SCREEN_HEIGHT) return false;
    return true;
}

void Maze::draw(SDL_Renderer* renderer) const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == WALL) {
                SDL_Rect rect = {x * MAZE_TILE_SIZE, y * MAZE_TILE_SIZE, MAZE_TILE_SIZE, MAZE_TILE_SIZE};
                SDL_SetRenderDrawColor(renderer, Colors::DARK_GRAY.r, Colors::DARK_GRAY.g, 
                                      Colors::DARK_GRAY.b, Colors::DARK_GRAY.a);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

// ============================================================================
// PLAYER CLASS IMPLEMENTATION
// ============================================================================

Player::Player(float x, float y)
    : pos(x, y), radius(15), speed(BASE_SPEED), stamina(maxStamina),
      maxStamina(100), sprinting(false), noiseLevel(0) {}

void Player::reset(float x, float y) {
    pos = Vector2(x, y);
    stamina = maxStamina;
    noiseLevel = 0;
    sprinting = false;
}

void Player::update(const Uint8* keys, const Maze& maze) {
    float dx = 0, dy = 0;
    if (keys[SDL_SCANCODE_W]) dy -= speed;
    if (keys[SDL_SCANCODE_S]) dy += speed;
    if (keys[SDL_SCANCODE_A]) dx -= speed;
    if (keys[SDL_SCANCODE_D]) dx += speed;

    bool hasMovement = (dx != 0 || dy != 0);
    sprinting = keys[SDL_SCANCODE_LSHIFT] && stamina > 0 && hasMovement;

    if (sprinting) {
        dx *= SPRINT_MULTIPLIER;
        dy *= SPRINT_MULTIPLIER;
        stamina -= SPRINT_DRAIN;
    } else {
        stamina = std::min(stamina + STAMINA_REGEN, maxStamina);
    }

    if (maze.isWalkable(pos.x + dx, pos.y + dy, radius)) {
        pos.x += dx;
        pos.y += dy;
    }

    if (sprinting) {
        noiseLevel = SPRINT_NOISE;
    } else if (hasMovement) {
        noiseLevel = WALK_NOISE;
    } else {
        noiseLevel = std::max(0.0f, noiseLevel - NOISE_DECAY);
    }
}

void Player::draw(SDL_Renderer* renderer) const {
    drawFilledCircle(renderer, static_cast<int>(pos.x), static_cast<int>(pos.y), 
                     static_cast<int>(radius), Colors::BLUE);
    drawStaminaBar(renderer);
}

void Player::drawFilledCircle(SDL_Renderer* r, int x, int y, int rad, const SDL_Color& c) const {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    for (int i = -rad; i <= rad; i++) {
        for (int j = -rad; j <= rad; j++) {
            if (i * i + j * j <= rad * rad) {
                SDL_RenderDrawPoint(r, x + i, y + j);
            }
        }
    }
}

void Player::drawStaminaBar(SDL_Renderer* renderer) const {
    const int BAR_WIDTH = 100;
    const int BAR_HEIGHT = 10;
    const int BAR_X = 10;
    const int BAR_Y = 10;

    SDL_Rect background = {BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT};
    SDL_SetRenderDrawColor(renderer, Colors::DARK_GRAY.r, Colors::DARK_GRAY.g,
                          Colors::DARK_GRAY.b, Colors::DARK_GRAY.a);
    SDL_RenderFillRect(renderer, &background);

    SDL_Rect bar = {BAR_X, BAR_Y, static_cast<int>(BAR_WIDTH * (stamina / maxStamina)), BAR_HEIGHT};
    SDL_Color barColor = stamina < 30 ? Colors::RED : Colors::GREEN;
    SDL_SetRenderDrawColor(renderer, barColor.r, barColor.g, barColor.b, barColor.a);
    SDL_RenderFillRect(renderer, &bar);
}

// ============================================================================
// MONSTER CLASS IMPLEMENTATION
// ============================================================================

Monster::Monster(float x, float y)
    : pos(x, y), target(x, y), radius(20), speed(PATROL_SPEED), currentSpeed(0),
      detectionRange(300), hearingRange(250), alertLevel(0) {}

void Monster::reset(float x, float y) {
    pos = Vector2(x, y);
    target = Vector2(x, y);
    alertLevel = 0;
    currentSpeed = 0;
}

void Monster::update(const Player& player, const Maze& maze) {
    updateAI(player);
    moveTowardTarget(maze);
}

void Monster::updateAI(const Player& player) {
    float dist = pos.distance(player.pos);

    if (dist < detectionRange) {
        alertLevel = std::min(100.0f, alertLevel + ALERT_INCREASE_SIGHT);
    } else if (dist < hearingRange && player.noiseLevel > 30) {
        alertLevel = std::min(100.0f, alertLevel + ALERT_INCREASE_SOUND);
    } else {
        alertLevel = std::max(0.0f, alertLevel - ALERT_DECREASE);
    }

    if (alertLevel > ALERT_CHASE_THRESHOLD) {
        target = player.pos;
        speed = CHASE_SPEED;
        currentSpeed = std::min(currentSpeed + ACCELERATION, speed);
    } else if (alertLevel > ALERT_INVESTIGATE_THRESHOLD) {
        if (rand() % 100 < 2) {
            target.x = player.pos.x + (rand() % 200 - 100);
            target.y = player.pos.y + (rand() % 200 - 100);
        }
        speed = INVESTIGATE_SPEED;
        currentSpeed = std::min(currentSpeed + ACCELERATION, speed);
    } else {
        if (rand() % 100 < 1) {
            target.x = pos.x + (rand() % 400 - 200);
            target.y = pos.y + (rand() % 400 - 200);
        }
        speed = PATROL_SPEED;
        currentSpeed = std::max(0.0f, currentSpeed - DECELERATION);
    }
}

void Monster::moveTowardTarget(const Maze& maze) {
    Vector2 direction = target - pos;
    float dist = direction.magnitude();

    if (dist > 5) {
        direction = direction.normalize();
        Vector2 movement = direction * currentSpeed;

        if (maze.isWalkable(pos.x + movement.x, pos.y + movement.y, radius)) {
            pos = pos + movement;
        }
    }
}

void Monster::draw(SDL_Renderer* renderer) const {
    drawFilledCircle(renderer, static_cast<int>(pos.x), static_cast<int>(pos.y), 15, Colors::PALE);
    drawFilledCircle(renderer, static_cast<int>(pos.x), static_cast<int>(pos.y - 35), 12, Colors::PALE);

    int eyeRed = static_cast<int>(50 + alertLevel * 2);
    SDL_Color eyeColor = {255, static_cast<Uint8>(eyeRed), static_cast<Uint8>(eyeRed), 255};
    drawFilledCircle(renderer, static_cast<int>(pos.x - 5), static_cast<int>(pos.y - 37), 3, eyeColor);
    drawFilledCircle(renderer, static_cast<int>(pos.x + 5), static_cast<int>(pos.y - 37), 3, eyeColor);
}

void Monster::drawFilledCircle(SDL_Renderer* r, int x, int y, int rad, const SDL_Color& c) const {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    for (int i = -rad; i <= rad; i++) {
        for (int j = -rad; j <= rad; j++) {
            if (i * i + j * j <= rad * rad) {
                SDL_RenderDrawPoint(r, x + i, y + j);
            }
        }
    }
}

// ============================================================================
// GAME CLASS IMPLEMENTATION
// ============================================================================

Game::Game()
    : window(nullptr), renderer(nullptr), state(GameState::MENU),
      previousState(GameState::MENU), running(true),
      player(nullptr), monster(nullptr), maze(nullptr),
      survivalTime(0), targetSurvivalTime(180) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    window = SDL_CreateWindow(
        "THE KIL - Survive the Maze",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    srand(static_cast<unsigned>(time(nullptr)));
}

Game::~Game() {
    player.reset();
    monster.reset();
    maze.reset();

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

Vector2 Game::findValidSpawnPoint(const Vector2& avoidPos, float minDistance) {
    Vector2 spawnPos;
    bool validSpawn = false;

    while (!validSpawn) {
        spawnPos.x = rand() % (SCREEN_WIDTH - 100) + 50;
        spawnPos.y = rand() % (SCREEN_HEIGHT - 100) + 50;

        if (maze->isWalkable(spawnPos.x, spawnPos.y)) {
            if (avoidPos.x < 0 || spawnPos.distance(avoidPos) > minDistance) {
                validSpawn = true;
            }
        }
    }

    return spawnPos;
}

void Game::resetGame() {
    maze = std::make_unique<Maze>(SCREEN_WIDTH / MAZE_TILE_SIZE, SCREEN_HEIGHT / MAZE_TILE_SIZE);

    Vector2 playerPos = findValidSpawnPoint();
    player = std::make_unique<Player>(playerPos.x, playerPos.y);

    Vector2 monsterPos = findValidSpawnPoint(playerPos, 300);
    monster = std::make_unique<Monster>(monsterPos.x, monsterPos.y);

    survivalTime = 0;
    state = GameState::PLAYING;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (state) {
                case GameState::MENU:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        resetGame();
                    }
                    break;
                case GameState::PLAYING:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        previousState = GameState::PLAYING;
                        state = GameState::PAUSED;
                    }
                    break;
                case GameState::PAUSED:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        state = GameState::PLAYING;
                    } else if (event.key.keysym.sym == SDLK_SPACE) {
                        state = GameState::MENU;
                    }
                    break;
                case GameState::GAME_OVER:
                case GameState::WIN:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        state = GameState::MENU;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::update() {
    if (state == GameState::PLAYING) {
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        player->update(keys, *maze);
        monster->update(*player, *maze);
        survivalTime += FRAME_TIME;

        float dist = player->pos.distance(monster->pos);
        if (dist < player->radius + monster->radius) {
            state = GameState::GAME_OVER;
        } else if (survivalTime > targetSurvivalTime) {
            state = GameState::WIN;
        }
    }
}

void Game::draw() {
    SDL_SetRenderDrawColor(renderer, Colors::BLACK.r, Colors::BLACK.g,
                          Colors::BLACK.b, Colors::BLACK.a);
    SDL_RenderClear(renderer);

    switch (state) {
        case GameState::MENU:
            drawMenu();
            break;
        case GameState::PLAYING:
            maze->draw(renderer);
            player->draw(renderer);
            monster->draw(renderer);
            drawHUD();
            break;
        case GameState::PAUSED:
            maze->draw(renderer);
            player->draw(renderer);
            monster->draw(renderer);
            drawHUD();
            drawPauseMenu();
            break;
        case GameState::GAME_OVER:
            drawGameOver();
            break;
        case GameState::WIN:
            drawWin();
            break;
    }

    SDL_RenderPresent(renderer);
}

void Game::drawMenu() const {
    SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 150, 100, 300, 100};
    SDL_SetRenderDrawColor(renderer, Colors::RED.r, Colors::RED.g,
                          Colors::RED.b, Colors::RED.a);
    SDL_RenderFillRect(renderer, &titleRect);

    SDL_Rect instructionsRect = {SCREEN_WIDTH / 2 - 200, 300, 400, 200};
    SDL_SetRenderDrawColor(renderer, Colors::GRAY.r, Colors::GRAY.g,
                          Colors::GRAY.b, Colors::GRAY.a);
    SDL_RenderFillRect(renderer, &instructionsRect);
}

void Game::drawPauseMenu() const {
    SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 200};
    SDL_SetRenderDrawColor(renderer, Colors::DARK_GRAY.r, Colors::DARK_GRAY.g,
                          Colors::DARK_GRAY.b, 200);
    SDL_RenderFillRect(renderer, &pauseRect);
}

void Game::drawGameOver() const {
    SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - 200, 200, 400, 150};
    SDL_SetRenderDrawColor(renderer, Colors::BLOOD_RED.r, Colors::BLOOD_RED.g,
                          Colors::BLOOD_RED.b, Colors::BLOOD_RED.a);
    SDL_RenderFillRect(renderer, &gameOverRect);
}

void Game::drawWin() const {
    SDL_Rect winRect = {SCREEN_WIDTH / 2 - 200, 200, 400, 150};
    SDL_SetRenderDrawColor(renderer, Colors::GREEN.r, Colors::GREEN.g,
                          Colors::GREEN.b, Colors::GREEN.a);
    SDL_RenderFillRect(renderer, &winRect);
}

void Game::drawHUD() const {
    SDL_Rect monsterAlertBox = {SCREEN_WIDTH - 150, 10, 140, 40};
    SDL_SetRenderDrawColor(renderer, Colors::DARK_GRAY.r, Colors::DARK_GRAY.g,
                          Colors::DARK_GRAY.b, Colors::DARK_GRAY.a);
    SDL_RenderFillRect(renderer, &monsterAlertBox);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << survivalTime << "s";

    SDL_Rect timeBox = {10, 30, 140, 40};
    SDL_SetRenderDrawColor(renderer, Colors::DARK_GRAY.r, Colors::DARK_GRAY.g,
                          Colors::DARK_GRAY.b, Colors::DARK_GRAY.a);
    SDL_RenderFillRect(renderer, &timeBox);
}

void Game::drawText(int x, int y, const std::string& text, const SDL_Color& color) const {
    // Placeholder for text rendering
    // In a full implementation, you'd use SDL_ttf for this
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        draw();
        SDL_Delay(1000 / FPS);
    }
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    Game game;
    game.run();
    return 0;
}
