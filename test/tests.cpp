#include "ordeal.hpp"

namespace Tests
{
	using namespace Slate::Ordeal;

	class Int_Tests : public Unit_Test<Int_Tests>
	{
	public:
        Int_Tests() : Unit_Test{ "integer tests" } {}
		
		auto run(Test<0>)
		{
			return "basic integer addition"_name = Value{ 1 + 2 } == Expected_Value{ 3 };
		}
	};
}