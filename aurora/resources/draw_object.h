//
// Created by liam on 12/7/22.
//

#ifndef AURORA_DRAW_OBJECT_H
#define AURORA_DRAW_OBJECT_H

#include "../graphics/obj_ref_base.h"
#include "../graphics/implementation.h"

namespace aurora {

	class DrawObject {
	private:
		ObjRefBase *m_Reference;

	public:
		explicit DrawObject(ObjRefBase *pReference);
		explicit DrawObject(const DrawObjectOptions &pOptions);
		virtual ~DrawObject();

		void draw();
	};

} // aurora

#endif //AURORA_DRAW_OBJECT_H
