/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "draw_object.h"
#include "../global.h"

namespace aurora {
	DrawObject::DrawObject(ObjRefBase *pReference) : m_Reference(pReference) {}

	DrawObject::DrawObject(const DrawObjectOptions &pOptions) {
		m_Reference = global->getImpl()->createDrawObject(pOptions);
	}

	DrawObject::~DrawObject() {
		global->getImpl()->destroyDrawObject(m_Reference);
	}

	void DrawObject::draw(const MatrixSet &pMatrices) {
		global->getImpl()->performDraw(m_Reference, pMatrices);
	}
} // aurora