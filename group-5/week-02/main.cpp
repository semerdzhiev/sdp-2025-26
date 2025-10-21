#include <iostream>
#include <string>

using std::string;

template <typename T, typename I>
T* map(const I* arr, int size, T (*func)(I)) { 
    T* result = new T[size];

    for (int i = 0; i < size; ++i) {
        result[i] = func(arr[i]);
    }

    return result;
}

string toString(int x) {
    return (std::to_string(x)+".0");
}

int addOne(int x) {
    return x + 1;
}

bool applyOperation(bool (*op)(int, int),
                        int x, int y)
{
    return (*op)(x, y);
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};

    int size = sizeof(arr) / sizeof(arr[0]);
    // map(arr, 5, &addOne);

    // for (size_t i = 0; i < size; i++)
    // {
    //     std::cout << arr[i] << " ";
    // }
    
    string* result = map<string, int>(arr, size, &toString);

    for (size_t i = 0; i < size; i++)
    {
        std::cout << result[i] << " ";
    }

    delete[] result;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}