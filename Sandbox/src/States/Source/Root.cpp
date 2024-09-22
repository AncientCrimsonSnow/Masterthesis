#include "States/Header/Root.h"

Root::Root()
{ }

void Root::Init()
{
	var main = std::make_shared<Main>(shared_from_this());
	var training = std::make_shared<Training>(shared_from_this());
	var trainingIdle = std::make_shared<TrainingIdle>(training);
	var trainingProcess = std::make_shared<TrainingProcess>(training);
	var trainingProcessCancel = std::make_shared<TrainingProcessCancel>(training);
	var trainingSave = std::make_shared<TrainingSave>(training);
	var evaluation = std::make_shared<Evaluation>(shared_from_this());
	var evaluationIdle = std::make_shared<EvaluationIdle>(evaluation);
	var evaluationProcessCancel = std::make_shared<EvaluationProcessCancel>(evaluation);
	var evaluationIdleIdle = std::make_shared<EvaluationIdleIdle>(evaluationIdle);
	var evaluationIdleGraph = std::make_shared<EvaluationIdleGraph>(evaluationIdle);
	var evaluationProcess = std::make_shared<EvaluationProcess>(evaluation);

	m_StateMachineManager->AddStates({
		main,
		training,
		trainingIdle,
		trainingProcess,
		trainingProcessCancel,
		trainingSave,
		evaluation,
		evaluationIdle,
		evaluationProcessCancel,
		evaluationIdleIdle,
		evaluationIdleGraph,
		evaluationProcess });
}

void Root::Update(Gengine::Timestep deltaTime)
{}

void Root::ImGuiRender()
{}
