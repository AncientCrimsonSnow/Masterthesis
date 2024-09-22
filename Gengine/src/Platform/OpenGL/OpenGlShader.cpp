#include "ggpch.h"
#include "OpenGlShader.h"
#include <glad/glad.h>
#include "Gengine/Utils/Vector.h"


#include <glm/gtc/type_ptr.hpp>

namespace Gengine {
	Gengine::OpenGlShader::OpenGlShader(const string& vertexSrc, const string& frameSrc) {

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			Vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			GG_CORE_ERROR("{0}", infoLog.data());
			GG_CORE_ASSERT(false, "Vertex Shader compilation failiure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = frameSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			Vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			GG_CORE_ERROR("{0}", infoLog.data());
			GG_CORE_ASSERT(false, "Fragment Shader compilation failiure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererId, vertexShader);
		glAttachShader(m_RendererId, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			Vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererId);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			GG_CORE_ERROR("{0}", infoLog.data());
			GG_CORE_ASSERT(false, "Shader link failiure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererId, vertexShader);
		glDetachShader(m_RendererId, fragmentShader);
	}

	OpenGlShader::~OpenGlShader() {
		glDeleteProgram(m_RendererId);
	}

	void Gengine::OpenGlShader::Bind() const {
		glUseProgram(m_RendererId);
	}

	void Gengine::OpenGlShader::Unbind() const {
		glUseProgram(0);
	}
	void OpenGlShader::UploadUniformInt(const string& name, int value) {
		GLint loaction = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1i(loaction, value);
	}

	void OpenGlShader::UploadUniformFloat1(const string& name, float value) {
		GLint loaction = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1f(loaction, value);
	}
	void OpenGlShader::UploadUniformFloat2(const string& name, const glm::vec2& values) {
		GLint loaction = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform2f(loaction, values.x, values.y);
	}
	void OpenGlShader::UploadUniformFloat3(const string& name, const glm::vec3& values) {
		GLint loaction = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform3f(loaction, values.x, values.y, values.z);
	}
	void OpenGlShader::UploadUniformFloat4(const string& name, const glm::vec4& values) {
		GLint loaction = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform4f(loaction, values.x, values.y, values.z, values.w);
	}

	void OpenGlShader::UploadUniformMat3(const string& name, const glm::mat3& matrix) {
		GLint loaction = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix3fv(loaction, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGlShader::UploadUniformMat4(const string& name, const glm::mat4& matrix) {
		GLint loaction = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(loaction, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}