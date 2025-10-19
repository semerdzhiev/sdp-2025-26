# Executable target for the unit tests
add_executable(test-containers)

target_link_libraries(
	test-containers
	PRIVATE
		containers
		Catch2::Catch2WithMain
)

target_sources(
	test-containers
	PRIVATE
		"test_array.cpp"
		"test_dynamic_array.cpp"
		"test_fixed_size_array.cpp"
)

catch_discover_tests(test-containers)
