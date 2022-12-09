//
// Created by liam on 12/2/22.
//

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
