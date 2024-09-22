#pragma once

#include "Gengine.h"
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/SpacePartitioningStrategy.h"
#include "States/Header/Evaluation.h"

class NeuralNetworkPartitioningStrategy : public Gengine::SpacePartitioningStrategy
{

public:
	explicit NeuralNetworkPartitioningStrategy(
		std::unique_ptr<Gengine::NeuralNetwork> model,
		glm::vec2 gridSize);
	void Insert(const Gengine::Collider& collider) override;
	Gengine::Vector<std::pair<Gengine::Collider, Gengine::Collider>> FindCollisions() const override;
	void Clear() override;
	string ToString() override
	{
		return "NeuralNetworkPartitioningStrategy";
	}

	static Gengine::Vector<int> GridCellCountToArray(
		const Gengine::ArrayGrid<Gengine::Collider>& grid,
		int sizeX,
		int sizeY);
	static Gengine::Grid<int> NormalizedIntArrayToGrid(
		const Gengine::Vector<int>& arr,
		int sizeX,
		int sizeY,
		glm::vec2 cellSize);
	static Gengine::Vector<int> NormalizeToIntArray(
		const Gengine::Vector<double>& arr,
		int normalizedValue);
	static Gengine::Vector<double> NormalizeToDoubleArray(
		const Gengine::Vector<int>& arr,
		int normalizedValue);
	static Gengine::Vector<Gengine::Vector<Gengine::Collider>> GetColliderGroups(
		const Gengine::ArrayGrid<Gengine::Collider> colliderGrid,
		const Gengine::Grid<int> cellGroupsGrid,
		int sizeX,
		int sizeY,
		int groupCount);
	static Gengine::Vector<std::pair<int, int>> GetNeighbourGroups(const Gengine::Grid<int> cellGroupsGrid);

	static double MapValue(double value, double min, double max, double newMin, double newMax)
	{
		return (value - min) * (newMax - newMin) / (max - min) + newMin;
	}
private:
	const std::unique_ptr<Gengine::NeuralNetwork> m_Model;
	const std::unique_ptr<Gengine::ArrayGrid<Gengine::Collider>> m_InputGrid;
	int m_ColliderCount = 0;
};

