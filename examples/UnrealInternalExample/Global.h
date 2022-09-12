#pragma once

// OPTIONS
#define CONSOLE_OUTPUT			1
#define MENU_RENDER				1

// SETTINGS
#define PROCESS_EVENT_INDEX		0x40
#define POST_RENDER_INDEX		0x5B
#define UNLOAD_KEY				VK_END

// MACROS
#define DELETE_HEAP(ptr) delete ptr; ptr = nullptr
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if CONSOLE_OUTPUT
#define LOG(format, ...) std::printf("%s:%s:%i " format "\n", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
#define LOG(format, ...)
#endif