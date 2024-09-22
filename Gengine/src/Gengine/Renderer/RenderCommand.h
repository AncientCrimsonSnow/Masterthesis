#pragma once

#include "RendererApi.h"

namespace Gengine {
	class RenderCommand {
	public:
		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererApi->SetClearColor(color);
		}
		inline static void Clear() {
			s_RendererApi->Clear();
		}
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			s_RendererApi->DrawIndexed(vertexArray);
		}
	private:
		static RendererApi* s_RendererApi;
	};
}