#pragma once
#include <SFML/Graphics.hpp>


class GameManager;  // forward declaration



class Ball {
public:
    Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager);
    ~Ball();
    void update(float dt);
    void render();
    void setVelocity(float coeff, float duration);
    void setFireBall(float duration);

private:
    sf::CircleShape _sprite;
    sf::Vector2f _direction;
    sf::RenderWindow* _window;
    float _velocity;
    bool _isAlive;
    bool _isFireBall;
    float _timeWithPowerupEffect;

    GameManager* _gameManager;  // Reference to the GameManager


    static constexpr float RADIUS = 10.0f;
    static constexpr float MIN_HORIZONTAL = 0.25f; //stops the ball bouncing straight up//
    static constexpr float VELOCITY = 200.0f;   // for reference.
};

