#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <cassert>


using namespace Log;

void Log::log(const char* source, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	printf("[%s] ", source);
	vprintf(fmt, args);
	va_end(args);
}

void Log::fxAssert(bool condition, const char* source, const char* fmt, ...) {
	if (!condition) {
		Log::log(source, fmt);	
		assert(condition);
	}
}
