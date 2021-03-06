#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>

//self made stuff to have handy
#include "TimeSingleton.h"
#include "Logger.h"
//#include "ServiceLocator.h"
//#include "SDLSoundSystem.h"

//disabling all warnings that come from this math library
#pragma warning(push)
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26819 )
#pragma warning( disable : 4201 )
#include <glm/glm.hpp>
#include <SDL.h>
#pragma warning (pop)