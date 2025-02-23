#include "VertexBuffer.h"

#include "../graphics/Format.h"
#include "../graphics/GraphicsDevice.h"

namespace Atlas {

	namespace Buffer {


		VertexBuffer::VertexBuffer(VkFormat format, size_t elementCount, void* data)
            : format(format), elementSize(Graphics::GetFormatSize(format)) {

            if (elementCount) {
                SetSize(elementCount, data);
            }

		}

        void VertexBuffer::SetSize(size_t elementCount, void *data) {

            // If the element count is the same we can reuse the old buffer
            if (this->elementCount == elementCount) {
                if (!data)
                    return;
                SetData(data, 0, elementCount);
                return;
            }

            this->elementCount = elementCount;

            Reallocate(data);

        }

		void VertexBuffer::SetData(void *data, size_t offset, size_t length) {

            buffer->SetData(data, offset * elementSize, length * elementSize);

		}

        void VertexBuffer::Reallocate(void *data) {

            auto device = Graphics::GraphicsDevice::DefaultDevice;

            auto sizeInBytes = elementCount * elementSize;

            Graphics::BufferDesc desc {
                .usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                .domain = Graphics::BufferDomain::Device,
                .data = data,
                .size = sizeInBytes
            };
            buffer = device->CreateBuffer(desc);

        }

	}

}