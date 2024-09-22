#pragma once

#include <unordered_map>
#include <cmath>
#include <utility>
#include <functional>
#include <glm/glm.hpp>
#include "Gengine/Utils/Vector.h"

namespace Gengine 
{
    template<typename TCell, typename TInsert>
    class GridBase
    {
    public:
        using CellKey = std::pair<int, int>;
        
        struct CellKeyHash
        {
            std::size_t operator()(const CellKey& key) const
            {
                return std::hash<int>()(key.first) ^ (std::hash<int>()(key.second) << 1);
            }
        };

        using CellsType = std::unordered_map<CellKey, TCell, CellKeyHash>;

        explicit GridBase(glm::vec2 cellSize) :
            m_cellSize(cellSize)
        {}

        virtual ~GridBase() = default;

        virtual void Insert(const TInsert& value, float x, float y) = 0;
        virtual bool TryGetCell(const CellKey& cellKey, TCell& outCell) const = 0;
        virtual const CellsType& GetCells() const = 0;

        Vector<CellKey> GetNeighborKeys(const CellKey& cellKey) const
        {
            Vector<CellKey> neighbors;
            for (int dx = -1; dx <= 1; ++dx)
            {
                for (int dy = -1; dy <= 1; ++dy)
                {
                    if (dx != 0 || dy != 0)
                    {
                        neighbors.emplace_back(cellKey.first + dx, cellKey.second + dy);
                    }
                }
            }
            return neighbors;
        }

        Vector<CellKey> GetNeighborKeysSingular(const CellKey& cellKey) const
        {
            Vector<CellKey> neighbors;
            for (int dx = 0; dx <= 1; ++dx)
                for (int dy = 0; dy <= 1; ++dy)
                    if (dx != 0 || dy != 0)
                        neighbors.emplace_back(cellKey.first + dx, cellKey.second + dy);
            neighbors.emplace_back(cellKey.first + 1, cellKey.second - 1);
            return neighbors;
        }

        void Clear()
        {
            m_cells.clear();
        }

        glm::vec2 GetCellSize() const
        {
            return m_cellSize;
        }

        class Iterator
        {
        public:
            using MapIterator = typename std::unordered_map<CellKey, TCell, CellKeyHash>::iterator;

            Iterator(MapIterator it) : m_it(it) {}

            Iterator& operator++()
            {
                ++m_it;
                return *this;
            }

            bool operator!=(const Iterator& other) const
            {
                return m_it != other.m_it;
            }

            TCell& operator*() const
            {
                return m_it->second;
            }

        private:
            MapIterator m_it;
        };

        class ConstIterator
        {
        public:
            using MapIterator = typename std::unordered_map<CellKey, TCell, CellKeyHash>::const_iterator;

            ConstIterator(MapIterator it) : m_it(it) {}

            ConstIterator& operator++()
            {
                ++m_it;
                return *this;
            }

            bool operator!=(const ConstIterator& other) const
            {
                return m_it != other.m_it;
            }

            const TCell& operator*() const
            {
                return m_it->second;
            }

        private:
            MapIterator m_it;
        };

        Iterator begin()
        {
            return Iterator(m_cells.begin());
        }

        Iterator end()
        {
            return Iterator(m_cells.end());
        }

        ConstIterator begin() const
        {
            return ConstIterator(m_cells.begin());
        }

        ConstIterator end() const
        {
            return ConstIterator(m_cells.end());
        }

    protected:
        glm::vec2 m_cellSize;
        std::unordered_map<CellKey, TCell, CellKeyHash> m_cells;

        CellKey GetCellKey(float x, float y) const
        {
            int cellX = static_cast<int>(std::floor(x / m_cellSize.x));
            int cellY = static_cast<int>(std::floor(y / m_cellSize.y));
            return { cellX, cellY };
        }
    };


    template<typename T>
    class ArrayGrid : public GridBase<Vector<T>, T>
    {
    public:
        using Base = GridBase<Vector<T>, T>;
        using Cell = Vector<T>;

        explicit ArrayGrid(glm::vec2 cellSize) :
            Base(cellSize)
        {}

        void Insert(const T& item, float x, float y) override
        {
            auto cellKey = this->GetCellKey(x, y);
            this->m_cells[cellKey].push_back(item);
        }

        bool TryGetCell(const typename Base::CellKey& cellKey, Cell& outCell) const override
        {
            const auto it = this->m_cells.find(cellKey);

            if (it != this->m_cells.end())
            {
                outCell = it->second;
                return true;
            }
            return false;
        }

        const typename Base::CellsType& GetCells() const override
        {
            return this->m_cells;
        }
    };

    template<typename T>
    class Grid : public GridBase<T, T>
    {
    public:
        using Base = GridBase<T, T>;

        Grid() : Base(glm::vec2(1.0f, 1.0f)) {}
        explicit Grid(glm::vec2 cellSize) :
            Base(cellSize)
        {}

        void Insert(const T& value, float x, float y) override
        {
            auto cellKey = this->GetCellKey(x, y);
            this->m_cells[cellKey] = value;
        }

        bool TryGetCell(const typename Base::CellKey& cellKey, T& outCell) const override
        {
            const auto it = this->m_cells.find(cellKey);
            if (it != this->m_cells.end())
            {
                outCell = it->second;
                return true;
            }
            return false;
        }

        const std::unordered_map<typename Base::CellKey, T, typename Base::CellKeyHash>& GetCells() const override
        {
            return this->m_cells;
        }
    };
}