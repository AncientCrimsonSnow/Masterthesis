#pragma once
#include "RenderCommand.h"

#include "Camera/OrthographicCamera.h"
#include "Shader.h"

namespace Gengine {

	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void SubmitMesh(
			const std::shared_ptr<Shader>& shader, 
			const std::shared_ptr<VertexArray>& vertexArray, 
			const glm::mat4& transform = glm::mat4(1.0f));
		inline static RendererApi::Api GetAPI() {
			return RendererApi::GetAPI();
		}
	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}


