#pragma once

#include "IInputConverter.h"

namespace Gengine
{
	class NoneConverter : public IInputConverter<Vector<double>>
	{
	public:
		Vector<double> Convert(const Vector<double>& input) const override;

		/*
		void Example(std::vector<int> value);

		void Example(std::vector<int>& value);
		void Example(std::vector<int>* value);
		void Example(std::unique_ptr<std::vector<int>> value);
		void Example(std::unique_ptr<std::vector<int>>& value);
		void Example(std::unique_ptr<std::vector<int>>* value);
		void Example(std::shared_ptr<std::vector<int>> value);
		void Example(std::shared_ptr<std::vector<int>>& value);
		void Example(std::shared_ptr<std::vector<int>>* value);
		void Example(std::unique_ptr<const std::vector<int>> value);
		void Example(std::unique_ptr<const std::vector<int>>& value);
		void Example(std::unique_ptr<const std::vector<int>>* value);
		void Example(std::shared_ptr<const std::vector<int>> value);
		void Example(std::shared_ptr<const std::vector<int>>& value);
		void Example(std::shared_ptr<const std::vector<int>>* value);
		void Example(const std::vector<int> value);
		void Example(const std::vector<int>&  value);
		void Example(const std::vector<int>* value);
		void Example(const std::unique_ptr<std::vector<int>> value);
		void Example(const std::unique_ptr<std::vector<int>>& value);
		void Example(const std::unique_ptr<std::vector<int>>* value);
		void Example(const std::shared_ptr<std::vector<int>> value);
		void Example(const std::shared_ptr<std::vector<int>>& value);
		void Example(const std::shared_ptr<std::vector<int>>* value);
		void Example(const std::unique_ptr<const std::vector<int>> value);
		void Example(const std::unique_ptr<const std::vector<int>>& value);
		void Example(const std::unique_ptr<const std::vector<int>>* value);
		void Example(const std::shared_ptr<const std::vector<int>> value);
		void Example(const std::shared_ptr<const std::vector<int>>& value);
		void Example(const std::shared_ptr<const std::vector<int>>* value);

		//READONLY NO COPY
		void Example(const std::vector<int>& value);

		//Keine Kopie, Zeige wird gemoved, Speicher dort, wo Methode gerufen wird.
		Vector<double> Example(std::vector<int> value){
			std::vector<double> temp = {1, 2, 3};  // Neuer Vektor wird erstellt
			return std::move(temp);
		}
		*/
	};
}