#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include "CONSTANTS.h"

class Paddle
{
public:
    Paddle(sf::RenderWindow* window);
    ~Paddle();

    void setInput(sf::Vector2i input);
    void update(float dt);
    void render();

    void grow(int segments = 1);
    bool checkCollision(const sf::FloatRect& other, sf::FloatRect* hitSegment = nullptr) const;
    bool isAlive() const;
    sf::Vector2i getInput() const;
    sf::Vector2i getDirection() const;

    void setWidth(float coeff, float duration);
    void setStarPower(float duration);

private:
    void step();
    sf::FloatRect getSegmentBounds(const sf::Vector2i& cell) const;

    sf::RenderWindow* _window;
    //One shape reused to draw every segment//
    sf::RectangleShape _segmentShape;

    std::deque<sf::Vector2i> _segments; //Grid cells, font() is the head//
    sf::Vector2i _direction; //direciton of the last step//
    sf::Vector2i _input; //Direction currently held, (0,0) = none//
    int _pendingGrowth;
    float _stepTimer;
    float _starTimer;
    bool _isAlive;

    int _cols;
    int _rows;
    float _areaTop;

    static constexpr float CELL_SIZE = 25.0f;
    static constexpr float AREA_HEIGHT = 250.0f; //height of the snakes play area//
    static constexpr float STEP_TIME = 0.08f; //seconds per grid step//
    static constexpr float STAR_STEP_TIME = 0.05f; //faster steps during star power//
    static constexpr int START_LENGTH = 3;
};
