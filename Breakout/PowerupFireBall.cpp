#include "PowerupFireBall.h"


PowerupFireBall::PowerupFireBall(sf::RenderWindow* window, Paddle* paddle, Ball* ball, float x_pos)
    : PowerupBase(window, paddle, ball, x_pos)
{
    _sprite.setFillColor(ballEffectsColour);
}

PowerupFireBall::~PowerupFireBall()
{  
}

std::pair<POWERUPS, float> PowerupFireBall::applyEffect()
{
    _ball->setFireBall(5.0f);
    return { fireBall, 5.0f };
}