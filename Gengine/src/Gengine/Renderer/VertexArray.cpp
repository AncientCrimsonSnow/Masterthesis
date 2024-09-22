#include "ggpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGlVertexArray.h"

namespace Gengine {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererApi::Api::None:		GG_CORE_ASSERT(false, "RenderAPI::NONE is currently not supported"); return nullptr;
		case RendererApi::Api::OpenGL:	return new OpenGlVertexArray();
		default:
			GG_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
	}
}