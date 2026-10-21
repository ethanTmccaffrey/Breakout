#include "PowerupBigPaddle.h"

PowerupBigPaddle::PowerupBigPaddle(sf::RenderWindow* window, Paddle* paddle, Ball* ball, float x_pos)
    : PowerupBase(window, paddle, ball, x_pos)
{
    _sprite.setFillColor(sf::Color::Yellow); // Same colour as smallPaddle
}

PowerupBigPaddle::~PowerupBigPaddle()
{
}

std::pair<POWERUPS,float> PowerupBigPaddle::applyEffect()
{
    _paddle->setStarPower(5.0f);
    return { starPower, 5.0f };
}