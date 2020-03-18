#pragma once
#include <functional>
#include <memory>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>

#include <reflection/reflection.hpp>
#include <reflection/variables.hpp>

namespace Slate::Ordeal
{
    class Test_Result
    {
        std::string name;
        bool passed;
    public:
        Test_Result(std::string const& name);
        Test_Result(bool passed);
        Test_Result(Test_Result const& test);
        Test_Result(Test_Result&& test);
        Test_Result& operator=(Test_Result const& test);
        Test_Result& operator=(Test_Result&& test);
        friend std::ostream& operator<<(std::ostream& stream, Test_Result const& test);
        bool passed_test() const;
    };
}

namespace Slate::Detail::Ordeal
{
    using Slate::Ordeal::Test_Result;

    class Unit_Test
    {
        std::string test_name;
        virtual void run_all(std::ostream& stream) = 0;
    protected:
        std::size_t failed_tests;
        std::size_t test_count;
    public:
        Unit_Test(std::string const& name);
        virtual ~Unit_Test() = default;
        std::size_t amount_failed() const;
        std::size_t total_amount() const;
        std::string const& name() const;
        friend std::ostream& operator<<(std::ostream& stream, Unit_Test& o);
    };

    std::vector<std::unique_ptr<Unit_Test>>& all_tests();

    namespace Detail
    {
        template <typename Object, typename Type, typename = void>
        class Is_Valid_Test : public std::false_type
        {};

        template <typename Object, typename Type>
        class Is_Valid_Test<Object, Type, std::void_t<decltype(std::declval<Object>().run(Type{}))>> : public std::true_type
        {};

        template <typename Type, typename=void>
        class Is_Container : public std::false_type
        {};

        template <typename Type>
        class Is_Container<Type, std::void_t<decltype(std::declval<Type>().begin(), std::declval<Type>().end())>> : public std::true_type
        {};
    }

    template <typename Object, typename Type>
    constexpr bool is_valid_test = Detail::Is_Valid_Test<Object, Type>::value;

    template <typename Type>
    constexpr bool is_container = Detail::Is_Container<Type>::value;
}


namespace Slate::Ordeal
{
    template <std::size_t id>
    class Test
    {};

    namespace V 
    {
        using Variable::Base;

        template <typename Type>
        class Value : public Base<Type>
        {
        public:
            auto& value() { return this->variable(); }
            auto const& value() const { return this->variable(); }
        };
    }

    template <typename Type>
    class Value;

    template <typename Type>
    class Expected_Value : public Is<Expected_Value<Type>, Variables<V::Value<Type>>>
    {
        std::optional<Type> delta;
        template <typename U, typename V>
        friend Test_Result operator==(Value<U> const& x, Expected_Value<V> const& y);
    public:
        Expected_Value(Type const& value) : Is<Expected_Value<Type>, Variables<V::Value<Type>>>{ V::Value<Type>{ value } }
        {}

        Expected_Value& within(Type const& delta)
        {
            this->delta = delta;
            return *this;
        }
    };

    template <typename Type>
    class Value : public Is<Value<Type>, Variables<V::Value<Type>>>
    {
    public:
        Value(Type const& value) : Is<Value<Type>, Variables<V::Value<Type>>>{ V::Value<Type>{ value } }
        {}

        template <typename Pred>
        auto expects(Pred&& cond)
        {
            if constexpr (Detail::Ordeal::is_container<Type>)
                return std::accumulate(this->value().begin(), this->value().end(), true, [&](bool r, auto const& x){ return r && cond(x); });
            else
                return cond(this->value());
        }

        template <typename U, typename V>
        friend Test_Result operator==(Value<U> const& x, Expected_Value<V> const& y);
    };

    template <typename U, typename V>
    Test_Result operator==(Value<U> const& x, Expected_Value<V> const& y)
    {
        if (y.delta)
            return x.value() >= y.value() - y.delta.value() && x.value() <= y.value() + y.delta.value();
        else
            return x.value() == y.value();
    }

    template <typename U, typename V>
    Test_Result operator==(Expected_Value<U> const& x, Value<V> const& y)
    {
        return y == x;
    }
        

    Test_Result operator""_name(char const* str, std::size_t count);

    template <typename Type>
    class Unit_Test : public Detail::Ordeal::Unit_Test
    {
        template <int id>
        void run_test(std::ostream& stream)
        {
            auto r = Meta::cast<Type>(*this).run(Test<id>{});
            failed_tests += !r.passed_test();
            test_count++;
            stream << "\t" << "test(id: " << id << "): " << r << std::endl;
            if constexpr (Detail::Ordeal::is_valid_test<Type, Test<id + 1>>)
                run_test<id + 1>(stream);
        }
        class Hook
        {
        public:
            Hook()
            {
                Detail::Ordeal::all_tests().push_back(std::make_unique<Type>());
            }
            void operator()(){}
        };
        void run_all(std::ostream& stream) final
        {
            if constexpr (Detail::Ordeal::is_valid_test<Type, Test<0>>)
                run_test<0>(stream);
        }
    public:
        static Hook hook;
        Unit_Test(std::string const& name) : Detail::Ordeal::Unit_Test{ name }
        {
            hook();
        }
    };

    template <typename Type>
    typename Unit_Test<Type>::Hook Unit_Test<Type>::hook;

    std::size_t run_tests();
}