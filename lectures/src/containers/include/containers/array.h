#pragma once

#include <utility>

namespace dsa {

template <typename T, size_t ArraySize>
class array {
    T arr[ArraySize];

public:
	T* data() noexcept
	{
		return arr;
	}

	const T* data() const noexcept
	{
		return arr;
	}

	T& at(size_t index)
	{
		if (index >= ArraySize)
			throw std::out_of_range("index is out of the bounds of the array");

		return arr[index];
	}

	const T& at(size_t index) const
	{
		if (index >= ArraySize)
			throw std::out_of_range("index is out of the bounds of the array");

		return arr[index];
	}

	T& operator[](size_t index) noexcept
	{
		return arr[index];
	}

	const T& operator[](size_t index) const noexcept
	{
		return arr[index];
	}

    bool operator==(const array& other) const
    {
        for(size_t i = 0; i < ArraySize; ++i)
            if( ! (arr[i] == other.arr[i]) )
                return false;
                
        return true;
    }

    size_t size() const noexcept
    {
        return ArraySize;
    }

    void swap(array& other)
    {
        using std::swap;
        for(size_t i = 0; i < ArraySize; ++i)
            swap(arr[i], other.arr[i]);
    }
};

}
