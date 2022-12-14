/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#ifndef AURORA_FRAMEBUFFER_H
#define AURORA_FRAMEBUFFER_H

#include "../graphics/obj_ref_base.h"

namespace aurora {

	class Framebuffer {
	private:
		ObjRefBase *m_Reference;

	public:
		explicit Framebuffer(ObjRefBase *pReference) : m_Reference(pReference) {}
		Framebuffer(int pWidth, int pHeight);
		virtual ~Framebuffer();

		static Framebuffer *getDefault();

		void reinitialize(int pWidth, int pHeight);
		void blit(Framebuffer *pTarget);

		[[nodiscard]] ObjRefBase *getReference() const {
			return m_Reference;
		}
	};

} // aurora

#endif //AURORA_FRAMEBUFFER_H
