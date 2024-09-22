#pragma once
#include "ggpch.h"

namespace Gengine {
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const string& vertexSrc, const string& frameSrc);
	};
}