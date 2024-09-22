#include <Gengine.h>
#include "Gengine/Core/EntryPoint.h"

#include "Platform/OpenGL/OpenGlShader.h"


#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "States/Header/Root.h"

#include "Gengine/StateMachine/Conditions/BooleanCondition.h"

class ExampleLayer : public Gengine::Layer 
{
public:
	ExampleLayer() : 
		Layer("Example"),
		m_StateMachine(std::make_shared<Gengine::StateMachineManager>()) 
	{
		m_StateMachine->AddState(std::make_shared<Root>());
	}

	void Update(Gengine::Timestep deltaTime) 
	{
		m_StateMachine->Update(deltaTime);
	}

	virtual void ImGuiRender() override
	{
		m_StateMachine->ImGuiRender();
 	}

	void OnEvent(Gengine::Event& event) override 
	{

	}

private:
	std::shared_ptr<Gengine::StateMachineManager> m_StateMachine;
};


class Sandbox : public Gengine::Application 
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() 
	{

	}
};

Gengine::Application* Gengine::CreateApplication() 
{
	return new Sandbox();
}