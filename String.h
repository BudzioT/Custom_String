#pragma once

#include <memory>
#include <initializer_list>
#include <iostream>

/*********************************************** CLASSES ***************************************************/

/*/////////////////////////////////////////// String class ////////////////////////////////////////////////*/

class String
{
private:
	//Helping functions
	void reallocate();
	void free();
	template<bool constness = false> class Iterator;
	template<bool constness = false> class Reverse_Iterator;
public:
	//Types
	typedef char value_type;
	typedef std::char_traits<char> char_traits;
	typedef std::allocator<char> allocator_type;
	typedef char& reference;
	typedef const char& const_reference;
	typedef char* pointer;
	typedef const char* const_pointer;
	typedef std::ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Iterator<> iterator;
	typedef Iterator<true> const_iterator;
	typedef Reverse_Iterator<> reverse_iterator;
	typedef Reverse_Iterator<true> const_reverse_iterator;

	//Public const member
	static const size_t npos = -1;

public:
	//Constructors, Destructor
	String() = default;
	String(const char* cptr);
	String(const char* cptr, size_t n);
	String(size_t n, char ch);
	String(std::initializer_list<char> lst);
	template<typename InputIterator> String(InputIterator first, InputIterator last);
	String(const String& str);
	String(const String& str, size_t subpos, size_t sublen = npos);
	String(String&& str) noexcept;
	~String() { free(); }

	//Assignment overloads
	String& operator=(const String& str);
	String& operator=(const char* cptr);
	String& operator=(char ch);
	String& operator=(std::initializer_list<char> lst);
	String& operator=(String&& str) noexcept;

	//Iterators
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;

	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;

	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;

	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;
	//Capacity
	size_t size() const noexcept { return sz; }
	size_t length() const noexcept { return sz; }
	size_t max_size() const noexcept { return -1; }
	size_t capacity() const noexcept { return cap; }
	bool empty() const noexcept { return !cp; }

	void clear() { free(); }
	void resize(size_t n);
	void resize(size_t n, char ch);
	void reserve(size_t n = 0);
	void shrink_to_fit();

	//Element access
	char& operator[](size_t n);
	const char& operator[](size_t n) const;

	char& at(size_t n);
	const char& at(size_t n) const;

	char& back();
	const char& back() const;

	char& front();
	const char& front() const;

	//Modifiers
	String& operator+=(const String& str);
	String& operator+=(const char* cptr);
	String& operator+=(char ch);
	String& operator+=(std::initializer_list<char> lst);

	String& append(const String& str);
	String& append(const String&, size_t str, size_t n = npos);
	String& append(const char* cptr);
	String& append(const char* cptr, size_t n);
	String& append(size_t n, char ch);
	template<typename InputIterator> String& append(InputIterator first, InputIterator last);
	String& append(std::initializer_list<char> lst);

	void push_back(char ch);

	String& assign(const String& str);
	String& assign(const String& str, size_t subpos, size_t sublen = npos);
	String& assign(const char* cptr);
	String& assign(const char* cptr, size_t n);
	String& assign(size_t n, char ch);
	template<typename InputIterator> String& assign(InputIterator first, InputIterator last);
	String& assign(std::initializer_list<char> lst);
	String& assign(String&& str) noexcept;

	String& insert(size_t pos, const String& str);
	String& insert(size_t pos, const String& str, size_t subpos, size_t sublen = npos);
	String& insert(size_t pos, const char* cptr);
	String& insert(size_t pos, const char* cptr, size_t n);
	String& insert(size_t pos, size_t n, char ch);
	iterator insert(const_iterator p, size_t n, char ch);
	iterator insert(const_iterator p, char ch);
	template<typename InputIterator> iterator insert(iterator p, InputIterator first, InputIterator last);
	String& insert(const_iterator p, std::initializer_list<char> lst);

	String& erase(size_t pos, size_t len = npos);
	iterator erase(const_iterator p);
	iterator erase(const_iterator first, const_iterator last);

	String& replace(size_t pos, size_t len, const String& str);
	String& replace(const_iterator first, const_iterator last, const String& str);
	String& replace(size_t pos, size_t len, const String& str, size_t subpos, size_t sublen = npos);
	String& replace(size_t pos, size_t len, const char* cptr);
	String& replace(const_iterator first, const_iterator last, const char* cptr);
	String& replace(size_t pos, size_t len, const char* cptr, size_t n);
	String& replace(const_iterator first, const_iterator last, const char* cptr, size_t n);
	String& replace(size_t pos, size_t len, size_t n, char ch);
	String& replace(const_iterator first, const_iterator last, size_t n, char ch);
	template<typename InputIterator> 
		String& replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last);
	String& replace(const_iterator first, const_iterator last, std::initializer_list<char> lst);

	void swap(String& str);

	void pop_back();

	//String operations
	const char* c_str();
	const char* data();
	allocator_type get_allocator() const noexcept;
	size_t copy(char* cptr, size_t len, size_t pos = 0) const;

	size_t find(const String& str, size_t pos = 0) const noexcept;
	size_t find(const char* cptr, size_t pos = 0) const;
	size_t find(const char* cptr, size_t pos, size_t n) const;
	size_t find(char c, size_t pos = 0) const noexcept;
	size_t rfind(const String& str, size_t pos = 0) const noexcept;
	size_t rfind(const char* cptr, size_t pos = 0) const;
	size_t rfind(const char* cptr, size_t pos, size_t n) const;
	size_t rfind(char c, size_t pos = 0) const noexcept;

	size_t find_first_of(const String& str, size_t pos = 0) const noexcept;
	size_t find_first_of(const char* cptr, size_t pos = 0) const;
	size_t find_first_of(const char* cptr, size_t pos, size_t n) const;
	size_t find_first_of(char ch, size_t pos = 0) const noexcept;
	size_t find_last_of(const String& str, size_t pos = 0) const noexcept;
	size_t find_last_of(const char* cptr, size_t pos = 0) const;
	size_t find_last_of(const char* cptr, size_t pos, size_t n) const;
	size_t find_last_of(char ch, size_t pos = 0) const noexcept;

	size_t find_first_not_of(const String& str, size_t pos = 0) const noexcept;
	size_t find_first_not_of(const char* cptr, size_t pos = 0) const;
	size_t find_first_not_of(const char* cptr, size_t pos, size_t n) const;
	size_t find_first_not_of(char ch, size_t pos = 0) const noexcept;
	size_t find_last_not_of(const String& str, size_t pos = 0) const noexcept;
	size_t find_last_not_of(const char* cptr, size_t pos = 0) const;
	size_t find_last_not_of(const char* cptr, size_t pos, size_t n) const;
	size_t find_last_not_of(char ch, size_t pos = 0) const noexcept;

	String substr(size_t pos = 0, size_t len = npos) const;
	int compare(const String& str) const noexcept;
	int compare(size_t pos, size_t len, const String& str) const;
	int compare(size_t pos, size_t len, const String& str, size_t subpos, size_t sublen = npos) const;
	int compare(const char* cptr) const;
	int compare(size_t pos, size_t len, const char* cptr) const;
	int compare(size_t pos, size_t len, const char* cptr, size_t n) const;

	//Non-member function overloads
	friend String operator+(const String&, const String&);
	friend String operator+(const String&, String&&);
	friend String operator+(String&&, const String&);
	friend String operator+(String&&, String&&);
	friend String operator+(const String&, const char*);
	friend String operator+(String&&, const char*);
	friend String operator+(const char*, const String&);
	friend String operator+(const char*, String&&);
	friend String operator+(const String&, char);
	friend String operator+(String&&, char);
	friend String operator+(char, const String&);
	friend String operator+(char, String&&);

	friend bool operator==(const String&, const String&) noexcept;
	friend bool operator==(const char*, const String&);
	friend bool operator==(const String&, const char*);
	friend bool operator!=(const String&, const String&) noexcept;
	friend bool operator!=(const char*, const String&);
	friend bool operator!=(const String&, const char*);
	friend bool operator<(const String&, const String&) noexcept;
	friend bool operator<(const char*, const String&);
	friend bool operator<(const String&, const char*);
	friend bool operator<=(const String&, const String&) noexcept;
	friend bool operator<=(const char*, const String&);
	friend bool operator<=(const String&, const char*);
	friend bool operator>(const String&, const String&) noexcept;
	friend bool operator>(const char*, const String&);
	friend bool operator>(const String&, const char*);
	friend bool operator>=(const String&, const String&) noexcept;
	friend bool operator>=(const char*, const String&);
	friend bool operator>=(const String&, const char*);

	friend void swap(String&, String&);

	friend std::istream& operator>>(std::istream&, const String&);
	friend std::ostream& operator<<(std::ostream&, const String&);

	friend std::istream& getline(std::istream&, String&, char);
	friend std::istream& getline(std::istream&&, String&, char);
	friend std::istream& getline(std::istream&, String&);
	friend std::istream& getline(std::istream&&, String&);
private:
	static std::allocator<char> alloc;
	size_t sz = 0;
	char* cp = nullptr;
	size_t cap = 0;
};


/*//////////////////////////////////////////// Iterator class ////////////////////////////////////////////////*/

/* String's iterator. Based of a random-access iterator */
template<bool constness> class String::Iterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename std::conditional_t<constness, const char, char>;
	using difference_type = std::ptrdiff_t;
	using reference = typename std::conditional_t<constness, const char&, char&>;
	using pointer = typename std::conditional_t<constness, const char*, char*>;
public:
	explicit Iterator(char* cp) : m_cp(cp) {}

	/* Conversion */
	operator const_iterator() const { return const_iterator(m_cp); }
	friend iterator;

	/* Iterate operations */
	Iterator& operator++();
	Iterator operator++(int);
	Iterator& operator--();
	Iterator operator--(int);

	Iterator operator+(size_t n) const;
	Iterator& operator+=(size_t n);
	Iterator operator-(size_t n) const;
	Iterator& operator-=(size_t n);

	/* Access operations */
	template<bool _constness = constness> std::enable_if_t<_constness, reference>
	operator*() const;
	template<bool _constness = constness> std::enable_if_t<!_constness, reference>
	operator*();

	template<bool _constness = constness> std::enable_if_t<_constness, reference>
	operator[](size_t index) const;
	template<bool _constness = constness> std::enable_if_t<!_constness, reference>
	operator[](size_t index);

	/* Rational operations */
	bool operator<(const const_iterator& rhs) const;
	bool operator<=(const const_iterator& rhs) const;
	bool operator>(const const_iterator& rhs) const;
	bool operator>=(const const_iterator& rhs) const;
	bool operator==(const const_iterator& rhs) const;
	bool operator!=(const const_iterator& rhs) const;
private:
	pointer m_cp;
};


/* String's reverse iterator, based of normal String's Iterator */
template <bool constness> class String::Reverse_Iterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename std::conditional_t<constness, const char, char>;
	using difference_type = std::ptrdiff_t;
	using reference = typename std::conditional_t<constness, const char&, char&>;
	using pointer = typename std::conditional_t<constness, const char*, char*>;
public:
	Reverse_Iterator(char* cp) : m_cp(cp) {}

	/* Conversion */
	operator const_reverse_iterator() { return const_reverse_iterator(m_cp); }
	friend reverse_iterator;

	/* Iterate operations */
	Reverse_Iterator& operator++();
	Reverse_Iterator operator++(int);
	Reverse_Iterator& operator--();
	Reverse_Iterator operator--(int);

	Reverse_Iterator operator+(size_t n) const;
	Reverse_Iterator& operator+=(size_t n);
	Reverse_Iterator operator-(size_t n) const;
	Reverse_Iterator& operator-=(size_t n);

	/* Access operations */
	template<bool _constness = constness> std::enable_if_t<_constness, reference>
	operator*() const;
	template<bool _constness = constness> std::enable_if_t<!_constness, reference>
	operator*();

	template<bool _constness = constness> std::enable_if_t<_constness, reference>
	operator[](size_t index) const;
	template<bool _constness = constness> std::enable_if_t<!_constness, reference>
	operator[](size_t index);

	/* Rational operations */
	bool operator<(const const_reverse_iterator& rhs) const;
	bool operator<=(const const_reverse_iterator& rhs) const;
	bool operator>(const const_reverse_iterator& rhs) const;
	bool operator>=(const const_reverse_iterator& rhs) const;
	bool operator==(const const_reverse_iterator& rhs) const;
	bool operator!=(const const_reverse_iterator& rhs) const;
private:
	pointer m_cp;
};

/****************************************** FUNCTIONS DECLARATIONS *********************************************/

String operator+(const String& lhs, const String& rhs);
String operator+(const String& lhs, String&& rhs);
String operator+(String&& lhs, const String& rhs);
String operator+(String&& lhs, String&& rhs);
String operator+(const String& lhs, const char* rhs);
String operator+(String&& lhs, const char* rhs);
String operator+(const char* lhs, const String& rhs);
String operator+(const char* lhs, String&& rhs);
String operator+(const String& lhs, char rhs);
String operator+(String&& lhs, char rhs);
String operator+(char lhs, const String& rhs);
String operator+(char lhs, String&& rhs);

bool operator==(const String& lhs, const String& rhs) noexcept;
bool operator==(const char* lhs, const String& rhs);
bool operator==(const String& lhs, const char* rhs);
bool operator!=(const String& lhs, const String& rhs) noexcept;
bool operator!=(const char* lhs, const String& rhs);
bool operator!=(const String& lhs, const char* rhs);
bool operator<(const String& lhs, const String& rhs) noexcept;
bool operator<(const char* lhs, const String& rhs);
bool operator<(const String& lhs, const char* rhs);
bool operator<=(const String& lhs, const String& rhs) noexcept;
bool operator<=(const char* lhs, const String& rhs);
bool operator<=(const String& lhs, const char* rhs);
bool operator>(const String& lhs, const String& rhs) noexcept;
bool operator>(const char* lhs, const String& rhs);
bool operator>(const String& lhs, const char* rhs);
bool operator>=(const String& lhs, const String& rhs) noexcept;
bool operator>=(const char* lhs, const String& rhs);
bool operator>=(const String& lhs, const char* rhs);

void swap(String& lhs, String& rhs);

std::istream& operator>>(std::istream& is, String& str);
std::ostream& operator<<(std::ostream& os, const String& str);
std::istream& getline(std::istream& is, String& str, char delim);
std::istream& getline(std::istream&& is, String& str, char delim);
std::istream& getline(std::istream& is, String& str);
std::istream& getline(std::istream&& is, String& str);

/******************************************** ITERATOR FUNCTIONS ********************************************/

/* Dereferences the pointer */
template<bool constness>
template<bool _constness> std::enable_if_t<_constness, typename String::Iterator<constness>::reference>
String::Iterator<constness>::operator*() const
{
	return *m_cp;
}

/* Dereferences the pointer */
template<bool constness>
template<bool _constness> std::enable_if_t<!_constness, typename String::Iterator<constness>::reference>
String::Iterator<constness>::operator*()
{
	return *m_cp;
}

/* Iterates one place forward */
template<typename bool constness>
String::Iterator<constness>& String::Iterator<constness>::operator++()
{
	++m_cp;
	return *this;
}

/* Iterates one place forward, returns the old pointer */
template<typename bool constness>
String::Iterator<constness> String::Iterator<constness>::operator++(int)
{
	Iterator result(m_cp);
	++m_cp;
	return result;
}

/* Iterates one place backward */
template<typename bool constness>
String::Iterator<constness>& String::Iterator<constness>::operator--()
{
	--m_cp;
	return *this;
}

/* Iterates one place backward, returns the old pointer */
template<typename bool constness>
String::Iterator<constness> String::Iterator<constness>::operator--(int)
{
	Iterator result(m_cp);
	--m_cp;
	return result;
}

/* Returns the dereferenced character at the given index */
template<bool constness> template<bool _constness>
std::enable_if_t<_constness, typename String::Iterator<constness>::reference>
String::Iterator<constness>::operator[](size_t index) const
{
	return *(m_cp + index);
}

/* Returns the dereferenced character at the given index */
template<bool constness> template<bool _constness>
std::enable_if_t<!_constness, typename String::Iterator<constness>::reference>
String::Iterator<constness>::operator[](size_t index)
{
	return *(m_cp + index);
}

/* Returns the iterator, that's incremented (n) times */
template<bool constness>
String::Iterator<constness> String::Iterator<constness>::operator+(size_t n) const
{
	return Iterator(m_cp + n);
}

/* Returns the iterator, that's decremented (n) times */
template<bool constness>
String::Iterator<constness> String::Iterator<constness>::operator-(size_t n) const
{
	return Iterator(m_cp + n);
}

/* Moves this iterator forward (n) times */
template<bool constness>
String::Iterator<constness>& String::Iterator<constness>::operator+=(size_t n)
{
	m_cp += n;
	return *this;
}

/* Moves this iterator backward (n) times */
template<bool constness>
String::Iterator<constness>& String::Iterator<constness>::operator-=(size_t n)
{
	m_cp -= n;
	return *this;
}

/* Checks if the iterators are equal to each other */
template<bool constness>
bool String::Iterator<constness>::operator==(const const_iterator& rhs) const
{
	return m_cp == rhs.m_cp;
}

/* Checks if the iterators aren't equal to each other */
template<bool constness>
bool String::Iterator<constness>::operator!=(const const_iterator& rhs) const
{
	return !(*this == rhs);
}

/* Checks if the first iterator is lesser than the second */
template<bool constness>
bool String::Iterator<constness>::operator<(const const_iterator& rhs) const
{
	return m_cp < rhs.m_cp;
}

/* Checks if the first iterator is lesser than or equal to the second */
template<bool constness>
bool String::Iterator<constness>::operator<=(const const_iterator& rhs) const
{
	return (*this < rhs) || (*this == rhs);
}

/* Checks if the first iterator is higher than the second */
template<bool constness>
bool String::Iterator<constness>::operator>(const const_iterator& rhs) const
{
	return !(*this < rhs) && (*this != rhs);
}

/* Checks if the first iterator is higher than or equal to the second */
template<bool constness>
bool String::Iterator<constness>::operator>=(const const_iterator& rhs) const
{
	return (*this > rhs) || (*this == rhs);
}


/* Iterates this reverse iterator "forwards"*/
template <bool constness>
String::Reverse_Iterator<constness>& String::Reverse_Iterator<constness>::operator++()
{
	--m_cp;
	return *this;
}

/* Iterates this reverse iterator "forwards", returns the old reverse iterator */
template <bool constness>
String::Reverse_Iterator<constness> String::Reverse_Iterator<constness>::operator++(int)
{
	Reverse_Iterator result(m_cp);
	--m_cp;
	return result;
}

/* Iterates this reverse iterator "backwards" */
template <bool constness>
String::Reverse_Iterator<constness>& String::Reverse_Iterator<constness>::operator--()
{
	++m_cp;
	return *this;
}

/* Iterates this reverse iterator "backwards", returns the old reverse iterator */
template <bool constness>
String::Reverse_Iterator<constness> String::Reverse_Iterator<constness>::operator--(int)
{
	Reverse_Iterator result(m_cp);
	++m_cp;
	return result;
}

/* Moves the reverse iterator "fowards" (n) times, returns it */
template<bool constness>
String::Reverse_Iterator<constness> String::Reverse_Iterator<constness>::operator+(size_t n) const
{
	return Reverse_Iterator(m_cp - n);
}

/* Moves the reverse iterator "backwards" (n) times, returns it */
template<bool constness>
String::Reverse_Iterator<constness> String::Reverse_Iterator<constness>::operator-(size_t n) const
{
	return Reverse_Iterator(m_cp + n);
}

/* Moves this reverse iterator "fowards" (n) times, returns it*/
template<bool constness>
String::Reverse_Iterator<constness>& String::Reverse_Iterator<constness>::operator+=(size_t n)
{
	m_cp -= n;
	return *this;
}

/* Moves this reverse iterator "backwards" (n) times, returns it */
template <bool constness>
String::Reverse_Iterator<constness>& String::Reverse_Iterator<constness>::operator-=(size_t n)
{
	m_cp += n;
	return *this;
}

/* Dereferences this reverse iterator */
template<bool constness> template<bool _constness> 
std::enable_if_t<_constness, typename String::Reverse_Iterator<constness>::reference>
String::Reverse_Iterator<constness>::operator*() const
{
	return *m_cp;
}

/* Dereferences this reverse iterator */
template<bool constness> template<bool _constness>
std::enable_if_t<!_constness, typename String::Reverse_Iterator<constness>::reference>
String::Reverse_Iterator<constness>::operator*()
{
	return *m_cp;
}

/* Returns the dereferenced character at the given index */
template<bool constness> template<bool _constness>
std::enable_if_t<_constness, typename String::Reverse_Iterator<constness>::reference>
String::Reverse_Iterator<constness>::operator[](size_t index) const
{
	return *(m_cp + index);
}

/* Returns the dereferenced character at the given index */
template<bool constness> template<bool _constness>
std::enable_if_t<!_constness, typename String::Reverse_Iterator<constness>::reference>
String::Reverse_Iterator<constness>::operator[](size_t index)
{
	return *(m_cp + index);
}

/* Checks if both reverse iterators are equal to each other */
template <bool constness>
bool String::Reverse_Iterator<constness>::operator==(const const_reverse_iterator& rhs) const
{
	return (m_cp == rhs.m_cp);
}

/* Checks if both reverse iterators aren't equal to each other */
template <bool constness>
bool String::Reverse_Iterator<constness>::operator!=(const const_reverse_iterator& rhs) const
{
	return !(*this == rhs);
}

/* Checks if this reverse iterator is lesser than the second */
template <bool constness>
bool String::Reverse_Iterator<constness>::operator<(const const_reverse_iterator& rhs) const
{
	return m_cp < rhs.m_cp;
}

/* Checks if this reverse iterator is lesser than or equal to the second */
template <bool constness>
bool String::Reverse_Iterator<constness>::operator<=(const const_reverse_iterator& rhs) const
{
	return (*this < rhs) || (*this == rhs);
}

/* Checks if this reverse iterator is higher than the second */
template <bool constness>
bool String::Reverse_Iterator<constness>::operator>(const const_reverse_iterator& rhs) const
{
	return (*this != rhs) && !(*this < rhs);
}

/* Checks if this reverse iterator is higher than or equal to the second */
template <bool constness>
bool String::Reverse_Iterator<constness>::operator>=(const const_reverse_iterator& rhs) const
{
	return (*this > rhs) || (*this == rhs);
}

/************************************* STRING ITERATOR FUNCTIONS ****************************************/

/* Creates String from two the range in between two input iterators */
template<typename InputIterator> String::String(InputIterator first, InputIterator last)
{
	size_t size = 0;
	for (InputIterator beg = first; beg != last; beg++, size++)
		;
	cap = sz = size;
	cp = alloc.allocate(cap);

	size_t i = 0;
	for (InputIterator beg = first; beg != last; beg++, i++)
		alloc.construct(cp + i, *beg);
}

/* Appends the String created from the given range, to this String */
template<typename InputIterator> String& String::append(InputIterator first, InputIterator last)
{
	size_t itSize = 0;
	for (InputIterator beg = first; beg != last; beg++, itSize++)
		;
	size_t newSize = sz + itSize;

	InputIterator beg = first;
	if (cap >= newSize) {
		for (size_t i = 0; i < itSize; i++)
			alloc.construct(cp + sz++, *(beg + i));
	}
	else {
		auto newCp = alloc.allocate(newSize);
		size_t i = 0;
		for (; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		for (size_t j = 0; j < itSize; j++)
			alloc.construct(newCp + i++, *(beg + j));
		cp = newCp;
		sz = cap = newSize;
	}
	return *this;
}

/* Assigns this String to the String created from the given range */
template<typename InputIterator> String& String::assign(InputIterator first, InputIterator last)
{
	size_t itSize = 0;
	for (InputIterator beg = first; beg != last; beg++, itSize++)
		;

	InputIterator beg = first;
	if (cap >= itSize) {
		for (int i = sz; i >= 0; i--)
			alloc.destroy(cp + i);
		for (size_t j = 0; j < itSize; j++)
			alloc.construct(cp + j, *beg++);
		sz = itSize;
	}
	else {
		free();
		sz = cap = itSize;
		cp = alloc.allocate(cap);
		for (size_t i = 0; i < sz; i++)
			alloc.construct(cp + i, *beg++);
	}
	return *this;
}

/* Inserts the characters from the given range into the place where iterator points to */
template<typename InputIterator> 
String::iterator String::insert(iterator p, InputIterator first, InputIterator last)
{
	bool match = false;
	size_t index = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (beg == p) {
			match = true;
			break;
		}
	}
	if (match == false) {
		match = (end() == p);
		if (match == false)
			return last;
	}
	
	size_t rangeSize = 0;
	for (InputIterator beg = first; beg != last; beg++)
		rangeSize++;
	const size_t newSize = sz + rangeSize;
	String old(*this);
	if (cap >= newSize) {
		if (cp) {
			for (int i = sz - 1; i >= 0; i--)
				alloc.destroy(cp + i);
		}
		sz = newSize;
	}
	else {
		free();
		cp = alloc.allocate(newSize);
		sz = cap = newSize;
	}

	InputIterator beg = first;
	size_t i = 0;
	for (; i < index; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t stopPlace = i;
	for (size_t j = 0; j < rangeSize; j++)
		alloc.construct(cp + i++, *beg++);
	for (; stopPlace < old.sz; stopPlace++)
		alloc.construct(cp + i++, *(old.cp + stopPlace));
	return (begin() + index + rangeSize);
}

/* Replaces the given range with characters from the second range */
template<typename InputIterator>
String& String::replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last)
{
	bool match1 = false;
	bool match2 = false;
	size_t index_last = 0, index = 0;
	size_t index_first = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (i1 == beg) {
			match1 = true;
			index_first = index;
		}
		if (i2 == beg) {
			match2 = true;
			index_last = index;
		}
	}
	if (match2 == false) {
		match2 = (i2 == end());
		index_last = index;
	}
	if (match1 == false || match2 == false)
		return *this;
	
	size_t rangeSize = 0;
	for (InputIterator beg = first; beg != last; beg++)
		rangeSize++;
	size_t newSize = sz - (index_last - index_first) + rangeSize;
	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);

	size_t i = 0;
	for (; i < index_first; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + (index_last - index_first);

	InputIterator iter = first;
	for (size_t j = 0; j < rangeSize; j++)
		alloc.construct(newCp + i++, *iter++);
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}