//
// Created by liam on 12/7/22.
//

#include "draw_object.h"
#include "aurora/global.h"

namespace aurora {
	DrawObject::DrawObject(ObjRefBase *pReference) : m_Reference(pReference) {}

	DrawObject::DrawObject(const DrawObjectOptions &pOptions) {
		m_Reference = global->getImpl()->createDrawObject(pOptions);
	}

	DrawObject::~DrawObject() {
		global->getImpl()->destroyDrawObject(m_Reference);
	}

	void DrawObject::draw() {
		global->getImpl()->performDraw(m_Reference);
	}
} // aurora