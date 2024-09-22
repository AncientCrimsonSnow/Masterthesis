#include "ggpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGlShader.h"

namespace Gengine {
	Shader* Shader::Create(const string& vertexSrc, const string& frameSrc){
	switch (Renderer::GetAPI()) {
		case RendererApi::Api::None:		GG_CORE_ASSERT(false, "RenderAPI::NONE is currently not supported"); return nullptr;
		case RendererApi::Api::OpenGL:	return new OpenGlShader(vertexSrc, frameSrc);
		default:
			GG_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
	}
}