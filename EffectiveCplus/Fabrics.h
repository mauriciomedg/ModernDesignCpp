#pragma once
#include <map>
#include <memory>
#include <functional>
#include <iostream>

template < class AbstractProduct,
	class IdentifyerType, 
	class ProductCreator >

class Factory
{

protected:
	Factory() = default;

public:
	Factory(Factory&) = delete;
	Factory& operator =(const Factory&) = delete;
	Factory(Factory&&) = delete;
	Factory& operator=(Factory&&) = delete;

	static Factory& GetInstance()
	{
		static Factory obj;
		return obj;
	}

	bool Register(IdentifyerType id, ProductCreator creator)
	{
		return mAssociation.insert(std::make_pair(id, creator)).second; // AssocMap::value_type(id, creator)).second;
	}

	bool Unregister(IdentifyerType id)
	{
		return mAssociation.erase(id) == 1;
	}

	std::unique_ptr<AbstractProduct> CreateObject(IdentifyerType id)
	{
		auto itt = mAssociation.find(id);

		if (itt != mAssociation.end())
		{
			return (itt->second)();
		}

		return nullptr;
	}

private:
	using AssocMap = std::map<IdentifyerType, ProductCreator>;

	AssocMap mAssociation;
};


namespace
{
	

	//void UseFabric()
	//{
	//	using FactoryForShape = Factory<Shape, int, std::function<std::unique_ptr<Shape>()>>;
	//	auto obj = FactoryForShape::GetInstance().CreateObject(1);
	//
	//	obj->Draw();
	//	obj->Draw();
	//}
}
