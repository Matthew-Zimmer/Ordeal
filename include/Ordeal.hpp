#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

namespace Ordeal
{
	template <int id>
	class Test {};

	template <typename T>
	class Correct_Value;

	class Result
	{
		bool passed;
		std::string msg;
	public:
		inline Result(bool passed, const std::string& msg = "") : passed{ passed }, msg{ msg }
		{}
		inline operator bool()
		{
			return passed;
		}
		inline std::string Why()
		{
			return msg;
		}
	};

	class Status
	{
	public:
		std::string name;
		Result result;	
		inline Status(const std::string& name, const Result& result) : name{ name }, result{result}
		{}
		inline operator std::pair<const std::string, Result>()
		{
			return std::make_pair(name, result);
		}
	};

	namespace Imp
	{
		namespace Imp
		{
			template <typename Type, typename=void>
			class Can_To_String : public std::false_type {};

			template <typename Type>
			class Can_To_String<Type, std::void_t<decltype(std::to_string(std::declval<Type>()))>> : public std::true_type {};
		
			template <typename Object, typename Type, typename = void>
			class Is_Valid_Test : public std::false_type
			{};

			template <typename Object, typename Type>
			class Is_Valid_Test<Object, Type, std::void_t<decltype(std::declval<Object>().Run(Type{}))>> : public std::true_type
			{};
		}	

		template <typename Type>
		constexpr bool Can_To_String = Imp::Can_To_String<Type>::value;
		
		template <typename Object, typename Type>
		constexpr bool Is_Valid_Test = Imp::Is_Valid_Test<Object, Type>::value;

		template <typename Type, typename = void>
		class Value
		{
		public:
			Type value;
			Value(const Type& value) : value{ value }
			{}
			operator std::string() const
			{
				return "Error: cannot convert to a string";
			}
		};

		template <typename Type>
		class Value<Type, std::enable_if_t<Can_To_String<Type> && !std::is_convertible_v<Type, std::string>>> 
		{
		public:
			Type value;
			Value(const Type& value) : value{ value }
			{}
			operator std::string() const
			{
				return std::to_string(value);
			}
		};

		template <typename Type>
		class Value<Type, std::enable_if_t<!Can_To_String<Type> && std::is_convertible_v<Type, std::string>>>
		{
		public:
			Type value;
			Value(const Type& value) : value{ value }
			{}
			operator std::string() const
			{
				return static_cast<std::string>(value);
			}
		};

		

		class Base_Unit_Test
		{
		public:
			std::string name;
			std::unordered_map<std::string, Result> test_results;
		public:
			inline Base_Unit_Test(const std::string& name) : name{ name } {}
			virtual ~Base_Unit_Test() = default;
			virtual std::unordered_map<std::string, Result>& operator()(std::size_t& count, std::vector<std::string>& failed) = 0;
		};

		inline std::vector<std::unique_ptr<Base_Unit_Test>>& All_Tests()
		{
			static std::vector<std::unique_ptr<Base_Unit_Test>> v{};
			return v;
		}
	}

	

	template <typename Type>
	class My_Value : public Imp::Value<Type>
	{
	public:
		inline My_Value(const Type& value) : Imp::Value<Type>{ value }
		{}

		template <typename T>
		friend class Correct_Value;
	private:
		template <typename Other_Type>
		friend Result operator==(const My_Value<Type>& t1, const Correct_Value<Other_Type>& t2)
		{
			return t2 == t1;
		}

		template <typename Other_Type>
		friend Result operator!=(const My_Value<Type>& t1, const Correct_Value<Other_Type>& t2)
		{
			return t2 != t1;
		}
		
		template <typename Other_Type>
		friend Result operator<(const My_Value<Type>& t1, const Correct_Value<Other_Type>& t2)
		{
			return t2 >= t1;
		}
		
		template <typename Other_Type>
		friend Result operator<=(const My_Value<Type>& t1, const Correct_Value<Other_Type>& t2)
		{
			return t2 > t1;
		}
		
		template <typename Other_Type>
		friend Result operator>(const My_Value<Type>& t1, const Correct_Value<Other_Type>& t2)
		{
			return t2 <= t1;
		}
		
		template <typename Other_Type>
		friend Result operator>=(const My_Value<Type>& t1, const Correct_Value<Other_Type>& t2)
		{
			return t2 < t1;
		}
	};

	template <typename Type>
	class Correct_Value : public Imp::Value<Type>
	{
	public:
		inline Correct_Value(const Type& value) : Imp::Value<Type>{ value }
		{}

		template <typename T>
		friend class My_Value;
	private:
		template <typename Other_Type>
		friend Result operator==(const Correct_Value<Type>& t1, const My_Value<Other_Type>& t2)
		{
			if (t1.value == t2.value)
				return true;
			std::string s1 = t1, s2 = t2;
			return Result{ false, "values are different\ncorrect value: " + s1 + "\nyour value: " + s2 };
		}

		template <typename Other_Type>
		friend Result operator!=(const Correct_Value<Type>& t1, const My_Value<Other_Type>& t2)
		{
			if (t1.value != t2.value)
				return true;
			return Result{ false, "values are the same but the should be different\nvalue: " + static_cast<std::string>(t1) };
		}

		template <typename Other_Type>
		friend Result operator<(const Correct_Value<Type>& t1, const My_Value<Other_Type>& t2)
		{
			if (t1.value < t2.value)
				return true;
			return Result{ false, "your value is less than or equal to the correct value:\ncorrect value: " + static_cast<std::string>(t1) + "\nyour value: " + static_cast<std::string>(t2) };
		}

		template <typename Other_Type>
		friend Result operator<=(const Correct_Value<Type>& t1, const My_Value<Other_Type>& t2)
		{
			if (t1.value <= t2.value)
				return true;
			return Result{ false, "your value is less than the correct value:\ncorrect value: " + static_cast<std::string>(t1) + "\nyour value: " + static_cast<std::string>(t2) };
		}

		template <typename Other_Type>
		friend Result operator>(const Correct_Value<Type>& t1, const My_Value<Other_Type>& t2)
		{
			if (t1.value > t2.value)
				return true;
			return Result{ false, "your value is greater than or equal to the correct value:\ncorrect value: " + static_cast<std::string>(t1) + "\nyour value: " + static_cast<std::string>(t2) };
		}

		template <typename Other_Type>
		friend Result operator>=(const Correct_Value<Type>& t1, const My_Value<Other_Type>& t2)
		{
			if (t1.value >= t2.value)
				return true;
			return Result{ false, "your value is greater than the correct value:\ncorrect value: " + static_cast<std::string>(t1) + "\nyour value: " + static_cast<std::string>(t2) };
		}
	};

	template <typename Type>
	class Unit_Test : public Imp::Base_Unit_Test
	{
		template <int id>
		void Call_Test(std::size_t& count, std::vector<std::string>& failed)
		{
			auto x = static_cast<Type&>(*this).Run(Test<id>{});
			test_results.insert(x);
			std::cout << "Running test " << this->name << "::" << x.name;
			count++;
			if(x.result)
				std::cout << ", Passed :)\n";
			else
			{
				std::cout << ""<< ", Failed: because " << x.result.Why() << "\n";
				failed.push_back(this->name + "::" + x.name);
			}
			if constexpr(Imp::Is_Valid_Test<Type, Test<id + 1>>)
				Call_Test<id + 1>(count, failed);
		}
		class Hook
		{
		public:
			Hook()
			{
				Imp::All_Tests().push_back(std::make_unique<Type>());
			}
			void operator()(){}
		};
	public:
		static Hook hook;
		Unit_Test(const std::string& name) : Imp::Base_Unit_Test{ name }
		{
			hook();
		}

		inline std::unordered_map<std::string, Result>& operator()(std::size_t& count, std::vector<std::string>& failed) override
		{
			if constexpr(Imp::Is_Valid_Test<Type, Test<0>>)
				Call_Test<0>(count, failed);
			return test_results;
		}
	};

	template <typename Type>
	typename Unit_Test<Type>::Hook Unit_Test<Type>::hook;

	inline std::size_t Run_Tests()
	{
		class Delimiter
		{
			std::string delimiter;
		public:
			Delimiter(const std::string& delimiter = "") : delimiter{ delimiter }
			{}
			std::string operator=(const std::string& x)
			{
				std::string copy = delimiter;
				delimiter = x;
				return copy;
			}
		};

		std::vector<std::string> failed_tests;
		std::size_t test_count = 0;
		for(auto& test : Imp::All_Tests())
		{
			std::cout << "Starting test group: " << test->name << "\n";
			std::cout << "<----------------------------------->" << "\n";
			(*test)(test_count, failed_tests);
		}
		std::cout << "<----------------------------------->" << std::endl;
		std::cout << "Passed (" << std::to_string(test_count - failed_tests.size()) << "/" << std::to_string(test_count) << "), "
				<< "Failed (" << std::to_string(failed_tests.size()) << "/" << std::to_string(test_count) << ")";
		Delimiter delimiter{": "};
		for(auto& failed_test : failed_tests)
			std::cout << (delimiter = ", ") << failed_test;
		std::cout << std::endl;

		return failed_tests.size();
	}	
}