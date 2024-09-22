#include "States/Header/EvaluationIdleGraph.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGlShader.h"
#include "Config.h"

EvaluationIdleGraph::EvaluationIdleGraph(
	std::shared_ptr<Gengine::State> parent) :
	Gengine::State(parent),
	m_Camera(Config::CameraPos, Config::CameraDim) {}

void EvaluationIdleGraph::Init() 
{
	m_StateMachineManager->TryGetState(m_EvaluationState);
}

void EvaluationIdleGraph::Enter() 
{
	GG_INFO("Enter {0}-State", ToString());
	InitGraphics();
}

void EvaluationIdleGraph::InitGraphics()
{
	m_VertexArray.reset(Gengine::VertexArray::Create());

	float quadWidth = Config::GridSize.x / Config::DefaultGridDim.x;
	float quadHeight = Config::GridSize.y / Config::DefaultGridDim.y;

	float quadVertices[3 * 4] =
	{
		0.0f,        0.0f,        0.0f,
		quadWidth,   0.0f,        0.0f,
		quadWidth,   quadHeight,  0.0f,
		0.0f,        quadHeight,  0.0f
	};


	std::shared_ptr<Gengine::VertexBuffer> vertexBuffer;
	vertexBuffer.reset(Gengine::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));

	vertexBuffer->SetLayout({
		{ Gengine::ShaderDataType::Float3, "a_Position" } });

	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };

	std::shared_ptr<Gengine::IndexBuffer> indexBuffer;
	indexBuffer.reset(Gengine::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));

	m_VertexArray->SetIndexBuffer(indexBuffer);

	string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

	string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec4 u_Color;


			void main()
			{
				color = u_Color;
			}
		)";

	m_Shader.reset(Gengine::Shader::Create(vertexSrc, fragmentSrc));
}

void EvaluationIdleGraph::Update(Gengine::Timestep deltaTime) {
	Gengine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Gengine::RenderCommand::Clear();
	Gengine::Renderer::BeginScene(m_Camera);
	DrawColliderTileMap(m_EvaluationState->GetCollider(Config::ColliderSliceToRender));
	DrawAiOutputMap();
	Gengine::Renderer::EndScene();
}

void EvaluationIdleGraph::ImGuiRender() {
	ImGui::Begin("EvaluationGraph");

	int sliceToRender = Config::ColliderSliceToRender;
	ImGui::InputInt("Slice to Render", &sliceToRender);
	Config::ColliderSliceToRender = sliceToRender;

	const char* axisOptions[] = { "collider", "collisionCheckCount", "runtime", "errorValue", "collisionsCount" };
	static int xAxisSelection = 0;
	static int yAxisSelection = 2;

	ImGui::Combo("X Axis", &xAxisSelection, axisOptions, IM_ARRAYSIZE(axisOptions));
	ImGui::Combo("Y Axis", &yAxisSelection, axisOptions, IM_ARRAYSIZE(axisOptions));

	std::string xAxisName = axisOptions[xAxisSelection];
	std::string yAxisName = axisOptions[yAxisSelection];

	const var& graphData = m_EvaluationState->GetGraphData();
	DrawGraph(graphData, xAxisName, yAxisName);
	ImGui::End();
}

void EvaluationIdleGraph::DrawColliderTileMap(Gengine::Vector<Gengine::Collider> collider) const {
	if (collider.empty())
		return;

	Gengine::ArrayGrid<Gengine::Collider> grid(Config::CellSize);
	for (const var& collider : m_EvaluationState->GetCollider(collider.size()))
		grid.Insert(collider, collider.GetX(), collider.GetY());

	int max = 0;
	for (var& cell : grid)
		if (max < cell.size())
			max = cell.size();

	for (int y = 0; y < Config::DefaultGridDim.y; y++)
	{
		for (int x = 0; x < Config::DefaultGridDim.x; x++)
		{
			Gengine::Vector<Gengine::Collider> cell;
			float color;
			if (grid.TryGetCell(std::pair(x, y), cell))
				color = (float)cell.size() / (float)max;
			else
				color = 0.f;

			glm::vec3 pos(x * Config::CellSize.x + Config::GridInputPos.x, y * Config::CellSize.y + Config::GridInputPos.y, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
			std::dynamic_pointer_cast<Gengine::OpenGlShader>(m_Shader)->UploadUniformFloat4("u_Color", glm::vec4(color, color, color, 1.0f));
			Gengine::Renderer::SubmitMesh(m_Shader, m_VertexArray, transform);
		}
	}
}

void EvaluationIdleGraph::DrawAiOutputMap() const
{
	Gengine::Vector<glm::vec3> colors = GenerateColors(Config::AiGridGroupsCount);
	Gengine::Grid<int> grid = Evaluation::GetGridAt(Config::ColliderSliceToRender);

	for (int y = 0; y < Config::DefaultGridDim.y; y++)
	{
		for (int x = 0; x < Config::DefaultGridDim.x; x++)
		{
			int cell;
			glm::vec3 color;
			if (grid.TryGetCell(std::pair(x, y), cell))
				color = colors[cell];
			else
				color = glm::vec3(0.f);

			glm::vec3 pos(x * Config::CellSize.x + Config::GridOutputPos.x, y * Config::CellSize.y + Config::GridOutputPos.y, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
			std::dynamic_pointer_cast<Gengine::OpenGlShader>(m_Shader)->UploadUniformFloat4("u_Color", glm::vec4(color, 1.0f));
			Gengine::Renderer::SubmitMesh(m_Shader, m_VertexArray, transform);
		}
	}
}

Gengine::Vector<glm::vec3> EvaluationIdleGraph::GenerateColors(int numColors)
{
	Gengine::Vector<glm::vec3> colors;
	if (numColors <= 0) return colors;

	for (int i = 0; i < numColors; ++i)
	{
		float hue = static_cast<float>(i) / static_cast<float>(numColors);
		glm::vec3 rgb = HslToRgb(hue, 1.0f, 0.5f);
		colors.push_back(rgb);
	}
	return colors;
}

glm::vec3 EvaluationIdleGraph::HslToRgb(float h, float s, float l)
{
	float r, g, b;

	if (s == 0)
	{
		r = g = b = l;
	}
	else
	{
		auto hue2rgb = [](float p, float q, float t)
			{
				if (t < 0.0f) t += 1.0f;
				if (t > 1.0f) t -= 1.0f;
				if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
				if (t < 1.0f / 3.0f) return q;
				if (t < 1.0f / 2.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
				return p;
			};

		float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
		float p = 2.0f * l - q;

		r = hue2rgb(p, q, h + 1.0f / 3.0f);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1.0f / 3.0f);
	}

	return glm::vec3(r, g, b);
}

void EvaluationIdleGraph::DrawGraph(const std::unordered_map<std::string, Gengine::Vector<EvaluationResult>>& graphData, const std::string& xAxisName, const std::string& yAxisName)
{
	Gengine::Vector<Gengine::Vector<float>> allXValues;
	Gengine::Vector<Gengine::Vector<float>> allYValues;
	Gengine::Vector<const char*> strategyNames;

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();

	for (const var& [strategyName, results] : graphData)
	{
		Gengine::Vector<float> xValues;
		Gengine::Vector<float> yValues;

		for (size_t i = 0; i < results.size(); ++i)
		{
			const var& result = results[i];
			float x = result.GetValueByName(xAxisName);
			float y = result.GetValueByName(yAxisName);

			if (x < 0)
				x = i;
			if (y < 0)
				y = i;

			xValues.push_back(x);
			yValues.push_back(y);

			if (x < minX) minX = x;
			if (x > maxX) maxX = x;
			if (y < minY) minY = y;
			if (y > maxY) maxY = y;
		}

		allXValues.push_back(std::move(xValues));
		allYValues.push_back(std::move(yValues));
		strategyNames.push_back(strategyName.c_str());
	}

	if (ImPlot::BeginPlot("Comparison Plot"))
	{
		ImPlot::SetupAxesLimits(minX, maxX, minY, maxY);

		for (size_t i = 0; i < strategyNames.size(); ++i)
		{
			ImPlot::PlotLine(strategyNames[i], allXValues[i].data(), allYValues[i].data(), allXValues[i].size());
		}
		ImPlot::EndPlot();
	}
}
