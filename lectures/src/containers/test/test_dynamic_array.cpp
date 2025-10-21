#include "catch2/catch_all.hpp"

#include <cassert>

#include "containers/dynamic_array.h"

using dsa::dynamic_array;

//----------------------------------------------------------------------
// Helper functions
//

template <typename T>
void checkEmpty(dynamic_array<T>& arr)
{
  const dynamic_array<T>& cref = arr;
  CHECK(arr.size() == 0);
  CHECK(arr.capacity() == 0);
  CHECK(arr.data() == nullptr);
  CHECK(cref.data() == nullptr);
}

template <typename T>
void checkNotEmpty(dynamic_array<T>& arr, size_t expectedSize)
{
  const dynamic_array<T>& cref = arr;
  CHECK(arr.size() == expectedSize);
  CHECK(arr.capacity() >= arr.size());
  CHECK(arr.data() != nullptr);
  CHECK(cref.data() == arr.data());
}


//----------------------------------------------------------------------
// Constructors
//

TEST_CASE("dynamic_array::dynamic_array() constructs an empty array", "[dynamic_array]")
{
  dynamic_array<int> obj;
  checkEmpty(obj);
}

TEST_CASE("dynamic_array::dynamic_array(N=0) constructs an empty object", "[dynamic_array]")
{
  dynamic_array<int> obj(0);
  checkEmpty(obj);
}

class NonEmptyArrayFixture {
protected:
  const size_t initialSize = 5;
  dynamic_array<size_t> arr = dynamic_array<size_t>(initialSize);
  const dynamic_array<size_t>& cref = arr;
};

TEST_CASE_METHOD(NonEmptyArrayFixture, "dynamic_array::dynamic_array(N>0) constructs an object with correct size and capacity", "[dynamic_array]")
{
  checkNotEmpty(arr, initialSize);
}

TEST_CASE("dynamic_array::dynamic_array(N>0) throws when memory allocation fails", "[dynamic_array]")
{
  const size_t sizeTooLargeForTheHeap = 100'000'000'000;
  REQUIRE_THROWS_AS(dynamic_array<int>(sizeTooLargeForTheHeap), std::bad_alloc);
}


//----------------------------------------------------------------------
// Selectors and operations with elements
//

TEST_CASE_METHOD(NonEmptyArrayFixture, "dynamic_array::at() can be used to access all elements of the array", "[dynamic_array]")
{
  for (int i = 0; i < arr.size(); ++i)
    arr.at(i) = i;

  SECTION("at() can retrieve the elements")
  {
    for (size_t i = 0; i < arr.size(); ++i)
      REQUIRE(arr.at(i) == i);
  }

  SECTION("at() const const can retrieve the elements")
  {
    for (size_t i = 0; i < cref.size(); ++i)
      REQUIRE(cref.at(i) == i);
  }
}

TEST_CASE_METHOD(NonEmptyArrayFixture, "dynamic_array::at() throws if the index is not valid", "[dynamic_array]")
{
  REQUIRE_THROWS_AS(arr.at(arr.size()), std::out_of_range);
}

TEST_CASE_METHOD(NonEmptyArrayFixture, "dynamic_array::at() const throws if the index is not valid", "[dynamic_array]")
{
  REQUIRE_THROWS_AS(cref.at(cref.size()), std::out_of_range);
}

TEST_CASE_METHOD(NonEmptyArrayFixture, "dynamic_array::operator[] can be used to access all elements of the array", "[dynamic_array]")
{
  for (size_t i = 0; i < arr.size(); ++i)
    arr[i] = i;

  SECTION("operator[] can retrieve the elements")
  {
    for (size_t i = 0; i < arr.size(); ++i)
      REQUIRE(arr[i] == i);
  }

  SECTION("operator[] const can retrieve the elements")
  {
    for (size_t i = 0; i < cref.size(); ++i)
      REQUIRE(cref[i] == i);
  }
}

/// Checks whether arr contains all numbers in [begin,end], ordered ascendingly.
bool containsAllNumbersBetween(const dynamic_array<size_t>& arr, size_t begin, size_t end)
{
  size_t expected_size = end - begin + 1;

  if (arr.size() != expected_size)
    return false;

  for (size_t i = 0; i < arr.size(); ++i) {
    if (arr.at(i) != i)
      return false;
  }

  return true;
}

/// Checks whether two arrays have the same size and contain the same sequence of elements
template <typename T>
bool sameContents(const dynamic_array<T>& arr1, const dynamic_array<T>& arr2) noexcept
{
  if (arr1.size() != arr2.size())
    return false;

  for (size_t i = 0; i < arr1.size(); ++i) {
    if (arr1.at(i) != arr2.at(i))
      return false;
  }

  return true;
}

// Fill arr with all numbers in [begin,end], ordered ascendingly.
void fillWithAllNumbersBetween(dynamic_array<size_t>& arr, size_t begin, size_t end)
{
  assert(begin <= end);
  size_t size = end - begin + 1;
  for (size_t i = 0; i < size; i++) {
    arr[i] = i + begin;
  }
}

class ConsecutiveNumbersFixture {
protected:
  /// @brief Number of elements in the sample array
  const size_t initialSize = 5;

  /// @brief An array of all natural numbers in [0, initialSize-1]
  dynamic_array<size_t> arr = dynamic_array<size_t>(initialSize);

  /// @brief Initial capacity of the array.
  const size_t initialCapacity = arr.capacity();

  /// @brief Constant reference to the same array
  const dynamic_array<size_t>& cref = arr;

protected:
  /// @brief Initializes the contents of the array
  ConsecutiveNumbersFixture()
  {
    fillWithAllNumbersBetween(arr, 0, initialSize - 1);
  }

  /// @brief Checks whether the capacity of the array is the same as the initial capacity
  bool capacityRemainsTheSame()
  {
    return arr.capacity() == initialCapacity;
  }

  /// @brief Checks whether the size of the array is the same as the initial size
  bool sizeRemainsTheSame()
  {
    return arr.size() == initialSize;
  }

  /// @brief Checks whether the array contains the same elements as in the beginning of the test
  /// This check does NOT ensure that the capacity did not change
  bool contentsRemainTheSame()
  {
    return sizeRemainsTheSame() && containsAllNumbersBetween(arr, 0, initialSize-1);
  }

};

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::pop_back() decrements the size", "[dynamic_array]")
{
  arr.pop_back();
  REQUIRE(arr.size() == initialSize - 1);
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::pop_back() does not alter capacity", "[dynamic_array]")
{
  arr.pop_back();
  REQUIRE(capacityRemainsTheSame());
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::pop_back() does not alter the rest of the elements", "[dynamic_array]")
{
  arr.pop_back();
  REQUIRE(containsAllNumbersBetween(arr, 0, initialSize - 2));
}

TEST_CASE("dynamic_array::pop_back() throws when the array is empty", "[dynamic_array]")
{
  dynamic_array<int> arr;
  REQUIRE_THROWS_AS(arr.pop_back(), dynamic_array<int>::EmptyArrayException);
}

TEST_CASE("dynamic_array::push_back() apends elements to the back of the array", "[dynamic_array]")
{
  // Arrange
  dynamic_array<size_t> arr;

  // Act
  const size_t size = 10;
  for (size_t i = 0; i < size; i++)
    arr.push_back(i);

  // Assert
  REQUIRE(containsAllNumbersBetween(arr, 0, size-1));
}


//----------------------------------------------------------------------
// Resizing
//

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::reserve(N < capacity) does not alter the array", "[dynamic_array]")
{
  for (size_t i = 0; i <= initialCapacity; i++) {
    arr.reserve(i);
    REQUIRE(capacityRemainsTheSame());
    REQUIRE(contentsRemainTheSame());
  }
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::reserve(capacity < N < 2*capacity) increases capacity with more than N and does not alter the contents", "[dynamic_array]")
{
  const size_t requestedCapacity = initialCapacity + 1; // pick a number, which will be less than <capacity> * <resize_factor>
  arr.reserve(requestedCapacity);
  REQUIRE(arr.capacity() > requestedCapacity);
  REQUIRE(contentsRemainTheSame());
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::reserve(N > 2*capacity) increases capacity to exactly N and does not alter the contents", "[dynamic_array]")
{
  const size_t requestedCapacity = initialCapacity * 3; // pick a multiplier that exceeds the resize factor
  arr.reserve(requestedCapacity);  
  REQUIRE(arr.capacity() == requestedCapacity);
  REQUIRE(contentsRemainTheSame());
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::reserve() throws when the requested capacity is too large and the array remain unchanged (strong exception safety)")
{
  const size_t sizeTooLargeForTheHeap = 100'000'000'000;
  dynamic_array<int> arr;
  
  REQUIRE_THROWS_AS(arr.reserve(sizeTooLargeForTheHeap), std::bad_alloc);

  REQUIRE(capacityRemainsTheSame());
  REQUIRE(contentsRemainTheSame());
}


//----------------------------------------------------------------------
// Copying data from other arrays
//

TEST_CASE("dynamic_array::dynamic_array(const dynamic_array&) correctly copies an empty array", "[dynamic_array]")
{
  dynamic_array<size_t> empty;
  dynamic_array<size_t> copy = empty;
  checkEmpty(copy);
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::dynamic_array(const dynamic_array&) correctly copies a non-empty array", "[dynamic_array]")
{
  dynamic_array<size_t> copy = arr;
  const dynamic_array<size_t>& cref_copy = copy;

  SECTION("Copy has the same size and elements") {
    REQUIRE(sameContents(copy, arr));
  }  
  SECTION("data() and data() const of the copied object return the same address") {
    REQUIRE(cref_copy.data() == copy.data());
  }  
  SECTION("The underlying buffers of the two arrays are different") {
    REQUIRE(copy.data() != arr.data());
  }
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::operator=(const dynamic_array&) sets the target to an empty state when the source is an empty array", "[dynamic_array]")
{
  dynamic_array<size_t> empty;
  arr = empty;
  checkEmpty(arr);
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::operator=(const dynamic_array&) correctly copies a non-empty array", "[dynamic_array]")
{
  dynamic_array<size_t> copy;
  copy = arr;
  const dynamic_array<size_t>& cref_copy = copy;

  SECTION("Copy has the same size and elements") {
    REQUIRE(sameContents(copy, arr));
  }
  SECTION("data() and data() const of the copied object return the same address") {
    REQUIRE(cref_copy.data() == copy.data());
  }
  SECTION("The underlying buffers of the two arrays are different") {
    REQUIRE(copy.data() != arr.data());
  }
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::dynamic_array(dynamic_array&&) correctly moves contents", "[dynamic_array]")
{
  const size_t* buffer = arr.data();
  dynamic_array<size_t> movedTo(std::move(arr));

  SECTION("The move-constructed array has the same contents as the original") {
    REQUIRE(movedTo.size() == initialSize);
    REQUIRE(containsAllNumbersBetween(movedTo, 0, initialSize - 1));
  }
  SECTION("The move-constructed array points to the same buffer") {
    REQUIRE(movedTo.data() == buffer);
    const dynamic_array<size_t>& cref_movedTo = movedTo;
    REQUIRE(cref_movedTo.data() == buffer);
  }
  SECTION("The move-constructed array has the same capacity as the original") {
    REQUIRE(movedTo.capacity() == initialCapacity);
  }
  SECTION("The source array is empty") {
    checkEmpty(arr);
  }
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::operator=(dynamic_array&&) correctly moves to an empty array", "[dynamic_array]")
{
  const size_t* buffer = arr.data();
  dynamic_array<size_t> movedTo;
  movedTo = std::move(arr);

  SECTION("The move-constructed array has the same contents as the original") {
    REQUIRE(movedTo.size() == initialSize);
    REQUIRE(containsAllNumbersBetween(movedTo, 0, initialSize - 1));
  }
  SECTION("The move-constructed array points to the same buffer") {
    REQUIRE(movedTo.data() == buffer);
    const dynamic_array<size_t>& cref_movedTo = movedTo;
    REQUIRE(cref_movedTo.data() == buffer);
  }
  SECTION("The move-constructed array has the same capacity as the original") {
    REQUIRE(movedTo.capacity() == initialCapacity);
  }
  SECTION("The source array is empty") {
    checkEmpty(arr);
  }
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::operator=(dynamic_array&&) correctly moves to a non-empty array", "[dynamic_array]")
{
  const size_t* buffer = arr.data();

  // Create an array with different size and contents
  dynamic_array<size_t> movedTo(3);
  fillWithAllNumbersBetween(movedTo, 11, 13);

  // Move the contents of the original
  movedTo = std::move(arr);

  SECTION("The move-constructed array has the same contents as the original") {
    REQUIRE(movedTo.size() == initialSize);
    REQUIRE(containsAllNumbersBetween(movedTo, 0, initialSize - 1));
  }
  SECTION("The move-constructed array points to the same buffer") {
    REQUIRE(movedTo.data() == buffer);
    const dynamic_array<size_t>& cref_movedTo = movedTo;
    REQUIRE(cref_movedTo.data() == buffer);
  }
  SECTION("The move-constructed array has the same capacity as the original") {
    REQUIRE(movedTo.capacity() == initialCapacity);
  }
  SECTION("The source array is empty") {
    checkEmpty(arr);
  }
}

TEST_CASE_METHOD(ConsecutiveNumbersFixture, "dynamic_array::swap() correctly swaps the contents of two arrays", "[dynamic_array]")
{
  // Create an array with different size and contents
  dynamic_array<size_t> another(3);
  fillWithAllNumbersBetween(another, 11, 13);

  size_t sizeAnother = another.size();
  size_t capacityAnother = another.capacity();

  const size_t* bufferArr = arr.data();
  const size_t* bufferAnother = another.data();

  arr.swap(another);

  SECTION("Buffers are swapped correctly") {
    CHECK(arr.data() == bufferAnother);
    CHECK(another.data() == bufferArr);
  }
  SECTION("Sizes are swapped correctly") {
    CHECK(arr.size() == sizeAnother);
    CHECK(another.size() == initialSize);
  }
  SECTION("Capacities are swapped correctly") {
    CHECK(arr.capacity() == capacityAnother);
    CHECK(another.capacity() == initialCapacity);
  }
}
