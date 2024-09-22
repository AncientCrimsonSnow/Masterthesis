#pragma once

#include "ggpch.h"
#include <glm/glm.hpp>
#include "Gengine/Renderer/Shader.h"


namespace Gengine {
	class OpenGlShader : public Shader {
	public:
		OpenGlShader(const string& vertexSrc, const string& frameSrc);
		virtual ~OpenGlShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const string& name, int values);

		void UploadUniformFloat1(const string& name, float values);
		void UploadUniformFloat2(const string& name, const glm::vec2& values);
		void UploadUniformFloat3(const string& name, const glm::vec3& values);
		void UploadUniformFloat4(const string& name, const glm::vec4& values);

		void UploadUniformMat3(const string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererId;
	};
}