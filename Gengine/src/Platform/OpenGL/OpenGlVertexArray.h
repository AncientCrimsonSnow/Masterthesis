#pragma once

#include "Gengine/Renderer/VertexArray.h"

namespace Gengine {
	class OpenGlVertexArray : public VertexArray {
	public:
		OpenGlVertexArray();
		virtual ~OpenGlVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const Vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers;  }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererId;
		Vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}


