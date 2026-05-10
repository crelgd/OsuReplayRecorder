#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#define SDL_MAIN_HANDLED
#include "SDL/SDL.h"

#include "GL/glew.h"

#include "GL/glm/glm.hpp"
#include "GL/glm/gtc/matrix_transform.hpp"
#include "GL/glm/gtc/type_ptr.hpp"