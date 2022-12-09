//
// Created by liam on 12/7/22.
//

#ifndef AURORA_BUFFER_H
#define AURORA_BUFFER_H

#include "../graphics/obj_ref_base.h"
#include "../graphics/implementation.h"

namespace aurora {

	class Buffer {
	private:
		ObjRefBase *m_Reference;

	public:
		explicit Buffer(ObjRefBase *pReference);
		explicit Buffer(BufferType pType);
		virtual ~Buffer();

		virtual void update(void* pData, size_t pSize);
		virtual void update(void* pData, size_t pSize, size_t pOffset);
		virtual void retrieve(void* pData, size_t pSize, size_t pOffset);

		ObjRefBase *getReference() const {
			return m_Reference;
		}
	};

} // aurora

#endif //AURORA_BUFFER_H
