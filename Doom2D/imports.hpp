#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <filesystem>
#include <iostream>
#include <memory>
#include <random>
#include <regex>
#include <fstream>

namespace file = std::filesystem;

enum player_animation_state { IDLE = 0, LEFT, RIGHT };

bool generateRandomBool();
float generateRandomFloat(const float& min,const float& max);