#pragma once

//For use by Gengine applications
#include "Gengine/Core/Application.h"
#include "Gengine/Core/Log.h"
#include "Gengine/Core/Layer.h"

#include "Gengine/Core/Input.h"
#include "Gengine/Core/KeyCodes.h"
#include "Gengine/Core/MouseBtnCodes.h"

#include "Gengine/ImGui/ImGuiLayer.h"
#include "Gengine/Core/Time.h"
#include "Gengine/Utils/Random.h"

// ---Renderer---
#include "Gengine/Renderer/Renderer.h"
#include "Gengine/Renderer/RenderCommand.h"

#include "Gengine/Renderer/Buffer.h"
#include "Gengine/Renderer/Shader.h"
#include "Gengine/Renderer/VertexArray.h"

#include "Gengine/Renderer/Camera/OrthographicCamera.h"

#include "Gengine/Physics/CollisionSystem/Testing/Utils.h"
#include "Gengine/Utils/Grid.h"

//---StateMachine---
#include "Gengine/StateMachine/StateMachineManager.h"
//--Conditions--
#include "Gengine/StateMachine/Conditions/EventCondition.h"
#include "Gengine/StateMachine/Conditions/BooleanCondition.h"
#include "Gengine/StateMachine/Conditions/NumberCondition.h"

#include <imgui/imgui.h>
#include <imgui/implot.h>

//---Utils---
#include "Gengine/Utils/Vector.h"

#include "Defines.h"
#include "Usings.h"

//---AI---
#include "Gengine/Artificial Intelligence/Deep Learning/NeuralNetwork/NeuralNetworkTrainer.h"