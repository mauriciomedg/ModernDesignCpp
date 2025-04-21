#include <iostream>
#include "../EffectiveCplus/SmartPointers.h"
#include "../EffectiveCplus/LoadShapes.h"

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

int main()
{
	LoadShapes managerShapes;
	managerShapes.FastLoadShape(1);
	managerShapes.saveAllShapes();

	runSmart();
	Widget w;
	//Widget ww(); // this is a function that returns a Widget and takes no parameters
	Widget www{};

	std::cout << w.a << std::endl;

	A a(2); 
	A b(5);

	b = a;






	return 0;
}