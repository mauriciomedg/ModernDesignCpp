#include <iostream>
#include "../EffectiveCplus/SmartPointers.h"
#include "../EffectiveCplus/LoadShapes.h"
#include <set>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <mutex>

class A
{
public:

	A(int ParamIn)
		: Param(ParamIn)
	{
		std::cout << "Call default" << std::endl;
	}

	A(const A& OBJ)
		: Param(OBJ.Param)
	{
		std::cout << "Call copy" << std::endl;
	}

	A& operator =(const A& OBJ)
	{
		Param = OBJ.Param;
		std::cout << "Call asign" << std::endl;

		return *this;
	}

	int Param;
};

struct Widget
{
	Widget()
		: a(1)
	{
		std::cout << "default ctor" << std::endl;
	}

	int a;
};

void removePreserveOrder(std::vector<int>& intVector)
{
	std::unordered_set<int> removedElemsMap;
	std::vector<int> result;

	for (int element : intVector)
	{
		if (removedElemsMap.insert(element).second)
		{
			result.push_back(element);
		}
	}
	intVector = std::move(result);

}

namespace{

	template<
		class IdentifyerType,
		class Prototype = std::function<void()>
	>
	class CallBackContainer
	{

	public:

		void Insert(Prototype prototype, IdentifyerType indentifyer)
		{
			std::lock_guard<std::mutex> g(mMutex);

			mCallables[indentifyer] = prototype;
		}

		void Invoke(IdentifyerType indentifyer)
		{
			Prototype func = nullptr;

			{
				std::lock_guard<std::mutex> g(mMutex);

				auto itt = mCallables.find(indentifyer);

				if (itt != mCallables.end())
				{
					func = itt->second;
				}
			}
			
			if (func)
				func();
		}

		Prototype operator [](IdentifyerType indentifyer) const
		{
			auto itt = mCallables.find(indentifyer);
			return (itt != mCallables.end()) ? itt->second : nullptr;
		}
	private:

		std::unordered_map<IdentifyerType, Prototype> mCallables;
		std::mutex mMutex;
	};
}

//Small Buffer Optimization (SBO)
struct FasterStorage
{
	static const int StorageSize = 64;
	template<typename T>
	void TestDecay(T&& callable)
	{
		static_assert(sizeof(T) <= StorageSize, "Callable too large for inline storage");
		//static_assert(std::is_invocable_r_v<void, T>, "Callable must be void()");

		using CallableT = std::decay_t<T>;

		new (&mStorage) CallableT(std::forward<T>(callable));

		(*reinterpret_cast<CallableT*>(mStorage))();

		//mStorage();
	}

	alignas(std::max_align_t) char mStorage[StorageSize];
};


int main()
{
	std::vector<int> v(1000, 100);
	std::vector<int> w(1000, 100);
	
	auto&& ll = [v, w]() { 

		for (int e : v)
		{
			std::cout << e << std::endl;
		}

		for (int e : w)
		{
			std::cout << e << std::endl;
		}

		std::cout << "hi";
		
		};

	int size2 = sizeof(decltype(ll));
	int size3 = sizeof(ll);

	FasterStorage storage;
	storage.TestDecay(ll);

	//TestDecay(ll);

	//decltype(auto) Callable = ll;        // Maybe: const lambda&, or lambda&&
	//CallableT = std::decay_t<Callable>; /


	CallBackContainer<int> container;
	container.Insert([]() {std::cout << "print event" << std::endl; }, 0);
	container[0]();// .Invoke(0);
	container.Invoke(0);

	LoadShapes managerShapes;
	managerShapes.FastLoadShape(1);
	managerShapes.saveAllShapes();

	runSmart();
	Widget wid;
	//Widget ww(); // this is a function that returns a Widget and takes no parameters
	Widget www{};

	std::cout << wid.a << std::endl;

	A a(2); 
	A b(5);

	b = a;






	return 0;
}