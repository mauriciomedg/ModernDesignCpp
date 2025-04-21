#pragma once

#include "../EffectiveCplus/Fabrics.h"
#include <fstream>
#include <string>
#include <vector>

namespace
{
	enum class ShapeType
	{
		Square = 1,
		Circle = 2
	};

	class Shape
	{
	public:
		virtual void Draw() = 0;
		virtual void Save(std::ofstream& outFile) = 0;
		virtual void Read(std::ifstream& inFile) = 0;

		virtual ~Shape()
		{
			std::cout << "shape deleted\n";
		}
	};

	class Square : public Shape
	{
	public:
		virtual void Draw()
		{
			std::cout << "drawing Square with unique ptr\n";
		}

		virtual void Save(std::ofstream& outFile)
		{
			if (!outFile) {
				std::cerr << "Error opening file for writing.\n";
				return;
			}

			outFile << mX << "\n";
			outFile << mName << "\n";

			return;
		}

		virtual void Read(std::ifstream& inFile)
		{
			std::vector<std::string> params;

			std::string line;
			while (std::getline(inFile, line)) {
				params.push_back(line);
			}

			if (!params.empty())
			{
				mX = std::stoi(params[0]);
				mName = params[1];
			}

			//while (inFile) {

			//	std::cout << line << '\n'; // Print each line to the console
			//}
		}

	private:

		float mX = 10.0f;
		std::string mName{ "Square" };
	};

	struct SquareRegister
	{
		SquareRegister()
		{
			using FactoryForShape = Factory<Shape, int, std::function<std::unique_ptr<Shape>()>>;

			FactoryForShape::GetInstance().Register(static_cast<int>(ShapeType::Square), []()->std::unique_ptr<Shape> {
				return std::make_unique<Square>();
				});
		}
	};

	static SquareRegister registerSquare;

	class LoadShapes
	{

	private:

		std::unique_ptr<Shape> LoadShape(int shapeId)
		{
			std::ifstream inFile("ShapesTest.txt"); // Open the file for reading

			if (!inFile) {
				std::cerr << "Error opening file for reading.\n";
				return std::unique_ptr<Shape>(nullptr);
			}

			using FactoryForShape = Factory<Shape, int, std::function<std::unique_ptr<Shape>()>>;
			auto obj = FactoryForShape::GetInstance().CreateObject(static_cast<int>(ShapeType::Square));

			obj->Read(inFile);

			inFile.close(); // Close the file

			return obj;
		}

	public:

		void saveAllShapes()
		{
			std::ofstream outFile("ShapesTest.txt"); // Create and open the file

			for (auto shape : mCache)
			{
				shape.second->Save(outFile);
			}

			outFile.close(); // Close the file
			std::cout << "File written successfully.\n";
		}

		std::shared_ptr<Shape> FastLoadShape(int shapeId)
		{
			auto obj = mCache[shapeId];

			if (!obj)
			{
				obj = LoadShape(shapeId);
				mCache[shapeId] = obj;
			}
			
			return obj;
		}


	private:

		std::map<int, std::shared_ptr<Shape>> mCache;
	};
}