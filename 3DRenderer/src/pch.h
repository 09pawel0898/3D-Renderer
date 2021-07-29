#pragma once

#define DEBUG

#ifdef DEBUG
#define DBGMSG(x) std::cerr << x << "\n"
#else
#define DBGMSG(x)

#endif
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <assert.h>
#include <sstream>
#include <fstream>
#include <unordered_set>

typedef sf::Vector2i vec2i;
typedef sf::Vector2f vec2f;
typedef sf::Vector2u vec2u;
