/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_IMPLEMENTATION_FINDER_H
#define AURORA_IMPLEMENTATION_FINDER_H

#include "implementation.h"
#include <vector>

namespace aurora {

	class ImplementationFinder {
	private:
		std::vector<Implementation::Node *> m_Nodes;

	public:
		void registerImpl(Implementation::Node *pNode);
		Implementation *construct();
	};

}// namespace aurora

#endif// AURORA_IMPLEMENTATION_FINDER_H
