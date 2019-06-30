#include "Ordeal.hpp"

int Add(int a, int b)
{
	return a + b;
}

int Sub(int a, int b)
{
	return a - b;
}

namespace Tests
{
	using namespace Ordeal;
	using namespace std::literals;

	class Add : public Unit_Test<Add>
	{
	public:
        Add() : Unit_Test{ "Add" } {}
		
		auto Run(Test<0>)
		{
			return Status{ "postives", 
				My_Value{ ::Add(1, 2) } == Correct_Value{ 3 } };
		}

		auto Run(Test<1>)
		{
			return Status{ "zeros", 
				My_Value{ ::Add(1, 2) } == Correct_Value{ 2 } };
		}
	};
}