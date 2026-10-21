# Breakout

W Kavanagh & N Merchant. Summer 2024 (last maintained, Oct 26)

## Controls

A/D to move the paddle right and left.
P to pause.

## Powerups

big/small paddle (blue)
fast/slow ball (fancy yellow)
fire ball (green)

# Tasklist

You are to spend between three and eight hours improving this simple Breakout implementation in a way to best demonstrate your capabilities as a games programmer. 

## Suggested tasks

* Implement mouse input for pad
* Improved VFX (ball trail, paddle hit response, brick particle destruction)
* Better UI (progress bar rather than timer for countdown).
* Add an actual game loop
* Better ball physics (Box2D)
* Leaderboards
* More ball types (e.g., multiball, sticky ball, brick-tracking ball)
    * Note: smallPaddle is too simple to be impressive.
* Sounds with increasing tone between bounces.
* Implement commentary with calls to an LLM such as Ollama

# Time Details and Changelist
_TO DO_

<Add information to this section about the time you've taken for this task along with a professional changelist.>

# General Bug fixes - 40Mins

> (PowerupFireBall) - destructor had "new int()" and never deletes it, this creates a memory leak everytime a fireball is destroyed.
> (PowerupManager::Update) - calls "checkCollision()" inside the loop over powerups, it should be called once before the loop.
> (UI::Render) loops with "for (sf::CircleShape life : _lives), which copies every shape every frame. It should be "const sf::CircleShape&"
> (GameManager) use "new" for everything, but never deletes any of them. added the deletes.
> (PowerupBigPaddle.h) circular include. replaced "#include "powerupManager.h"" with "#include "powerupBase.h"".


# Paddle to Snake changes - 3hrs 20mins
Paddle.h

> std::deque of gird cells. Each step pushes a new head onto the front and pops the tail off the back. Both of these are O(1) on a deque. With a std::vector, inserting at the front would shfit every element each step.
> Grid coordinates (vector2i) instwad of pixels. Self collision becomes exact == check between ints rather than comparing float rectangles.
> One RectangleShape reused for drawing. Its positon is moved and its drawn once per segment meaning a seperate shape isnt drawn for each segment.
> _pendingGrowth counter. grow() can be called at any time, even when the snake is still.
> checkCollision replaces getBounds(). A snake isnt one rectangle any more so anything that collides with it asks the paddle directly.
> moveLeft/moveRight replaced by setInput. Game manager reads the keyboard, which keeps the existing pattern paddle just recieves a direction.

Paddle.cpp

> The grid is worked out from the window size and adapts to the window change.
> Ressting _stepTimer to STEP_TIME when no key is held. Without this, tapping a key would wait up to 0.08s before moving which could be confused with lag.
> The reverse check, if you're heading right and hold a, the new head would land on your second segment causing a game over. To turn around you must go up or down.
> The while loop, if a frame takes longer than one step (lag spike), the snake catches up by taking several steps. That keeps the speed independent of frame rate.
> Wrapping. (x + _cols) % _cols turns -1 into _cols - 1 and _cols back into 0. The + _cols is needed because in c++, % of a negative number givess a negative result/
> Skipping the tail in the self-collision check. On a normal step, the tail leaves its cell at the same moment the head moves so chaing your own tail in a tight loop is legal.
> Drawing and collision both use getSegmentBounds, so what you see is exactly what the game collides with.
> to skip reversing a key, gamemanger needs to know which way the snake last moved.

Ball.cpp

> -std::abs instead of *= -1. The snake can now be anywhere in the band and it can move into the ball. flipping the direction could send the ball downward if it was already heading up. 
> Nudging the angle instead of setting it, a segement is now only 25px compared to the preivious 150px paddle meaning the old check would have made the angle change dramatically.

PowerupBase.cpp

> getBounds() no longer exists, powerups dont need to know which segment they hit, so the second parameter is left as a nullptr

GameManger.cpp

> Input stays in GameManager, matching how the code already worked, the else if chain means only one direction counts per frame.
> The game over check reuses existing game over screen and paddle already had an unused _isAlive memeber that now has a purpose.
> Detecting a new press, A key thats down but not in the list was just pressed so it goes to the back as the newest. A key that in the list but no longer down was released so it comes out.
> Reading the list backwards(rbegin). this checks the newest key first, if that key would reverse the snake it falls back to the next newest.
> keyToDirection is static, it only exists in this file and doesnt touch any GameManger data, so it doesnt need to be a member function.

BrickManager.cpp

> This is the one place a brick is destroyed and brick manager already holds a gamemanger pointer. it also works durign fireball mode.

Ball

> holding A or D while the ball hits the snake pushed the ball in that direction. safety net created to ensure the ball is between 0.25 and 1 in either direction

# Rainbow Star Snake changes - 1hr 30 mins

Paddle.h

> Star power is just a timer, while above 0 the snake uses a shorter step time and draws differently. 60% faster at 0.05s

Paddle.cpp

> The timer ticks before the "no input" early return. Otherwise star power would pause whenever you stopped moving and you could make it last forever by standing still.
> stepTime is worked out once per frame. The speed is decided in one place and the rest of the function uses that value.
> using HSV instead of RGB cycling the hue while keeping satuation and brightness full gives a clean rainbow.
> _starTimer * 720. This cycles the full rainbow twice a second.
> + i * 30. Each segment is 30 degress further round the colour wheel than the one in front of it, to create a ripple across the boddy.

PowerUpBigPaddle.h

> The falling power up was the same colour as the paddle changed to be yellow.

CONSTANT.h

> renamed bigPaddle to starPower.

PowerUpManager

> Two versions of the spawnPowerUp function, one calls the other with a random number between 1 and 5, the other allows for a specific value to be called

# 5B restart button - 8 mins

> total time spent = 5hrs 38mins
---

### Change log
* 2025 - Naman made it harder
* 2026 - William made it easier again
* 2026 - Ethan made it be snake *wow* 

---

Curse you, fast ball!