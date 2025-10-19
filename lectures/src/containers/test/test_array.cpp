#include "catch2/catch_all.hpp"

#include "containers/array.h"

using dsa::array;

template<size_t Begin, size_t End>
class consecutive_numbers_fixture {
    static_assert(Begin <= End);
public:
    static constexpr size_t size = End - Begin;
    array<size_t, size> arr;

    consecutive_numbers_fixture()
    {
        for(size_t i = 0; i < size; ++i)
            arr[i] = Begin + i;
    }
};

TEST_CASE("array::size() returns the array's size", "[array]")
{
    consecutive_numbers_fixture<0,9> fx;
    REQUIRE(fx.arr.size() == fx.size);
}

TEST_CASE("array::at() can be used to access all elements of the array", "[array]")
{
    consecutive_numbers_fixture<0,9> fx;
    decltype(fx.arr)& cref = fx.arr;

    for(size_t i = 0; i < fx.size; ++i) {
        CHECK(fx.arr.at(i) == i);
        CHECK(cref.at(i) == i);
    }
}

TEST_CASE("array::at() throws when the index is outside the bonds of the array", "[array]")
{
    consecutive_numbers_fixture<0,9> fx;
    REQUIRE_THROWS_AS(fx.arr.at(fx.size), std::out_of_range);
}

TEST_CASE("array::operator[] can be used to access all elements of the array", "[array]")
{
    consecutive_numbers_fixture<0,9> fx;
    decltype(fx.arr)& cref = fx.arr;

    for(size_t i = 0; i < fx.size; ++i) {
        CHECK(fx.arr.at(i) == i);
        CHECK(cref.at(i) == i);
    }
}

TEST_CASE("array::operator==() correctly identifies equal arrays", "[array]")
{
    consecutive_numbers_fixture<0,9> fx1;
    consecutive_numbers_fixture<0,9> fx2;
    REQUIRE(fx1.arr == fx2.arr);
}

TEST_CASE("array::operator==() returns false for different arrays", "[array]")
{
    consecutive_numbers_fixture <0,9> fx1;
    consecutive_numbers_fixture <10,19> fx2;
    REQUIRE_FALSE(fx1.arr == fx2.arr);
}

TEST_CASE("array::swap() exchanges the contents of two arrays", "[array]")
{
    consecutive_numbers_fixture <0,9> fx1;
    consecutive_numbers_fixture <10,19> fx2;
    fx1.arr.swap(fx2.arr);
    CHECK(fx1.arr == consecutive_numbers_fixture <10,19>().arr);
    CHECK(fx2.arr == consecutive_numbers_fixture <0,9>().arr);
}
