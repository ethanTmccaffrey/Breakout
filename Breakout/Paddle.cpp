#include "Paddle.h"
#include <iostream>
#include <cmath>

// Converts a hue (0-360) to a fully saturated, full brightness colour//
static sf::Color hueToColour(float hue)
{
	float h = hue / 60.0f;
	float x = 1.0f - std::abs(std::fmod(h, 2.0f) - 1.0f);
	float r = 0.0f, g = 0.0f, b = 0.0f;

	if (h < 1) { r = 1, g = x; }
	else if (h < 2) { r = x, g = 1; }
	else if (h < 3) { g = 1, b = x; }
	else if (h < 4) { g = x, b = 1; }
	else if (h < 5) { r = x, b = 1; }
	else { r = 1, b = x; }

	return sf::Color(static_cast<sf::Uint8>(r * 255), static_cast<sf::Uint8>(g * 255), static_cast<sf::Uint8>(b * 255));
}

Paddle::Paddle(sf::RenderWindow* window) : _window(window), _direction(1, 0), _input(0, 0), _pendingGrowth(0), _stepTimer(STEP_TIME), _isAlive(true)
{
	sf::Vector2u windowSize = window->getSize();
	_cols = static_cast<int>(windowSize.x / CELL_SIZE);
	_rows = static_cast<int>(AREA_HEIGHT / CELL_SIZE);
	_areaTop = windowSize.y - AREA_HEIGHT;

	//Start in the middle of the play area, facing right, body trailling from the left//
	sf::Vector2i start(_cols / 2, _rows / 2);
	for (int i = 0; i < START_LENGTH; ++i) _segments.push_back(start - sf::Vector2i(i, 0));
	
	_segmentShape.setSize(sf::Vector2f(CELL_SIZE - 2.0f, CELL_SIZE - 2.0f));
}

Paddle::~Paddle()
{

}

void Paddle::setInput(sf::Vector2i input)
{
	_input = input;
}

void Paddle::update(float dt)
{
	if (!_isAlive) return;
	_starTimer -= dt;

	float stepTime = (_starTimer > 0.0f) ? STAR_STEP_TIME : STEP_TIME;

	//Only move while a key is held//
	if (_input == sf::Vector2(0, 0))
	{
		_stepTimer = STEP_TIME; //primed so the next key press moves instantly//
		return;
	}

	//cant reverse straight back into yourself//
	if (_input == -_direction && _segments.size() > 1) return;

	_stepTimer += dt;
	while (_stepTimer >= stepTime && _isAlive)
	{
		_stepTimer -= stepTime;
		step();
	}
}

void Paddle::step()
{
	_direction = _input;

	//new head postion, wrapping round the edges of the play area//
	sf::Vector2i newHead = _segments.front() + _direction;
	newHead.x = (newHead.x + _cols) % _cols;
	newHead.y = (newHead.y + _rows) % _rows;

	//The tail moves out fo the way this step unless we're growing, so moving into the tails current cell is allowed//
	bool growing = _pendingGrowth > 0;
	auto end = growing ? _segments.end() : _segments.end() - 1;
	for (auto it = _segments.begin(); it != end; ++it)
	{
		if (*it == newHead)
		{
			_isAlive = false;
			return;
		}
	}

	_segments.push_front(newHead);
	if (growing) _pendingGrowth--;
	else _segments.pop_back();
}

void Paddle::render()
{
	for (size_t i = 0; i < _segments.size(); ++i)
	{
		sf::FloatRect bounds = getSegmentBounds(_segments[i]);
		_segmentShape.setPosition(bounds.left, bounds.top);
		if (_starTimer > 0.0f)
		{
			//Rainbow that cycles over time, offset per segment so it ripples down the body//
			float hue = std::fmod(_starTimer * 720.0f + i * 30.0f, 360.0f);
			_segmentShape.setFillColor(hueToColour(hue));
		}
		else
		{
			_segmentShape.setFillColor(i == 0 ? sf::Color::White : sf::Color::Cyan); //head different to body//
		}
		_window->draw(_segmentShape);
	}
}

void Paddle::grow(int segments)
{
	_pendingGrowth += segments;
}

bool Paddle::checkCollision(const sf::FloatRect& other, sf::FloatRect* hitSegment) const
{
	for (const sf::Vector2i& cell : _segments)
	{
		sf::FloatRect bounds = getSegmentBounds(cell);
		if (bounds.intersects(other))
		{
			if (hitSegment) *hitSegment = bounds;
			return true;
		}
	}
	return false;
}

bool Paddle::isAlive() const
{
	return _isAlive;
}

sf::Vector2i Paddle::getInput() const
{
	return _input;
}

sf::Vector2i Paddle::getDirection() const
{
	return _direction;
}

void Paddle::setWidth(float coeff, float duration)
{
	//Todo: big/small paddle powerups for the snake//
}

void Paddle::setStarPower(float duration)
{
	_starTimer = duration;
}

//converts a grid cell to its on-screen rectangle (inset by 1px so segmetns have a gap)//
sf::FloatRect Paddle::getSegmentBounds(const sf::Vector2i& cell) const
{
	return sf::FloatRect(cell.x * CELL_SIZE + 1.0f, _areaTop + cell.y * CELL_SIZE + 1.0f, CELL_SIZE - 2.0f, CELL_SIZE - 2.0f);
}