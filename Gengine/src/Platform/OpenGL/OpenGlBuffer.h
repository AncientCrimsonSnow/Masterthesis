#pragma once

#include "Gengine/Renderer/Buffer.h"

namespace Gengine {

	class OpenGlVertexBuffer : public VertexBuffer {
	public:
		OpenGlVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGlVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override {
			return m_Layout;
		}
		virtual void SetLayout(const BufferLayout& layout) override {
			m_Layout = layout;
		}
	private:
		uint32_t m_RendererId;
		BufferLayout m_Layout;
	};

	class OpenGlIndexBuffer : public IndexBuffer {
	public:
		OpenGlIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGlIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const {
			return m_Count;
		}
	private:
		uint32_t m_RendererId;
		uint32_t m_Count;
	};
}