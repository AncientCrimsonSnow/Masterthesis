#include "GridConverter.h"
#include "Gengine.h"
#include "Config.h"

Gengine::Vector<double> GridConverter::Convert(const Gengine::ArrayGrid<Gengine::Collider>& input) const
{
    int maxCollidersInCell = 0;
    for (var& cell : input)
        if (maxCollidersInCell < cell.size())
            maxCollidersInCell = cell.size();

    Gengine::Vector<double> trainingData(Config::DefaultGridDim.x * Config::DefaultGridDim.y);

    if (maxCollidersInCell == 0)
        return trainingData;


    for (int y = 0; y < Config::DefaultGridDim.y; y++)
    {
        for (int x = 0; x < Config::DefaultGridDim.x; x++)
        {
            int index = x + y * Config::DefaultGridDim.x;

            Gengine::Vector<Gengine::Collider> cell;
            if (input.TryGetCell({ x,y }, cell))
                trainingData[index] = static_cast<double>(cell.size()) / maxCollidersInCell;
        }
    }
    return trainingData;
}
