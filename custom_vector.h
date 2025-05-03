#pragma once
#include <stdlib.h>
#include <mem.h>

template <typename T>
struct custom_vector
{
	custom_vector()
	{
		data = malloc(0);
		size = 0;
	}

	int len()
	{
		return size;
	}

	void push(T &a)
	{
		data = realloc(data, size * sizeof(T) + sizeof(T));
		data[size++] = a;
	}

	bool remove(T &a)
	{
		for (T *h = data; h < h + size; h++)
		{
			if (*h == a)
			{
				if (h - data != size - 1)
					memmove(h, h + 1, (size - (h - data) - 1) * sizeof(T));
				data = realloc(data, size * sizeof(T) - sizeof(T));
				--size;
				return true;
			}
		}
		return false;
	}
	void clean()
	{
		free(data);
		data = malloc(0);
		size = 0;
	}

	T &operator[](int i)
	{
		return data[i];
	}

	~custom_vector()
	{
		free(data);
	}

	struct iterator
	{
		typedef int difference_type;
		typedef T value_type;
		typedef std::random_access_iterator_tag iterator_category;
		typedef iterator self_type;
		typedef T *pointer;
		typedef T &reference;
		iterator(T *ait)
		{
			it = ait;
		}
		~iterator()
		{
		}

		T &operator*()
		{
			return *it;
		}

		iterator operator+=(int a)
		{
			it += a;
			return *this;
		}
		iterator operator++(int a)
		{
			it++;
			return *this;
		}
		iterator operator++()
		{
			iterator buf(it);
			it++;
			return buf;
		}
		iterator operator+(int a)
		{
			iterator buf(it);
			buf += a;
			return buf;
		}

		iterator operator-=(int a)
		{
			it -= a;
			return *this;
		}
		iterator operator-(int a)
		{
			iterator buf(it);
			buf -= a;
			return buf;
		}

		int operator-(iterator b)
		{
			return it - b.it;
		}

		bool operator==(iterator b)
		{
			return it == b.it;
		}
		bool operator!=(iterator b)
		{
			return it != b.it;
		}
		bool operator<(iterator b)
		{
			return it < b.it;
		}
		bool operator>(iterator b)
		{
			return it > b.it;
		}

	private:
		T *it;
	};

	iterator begin()
	{
		return iterator(data);
	}
	iterator end()
	{
		return iterator(data + size);
	}

private:
	T *data;
	size_t size;
};