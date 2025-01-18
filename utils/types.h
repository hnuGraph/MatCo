#ifndef UTILS_TYPES
#define UTILS_TYPES

#include <chrono>
#include <climits>
#include <functional>
#include <stdlib.h>

#define NOT_EXIST UINT_MAX
#define UNMATCHED UINT_MAX

#define FullCoverage

#define CP2LE

typedef uint32_t VertexID;
typedef uint32_t LabelID;
#define INVALID_VERTEX_ID 100000000

#define LOG(message) std::cout << message << std::endl;


#endif //UTILS_TYPES
