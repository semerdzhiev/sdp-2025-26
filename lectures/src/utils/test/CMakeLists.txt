# Executable target for the unit tests
add_executable(test-utilities)

target_link_libraries(
	test-utilities
	PRIVATE
        utils
		Catch2::Catch2WithMain
)

target_sources(
	test-utilities
	PRIVATE
		"Test-Allocator.cpp"
		"Test-MockingObjects.cpp"
)

catch_discover_tests(test-utilities ADD_TAGS_AS_LABELS)
