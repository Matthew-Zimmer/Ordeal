#include "Ordeal.hpp"

namespace Tests
{
	using namespace Ordeal;
	using namespace std::literals;

	class My_Test : public Unit_Test<My_Test>
	{
	public:
        My_Test() : Unit_Test{ "My_Test" } {}
		
		auto Run(Test<0>)
		{
			return Status{ "pass", 
				My_Value{ 1 + 2 } == Correct_Value{ 3 } };
		}
	};
}