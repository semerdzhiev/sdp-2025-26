#include "utils/stopwatch.h"

int main()
{
	const size_t rows = 5'000;
	const size_t columns = 300'000;

	int *arr = new int[rows * columns];

	size_t r, c;
	unsigned long long sum;
	stopwatch sw;

	//
	// Initialize the elements of the array
	//
	std::cout << "Initializing the elements of the array...";
	sw.start();

	for (r = 0; r < rows; ++r)
		for (c = 0; c < columns; ++c)
			arr[columns * r + c] = static_cast<int>(r);

	sw.stop();
	std::cout << "\n    execution took " << sw << "\n\n";


	//
	// Iterate over the columns and then the rows and then the columns
	//
	std::cout << "Iterating by columns and then rows...";

	sum = 0;
	sw.start();

	for (c = 0; c < columns; ++c)
		for (r = 0; r < rows; ++r)
			sum += arr[columns * r + c];

	sw.stop();
	std::cout << "\n    execution took " << sw << "\n\n";


	//
	// Iterate over the rows and then the columns
	//
	std::cout << "Iterating by rows and then columns...";

	sum = 0;
	sw.start();

	for (r = 0; r < rows; ++r)
		for (c = 0; c < columns; ++c)
			sum += arr[columns * r + c];

	sw.stop();
	std::cout << "\n    execution took " << sw << "\n\n";

	delete[] arr;

	return 0;
}