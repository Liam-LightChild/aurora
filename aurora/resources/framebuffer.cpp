/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#include "framebuffer.h"
#include "aurora/global.h"

namespace aurora {
	Framebuffer::Framebuffer(int pWidth, int pHeight) {
		m_Reference = global->getImpl()->createFramebuffer(pWidth, pHeight);
		global->getWindow()->addFramebuffer(this);
	}

	Framebuffer::~Framebuffer() {
		global->getWindow()->removeFramebuffer(this);
		global->getImpl()->destroyFramebuffer(m_Reference);
	}

	void Framebuffer::reinitialize(int pWidth, int pHeight) {
		global->getImpl()->reinitializeFramebuffer(m_Reference, pWidth, pHeight);
	}

	void Framebuffer::blit(Framebuffer *pTarget) {
		global->getImpl()->performBlitFramebuffer(m_Reference, pTarget->m_Reference);
	}

	Framebuffer *Framebuffer::getDefault() {
		return new Framebuffer(global->getImpl()->getDefaultFramebuffer());
	}
} // aurora