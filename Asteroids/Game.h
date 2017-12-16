#pragma once
#include <SFML/Graphics.hpp>

// Graphics sizes
const int WIDTH = 1200;
const int HEIGHT = 800;
const int SCALE = 10;
const int SMALL_TEXT = 10;
const int LARGE_TEXT = SMALL_TEXT * 2;
const int VERY_LARGE_TEXT = SMALL_TEXT * 4;
const int ASTEROID_LARGE = 21;
const int ASTEROID_MEDIUM = 14;
const int ASTEROID_SMALL = 7;
const int SAUCER_LARGE = 14;
const int SAUCER_SMALL = 7;
// Screens
const int INTRO = 0;
const int PLAY = 1;
const int HIGH = 2;
// Speeds in pixels per second
const float SHIP_ACCELERATION = 300.0f;
const float SHIP_MAX_SPEED = 500.0f;
const float SHIP_ROTATE = 240.0f;				// Degrees per second
const float BULLET_SPEED = 300.0f;
const float SMALL_SAUCER_SPEED = 100.0f;
const float LARGE_SAUCER_SPEED = 70.0f;
const float SAUCER_WAIT = 3.0f;
const float ASTEROID_MIN_SPEED = 100.0f;
const float ASTEROID_MAX_SPEED = 300.0f;
const float MAX_HEARTBEAT = 1.5f;
const float MIN_HEARTBEAT = 0.2f;
// Miscellaneous
const float DTR = 0.017453f; // Degrees to radians PI / 180
const sf::Color border = sf::Color(25, 25, 25);

