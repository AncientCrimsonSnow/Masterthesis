#include "ggpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGlRendererApi.h"

namespace Gengine {
	RendererApi* RenderCommand::s_RendererApi = new OpenGlRendererApi;
}