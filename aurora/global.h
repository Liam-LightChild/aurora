/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_GLOBAL_H
#define AURORA_GLOBAL_H

#include "instance.h"

namespace aurora {
	extern Instance *global;

	void init();
	void terminate();
}

#endif// AURORA_GLOBAL_H
