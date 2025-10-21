#pragma once

namespace dsa {

template <typename T>
class fixed_size_array {
	T* m_data = nullptr;
	size_t m_size = 0;

public:

	/// Constructs an empty array
	fixed_size_array() noexcept = default;

	/// Creates an array with a specified size
	/// @exception std::bad_alloc if memory allocation fails
	explicit fixed_size_array(size_t size)
	{
		if (size != 0) {
			m_data = new T[size];
			m_size = size;
		}
	}

	///
	/// Copies the values from another array into the current object
	/// 
	/// The function copies min(size(), other.size()) elements
	/// 
	void fill_from(const fixed_size_array& other)
	{
		size_t limit = std::min(size(), other.size());
			
		for (size_t i = 0; i < limit; ++i)
			m_data[i] = other.m_data[i];
	}

	/// Creates a copy of another array
	fixed_size_array(const fixed_size_array& other)
		: fixed_size_array(other.m_size)
	{
		fill_from(other);
	}

	/// Copies the contents of another array
	fixed_size_array& operator=(const fixed_size_array& other)
	{
		if(this != &other) {
			fixed_size_array copy(other);
			swap(copy);
		}

		return *this;
	}

	fixed_size_array(fixed_size_array&& other) noexcept
	{
		swap(other);
	}

	fixed_size_array& operator=(fixed_size_array&& other) noexcept
	{
		fixed_size_array temp(std::move(other));
		swap(temp);
		return *this;
	}

	~fixed_size_array() noexcept
	{
		delete [] m_data;
	}

	size_t size() const noexcept
	{
		return m_size;
	}

	bool empty() const noexcept
	{
		return m_size == 0;
	}

	T* data() noexcept
	{
		return m_data;
	}

	const T* data() const noexcept
	{
		return m_data;
	}

	T& at(size_t index)
	{
		if (index >= m_size)
			throw std::out_of_range("index is out of the bounds of the array");

		return m_data[index];
	}

	const T& at(size_t index) const
	{
		if (index >= m_size)
			throw std::out_of_range("index is out of the bounds of the array");

		return m_data[index];
	}

	T& operator[](size_t index) noexcept
	{
		return m_data[index];
	}

	const T& operator[](size_t index) const noexcept
	{
		return m_data[index];
	}

	void swap(fixed_size_array& other) noexcept
	{
		std::swap(m_data, other.m_data);
		std::swap(m_size, other.m_size);
	}

	///
	/// @brief Checks whether two arrays have the same size and contain the same sequence of elements
	///
	/// The elements of the array must be comparable with `==`.
	/// 
	bool operator==(const fixed_size_array& other) const
	{
		if (size() != other.size())
			return false;
		
		for (size_t i = 0; i < size(); i++) {
			if (m_data[i] != other.m_data[i])
				return false;
		}

		return true;
	}
};

} // namespace
