#include "Config.h"
#include "NeuralNetworkPartitioningStrategy.h"

NeuralNetworkPartitioningStrategy::NeuralNetworkPartitioningStrategy(
	std::unique_ptr<Gengine::NeuralNetwork> model,
	glm::vec2 cellSize) :
	m_Model(std::move(model)),
	m_InputGrid(std::make_unique<Gengine::ArrayGrid<Gengine::Collider>>(cellSize))
{

}

void NeuralNetworkPartitioningStrategy::Insert(const Gengine::Collider & collider)
{
	m_InputGrid->Insert(collider, collider.GetX(), collider.GetY());
	m_ColliderCount++;
}

Gengine::Vector<std::pair<Gengine::Collider, Gengine::Collider>> NeuralNetworkPartitioningStrategy::FindCollisions() const
{
	Gengine::Vector<int> cellCountArray = GridCellCountToArray(
		*m_InputGrid,
		Config::DefaultGridDim.x,
		Config::DefaultGridDim.y);

	Gengine::Vector<double> input = NormalizeToDoubleArray(
		cellCountArray,
		1);

	Gengine::Vector<double> output = m_Model->Inference(input);

	Gengine::Vector<int> intArray = NormalizeToIntArray(
		output,
		Config::AiGridGroupsCount - 1);

	Gengine::Grid<int> cellGroupsGrid = NormalizedIntArrayToGrid(
		intArray,
		Config::DefaultGridDim.x,
		Config::DefaultGridDim.y,
		Config::CellSize);

	Evaluation::SetGrid(cellGroupsGrid, m_ColliderCount);

	Gengine::Vector<Gengine::Vector<Gengine::Collider>> colliderGroups = GetColliderGroups(
		*m_InputGrid,
		cellGroupsGrid,
		Config::DefaultGridDim.x,
		Config::DefaultGridDim.y,
		Config::AiGridGroupsCount);

	Gengine::Vector<std::pair<int, int>> groupNeighbourPairs = GetNeighbourGroups(cellGroupsGrid);

	Gengine::Vector<std::pair<Gengine::Collider, Gengine::Collider>> collisions;

	for (const var& colliderGroup : colliderGroups)
	{
		for (size_t i = 0; i < colliderGroup.size(); ++i)
		{
			for (size_t j = i + 1; j < colliderGroup.size(); ++j)
			{
				IncrementCollisionCount();
				if (colliderGroup[i].CollidesWith(colliderGroup[j]))
				{
					collisions.emplace_back(colliderGroup[i], colliderGroup[j]);
				}
			}
		}
	}

	for (const var& pair : groupNeighbourPairs)
	{
		const var& colliderGroupOne = colliderGroups[pair.first];
		const var& colliderGroupTwo = colliderGroups[pair.second];

		for (const var& colliderOne : colliderGroupOne)
		{
			for (const var& colliderTwo : colliderGroupTwo)
			{
				IncrementCollisionCount();
				if (colliderOne.CollidesWith(colliderTwo))
				{
					collisions.emplace_back(colliderOne, colliderTwo);
				}
			}
		}
	}
	return collisions;
}

void NeuralNetworkPartitioningStrategy::Clear()
{
	m_InputGrid->Clear();
	m_ColliderCount = 0;
}

Gengine::Vector<int> NeuralNetworkPartitioningStrategy::GridCellCountToArray(
	const Gengine::ArrayGrid<Gengine::Collider>& grid,
	int sizeX,
	int sizeY)
{
	Gengine::Vector<int> result(sizeY * sizeX);

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			int index = x + y * sizeX;

			Gengine::Vector<Gengine::Collider> cell;
			if (grid.TryGetCell(std::pair(x, y), cell))
				result[index] = cell.size();
			else
				result[index] = 0.f;
		}
	}

	return result;
}

Gengine::Grid<int> NeuralNetworkPartitioningStrategy::NormalizedIntArrayToGrid(
	const Gengine::Vector<int>& arr, 
	int sizeX, 
	int sizeY,
	glm::vec2 cellSize)
{
	Gengine::Grid<int> result(cellSize);

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			int index = x + y * sizeX;
			result.Insert(arr[index], x * cellSize.x, y * cellSize.y);
		}
	}
	return result;
}

Gengine::Vector<int> NeuralNetworkPartitioningStrategy::NormalizeToIntArray(
	const Gengine::Vector<double>& arr,
	int normalizedValue)
{
	var [min, max] = std::minmax_element(arr.begin(), arr.end());

	Gengine::Vector<int> normalizedArr(arr.size());

	if (min == max)
		return normalizedArr;

	for (size_t i = 0; i < arr.size(); ++i)
		normalizedArr[i] = static_cast<int>(std::round(MapValue(
			arr[i], 
			*min,
			*max,
			0.0, 
			static_cast<double>(normalizedValue))));
	return normalizedArr;
}

Gengine::Vector<double> NeuralNetworkPartitioningStrategy::NormalizeToDoubleArray(
	const Gengine::Vector<int>& arr, 
	int normalizedValue)
{
	int maxValue = *std::max_element(arr.begin(), arr.end());

	Gengine::Vector<double> normalizedArr(arr.size());

	if (maxValue == 0)
		return normalizedArr;

	for (size_t i = 0; i < arr.size(); ++i)
		normalizedArr[i] = (static_cast<double>(arr[i]) * normalizedValue / maxValue);

	return normalizedArr;
}

Gengine::Vector<Gengine::Vector<Gengine::Collider>> NeuralNetworkPartitioningStrategy::GetColliderGroups(
	const Gengine::ArrayGrid<Gengine::Collider> colliderGrid, 
	const Gengine::Grid<int> cellGroupsGrid,
	int sizeX,
	int sizeY,
	int groupCount)
{
	Gengine::Vector<Gengine::Vector<Gengine::Collider>> colliderGroups(groupCount);

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			int groupId;
			cellGroupsGrid.TryGetCell({ x,y }, groupId);
			
			Gengine::Vector<Gengine::Collider> colliderInCell;
			if (colliderGrid.TryGetCell({ x,y }, colliderInCell))
				colliderGroups[groupId].Append(colliderInCell);
		}
	}
	return colliderGroups;
}

Gengine::Vector<std::pair<int, int>> NeuralNetworkPartitioningStrategy::GetNeighbourGroups(const Gengine::Grid<int> cellGroupsGrid)
{
	Gengine::Vector<std::pair<int, int>> neighbourMap;
	for (const var& [cellKey, groupId] : cellGroupsGrid.GetCells())
	{
		var neighborsKeys = cellGroupsGrid.GetNeighborKeysSingular(cellKey);
		for (const var& neighborKey : neighborsKeys)
		{
			int neighbourGroupId;
			if (cellGroupsGrid.TryGetCell(neighborKey, neighbourGroupId))
			{
				std::pair<int, int> pairOne = { groupId, neighbourGroupId };
				std::pair<int, int> pairTwo = { neighbourGroupId, groupId };
				if (!(neighbourMap.Contains(pairOne) || neighbourMap.Contains(pairTwo)))
					neighbourMap.emplace_back(pairOne);
			}
			
		}
	}
	return neighbourMap;
}


