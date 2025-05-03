#pragma once
#include <stdlib.h>
#include <mem.h>

struct custom_wstr
{
	custom_wstr()
	{
		data = malloc(sizeof(wchar_t));
		size = 0;
		data[size] = L'\0';
	}
	custom_wstr(wchar_t* str)
	{
		size_t s = wcslen(str);
		data = malloc(s * sizeof(wchar_t) + sizeof(wchar_t));
		size = s;
		memmove(data, str, s * sizeof(wchar_t));
		data[size] = L'\0';
	}

	int len()
	{
		return size;
	}

	void push(wchar_t &a)
	{
		data = realloc(data, (size + 1) * sizeof(wchar_t) + sizeof(wchar_t));
		data[size++] = a;
		data[size] = L'\0';
	}
	void add(wchar_t &a, int cursor)
	{
		data = realloc(data, (size + 1) * sizeof(wchar_t) + sizeof(wchar_t));
		if(size - cursor)
			memmove(data + 1 + cursor, data + cursor, (size - cursor) * sizeof(wchar_t));
		data[cursor] = a;
		size++;
		data[size] = L'\0';
	}
	void del(int cursor)
	{
		if(!cursor)
			return;
		if(size - cursor + 1 > 0)
			memmove(data + cursor - 1, data + cursor, (size - cursor + 1) * sizeof(wchar_t));
		data = realloc(data, size * sizeof(wchar_t));
		size--;
		data[size] = L'\0';
	}

	bool remove(wchar_t &a)
	{
		for (wchar_t *h = data; h < h + size; h++)
		{
			if (*h == a)
			{
				if (h - data != size - 1)
					memmove(h, h + 1, (size - (h - data) - 1) * sizeof(wchar_t));
				data = realloc(data, size * sizeof(wchar_t) - sizeof(wchar_t));
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

	wchar_t &operator[](int i)
	{
		return data[i];
	}
	void operator=(wchar_t* str){
		size_t s = wcslen(str);
		data = realloc(data, s * sizeof(wchar_t) + sizeof(wchar_t));
		size = s;
		memmove(data, str, s * sizeof(wchar_t));
		data[size] = L'\0';
	}

	~custom_wstr()
	{
		free(data);
	}

	struct iterator
	{
		typedef int difference_type;
		typedef wchar_t value_type;
		typedef std::random_access_iterator_tag iterator_category;
		typedef iterator self_type;
		typedef wchar_t* pointer;
		typedef wchar_t& reference;
		iterator(wchar_t *ait)
		{
			it = ait;
		}
		~iterator()
		{
		}

		wchar_t &operator*()
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
		wchar_t *it;
	};

	iterator begin()
	{
		return iterator(data);
	}
	iterator end()
	{
		return iterator(data + size);
	}

	wchar_t *data;
	size_t size;
};