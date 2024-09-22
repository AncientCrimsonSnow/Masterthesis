#include "ggpch.h"
#include "OpenGlBuffer.h"

#include <glad/glad.h>

namespace Gengine {
	OpenGlVertexBuffer::OpenGlVertexBuffer(float* vertices, uint32_t size) {
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	OpenGlVertexBuffer::~OpenGlVertexBuffer() {
		glDeleteBuffers(1, &m_RendererId);
	}
	void OpenGlVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}
	void OpenGlVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t* indices, uint32_t count)
	: m_Count(count) {
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer() {
		glDeleteBuffers(1, &m_RendererId);
	}
	void OpenGlIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}
	void OpenGlIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}