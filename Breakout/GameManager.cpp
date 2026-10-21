#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>
#include <algorithm>

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);
}

GameManager::~GameManager()
{
    cleanup();
}

void GameManager::cleanup()
{
    delete _ui;
    _ui = nullptr;
    delete _powerupManager;
    _powerupManager = nullptr;
    delete _ball;
    _ball = nullptr;
    delete _messagingSystem;
    _messagingSystem = nullptr;
    delete _brickManager;
    _brickManager = nullptr;
    delete _paddle;
    _paddle = nullptr;
}

void GameManager::restart()
{
    _pause = false;
    _pauseHold = 0.f;
    _time = 0.f;
    _timeLastPowerupSpawned = 0.f;
    _lives = INITIAL_LIVES;
    _levelComplete = false;
    _powerupInEffect = { none, 0.f };
    _heldKeys.clear();
    _masterText.setString("");

    initialize();
}



void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    _ball = new Ball(_window, 400.0f, this); 
    _powerupManager = new PowerupManager(_window, _paddle, _ball);
    _ui = new UI(_window, _lives, this);

    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
}

void GameManager::update(float dt)
{
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect);
    _powerupInEffect.second -= dt;
    

    bool gameOver = _lives <= 0 || !_paddle->isAlive();
    if (gameOver || _levelComplete)
    {
        _masterText.setString(gameOver ? "Game over.\nPress R to restart." : "Level Completed.\nPress R to restart.");

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) restart();

        return;
    }
    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        return;
    }

    // timer.
    _time += dt;

#ifdef _DEBUG
    //debug: press 1 to spawn a star power up//
    bool spawnKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
    if (spawnKey && !_debugSpawnHeld) _powerupManager->spawnPowerup(0);
    _debugSpawnHeld = spawnKey;
#endif



    if (_time > _timeLastPowerupSpawned + POWERUP_FREQUENCY && rand()%700 == 0)      // TODO parameterise
    {
        _powerupManager->spawnPowerup();
        _timeLastPowerupSpawned = _time;
    }

    // steer snake: newest held key winds (only moves while a key is held) //
    _paddle->setInput(getSnakeInput());

    // update everything 
    _paddle->update(dt);
    _ball->update(dt);
    _powerupManager->update(dt);
}

//Converts a movment key to a grid direction//
static sf::Vector2i keyToDirection(sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::W: return { 0, -1 };
    case sf::Keyboard::A: return { -1, 0 };
    case sf::Keyboard::S: return { 0, 1 };
    case sf::Keyboard::D: return { 1, 0 };
    default: return { 0, 0 };
    }
}

//Tracks WASD in the order they were pressed so the newest key wins, skipping any key that would reverse the snake into itself//
sf::Vector2i GameManager::getSnakeInput()
{
    const sf::Keyboard::Key keys[] = { sf::Keyboard::W, sf::Keyboard::A, sf::Keyboard::S, sf::Keyboard::D };

    for (sf::Keyboard::Key key : keys)
    {
        bool pressed = sf::Keyboard::isKeyPressed(key);
        auto it = std::find(_heldKeys.begin(), _heldKeys.end(), key);

        if (pressed && it == _heldKeys.end()) _heldKeys.push_back(key); // just pressed, so its newest//
        else if (!pressed && it != _heldKeys.end()) _heldKeys.erase(it); // released//
    }

    //Check newest first and skip direction that would reverse the snake//
    sf::Vector2i current = _paddle->getDirection();
    for (auto it = _heldKeys.rbegin(); it != _heldKeys.rend(); ++it)
    {
        sf::Vector2i dir = keyToDirection(*it);
        if (dir != -current) return dir;
    }
    return { 0,0 };
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

    // TODO screen shake.
}

void GameManager::render()
{
    _paddle->render();
    _ball->render();
    _brickManager->render();
    _powerupManager->render();
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }
