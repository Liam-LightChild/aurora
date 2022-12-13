/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_GRAPHICS_H
#define AURORA_GRAPHICS_H

#include "implementation.h"

namespace aurora {

	class Graphics {
	private:
		Implementation *m_Implementation;

	public:
		explicit Graphics(Implementation *pImplementation) : m_Implementation(pImplementation) {}

		void setClearColor(float pRed, float pGreen, float pBlue, float pAlpha = 1.0f) {
			m_Implementation->setClearColor(pRed, pGreen, pBlue, pAlpha);
		}

		void clear(ClearOptions pOptions = ClearOptions()) {
			m_Implementation->performClear(pOptions);
		}

		void clear(float pRed, float pGreen, float pBlue, ClearOptions pOptions = ClearOptions()) {
			setClearColor(pRed, pGreen, pBlue, 1);
			clear(pOptions);
		}

		void clear(float pRed, float pGreen, float pBlue, float pAlpha, ClearOptions pOptions = ClearOptions()) {
			setClearColor(pRed, pGreen, pBlue, pAlpha);
			clear(pOptions);
		}
	};

}// namespace aurora

#endif// AURORA_GRAPHICS_H
