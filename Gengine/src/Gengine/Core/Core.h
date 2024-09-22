#pragma once

#ifdef GG_PLATFORM_WINDOWS
#else
	#error Gengine only support Windows
#endif

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef GG_ENABLE_ASSERTS
	#define GG_ASSERT(x, ...) {if(!x)) {GG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GG_CORE_ASSERT(x, ...) {if(!x)) {GG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GG_ASSERT(x, ...)
	#define GG_CORE_ASSERT(x, ...)
#endif
