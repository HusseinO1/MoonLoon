#pragma once
#include "Common.h"

template<typename T>
class myvector
{
private:
	uint size = 0;
	T* data = nullptr;
	uint capacity = 0;
public:
	myvector(uint count)
	{
		Reserve(count);
		size = count;
	}

	myvector(uint count, const T& value)
	{
		Reserve(count);
		for (uint i = 0; i < count; ++i)
			data[i] = value;
		size = count;
	}

	myvector(const myvector<T>& other)
	{
		Reserve(other.size);

		for (uint i = 0; i < other.size; ++i)
		{
			data[i] = other.data[i];
		}

		size = other.size;
	}

	inline uint Size() const
	{
		return size;
	}

	uint Capacity() const
	{
		return capacity;
	}

	inline bool Empty() const
	{
		return (size == 0);
	}

	inline void Reserve(uint newCap)
	{
		if (newCap <= capacity) return;

		T* newData = new T[newCap]();

		for (uint i = 0; i < size; ++i)
		{
			newData[i] = data[i];
		}

		delete[] data;
		data = newData;
		capacity = newCap;
	}

	void PushBack(const T value)
	{
		if (size >= capacity) Reserve(capacity == 0 ? 1 : capacity * 2);
		data[size] = value;
		++size;
	}

	void PopBack()
	{
		if (size == 0) return;
		size--;
	}

	void Clear()
	{
		size = 0;
	}

	T& Front() const
	{
		return data[0];
	}

	T& Back() const
	{
		return data[size - 1];
	}

	const T& operator[](uint index) const
	{
		return data[index];
	}

	T& operator[](uint index)
	{
		return data[index];
	}

	myvector<T>& operator=(const myvector<T>& other)
	{
		if (this == &other) return *this;

		Reserve(other.size);

		for (uint i = 0; i < other.size; ++i)
		{
			data[i] = other.data[i];
		}

		size = other.size;

		return *this;
	}

	~myvector()
	{
		delete[] data;
	}
};