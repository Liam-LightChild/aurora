/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "implementation_finder.h"
#include <algorithm>
#include <stdexcept>

namespace aurora {
	void ImplementationFinder::registerImpl(Implementation::Node *pNode) {
		m_Nodes.emplace_back(pNode);
	}

	Implementation *ImplementationFinder::construct() {
		std::stable_sort(m_Nodes.begin(), m_Nodes.end(), [](auto pItem1, auto pItem2) {
			return pItem1->getPriority() > pItem2->getPriority();
		});

		for(const auto &item: m_Nodes) {
			if(item->isValid()) { return item->create(); }
		}

		throw std::runtime_error("no valid impls");
	}
}// namespace aurora