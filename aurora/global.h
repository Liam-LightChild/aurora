//
// Created by liam on 12/2/22.
//

#ifndef AURORA_GLOBAL_H
#define AURORA_GLOBAL_H

#include "instance.h"

namespace aurora {
	extern Instance *global;

	void init();
	void terminate();
}

#endif// AURORA_GLOBAL_H
