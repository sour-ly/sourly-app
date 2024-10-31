#pragma once
#include <stdio.h>

namespace Log {
	void log(const char* source, const char *fmt, ...);
	void fxAssert(bool condition, const char* source, const char* fmt, ...) ;
}
