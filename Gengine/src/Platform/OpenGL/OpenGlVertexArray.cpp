#include "ggpch.h"
#include "OpenGlVertexArray.h"

#include <glad/glad.h>

namespace Gengine {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case Gengine::ShaderDataType::None:		return GL_NONE;
		case Gengine::ShaderDataType::Float:	return GL_FLOAT;
		case Gengine::ShaderDataType::Float2:	return GL_FLOAT;
		case Gengine::ShaderDataType::Float3:	return GL_FLOAT;
		case Gengine::ShaderDataType::Float4:	return GL_FLOAT;
		case Gengine::ShaderDataType::Mat3:		return GL_FLOAT;
		case Gengine::ShaderDataType::Mat4:		return GL_FLOAT;
		case Gengine::ShaderDataType::Int:		return GL_INT;
		case Gengine::ShaderDataType::Int2:		return GL_INT;
		case Gengine::ShaderDataType::Int3:		return GL_INT;
		case Gengine::ShaderDataType::Int4:		return GL_INT;
		case Gengine::ShaderDataType::Bool:		return GL_BOOL;
		default:
			GG_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	}

	OpenGlVertexArray::OpenGlVertexArray() {
		glCreateVertexArrays(1, &m_RendererId);
	}
	OpenGlVertexArray::~OpenGlVertexArray() {
		glDeleteVertexArrays(1, &m_RendererId);
	}
	void OpenGlVertexArray::Bind() const {
		glBindVertexArray(m_RendererId);
	}
	void OpenGlVertexArray::Unbind() const {
		glBindVertexArray(0);
	}
	void OpenGlVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
		GG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size, "Vertexbuffer has no layout")

		glBindVertexArray(m_RendererId);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const var& layout = vertexBuffer->GetLayout();
		for (const var& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGlVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_RendererId);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
