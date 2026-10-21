#include "PowerupBase.h"
#include <cmath>
#include <iostream>

PowerupBase::PowerupBase(sf::RenderWindow* window, Paddle* paddle, Ball* ball, float spawnX)
    : _time(0.0f) // Initialize the time variable
{
    _window = window;
    _paddle = paddle;
    _sprite.setRadius(RADIUS);
    _isAlive = true;
    _ball = ball;

    // Initial position and direction with some variability
    float initialX = spawnX;
    _sprite.setPosition(initialX, 5);
    _direction = { 0.0f, FLOAT_DOWN_SPEED };

}

PowerupBase::~PowerupBase()
{
}

void PowerupBase::update(float dt)
{
    _time += dt; // Increment time

    // Smooth side-to-side movement using a sine wave
    float sineWave = std::sin(_time * 2.0f); // Adjust the frequency as needed
    _direction.x = sineWave * AMPLITUDE_OF_FALL; // Adjust the amplitude as needed

    // Move the power-up
    _sprite.move(_direction.x * dt, _direction.y * dt);

    //// Smooth color changes
    //for (int i = 0; i < _colours.size(); ++i)
    //{
    //    _colours[i] += 50 * dt; // Increment smoothly over time
    //    if (_colours[i] > 255) _colours[i] -= 255; // Wrap around to stay within valid range
    //}
    //_sprite.setFillColor(sf::Color(static_cast<sf::Uint8>(_colours[0]), static_cast<sf::Uint8>(_colours[1]), static_cast<sf::Uint8>(_colours[2]), 255));

    // Collide with floor (i.e., was missed)
    if (_sprite.getPosition().y + RADIUS * 2 >= _window->getSize().y)
    {
        _isAlive = false;
    }

}

void PowerupBase::render()
{
    _window->draw(_sprite);
}

bool PowerupBase::checkCollisionWithPaddle()
{
    if (_paddle->checkCollision(_sprite.getGlobalBounds()))
    {
        _isAlive = false;
        return true;
    }
    return false;
}

void PowerupBase::setAlive(bool alive)
{
    _isAlive = alive;
}

bool PowerupBase::isAlive()
{
    return _isAlive;
}