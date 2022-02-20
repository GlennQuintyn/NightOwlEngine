#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//disabling all warnings that come from this math library
#pragma warning(push)
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26819 )
#pragma warning( disable : 4201 )
#include <glm/glm.hpp>
#include <SDL.h>
#pragma warning (pop)