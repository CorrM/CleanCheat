#pragma once

// SETTINGS
#define USE_LOGGER              1

// KEYWORDS
#define ABSTRACT

// MACROS
#define DELETE_HEAP(ptr) delete ptr; ptr = nullptr

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if USE_LOGGER
#include <cstdio>
#include <cstring>
#include <cstdio>
#define LOG(format, ...) std::printf("[%s:%s:%i] " format "\n", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
#define LOG(format, ...)
#endif
