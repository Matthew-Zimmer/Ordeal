#include "ordeal.hpp"

#include <numeric>
#include <iostream>

namespace Slate::Ordeal
{
    Test_Result::Test_Result(std::string const& name) : name{ name }, passed{ false }
    {}
 
    Test_Result::Test_Result(bool passed) : passed{ passed }, name{ "" }
    {}
 
    Test_Result::Test_Result(Test_Result const& test) : passed{ test.passed }
    {
        if (test.name.size())
            name = test.name;
    }
 
    Test_Result::Test_Result(Test_Result&& test) : passed{ test.passed }
    {
        if (test.name.size())
            name = std::move(test.name);
    }
 
    Test_Result& Test_Result::operator=(Test_Result const& test)
    {
        passed = test.passed;
        if (test.name.size())
            name = test.name;
        return *this;
    }
 
    Test_Result& Test_Result::operator=(Test_Result&& test)
    {
        passed = test.passed;
        if (test.name.size())
            name =  std::move(test.name);
        return *this;
    }

    std::ostream& operator<<(std::ostream& stream, Test_Result const& test)
    {
        return stream << test.name << (test.name.size() ? ": " : "") << (test.passed ? "passed" : "failed");
    }

    bool Test_Result::passed_test() const
    {
        return passed;
    }
}

namespace Slate::Detail::Ordeal
{
    Unit_Test::Unit_Test(std::string const& name) : test_name{ name }
    {}

    std::size_t Unit_Test::amount_failed() const
    {
        return failed_tests;
    }

    std::size_t Unit_Test::total_amount() const
    {
        return test_count;
    }
    
    std::string const& Unit_Test::name() const
    {
        return test_name;
    }

    std::ostream& operator<<(std::ostream& stream, Unit_Test& o)
    {
        o.run_all(stream);
        return stream;
    }

    std::vector<std::unique_ptr<Unit_Test>>& all_tests()
    {
        static std::vector<std::unique_ptr<Unit_Test>> tests;
        return tests;
    }
}


namespace Slate::Ordeal
{
    Test_Result operator""_name(char const* str, std::size_t count)
    {   
        return Test_Result{ std::string{ str, count } };
    }

    std::size_t run_tests()
    {
        auto& tests = Detail::Ordeal::all_tests();
        std::size_t failed = 0, total = 0;
        for (auto& test : tests)
        {
            std::cout << test->name() << ":" << std::endl;
            std::cout << *test;
            auto f = test->amount_failed(), t = test->total_amount();
            failed += f;
            total += t;
            std::cout << "\t" << t - f << "/" << t << " passed" << std::endl;
            std::cout << "\t" << f << " failed" << std::endl;
        }
        std::cout << total - failed << "/" << total << " passed" << std::endl;
        std::cout << failed << " failed" << std::endl;
        return failed;
    }
}