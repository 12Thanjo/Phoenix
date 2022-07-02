#pragma once

// strings
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>

// time
// #include <ctime>
// #include <chrono>

// misc
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>
#include <thread>

//////////////////////////////////////

// glm
#include <lib/glm/glm/glm.hpp>
#include <lib/glm/glm/gtc/matrix_transform.hpp>
#include <lib/glm/glm/gtc/type_ptr.hpp>

// core
#include "src/core/logging.h"
#include "src/core/types.h"
#include "src/core/colors.h"
#include "src/core/UUID.h"
#include "src/core/timings.h"

// utils
#include "src/utils/strings.h"
#include "src/utils/files.h"
#include "src/utils/bimap.h"

// misc
#include "src/naml/Serialize.h"
#include "src/naml/Deserialize.h"