#include "catch2/catch_all.hpp"

#include "containers/list.h"
#include "utils/MockingObjects.h"

#include <string>

class single_node_chain {
public:
	const int value = 42;
	list<int>::node head = list<int>::node(value);
};

class two_nodes_chain {
public:
	const int tail_value = 57;
	const int head_value = 42;
	list<int>::node tail = list<int>::node(tail_value);
	list<int>::node head = list<int>::node(head_value, &tail);
};

class four_nodes_chain {
public:
	list<int>::node last   = list<int>::node(40);
	list<int>::node third  = list<int>::node(30, &last);
	list<int>::node second = list<int>::node(20, &third);
	list<int>::node first  = list<int>::node(10, &second);
	list<int>::node* head = &first;
};

TEST_CASE("list::node::node() constructs a node with no successor and a default-constructed value", "[list]")
{
	list<ConstructorSpy>::node empty;

	CHECK(empty.next == nullptr);
	CHECK(empty.value.constructedBy == ConstructorSpy::ConstructorType::Default);
}

TEST_CASE_METHOD(single_node_chain, "list::node::node(v) correctly initializes an object", "[list]")
{
	CHECK(head.value == value);
	CHECK(head.next == nullptr);
}

TEST_CASE_METHOD(two_nodes_chain, "list::node::node(v, n) correctly initializes an object", "[list]")
{
	CHECK(head.value == head_value);
	CHECK(head.next == &tail);
}

TEST_CASE_METHOD(single_node_chain, "list::node::has_next() returns false for nodes that have no successors", "[list]")
{
	CHECK_FALSE(head.has_next());
}

TEST_CASE_METHOD(two_nodes_chain, "list::node::has_next() returns true for nodes that have successors", "[list]")
{
	CHECK(head.has_next());
}

TEST_CASE_METHOD(four_nodes_chain, "list::chain_operations::identical() returns true when two chains contain the same values", "[list]")
{
	four_nodes_chain other;
	CHECK(list<int>::chain_operations::identical(head, other.head));
}

TEST_CASE_METHOD(four_nodes_chain, "list::chain_operations::identical() returns false when two chains contain different elements", "[list]")
{
	four_nodes_chain other;
	
	SECTION("...when the head elements differ") {
		other.first.value *= 10;
		CHECK_FALSE(list<int>::chain_operations::identical(head, other.head));
	}
	SECTION("...when the tail elements differ") {
		other.last.value *= 10;
		CHECK_FALSE(list<int>::chain_operations::identical(head, other.head));
	}
	SECTION("...when two of the middle elements differ") {
		other.second.value *= 10;
		CHECK_FALSE(list<int>::chain_operations::identical(head, other.head));
	}
	SECTION("...when one of the lists is longer") {
		list<int>::node additional(42);
		other.last.next = &additional;
		CHECK_FALSE(list<int>::chain_operations::identical(head, other.head));
	}
}

TEST_CASE_METHOD(four_nodes_chain, "list::chain_operations::clone() creates an identical chain", "[list]")
{
	list<int>::node* cloned = list<int>::chain_operations::clone(head);

	REQUIRE(list<int>::chain_operations::identical(head, cloned));

	list<int>::chain_operations::free(cloned);
}

TEST_CASE("list::chain_operations::clone() returns nullptr when cloning a nullptr", "[list]")
{
	REQUIRE(list<int>::chain_operations::clone(nullptr) == nullptr);
}

TEST_CASE("list::list() constructs a list with size 0", "[list]")
{
	list<int> empty;
	REQUIRE(empty.size() == 0);
}

TEST_CASE("list::front() throws for an empty array", "[list]")
{
	list<int> empty;
	const list<int> & cref = empty;

	REQUIRE_THROWS_AS(empty.front(), list<int>::empty_list_error);
	REQUIRE_THROWS_AS(cref.front(), list<int>::empty_list_error);
}

TEST_CASE("list::pop_front() throws for an empty array", "[list]")
{
	list<int> empty;
	REQUIRE_THROWS_AS(empty.pop_front(), list<int>::empty_list_error);
}

TEST_CASE("list::push_front(), list::pop_front(), list::front() and list::size() work in unison", "[list]")
{
	list<size_t> l;

	const size_t size = 10;
	
	for(size_t i = 0; i < size; ++i) {
		l.push_front(i);
		REQUIRE(l.front() == i);
		REQUIRE(l.size() == i+1);
	}

	for(size_t i = size; i > 0; --i) {
		REQUIRE(l.front() == i-1);
		REQUIRE(l.size() == i);
		l.pop_front();
	}
}

//TODO test constructors and assignments here
