#ifndef VEC_H
#define VEC_H

// a array wray to vec :v
// this is ugly and bad but std::vector is slooowww!

template <typename T>
struct vec8
{
	int size = 0;
	T pointer[8] = {};

	T & operator [] (unsigned int index) {
		return pointer[index];
	}

	void add(const T & element);
	void pop();
	void clear();
	void erase(int index);
	void insert(int index, const T & element);
};

template<typename T>
inline void vec8<T>::add(const T & element)
{
	if (size >= 8) return;
	++size;
	pointer[size - 1] = element;
}

template<typename T>
inline void vec8<T>::pop()
{
	if (size <= 0) return;
	--size;
}

template<typename T>
inline void vec8<T>::clear()
{
	size = 0;
}

template<typename T>
inline void vec8<T>::erase(int index)
{
	if (size <= 0) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = index; i < size - 1; ++i) {
		pointer[i] = pointer[i + 1];
	}
	--size;
}

template<typename T>
inline void vec8<T>::insert(int index, const T & element)
{
	if (size >= 8) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = size; i > index; --i) {
		pointer[i] = pointer[i - 1];
	}
	++size;
	pointer[index] = element;
}






template <typename T>
struct vec16
{
	int size = 0;
	T pointer[16] = {};

	T & operator [] (unsigned int index) {
		return pointer[index];
	}

	void add(const T & element);
	void pop();
	void clear();
	void erase(int index);
	void insert(int index, const T & element);
};

template<typename T>
inline void vec16<T>::add(const T & element)
{
	if (size >= 16) return;
	++size;
	pointer[size - 1] = element;
}

template<typename T>
inline void vec16<T>::pop()
{
	if (size <= 0) return;
	--size;
}

template<typename T>
inline void vec16<T>::clear()
{
	size = 0;
}

template<typename T>
inline void vec16<T>::erase(int index)
{
	if (size <= 0) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = index; i < size - 1; ++i) {
		pointer[i] = pointer[i + 1];
	}
	--size;
}

template<typename T>
inline void vec16<T>::insert(int index, const T & element)
{
	if (size >= 16) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = size; i > index; --i) {
		pointer[i] = pointer[i - 1];
	}
	++size;
	pointer[index] = element;
}






template <typename T>
struct vec32
{
	int size = 0;
	T pointer[32] = {};

	T & operator [] (unsigned int index) {
		return pointer[index];
	}

	void add(const T & element);
	void pop();
	void clear();
	void erase(int index);
	void insert(int index, const T & element);
};

template<typename T>
inline void vec32<T>::add(const T & element)
{
	if (size >= 32) return;
	++size;
	pointer[size - 1] = element;
}

template<typename T>
inline void vec32<T>::pop()
{
	if (size <= 0) return;
	--size;
}

template<typename T>
inline void vec32<T>::clear()
{
	size = 0;
}

template<typename T>
inline void vec32<T>::erase(int index)
{
	if (size <= 0) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = index; i < size - 1; ++i) {
		pointer[i] = pointer[i + 1];
	}
	--size;
}

template<typename T>
inline void vec32<T>::insert(int index, const T & element)
{
	if (size >= 32) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = size; i > index; --i) {
		pointer[i] = pointer[i - 1];
	}
	++size;
	pointer[index] = element;
}






template <typename T>
struct vec64
{
	int size = 0;
	T pointer[64] = {};

	T & operator [] (unsigned int index) {
		return pointer[index];
	}

	void add(const T & element);
	void pop();
	void clear();
	void erase(int index);
	void insert(int index, const T & element);
};

template<typename T>
inline void vec64<T>::add(const T & element)
{
	if (size >= 64) return;
	++size;
	pointer[size - 1] = element;
}

template<typename T>
inline void vec64<T>::pop()
{
	if (size <= 0) return;
	--size;
}

template<typename T>
inline void vec64<T>::clear()
{
	size = 0;
}

template<typename T>
inline void vec64<T>::erase(int index)
{
	if (size <= 0) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = index; i < size - 1; ++i) {
		pointer[i] = pointer[i + 1];
	}
	--size;
}

template<typename T>
inline void vec64<T>::insert(int index, const T & element)
{
	if (size >= 64) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = size; i > index; --i) {
		pointer[i] = pointer[i - 1];
	}
	++size;
	pointer[index] = element;
}






template <typename T>
struct vec128
{
	int size = 0;
	T pointer[128] = {};

	T & operator [] (unsigned int index) {
		return pointer[index];
	}

	void add(const T & element);
	void pop();
	void clear();
	void erase(int index);
	void insert(int index, const T & element);
};

template<typename T>
inline void vec128<T>::add(const T & element)
{
	if (size >= 128) return;
	++size;
	pointer[size - 1] = element;
}

template<typename T>
inline void vec128<T>::pop()
{
	if (size <= 0) return;
	--size;
}

template<typename T>
inline void vec128<T>::clear()
{
	size = 0;
}

template<typename T>
inline void vec128<T>::erase(int index)
{
	if (size <= 0) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = index; i < size - 1; ++i) {
		pointer[i] = pointer[i + 1];
	}
	--size;
}

template<typename T>
inline void vec128<T>::insert(int index, const T & element)
{
	if (size >= 128) return;
	if (index < 0) return;
	if (index >= size) return;
	for (int i = size; i > index; --i) {
		pointer[i] = pointer[i - 1];
	}
	++size;
	pointer[index] = element;
}

#endif