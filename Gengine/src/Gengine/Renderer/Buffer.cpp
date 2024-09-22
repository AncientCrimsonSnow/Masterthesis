#include "ggpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGlBuffer.h"

namespace Gengine {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {

		switch (Renderer::GetAPI()) {
		case RendererApi::Api::None:		GG_CORE_ASSERT(false, "RenderAPI::NONE is currently not supported"); return nullptr;
		case RendererApi::Api::OpenGL:	return new OpenGlVertexBuffer(vertices, size);
		default:
			GG_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {

		switch (Renderer::GetAPI()) {
		case RendererApi::Api::None:		GG_CORE_ASSERT(false, "RenderAPI::NONE is currently not supported"); return nullptr;
		case RendererApi::Api::OpenGL:	return new OpenGlIndexBuffer(indices, size);
		default:
			GG_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
	}
}