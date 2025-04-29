#include <bitset>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#define CADERA_APP_NAMESPACE cad
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// Windows MINGW64 gcc
#ifdef _DEBUG
#define CADERA_DEBUG
#endif

// Linux gcc
#ifndef NDEBUG
#define CADERA_DEBUG
#endif

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>