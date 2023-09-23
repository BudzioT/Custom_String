#include <cstring>
using std::strlen;
using std::strcat;

#include <stdexcept>
using std::out_of_range;
using std::runtime_error;

#include <memory>
using std::allocator;
using std::uninitialized_copy;

#include <initializer_list>
using std::initializer_list;

#include "String.h"

allocator<char> String::alloc;

//Allocates new memory, and moves the text to the new place
void String::reallocate()
{
	size_t s = sz ? sz * 2 : 1;
	char* newBeg = alloc.allocate(s);
	char* oldBeg = cp, *temp = newBeg;
	for (size_t i = 0; i < sz; i++)
		alloc.construct(newBeg++, std::move(*oldBeg++)); //Moves the text from the old place to the new
	free();
	cp = newBeg; //changes pointer to new allocator
	cap = s;
}

//Destructs elements, frees memory
void String::free()
{
	if (cp) {
		for (int i = sz - 1; i >= 0; i--)
			alloc.destroy(cp + i);
		alloc.deallocate(cp, cap);
	}
	cp = nullptr;
	sz = cap = 0;
}

//Copies text from const char*
String::String(const char* ptr) : sz(strlen(ptr)), cp(alloc.allocate(sz)), cap(sz)
{
	uninitialized_copy(ptr, ptr + sz, cp);
}

//Copies (len) characters from const char*
String::String(const char* ptr, size_t len) : sz((len < strlen(ptr)) ? len : strlen(ptr)),
	cp(alloc.allocate(sz)), cap(sz)
{
	uninitialized_copy(ptr, ptr + sz, cp);
}

//Copies character (len) times
String::String(size_t len, char ch) : sz(len), cp(alloc.allocate(sz)), cap(sz)
{
	for (size_t i = 0; i < len; i++)
		alloc.construct(cp + i, ch);
}

//Copies text from initializer_list
String::String(initializer_list<char> ls) : sz(ls.size()), cp(alloc.allocate(sz)), cap(sz)
{
	auto beg = ls.begin();
	for (size_t i = 0; i < sz; i++)
		alloc.construct(cp + i, *beg++);
}



//Copies other String
String::String(const String& str) : sz(str.sz), cp(alloc.allocate(sz)), cap(sz)
{
	uninitialized_copy(str.cp, str.cp + sz, cp);
}

//Copies (len) characters from String, starting from (pos) character
String::String(const String& str, size_t pos, size_t len)
{
	if (str.sz > pos)
		sz = len;
	else
		sz = str.sz;
	if (pos > str.sz)
		throw out_of_range("Position out of range!");
	if (len == npos) {
		sz = str.sz - pos;
	}
	cp = alloc.allocate(sz);
	cap = sz;
	for (size_t i = 0; i < sz; i++)
		alloc.construct(cp + i, *(str.cp + pos + i)); //Copies (sz) characters, starting from pos
}

//Moves the text from one String to the other
String::String(String&& str) noexcept : sz(str.sz), cp(str.cp), cap(str.cap)
{
	str.cp = nullptr;
	str.sz = str.cap = 0;
}

//Assigns one String, to the other
String& String::operator=(const String& str)
{
	if (cap >= str.cap) {
		if (cp) {
			for (int i = sz; i >= 0; i--)
				alloc.destroy(cp + i);
		}
	}
	else {
		free();
		cp = alloc.allocate(str.cap);
	}
	uninitialized_copy(str.cp, str.cp + str.sz, cp);
	sz = str.sz;
	cap = str.cap;
	return *this;
}

//Assings text from const char* to this String
String& String::operator=(const char* ptr)
{
	if (cap >= strlen(ptr)) {
		if (cp) {
			for (int i = sz - 1; i >= 0; i--)
				alloc.destroy(cp + i);
		}
		sz = strlen(ptr);
	}
	else {
		free();
		sz = cap = strlen(ptr);
		cp = alloc.allocate(cap);
	}
	uninitialized_copy(ptr, ptr + sz, cp);
	return *this;
}

//Assigns char to this String
String& String::operator=(char ch)
{
	if (cap >= 1) {
		if (cp) {
			for (int i = sz - 1; i >= 0; i--)
				alloc.destroy(cp + i);
		}
		sz = 1;
	}
	else {
		free();
		sz = cap = 1;
		cp = alloc.allocate(1);
	}
	alloc.construct(cp, ch);
	return *this;
}

//Assigns chars from initializer_list to this String
String& String::operator=(std::initializer_list<char> ls)
{
	if (cap >= ls.size()) {
		if (cp) {
			for (int i = sz - 1; i >= 0; i--)
				alloc.destroy(cp + i);
		}
		sz = ls.size();
	}
	else {
		free();
		sz = cap = ls.size();
		cp = alloc.allocate(cap);
	}
	uninitialized_copy(ls.begin(), ls.end(), cp);
	return *this;
}

//Assigns and moves one String to the other
String& String::operator=(String&& str) noexcept
{
	if (this != &str) {
		free();
		sz = str.sz;
		cp = str.cp;
		cap = str.cap;
		str.cp = nullptr;
		str.sz = str.cap = 0;
	}
	return *this;
}

/* Returns an iterator to the beginning of this String */
String::iterator String::begin() noexcept
{
	return iterator(cp);
}

/* Returns a const iterator to the beginning of this String */
String::const_iterator String::begin() const noexcept
{
	return const_iterator(cp);
}

/* Returns an iterator to the end of this String */
String::iterator String::end() noexcept
{
	return iterator(cp + sz);
}

/* Returns a const iterator to the end of this String */
String::const_iterator String::end() const noexcept
{
	return const_iterator(cp + sz);
}

/* Returns a reverse iterator to the reverse beginning of this String */
String::reverse_iterator String::rbegin() noexcept
{
	return reverse_iterator(cp + (sz - 1));
}

/* Returns a const reverse iterator to the reverse beginning of this String */
String::const_reverse_iterator String::rbegin() const noexcept
{
	return const_reverse_iterator(cp + (sz - 1));
}

/* Returns a reverse iterator to the reverse end of this String */
String::reverse_iterator String::rend() noexcept
{
	return reverse_iterator(cp - 1);
}

/* Returns a const reverse iterator to the reverse end of this String */
String::const_reverse_iterator String::rend() const noexcept
{
	return const_reverse_iterator(cp - 1);
}

/* Returns a const iterator to the beginning of this String */
String::const_iterator String::cbegin() const noexcept
{
	return const_iterator(cp);
}

/* Returns a const iterator to the end of this String */
String::const_iterator String::cend() const noexcept
{
	return const_iterator(cp + sz);
}

/* Returns a const reverse iterator to the reverse beginning of this String  */
String::const_reverse_iterator String::crbegin() const noexcept
{
	return const_reverse_iterator(cp + sz);
}

/* Returns a const reverse iterator to the reverse end of this String  */
String::const_reverse_iterator String::crend() const noexcept
{
	return const_reverse_iterator(cp - 1);
}

//Resizes String to (n) size, if n > sz - creates null characters,
//else if n < sz, destroys characters until n == sz
void String::resize(size_t n)
{
	if (n > sz) {
		if (n > cap) {
			auto newCp = alloc.allocate(n);
			size_t tempSz = sz;
			for (size_t i = 0; i < sz; i++)
				alloc.construct(newCp + i, std::move(*(cp + i)));
			free();
			cp = newCp;
			sz = tempSz;
			cap = n;
		}
		for (size_t i = sz; i < n; i++) {
			alloc.construct(cp + i, NULL);
		}
		sz = n;
	}
	else if (n < sz) {
		for (size_t i = sz - 1; i != n; i--)
			alloc.destroy(cp + i);
		sz = n;
	}
}

//The same as void String::resize(size_t n), but instead of creating null characters,
//creates (n - sz) copies of ch
void String::resize(size_t n, char ch)
{
	if (n > sz) {
		if (n > cap) {
			auto newCp = alloc.allocate(n);
			size_t tempSz = sz;
			for (size_t i = 0; i < sz; i++)
				alloc.construct(newCp + i, std::move(*(cp + i)));
			free();
			cp = newCp;
			sz = tempSz;
			cap = n;
		}
		for (size_t i = sz; i < n; i++) {
			alloc.construct(cp + i, ch);
		}
		sz = n;
	}
	else if (n < sz) {
		for (size_t i = sz - 1; i != n; i--)
			alloc.destroy(cp + i);
		sz = n;
	}
}

//Increases String's capacity to n, can't change the size and contents of the String
void String::reserve(size_t n)
{
	if (n > sz) {
		if (n > cap) {
			auto newCp = alloc.allocate(n);
			size_t tempSz = sz;
			for (size_t i = 0; i < sz; i++)
				alloc.construct(newCp + i, std::move(*(cp + i)));
			free();
			cap = n;
			sz = tempSz;
			cp = newCp;
		}
	}
}

//Shrinks the capacity to the size of String
void String::shrink_to_fit()
{
	if (cap != sz) {
		auto newCp = alloc.allocate(sz);
		size_t tempSz = sz;
		for (size_t i = 0; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		sz = cap = tempSz;
		cp = newCp;
	}
}

//Returns reference to the character at (pos) position
char& String::operator[](size_t pos)
{
	if (pos >= sz)
		throw out_of_range("Index is out of the range!");
	return *(cp + pos);
}

//Const version of char& String::operator[](size_t)
const char& String::operator[](size_t pos) const
{
	if (pos >= sz)
		throw out_of_range("Index is out of the range!");
	return *(cp + pos);
}

//The same as operator[]
char& String::at(size_t pos)
{
	if (pos >= sz)
		throw out_of_range("Index is out of the range!");
	return *(cp + pos);
}

//Const version of char& String::at(size_t)
const char& String::at(size_t pos) const
{
	if (pos >= sz)
		throw out_of_range("Index is out of the range!");
	return *(cp + pos);
}

//Returns reference to the last character of String, if String is empty, throws exception
char& String::back()
{
	if (empty())
		throw runtime_error("back() used on empty String!");
	return *(cp + sz - 1);   //last character is at the index sz - 1
}

//Const version of char& String::back()
const char& String::back() const
{
	if (empty())
		throw runtime_error("back() used on empty String!");
	return *(cp + sz - 1);
}

//Returns reference to the first character of String, if String is empty, throws exception
char& String::front()
{
	if (empty())
		throw runtime_error("back() used on empty String!");
	return *cp;
}

//Const version of char& String::front()
const char& String::front() const
{
	if (empty())
		throw runtime_error("back() used on empty String!");
	return *cp;
}

//Appends copy of the second String to the first
String& String::append(const String& str)
{
	size_t newSize = sz + str.sz;
	if (cap >= newSize) {
		for (size_t j = 0; j < str.sz; j++)
			alloc.construct(cp + sz++, *(str.cp + j));
	}
	else {
		auto newCp = alloc.allocate(newSize);
		size_t i;
		for (i = 0; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		for (size_t j = 0; j < str.sz; j++)
			alloc.construct(newCp + i++, *(str.cp + j));
		cp = newCp;
		sz = cap = newSize;
	}
	return *this;
}

//Appends copy of (sublen) characters of the second String, starting at the postion (subpos),
//if length isn't given, copies the entire String starting at position (subpos)
String& String::append(const String& str, size_t subpos, size_t sublen)
{
	if (subpos >= str.sz)
		throw out_of_range("Position out of the range!");

	//Calculate sublen, if it is higher than size (str.sz) at position (subpos)
	if ((str.sz - subpos) < sublen)
		sublen = str.sz - subpos;
	size_t newSize = sz + sublen;
	if (cap >= newSize) {
		for (size_t j = 0; j < sublen; j++)
			alloc.construct(cp + sz++, *(str.cp + j + subpos));
	}
	else {
		auto newCp = alloc.allocate(newSize);
		size_t i;
		for (i = 0; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		for (size_t j = 0; j < sublen; j++)
			alloc.construct(newCp + i++, *(str.cp + j + subpos));
		cp = newCp;
		sz = cap = newSize;
	}
	return *this;
}

//Appends const char* to String
String& String::append(const char* ptr)
{
	size_t ptrSize = strlen(ptr), newSize = sz + ptrSize;
	if (cap >= newSize) {
		for (size_t j = 0; j < ptrSize; j++)
			alloc.construct(cp + sz++, *(ptr + j));
	}
	else {
		auto newCp = alloc.allocate(newSize);
		size_t i;
		for (i = 0; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		for (size_t j = 0; j < ptrSize; j++)
			alloc.construct(newCp + i++, *(ptr + j));
		cp = newCp;
		sz = cap = newSize;
	}
	return *this;
}

//Appends (n) characters from const char* to String
String& String::append(const char* ptr, size_t n)
{
	if (n > strlen(ptr))
		n = strlen(ptr);
	size_t newSize = sz + n;
	if (cap >= newSize) {
		for (size_t j = 0; j < n; j++)
			alloc.construct(cp + sz++, *(ptr + j));
	}
	else {
		auto newCp = alloc.allocate(newSize);
		size_t i;
		for (i = 0; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		for (size_t j = 0; j < n; j++)
			alloc.construct(newCp + i++, *(ptr + j));
		cp = newCp;
		sz = cap = newSize;
	}
	return *this;
}

//Apends (n) copies of the character (ch) to String
String& String::append(size_t n, char ch)
{
	size_t newSize = sz + n;
	if (cap >= newSize) {
		for (size_t j = 0; j < n; j++)
			alloc.construct(cp + sz++, ch);
	}
	else {
		auto newCp = alloc.allocate(newSize);
		size_t i;
		for (i = 0; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		for (size_t j = 0; j < n; j++)
			alloc.construct(newCp + i++, ch);
		cp = newCp;
		sz = cap = newSize;
	}
	return *this;
}

//Appends copy of initializer_list to String
String& String::append(std::initializer_list<char> ls)
{
	size_t newSize = sz + ls.size();
	auto beg = ls.begin();
	size_t lstSize = ls.size();
	if (cap >= newSize) {
		for (size_t j = 0; j < lstSize; j++)
			alloc.construct(cp + sz++, *(beg + j));
	}
	else {
		auto newCp = alloc.allocate(newSize);
		size_t i;
		for (i = 0; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		for (size_t j = 0; j < lstSize; j++)
			alloc.construct(newCp + i++, *(beg + j));
		cp = newCp;
		sz = cap = newSize;
	}
	return *this;
}

//Returns String& String::append(const String&)
String& String::operator+=(const String& str)
{
	return append(str);
}

//Returns String& String::append(const char*)
String& String::operator+=(const char* ptr)
{
	return append(ptr);
}

//Returns String& String::append(size_t, char)
String& String::operator+=(char c)
{
	return append(1, c);
}

//Returns String& String::append(std::initializer_list<char>)
String& String::operator+=(std::initializer_list<char> ls)
{
	return append(ls);
}

//Pushes back character ch, if there is still unused capacity, it uses it,
//otherwise allocates new memory
void String::push_back(char ch)
{
	if (sz < cap) {
		alloc.construct(cp + sz, ch);
		sz++;
	}
	else {
		size_t newSize = sz + 1;
		auto newCp = alloc.allocate(newSize);
		for (size_t i = 0; i < sz; i++)
			alloc.construct(newCp + i, std::move(*(cp + i)));
		free();
		alloc.construct(newCp + newSize - 1, ch);
		cp = newCp;
		sz = cap = newSize;
	}
}

//Returns String& String::operator=(const String&)
String& String::assign(const String& str)
{
	return *this = str;
}

//Assigns (sublen) characters of the String (str) starting at the position (subpos)
//to this String
String& String::assign(const String& str, size_t subpos, size_t sublen)
{
	if (subpos >= str.sz)
		throw out_of_range("Position out of the range!");
	if ((str.sz - subpos) < sublen)
		sublen = str.sz - subpos;
	if (cap >= sublen) {
		for (int i = sz - 1; i >= 0; i--)
			alloc.destroy(cp + i);
		for (size_t j = 0; j < sublen; j++)
			alloc.construct(cp + j, *(str.cp + j + subpos));
		sz = sublen;
	}
	else {
		auto newCp = alloc.allocate(sublen);
		for (size_t i = 0; i < sublen; i++)
			alloc.construct(newCp + i, *(str.cp + i + subpos));
		free();
		cp = newCp;
		sz = cap = sublen;
	}
	return *this;
}

//Returns String& String::operator=(const char*)
String& String::assign(const char* ptr)
{
	return *this = ptr;
}

//Assigns first (n) characters of const char* to this String
String& String::assign(const char* ptr, size_t n)
{
	if (n > strlen(ptr))
		n = strlen(ptr);
	if (cap >= n) {
		for (int i = sz - 1; i >= 0; i--)
			alloc.destroy(cp + i);
		for (size_t j = 0; j < n; j++)
			alloc.construct(cp + j, *(ptr + j));
		sz = n;
	}
	else {
		auto newCp = alloc.allocate(n);
		free();
		for (size_t i = 0; i < n; i++)
			alloc.construct(newCp + i, *(ptr + i));
		cp = newCp;
		sz = cap = n;
	}
	return *this;
}

//Assigns (n) copies of character (ch) to this String
String& String::assign(size_t n, char ch)
{
	if (cap >= n) {
		for (int i = sz - 1; i >= 0; i--)
			alloc.destroy(cp + i);
		for (size_t j = 0; j < n; j++)
			alloc.construct(cp + j, ch);
		sz = n;
	}
	else {
		free();
		cp = alloc.allocate(n);
		for (size_t i = 0; i < n; i++)
			alloc.construct(cp + i, ch);
		sz = cap = n;
	}
	return *this;
}

//Assigns copy of the initializer_list<char> to this String
String& String::assign(std::initializer_list<char> ls)
{
	size_t lstSize = ls.size();
	auto beg = ls.begin();
	if (cap >= lstSize) {
		for (int i = sz - 1; i >= 0; i--)
			alloc.destroy(cp + i);
		for (size_t j = 0; j < lstSize; j++)
			alloc.construct(cp + j, *(beg + j));
		sz = lstSize;
	}
	else {
		free();
		sz = cap = lstSize;
		cp = alloc.allocate(sz);
		for (size_t i = 0; i < sz; i++)
			alloc.construct(cp + i, *(beg + i));
	}
	return *this;
}

//Assigns rvalue to this String
String& String::assign(String&& str) noexcept
{
	return operator=(std::move(str));
}

//Inserts second String into the first, at the given postition (pos)
String& String::insert(size_t pos, const String& str)
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	const size_t newSize = sz + str.sz;
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
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t stopPlace = i;
	for (size_t j = 0; j < str.sz; j++)
		alloc.construct(cp + i++, *(str.cp + j));
	for (; stopPlace < old.sz; stopPlace++)
		alloc.construct(cp + i++, *(old.cp + stopPlace));
	return *this;
}

//Inserts (sublen) characters of the String (str) to the position (pos), 
//starting copying at the posiition (subpos)
String& String::insert(size_t pos, const String& str, size_t subpos, size_t sublen)
{
	if (subpos >= str.sz)
		throw out_of_range("Position out of the range!");
	if ((str.sz - subpos) < sublen)
		sublen = str.sz - subpos;
	size_t newSize = sz + sublen;
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
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t stopPlace = i;
	for (size_t j = 0; j < sublen; j++)
		alloc.construct(cp + i++, *(str.cp + j + subpos));
	for (; stopPlace < old.sz; stopPlace++)
		alloc.construct(cp + i++, *(old.cp + stopPlace));
	return *this;
}

//Inserts const char* to this String, at the given position (pos)
String& String::insert(size_t pos, const char* ptr)
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	const size_t ptrSize = strlen(ptr);
	const size_t newSize = sz + ptrSize;
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
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t stopPlace = i;
	for (size_t j = 0; j < ptrSize; j++)
		alloc.construct(cp + i++, *(ptr + j));
	for (; stopPlace < old.sz; stopPlace++)
		alloc.construct(cp + i++, *(old.cp + stopPlace));
	return *this;
}

//Inserts copy of the first (n) characters of const char* to the given position (pos) 
//of this String
String& String::insert(size_t pos, const char* ptr, size_t n)
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (n > strlen(ptr))
		n = strlen(ptr);
	const size_t newSize = sz + n;
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
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t stopPlace = i;
	for (size_t j = 0; j < n; j++)
		alloc.construct(cp + i++, *(ptr + j));
	for (; stopPlace < old.sz; stopPlace++)
		alloc.construct(cp + i++, *(old.cp + stopPlace));
	return *this;
}

//Inserts (n) copies of character (ch) to the String at the given position (pos)
String& String::insert(size_t pos, size_t n, char ch)
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	const size_t newSize = sz + n;
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
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t stopPlace = i;
	for (size_t j = 0; j < n; j++)
		alloc.construct(cp + i++, ch);
	for (; stopPlace < old.sz; stopPlace++)
		alloc.construct(cp + i++, *(old.cp + stopPlace));
	return *this;
}

/* Inserts (n) copies of the given character in place where iterator points */
String::iterator String::insert(const_iterator p, size_t n, char ch)
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
			return end();
	}

	const size_t newSize = sz + n;
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

	size_t i = 0;
	for (; i < index; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t stopPlace = i;
	for (size_t j = 0; j < n; j++)
		alloc.construct(cp + i++, ch);
	for (; stopPlace < old.sz; stopPlace++)
		alloc.construct(cp + i++, *(old.cp + stopPlace));
	return (begin() + index + n);
}

/* Inserts the given character in the place where iterator points to */
String::iterator String::insert(const_iterator p, char ch)
{
	return insert(p, 1, ch);
}

/* Inserts the given initializer list in the place where iterator points to */
String& String::insert(const_iterator p, std::initializer_list<char> lst)
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
			return *this;
	}

	const size_t newSize = sz + lst.size();
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

	size_t i = 0;
	for (; i < index; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t stopPlace = i;
	auto beg = lst.begin();
	for (size_t j = 0; j < lst.size(); j++)
		alloc.construct(cp + i++, *beg++);
	for (; stopPlace < old.sz; stopPlace++)
		alloc.construct(cp + i++, *(old.cp + stopPlace));
	return *this;
}

//Erases (len) characters from this String, starting from the given position (pos)
String& String::erase(size_t pos, size_t len)
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > sz)
		len = sz - pos;
	String old(*this);
	if (cp) {
		for (int i = sz - 1; i >= 0; i--) 
			alloc.destroy(cp + i);
	}
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t rest = i + len;
	for (; rest < sz; rest++)
		alloc.construct(cp + i++, *(old.cp + rest));
	sz -= len;
	return *this;
}

/* Erases the character in this String pointed by the given iterator */
String::iterator String::erase(const_iterator p)
{
	bool match = false;
	size_t index = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (p == beg) {
			match = true;
			break;
		}
	}
	if (match == false)
		return end();

	String old(*this);
	for (int i = sz - 1; i >= 0; i--)
		alloc.destroy(cp + i);
	size_t i = 0;
	for (; i < index; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t rest = i + 1;
	for (; rest < sz; rest++, i++)
		alloc.construct(cp + i, *(old.cp + rest));
	sz -= 1;
	return iterator(cp + index);
}


/* Erases the characters in this String pointed by the given iterators range */
String::iterator String::erase(const_iterator first, const_iterator last)
{
	bool match1 = false;
	bool match2 = false;
	size_t index_last = 0, index = 0;
	size_t index_first = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (first == beg) {
			match1 = true;
			index_first = index;
		}
		if (last == beg) {
			match2 = true;
			index_last = index;
		}
	}
	if (match2 == false) {
		match2 = (last == end());
		index_last = index;
	}
	if (match1 == false || match2 == false)
		return end();

	String old(*this);
	for (int i = sz - 1; i >= 0; i--)
		alloc.destroy(cp + i);
	size_t i = 0;
	for (; i < index_first; i++)
		alloc.construct(cp + i, *(old.cp + i));
	size_t rest = i + (index_last - index_first);
	for (; rest < sz; rest++, i++)
		alloc.construct(cp + i, *(old.cp + rest));
	sz -= (index_last - index_first);
	return iterator(cp + index_first);
}

//Swaps the contents of the String, updates the size and capacity
void String::swap(String& str)
{
	std::swap(cp, str.cp);
	std::swap(sz, str.sz);
	std::swap(cap, str.cap);
}

//Replaces (len) characters of the String, starting at (pos) with the second String
String& String::replace(size_t pos, size_t len, const String& str)
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;
	size_t newSize = sz - len + str.sz;

	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < pos; i++) //move the old text that is before (pos)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + len;
	for (size_t j = 0; j < str.sz; j++) //copy text from the second String
		alloc.construct(newCp + i++, *(str.cp + j));
	for (size_t j = left; j < sz; j++) //move the rest of the old text
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

/* Replaces the given range with the copy of the given String */
String& String::replace(const_iterator first, const_iterator last, const String& str)
{
	bool match1 = false;
	bool match2 = false;
	size_t index_last = 0, index = 0;
	size_t index_first = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (first == beg) {
			match1 = true;
			index_first = index;
		}
		if (last == beg) {
			match2 = true;
			index_last = index;
		}
	}
	if (match2 == false) {
		match2 = (last == end());
		index_last = index;
	}
	if (match1 == false || match2 == false)
		return *this;

	size_t newSize = sz - (index_last - index_first) + str.sz;
	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < index_first; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + (index_last - index_first);

	for (size_t j = 0; j < str.sz; j++)
		alloc.construct(newCp + i++, *(str.cp + j));
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

//Replaces (len) character of this String, starting at (pos), with (sublen) characters
//of the Second string, starting at the position (subpos)
String& String::replace(size_t pos, size_t len, const String& str, size_t subpos, size_t sublen)
{
	if (pos >= sz || subpos >= str.sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;
	if (sublen > (str.sz - subpos))
		sublen = sz - subpos;
	size_t newSize = sz - len + sublen;

	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + len;
	for (size_t j = 0; j < sublen; j++)
		alloc.construct(newCp + i++, *(str.cp + j + subpos));
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

//Replaces (len) characters of this String, starting at (pos), with const char*
String& String::replace(size_t pos, size_t len, const char* cptr)
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;
	size_t ptrSize = strlen(cptr);
	size_t newSize = sz - len + ptrSize;

	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + len;
	for (size_t j = 0; j < ptrSize; j++)
		alloc.construct(newCp + i++, *(cptr + j));
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

/* Replaces the given range with the copy of the const char* */
String& String::replace(const_iterator first, const_iterator last, const char* cptr)
{
	bool match1 = false;
	bool match2 = false;
	size_t index_last = 0, index = 0;
	size_t index_first = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (first == beg) {
			match1 = true;
			index_first = index;
		}
		if (last == beg) {
			match2 = true;
			index_last = index;
		}
	}
	if (match2 == false) {
		match2 = (last == end());
		index_last = index;
	}
	if (match1 == false || match2 == false)
		return *this;

	size_t ptrSize = strlen(cptr);
	size_t newSize = sz - (index_last - index_first) + ptrSize;
	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < index_first; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + (index_last - index_first);

	for (size_t j = 0; j < ptrSize; j++)
		alloc.construct(newCp + i++, *(cptr + j));
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

//Replaces (len) characters of this String, starting at (pos), with (n) characters
//of const char*
String& String::replace(size_t pos, size_t len, const char* cptr, size_t n)
{
	size_t ptrSize = strlen(cptr);
	if (n > ptrSize)
		n = ptrSize;
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;
	size_t newSize = sz - len + n;

	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + len;
	for (size_t j = 0; j < n; j++)
		alloc.construct(newCp + i++, *(cptr + j));
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

/* Replaces the given range with the copy of the (n) chatacters of const char* */
String& String::replace(const_iterator first, const_iterator last, const char* cptr, size_t n)
{
	bool match1 = false;
	bool match2 = false;
	size_t index_last = 0, index = 0;
	size_t index_first = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (first == beg) {
			match1 = true;
			index_first = index;
		}
		if (last == beg) {
			match2 = true;
			index_last = index;
		}
	}
	if (match2 == false) {
		match2 = (last == end());
		index_last = index;
	}
	if (match1 == false || match2 == false)
		return *this;

	size_t ptrSize = strlen(cptr);
	if (n > ptrSize)
		n = ptrSize;

	size_t newSize = sz - (index_last - index_first) + n;
	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < index_first; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + (index_last - index_first);

	for (size_t j = 0; j < n; j++)
		alloc.construct(newCp + i++, *(cptr + j));
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

//Replaces (len) characters of this String, starting at (pos), with (n) characters (ch)
String& String::replace(size_t pos, size_t len, size_t n, char ch)
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;
	size_t newSize = sz - len + n;

	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < pos; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + len;
	for (size_t j = 0; j < n; j++)
		alloc.construct(newCp + i++, ch);
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

/* Replaces the given range with (n) copies of the given character */
String& String::replace(const_iterator first, const_iterator last, size_t n, char ch)
{
	bool match1 = false;
	bool match2 = false;
	size_t index_last = 0, index = 0;
	size_t index_first = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (first == beg) {
			match1 = true;
			index_first = index;
		}
		if (last == beg) {
			match2 = true;
			index_last = index;
		}
	}
	if (match2 == false) {
		match2 = (last == end());
		index_last = index;
	}
	if (match1 == false || match2 == false)
		return *this;

	size_t newSize = sz - (index_last - index_first) + n;
	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < index_first; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + (index_last - index_first);

	for (size_t j = 0; j < n; j++)
		alloc.construct(newCp + i++, ch);
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

/* Replaces the given range with the copy of the initializer list */
String& String::replace(const_iterator first, const_iterator last, std::initializer_list<char> lst)
{
	bool match1 = false;
	bool match2 = false;
	size_t index_last = 0, index = 0;
	size_t index_first = 0;
	for (iterator beg = begin(); beg != end(); beg++, index++) {
		if (first == beg) {
			match1 = true;
			index_first = index;
		}
		if (last == beg) {
			match2 = true;
			index_last = index;
		}
	}
	if (match2 == false) {
		match2 = (last == end());
		index_last = index;
	}
	if (match1 == false || match2 == false)
		return *this;

	size_t lstSize = lst.size();
	size_t newSize = sz - (index_last - index_first) + lstSize;
	auto newCp = alloc.allocate((newSize > cap) ? newSize : cap);
	size_t i = 0;
	for (; i < index_first; i++)
		alloc.construct(newCp + i, std::move(*(cp + i)));
	size_t left = i + (index_last - index_first);

	auto beg = lst.begin();
	for (size_t j = 0; j < lstSize; j++)
		alloc.construct(newCp + i++, *beg++);
	for (size_t j = left; j < sz; j++)
		alloc.construct(newCp + i++, std::move(*(cp + j)));

	free();
	sz = cap = newSize;
	cp = newCp;
	return *this;
}

//Erases the last character of this String
void String::pop_back()
{
	alloc.destroy(cp + --sz);
}

/* Returns const char*, that points to the same place as String, adds null to the end */
const char* String::c_str()
{
	if (cap == sz) {
		char* newCp = alloc.allocate(sz + 1);
		size_t size = sz;
		uninitialized_copy(cp, cp + sz, newCp);
		newCp[sz] = '\0';
		free();
		sz = size;
		cp = newCp;
		cap = sz;
	}

	else
		cp[sz] = '\0';
	return cp;
}

/* Returns const char*, that points to the same place as String, adds null to the end */
const char* String::data()
{
	return this->c_str();
}

/* Returns the copy of the allocator, that this String uses */
String::allocator_type String::get_allocator() const noexcept
{
	return alloc;
}

/* Copies (len) characters of the String, starting at the position (pos), to the char* */
size_t String::copy(char* cptr, size_t len, size_t pos) const
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = (sz - pos);
	for (size_t i = 0, p = pos; i < len; p++)
		cptr[i++] = *(cp + p);
	return len;
}

/* Finds given String, in this String, starting at the given position */
size_t String::find(const String& str, size_t pos) const noexcept
{
	if (!str.cp)
		return npos;
	bool matched = false;
	for (size_t i = pos; i < sz; i++) {
		if (*(cp + i) == *(str.cp)) {
			matched = true;
			size_t found = i;
			for (size_t j = 1; j < str.sz; j++) {
				i++;
				if (i >= sz || *(cp + i) != *(str.cp + j)) {
					matched = false;
					break;
				}
			}
			if (matched)
				return found;
		}
	}
	return npos;
}

/* Finds given text, in this String, starting at the given position */
size_t String::find(const char* cptr, size_t pos) const
{
	if (!cptr)
		return npos;
	bool matched = false;
	size_t ptrSize = strlen(cptr);
	for (size_t i = pos; i < sz; i++) {
		if (*(cp + i) == *(cptr)) {
			matched = true;
			size_t found = i;
			for (size_t j = 1; j < ptrSize; j++) {
				i++;
				if (i >= sz || *(cp + i) != *(cptr + j)) {
					matched = false;
					break;
				}
			}
			if (matched)
				return found;
		}
	}
	return npos;
}

/* Finds (n) characters of the given text, in this String, starting at the given position */
size_t String::find(const char* cptr, size_t pos, size_t n) const
{
	if (!cptr)
		return npos;
	bool matched = false;
	if (n > strlen(cptr))
		n = strlen(cptr);
	for (size_t i = pos; i < sz; i++) {
		if (*(cp + i) == *(cptr)) {
			matched = true;
			size_t found = i;
			for (size_t j = 1; j < n; j++) {
				i++;
				if (i >= sz || *(cp + i) != *(cptr + j)) {
					matched = false;
					break;
				}
			}
			if (matched)
				return found;
		}
	}
	return npos;
}

/* Finds given character, in this String, starting at the position pos */
size_t String::find(char c, size_t pos) const noexcept
{
	for (size_t i = pos; i < sz; i++) {
		if (*(cp + i) == c)
			return i;
	}
	return npos;
}

/* Finds the last copy of the given String, in this String, starting at the given position (pos) */
size_t String::rfind(const String& str, size_t pos) const noexcept
{
	if (!str.cp)
		return false;
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		if (*(cp + i) == *(str.cp + (str.sz - 1))) {
			long long int found = i - str.sz + 1;
			for (long long int j = (long long int) str.sz - 2; j >= 0; j--) {
				i--;
				if ((i == 0 || j == 0) && (*(cp + i) == *(str.cp + j)))
					return (size_t)found;

				if (*(cp + i) != *(str.cp + j))
					break;
			}
		}
	}
	return npos;
}

/* Finds the last copy of the given text, in this String, starting at the given position (pos) */
size_t String::rfind(const char* cptr, size_t pos) const
{
	if (!cptr)
		return false;
	long long int ptrSize = strlen(cptr);
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		if (*(cp + i) == *(cptr + (ptrSize - 1))) {
			long long int found = i - ptrSize + 1;
			for (long long int j = (long long int) ptrSize - 2; j >= 0; j--) {
				i--;
				if ((i == 0 || j == 0) && (*(cp + i) == *(cptr + j)))
					return (size_t)found;

				if (*(cp + i) != *(cptr + j))
					break;
			}
		}
	}
	return npos;
}

/* Finds the last copy of the (n) characters of the given text, in this String, starting at the given position (pos) */
size_t String::rfind(const char* cptr, size_t pos, size_t n) const
{
	if (!cptr)
		return false;
	if (n > strlen(cptr))
		n = strlen(cptr);
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		if (*(cp + i) == *(cptr + (n - 1))) {
			long long int found = i - n + 1;
			for (long long int j = (long long int) n - 2; j >= 0; j--) {
				i--;
				if ((i == 0 || j == 0) && (*(cp + i) == *(cptr + j)))
					return (size_t)found;

				if (*(cp + i) != *(cptr + j))
					break;
			}
		}
	}
	return npos;
}

/* Finds the copy of the character, in this String, starting at the given position (pos) */
size_t String::rfind(char c, size_t pos) const noexcept
{
	for (long long int i = (long long int) sz - 1; i >= pos; i++) {
		if (*(cp + i) == c)
			return (size_t)i;
	}
	return npos;
}

/* Finds the first character that matches one of the characters in the String, 
starting at the given position (pos), returns its index */
size_t String::find_first_of(const String& str, size_t pos) const noexcept
{
	for (size_t i = pos; i < sz; i++) {
		for (size_t j = 0; j < str.sz; j++) {
			if (*(cp + i) == *(cp + j))
				return i;
		}
	}
	return npos;
}

/* Finds the first character that matches one of the characters of the text, 
starting at the given position (pos), returns its index */
size_t String::find_first_of(const char* cptr, size_t pos) const
{
	size_t ptrSize = strlen(cptr);
	for (size_t i = pos; i < sz; i++) {
		for (size_t j = 0; j < ptrSize; j++) {
			if (*(cp + i) == *(cptr + j))
				return i;
		}
	}
	return npos;
}

/* Finds the first character that matches one of the (n) characters of the text,
starting at the given position (pos), returns its index */
size_t String::find_first_of(const char* cptr, size_t pos, size_t n) const
{
	if (n > strlen(cptr))
		n = strlen(cptr);
	for (size_t i = pos; i < sz; i++) {
		for (size_t j = 0; j < n; j++) {
			if (*(cp + i) == *(cptr + j))
				return i;
		}
	}
	return npos;
}

/* Finds the first character that matches the given character
starting at the given position (pos), returns its index */
size_t String::find_first_of(char ch, size_t pos) const noexcept
{
	for (size_t i = pos; i < sz; i++) {
		if (*(cp + i) == ch)
			return i;
	}
	return npos;
}

/* Finds the last character that matches one of the characters in the String, 
starting at the given position (pos), returns its index */
size_t String::find_last_of(const String& str, size_t pos) const noexcept
{
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		for (size_t j = 0; j < str.sz; j++) {
			if (*(cp + i) == *(str.cp + j))
				return (size_t) i;
		}
	}
	return npos;
}

/* Finds the last character that matches one of the characters of the text,
starting at the given position (pos), returns its index */
size_t String::find_last_of(const char* cptr, size_t pos) const
{
	size_t ptrSize = strlen(cptr);
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		for (size_t j = 0; j < ptrSize; j++) {
			if (*(cp + i) == *(cptr + j))
				return (size_t) i;
		}
	}
	return npos;
}

/* Finds the last character that matches one of the (n) characters of the text,
starting at the given position (pos), returns its index */
size_t String::find_last_of(const char* cptr, size_t pos, size_t n) const
{
	if (n > strlen(cptr))
		n = strlen(cptr);
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		for (size_t j = 0; j < n; j++) {
			if (*(cp + i) == *(cptr + j))
				return (size_t) i;
		}
	}
	return npos;
}

/* Finds the last character that matches the given character
starting at the given position (pos), returns its index */
size_t String::find_last_of(char ch, size_t pos) const noexcept
{
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		if (*(cp + i) == ch)
			return (size_t) i;
	}
	return npos;
}

/* Finds the first character that isn't in the given String, starting at the given position (pos), returns its index */
size_t String::find_first_not_of(const String& str, size_t pos) const noexcept
{
	size_t counter;
	for (size_t i = pos; i < sz; i++) {
		for (counter = 0; counter < str.sz; counter++) {
			if (*(cp + i) == *(str.cp + counter))
				break;
		}
		if (counter == str.sz)
			return i;
	}
	return npos;
}

/* Finds the first character that isn't in the given text, starting at the given position (pos), returns its index */
size_t String::find_first_not_of(const char* cptr, size_t pos) const
{
	size_t ptrSize = strlen(cptr);
	size_t counter;
	for (size_t i = pos; i < sz; i++) {
		for (counter = 0; counter < ptrSize; counter++) {
			if (*(cp + i) == *(cptr + counter))
				break;
		}
		if (counter == ptrSize)
			return i;
	}
	return npos;
}

/* Finds the first character that isn't in the (n) characters of the given text,
starting at the given position (pos), returns it index */
size_t String::find_first_not_of(const char* cptr, size_t pos, size_t n) const
{
	if (n > strlen(cptr))
		n = strlen(cptr);
	size_t counter;
	for (size_t i = pos; i < sz; i++) {
		for (counter = 0; counter < n; counter++) {
			if (*(cp + i) == *(cptr + counter))
				break;
		}
		if (counter == n)
			return i;
	}
	return npos;
}

/* Finds the first character that isn't the given character, returns it index */
size_t String::find_first_not_of(char ch, size_t pos) const noexcept
{
	for (size_t i = pos; i < sz; i++) {
		if (*(cp + i) != ch)
			return i;
	}
	return npos;
}

/* Finds the last character that isn't in the given String, starting at the given position (pos), returns its index */
size_t String::find_last_not_of(const String& str, size_t pos) const noexcept
{
	size_t counter;
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		for (counter = 0; counter < str.sz; counter++) {
			if (*(cp + i) == *(str.cp + counter))
				break;
		}
		if (counter == str.sz)
			return (size_t) i;
	}
	return npos;
}

/* Finds the last character that isn't in the given text, starting at the given position (pos), returns its index */
size_t String::find_last_not_of(const char* cptr, size_t pos) const
{
	size_t ptrSize = strlen(cptr);
	size_t counter;
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		for (counter = 0; counter < ptrSize; counter++) {
			if (*(cp + i) == *(cptr + counter))
				break;
		}
		if (counter == ptrSize)
			return (size_t) i;
	}
	return npos;
}

/* Finds the last character that isn't in the (n) characters of the given text,
starting at the given position (pos), returns it index */
size_t String::find_last_not_of(const char* cptr, size_t pos, size_t n) const
{
	if (n > strlen(cptr))
		n = strlen(cptr);
	size_t counter;
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		for (counter = 0; counter < n; counter++) {
			if (*(cp + i) == *(cptr + counter))
				break;
		}
		if (counter == n)
			return (size_t) i;
	}
	return npos;
}

/* Finds the last character that isn't the given character, returns it index */
size_t String::find_last_not_of(char ch, size_t pos) const noexcept
{
	for (long long int i = (long long int) sz - 1; i >= pos; i--) {
		if (*(cp + i) != ch)
			return (size_t) i;
	}
	return npos;
}

/* Returns a copy of the (len) characters of this String, starting at the position (pos)*/
String String::substr(size_t pos, size_t len) const
{
	if (pos == sz)
		return String();
	else if (pos > sz)
		out_of_range("Position out of range!");
	if (len > sz - pos)
		len = sz - pos;
	String result;
	result.reserve(len);
	for (size_t i = 0; i < len; i++)
		result += *(cp + i + pos);
	return result;
}

//Compares one String to the other, 
// returns:
//	1 if this String is greater than str
//	-1 if this String is lower than str
//	0 if two Strings are equal
int String::compare(const String& str) const noexcept
{
	for (size_t i = 0; i < sz && i < str.sz; i++) {
		if (*(cp + i) > *(str.cp + i))
			return 1;
		if (*(cp + i) < *(str.cp + i))
			return -1;
	}
	
	if (sz > str.sz)
		return 1;
	if (sz < str.sz)
		return -1;
	return 0;
}

//Compares one (len) characters of this String 
// starting at the position (pos) to the other, 
// returns:
//	1 if this String is greater than str
//	-1 if this String is lower than str
//	0 if two Strings are equal
int String::compare(size_t pos, size_t len, const String & str) const
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;

	for (size_t i = 0; i < len && i < str.sz; i++) {
		if (*(cp + pos + i) > *(str.cp + i))
			return 1;
		if (*(cp + pos + i) < *(str.cp + i))
			return -1;
	}

	if (len > str.sz)
		return 1;
	if (len < str.sz)
		return -1;
	return 0;
}

//Compares one (len) characters of this String 
// starting at the position (pos) to the (sublen) characters of the other String, 
// starting at the position (subpos), if sublen isn't given, it is equal to String length - position (npos), 
// returns:
//	1 if this String is greater than str
//	-1 if this String is lower than str
//	0 if two Strings are equal
int String::compare(size_t pos, size_t len, const String& str, size_t subpos, size_t sublen) const
{
	if (pos >= sz || subpos >= str.sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;
	if (sublen > (str.sz - subpos))
		sublen = str.sz - subpos;

	for (size_t i = 0; i < len && i < sublen; ++i) {
		if (*(cp + pos + i) > *(str.cp + subpos + i))
			return 1;
		if (*(cp + pos + i) < *(str.cp + subpos + i))
			return -1;
	}

	if (len > sublen)
		return 1;
	if (len < sublen)
		return -1;
	return 0;
}

//Compares this String to the const char*, 
// returns:
//	1 if this String is greater than const char*
//	-1 if this String is lower than const char*
//	0 if this and const char* are equal
int String::compare(const char* cptr) const
{
	size_t sizeCptr = strlen(cptr);
	for (size_t i = 0; i < sz && i < sizeCptr; ++i) {
		if (*(cp + i) > *(cptr + i))
			return 1;
		if (*(cp + i) < *(cptr + i))
			return -1;
	}

	if (sz > sizeCptr)
		return 1;
	if (sz < sizeCptr)
		return -1;
	return 0;
}

//Compares this (len) characters of the String, 
//starting at the position (pos), to the const char*, 
// returns:
//	1 if this String is greater than const char*
//	-1 if this String is lower than const char*
//	0 if this and const char* are equal
int String::compare(size_t pos, size_t len, const char* cptr) const
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;

	size_t sizeCptr = strlen(cptr);
	for (size_t i = 0; i < len && i < sizeCptr; ++i) {
		if (*(cp + pos + i) > *(cptr + i))
			return 1;
		if (*(cp + pos + i) < *(cptr + i))
			return -1;
	}

	if (len > sizeCptr)
		return 1;
	if (len < sizeCptr)
		return -1;
	return 0;
}

//Compares this (len) characters of the String, 
//starting at the position (pos), to the (n) characters of const char*, 
// returns:
//	1 if this String is greater than const char*
//	-1 if this String is lower than const char*
//	0 if this and const char* are equal
int String::compare(size_t pos, size_t len, const char* cptr, size_t n) const
{
	if (pos >= sz)
		throw out_of_range("Position out of range!");
	if (len > (sz - pos))
		len = sz - pos;
	size_t sizeCptr = strlen(cptr);
	if (n > sizeCptr)
		n = sizeCptr;

	for (size_t i = 0; i < len && i < n; ++i) {
		if (*(cp + pos + i) > *(cptr + i))
			return 1;
		if (*(cp + pos + i) < *(cptr + i))
			return -1;
	}

	if (len > n)
		return 1;
	if (len < n)
		return -1;
	return 0;
}

//Returns local String copy by appending one String to the other
String operator+(const String& lhs, const String& rhs)
{
	String result;
	result.sz = result.cap = lhs.sz + rhs.sz;
	result.cp = result.alloc.allocate(result.sz);
	size_t i;
	for (i = 0; i < lhs.sz; i++)
		result.alloc.construct(result.cp + i, *(lhs.cp + i));
	for (size_t j = 0; j < rhs.sz; j++)
		result.alloc.construct(result.cp + i++, *(rhs.cp + j));
	return result;
}

//Returns copy of the local String built by appending one String, to other rvalue String
String operator+(const String& lhs, String&& rhs)
{
	String result;
	result.sz = result.cap = lhs.sz + rhs.sz;
	result.cp = result.alloc.allocate(result.sz);
	size_t i;
	for (i = 0; i < lhs.sz; i++)
		result.alloc.construct(result.cp + i, *(lhs.cp + i));
	for (size_t j = 0; j < rhs.sz; j++)
		result.alloc.construct(result.cp + i++, std::move(*(rhs.cp + j)));
	return result;
}

//Same as String operator+(const String&, String&&), but it has 
//diffrent order of the parameters
String operator+(String&& lhs, const String& rhs)
{
	String result;
	result.sz = result.cap = lhs.sz + rhs.sz;
	result.cp = result.alloc.allocate(result.sz);
	size_t i;
	for (i = 0; i < lhs.sz; i++)
		result.alloc.construct(result.cp + i, std::move(*(lhs.cp + i)));
	for (size_t j = 0; j < rhs.sz; j++)
		result.alloc.construct(result.cp + i++, *(rhs.cp + j));
	return result;
}

//Same as String operator+(const String&, String&&), but uses rvalues
String operator+(String&& lhs, String&& rhs)
{
	String result;
	result.sz = result.cap = lhs.sz + rhs.sz;
	result.cp = result.alloc.allocate(result.sz);
	size_t i;
	for (i = 0; i < lhs.sz; i++)
		result.alloc.construct(result.cp + i, std::move(*(lhs.cp + i)));
	for (size_t j = 0; j < rhs.sz; j++)
		result.alloc.construct(result.cp + i++, std::move(*(rhs.cp + j)));
	return result;
}

//Returns copy of the String constructed by appending String and const char*
String operator+(const String& lhs, const char* rhs)
{
	String result;
	size_t ptrSize = strlen(rhs);
	result.sz = result.cap = lhs.sz + ptrSize;
	result.cp = result.alloc.allocate(result.sz);
	size_t i;
	for (i = 0; i < lhs.sz; i++)
		result.alloc.construct(result.cp + i, *(lhs.cp + i));
	for (size_t j = 0; j < ptrSize; j++)
		result.alloc.construct(result.cp + i++, *(rhs + j));
	return result;
}

//Same as String operator+(const String&, const char*), but it uses rvalue
String operator+(String&& lhs, const char* rhs)
{
	String result;
	size_t ptrSize = strlen(rhs);
	result.sz = result.cap = lhs.sz + ptrSize;
	result.cp = result.alloc.allocate(result.sz);
	size_t i;
	for (i = 0; i < lhs.sz; i++)
		result.alloc.construct(result.cp + i, std::move(*(lhs.cp + i)));
	for (size_t j = 0; j < ptrSize; j++)
		result.alloc.construct(result.cp + i++, *(rhs + j));
	return result;
}

//Same as String operator+(const String&, const char*), but it has 
//diffrent order of the parameter list
String operator+(const char* lhs, const String& rhs)
{
	String result;
	size_t ptrSize = strlen(lhs);
	result.sz = result.cap = ptrSize + rhs.sz;
	result.cp = result.alloc.allocate(result.sz);
	size_t i;
	for (i = 0; i < ptrSize; i++)
		result.alloc.construct(result.cp + i, *(lhs + i));
	for (size_t j = 0; j < rhs.sz; j++)
		result.alloc.construct(result.cp + i++, *(rhs.cp + j));
	return result;
}

//Same as String operator+(const char*, const String&), but uses rvalue
String operator+(const char* lhs, String&& rhs)
{
	String result;
	size_t ptrSize = strlen(lhs);
	result.sz = result.cap = ptrSize + rhs.sz;
	result.cp = result.alloc.allocate(result.sz);
	size_t i;
	for (i = 0; i < ptrSize; i++)
		result.alloc.construct(result.cp + i, *(lhs + i));
	for (size_t j = 0; j < rhs.sz; j++)
		result.alloc.construct(result.cp + i++, std::move(*(rhs.cp + j)));
	return result;
}

//Returns copy of the String, that is created by appending String and char
String operator+(const String& lhs, char rhs)
{
	String result;
	result.sz = result.cap = lhs.sz + 1;
	result.cp = result.alloc.allocate(result.sz);
	for (size_t i = 0; i < lhs.sz; i++)
		result.alloc.construct(result.cp + i, *(lhs.cp + i));
	result.alloc.construct(result.cp + lhs.sz, rhs);
	return result;
}

//Same as String operator+(const String&, char), but uses rvalue
String operator+(String&& lhs, char rhs)
{
	String result;
	result.sz = result.cap = lhs.sz + 1;
	result.cp = result.alloc.allocate(result.sz);
	for (size_t i = 0; i < lhs.sz; i++)
		result.alloc.construct(result.cp + i, std::move(*(lhs.cp + i)));
	result.alloc.construct(result.cp + lhs.sz, rhs);
	return result;
}

//Same as String operator+(const String&, char), but it has
//diffrent parameters order
String operator+(char lhs, const String& rhs) 
{
	String result;
	result.sz = result.cap = rhs.sz + 1;
	result.cp = result.alloc.allocate(result.sz);
	result.alloc.construct(result.cp, lhs);
	size_t size = 1;
	for (size_t i = 0; i < rhs.sz; i++)
		result.alloc.construct(result.cp + size++, *(rhs.cp + i));
	return result;
}

//Same as String operator+(char, const String&), but uses rvalue
String operator+(char lhs, String&& rhs)
{
	String result;
	result.sz = result.cap = rhs.sz + 1;
	result.cp = result.alloc.allocate(result.sz);
	result.alloc.construct(result.cp, lhs);
	size_t size = 1;
	for (size_t i = 0; i < rhs.sz; i++)
		result.alloc.construct(result.cp + size++, std::move(*(rhs.cp + i)));
	return result;
}

//Checks if one String is equal to the other
bool operator==(const String& lhs, const String& rhs) noexcept
{
	return (lhs.compare(rhs) == 0);
}

//Checks if const char* is equal to the String
bool operator==(const char* lhs, const String& rhs)
{
	return (rhs.compare(lhs) == 0);
}

//Checks if String is equal to the const char*
bool operator==(const String& lhs, const char* rhs)
{
	return (lhs.compare(rhs) == 0);
}

//Checks if one String isn't equal to the other
bool operator!=(const String& lhs, const String& rhs) noexcept
{
	return !(lhs == rhs);
}

//Checks if const char* isn't equal to the String
bool operator!=(const char* lhs, const String& rhs)
{
	return !(lhs == rhs);
}

//Checks if String isn't equal to the const char*
bool operator!=(const String& lhs, const char* rhs)
{
	return !(lhs == rhs);
}

//Checks if the first String is lesser than the other one
bool operator<(const String& lhs, const String& rhs) noexcept
{
	return (lhs.compare(rhs) < 0);
}

//Checks if the const char* is lesser than the String
bool operator<(const char* lhs, const String& rhs)
{
	return (rhs.compare(lhs) > 0);
}

//Checks if the String is lesser than the const char*
bool operator<(const String& lhs, const char* rhs)
{
	return (lhs.compare(rhs) < 0);
}

//Checks if the first String is lesser than or equal to the second String
bool operator<=(const String& lhs, const String& rhs) noexcept
{
	return (lhs < rhs) || (lhs == rhs);
}

//Checks if the const char* is lesser than or equal to the String
bool operator<=(const char* lhs, const String& rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

//Checks if the String is lesser than or equal to the const char*
bool operator<=(const String& lhs, const char* rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

//Checks if the first String is greater than the other one
bool operator>(const String& lhs, const String& rhs) noexcept
{
	return !(lhs < rhs) && (lhs != rhs);
}

//Checks if the const char* is greater than the String
bool operator>(const char* lhs, const String& rhs)
{
	return !(lhs < rhs) && (lhs != rhs);
}

//Checks if the String is greater than the const char*
bool operator>(const String& lhs, const char* rhs)
{
	return !(lhs < rhs) && (lhs != rhs);
}

//Checks if the first String is greater than or equal to the second String
bool operator>=(const String& lhs, const String& rhs) noexcept
{
	return (lhs > rhs) || (lhs == rhs);
}

//Checks if the const char* is greater than or equal to the String
bool operator>=(const char* lhs, const String& rhs)
{
	return (lhs > rhs) || (lhs == rhs);
}

//Checks if the String is greater than or equal to the const char*
bool operator>=(const String& lhs, const char* rhs)
{
	return (lhs > rhs) || (lhs == rhs);
}

//Uses void String::swap(String&) to swap Strings contents
void swap(String& lhs, String& rhs)
{
	lhs.swap(rhs);
}

/* Reads characters into String from the input stream */
std::istream& operator>>(std::istream& is, String& str)
{
	if (!is)
		return is;
	char ch;
	str.clear();
	while (ch = is.get()) {
		if (std::isspace(ch))
			break;
		str.push_back(ch);
	}
	return is;
}

//Writes String's contents to output stream
std::ostream& operator<<(std::ostream& os, const String& str)
{
	if (!os)
		return os;
	for (size_t i = 0; i < str.sz; i++)
		os << *(str.cp + i);
	return os;
}

/* Reads the entire lines from the input stream, up to the (delim) character into the String */
std::istream& getline(std::istream& is, String& str, char delim)
{
	if (!is)
		return is;
	str.clear();
	char ch;
	while (is.get(ch) && ch != delim)
		str.push_back(ch);
	return is;
}

/* Reads the entire lines from the input stream, up to the (delim) character into the String */
std::istream& getline(std::istream&& is, String& str, char delim)
{
	if (!is)
		return is;
	str.clear();
	char ch;
	while (is.get(ch) && ch != delim)
		str.push_back(ch);
	return is;
}

/* Reads the entire line from the input stream, up to the newline character into the String */
std::istream& getline(std::istream& is, String& str)
{
	if (!is)
		return is;
	str.clear();
	char ch;
	while (is.get(ch) && ch != '\n')
		str.push_back(ch);
	return is;
}

/* Reads the entire line from the input stream, up to the newline character into the String */
std::istream& getline(std::istream&& is, String& str)
{
	if (!is)
		return is;
	str.clear();
	char ch;
	while (is.get(ch) && ch != '\n')
		str.push_back(ch);
	return is;
}