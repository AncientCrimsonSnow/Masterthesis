#include "ggpch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Gengine {
	OrthographicCamera::OrthographicCamera(
		float left, 
		float right, 
		float bottom, 
		float top) : 
		m_ProjectionMatrix(
			glm::ortho(
				left, 
				right, 
				bottom, 
				top,
				-1.0f,
				1.0f)),
		m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	OrthographicCamera::OrthographicCamera(
		glm::vec2 position,
		glm::vec2 dimensions)
		: m_ProjectionMatrix(
			glm::ortho(
				position.x - dimensions.x / 2.f,
				position.x + dimensions.x / 2.f,
				position.y - dimensions.y / 2.f,
				position.y + dimensions.y / 2.f,
				-1.0f,
				1.0f)),
		m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix() {
		glm::mat4 transform = glm::translate(
			glm::mat4(1.0f), m_Position) 
			* glm::rotate(
				glm::mat4(1.0f), 
				glm::radians(m_Rotation), 
				glm::vec3(0,0,1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
