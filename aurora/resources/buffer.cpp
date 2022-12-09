//
// Created by liam on 12/7/22.
//

#include "buffer.h"
#include "../global.h"

namespace aurora {
	Buffer::Buffer(ObjRefBase *pReference) : m_Reference(pReference) {}
	Buffer::Buffer(BufferType pType) : Buffer(global->getImpl()->createBuffer(pType)) {}

	Buffer::~Buffer() {
		global->getImpl()->destroyBuffer(m_Reference);
	}

	void Buffer::update(void *pData, size_t pSize) {
		global->getImpl()->updateBufferData(m_Reference, pData, pSize);
	}

	void Buffer::update(void *pData, size_t pSize, size_t pOffset) {
		global->getImpl()->updateBufferData(m_Reference, pData, pSize, pOffset);
	}

	void Buffer::retrieve(void *pData, size_t pSize, size_t pOffset) {
		global->getImpl()->retrieveBufferData(m_Reference, pData, pSize, pOffset);
	}
} // aurora