#include <cstdio>
#include <filesystem>

#include "utils/stopwatch.h"

namespace fs = std::filesystem;

void display_usage(const char* executablePath)
{
    try {
        fs::path ep(executablePath);

        std::cout
            << "Usage:\n\t"
            << ep.filename()
            << " <row_count> <column_count>\n";
    }
    catch (...) {
        std::cout << "Cannot parse path from argv[0]";
    }
}

void run_benchmarks(int* arr, size_t rows, size_t columns)
{
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
}

int main(int argc, char* argv[])
{
    size_t rows;
    size_t columns;

    // Verify the number of arguments
    if(argc < 3) {
        display_usage(argv[0]);
        return 1;
    }

    // Parse input
    if( ! sscanf(argv[1], "%zu", &rows) || ! sscanf(argv[2], "%zu", &columns) ) {
        std::cerr << "Cannot convert command line arguments to size_t\n";
        return 2;
    }

    // Allocate an array of the desired size
    size_t total_size = rows * columns;
    std::cout << "Allocating an array of " << total_size << " elements...\n\n";

    int* arr = nullptr;
    try {
        arr = new int[total_size];
    }
    catch(std::bad_alloc&) {
        std::cerr << "Failed to allocate enough memory!\n";
        return 3;
    }

    // Run the benchmarks
    run_benchmarks(arr, rows, columns);

    // Cleanup and exit
    delete [] arr;
    return 0;
}
