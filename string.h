#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <new>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <memory>
#include <cstring>

namespace XJB {

	class string {
	public:
		void show() const {
			for (const char& i : *this) {
				std::cout << i << " ";
			}
			std::cout << "\n";
			std::cout << "size=" << size() << ",capacity=" << capacity()<<'\n';
		}
		//1.���ǵ���������
		//(1)���س�ʼλ��
		char* begin() {
			return _str;
		}
		const char* begin() const {
			return _str;
		}
		//(2)���ؽ�β'\0'��ָ��
		char* end() {
			return _str + _size;
		}
		const char* end()const {
			return _str + _size;
		}


		//2.���캯��
		string(const char* str = "") {
			_size = strlen(str);
			_capacity = _size;//���ٿռ�ʱҪ�࿪��һ�������'\0'
			_str = new char[_capacity + 1];
			_str[_capacity] = '\0';
			strcpy(_str, str);
		}

		//3.�������캯��
		string(const string& s) {
			_str = new char[s._capacity + 1];
			_str[s._capacity] = '\0';
			strcpy(_str, s._str);
			_size = s.size();
			_capacity = s._capacity;
		}
		////4.�ƶ����캯��
		//string(string&& other)noexcept {
		//	_str = other._str;
		//	_size = other._size;
		//	_capacity = other._capacity;
		//	other._str = nullptr;
		//	other._size = other._capacity = 0;
		//}

		//��������  �ַ����Ƕ�̬���ٵģ�Ҫ�ڶ�������ʱ�ͷ��ڴ�
		~string() {
			delete[] _str;
			_str = nullptr;
			_size = _capacity = 0;
		}
		//������������ز��֣�
		//1.=����
		string& operator=(const string& s) {
			if (this != &s) {
				delete[] _str;
				_str = new char[s._capacity + 1] {'\0'};
				strcpy(_str, s._str);
				_size = s._size;
				_capacity = s._capacity;
			}
			return *this;
		}
		//2.�±���������
		char& operator[](size_t i) {
			if (i < _size) {
				return _str[i];
			}
		}
		const char& operator[](size_t i) const {
			if (i < _size) {
				return _str[i];
			}
		}
	
	//6.�������ַ���::��ʵ���ǽ������Ե�ָ��
	void swap(string& s) {
		std::swap(_str, s._str);
		std::swap(_size, s._size);
		std::swap(_capacity, s._capacity);
	}

	//7.��ȡֻ���ַ���
	const char* c_str() const{
		return _str;
	}

	//8.����ַ���  ֻ������ݣ����ͷſռ�
	void clear() {
		std::destroy(begin(), end());
		_size = 0;
	}


	//9.���㳤�ȵĺ���
	size_t size() {
		return _size;
	}
	const size_t size() const {
		return _size;
	}
	size_t capacity() {
		return _capacity;
	}
	const size_t capacity() const {
		return _capacity;
	}

	//10.reserve���ݺ���
	void reserve(size_t n) {
		if (n > _capacity) {
			char* tmp = new char[n + 1];
			tmp[n] = '\0';
			strcpy(tmp, _str);
			_str = tmp;
			_capacity = n;
		}
	}

	//11.insert�������
	//(1)�����ַ�����
	void insert(size_t pos, char c) {
		if (pos <= _size){
			if (_size == _capacity) {
				reserve(std::max(2 * _capacity, _capacity + 1));
			}
			size_t end = _size + 1;
			while (end > pos) {
				_str[end] = _str[end - 1];
				end--;
			}
			_str[pos] = c;
			_size++;
		}
	}

	//(2)�����ַ���
	void insert(size_t pos, const char* str) {
		if (pos > _size) {
			return;
		}
		size_t len = strlen(str);
		if (_size + len > _capacity) {
			size_t newcapacity = _capacity * 2;
			if (newcapacity < _size + len) {
				newcapacity = _size + len;
			}
			reserve(newcapacity);
		}
		size_t end = _size + len;
		while (end > pos + len - 1) {
			_str[end] = _str[end - len];
			end--;
		}
		for (size_t i = 0; i < len; i++) {
			_str[pos + i] = str[i];
		}
		_size += len;
	}

	//12.push_back β���ַ�
	void push_back(char c) {
		insert(_size, c);
	}

	//13.appendβ���ַ���
	void append(const char* str) {
		insert(_size, str);
	}
	
	//14.�ַ���������
	string& operator+=(char c) {
		insert(_size, c);
		return *this;
	}
	string& operator+=(const char* str) {
		insert(_size, str);
		return *this;
	}

	//15.ɾ��ָ��λ�õĶ���
	void erase(size_t pos, size_t len) {
		if (pos > _size) {
			return;
		}
		if (pos + len >= _size) {
			_str[pos] = '\0';
			_size = pos;
		}
		else {
			size_t end = pos + len;
			while (end <= _size) {
				_str[end - len] = _str[end];
				end++;
			}
			_size -= len;
		}
	}
	//16.����ָ��λ�ú��ָ��Ԫ��deλ��
	size_t find(char c, size_t pos) {
		if (pos > _size) {
			return 0;
		}
		for (size_t i = 0; i < _size; i++) {
			if (_str[i] == c) {
				return i;
			}
		}
		return -1;
	}
	//17.�����ַ���
	size_t find(const char* str, size_t pos) {
		if (pos >= _size) {
			return 0;
		}
		const char* p = strstr(_str + pos, str);//strstr�����������Ӵ���
		if (p == nullptr) {
			return -1;
		}
		return p - _str;
	}

	//18.����֪�ַ����������Ӵ�
	string substr(size_t pos = 0, size_t len=0) const{
		if (pos >= _size) {
			return 0;
		}
		if (len > (_size - pos)) {
			len = _size - pos;
		}
		string new_;
		for (size_t i = pos; i < len; i++) {
			new_ += _str[pos+i];
		}
		return new_;
	}
	//19.�ж��Ƿ�Ϊ���ַ���
	bool empty(const string* p) {
		return p->size() == 0;
	}

private:
	char* _str;
	size_t _size;
	size_t _capacity;

};
		//һЩ�ȽϺ������Ƿǳ�Ա����Ŷ��
	bool operator< (const string& a,const string& b) {
		return strcmp(a.c_str(), b.c_str()) < 0;
	}
	bool operator> (const string& a, const string& b) {
		return !(a<b);
	}
	bool operator<= (const string& a, const string& b) {
		return (a<b || a==b);
	}
	bool operator== (const string& a, const string& b) {
		return !(a>b||a<b);
	}
	bool operator>= (const string& a, const string& b) {
		return (a > b || a == b);
	}
	bool operator!= (const string& a, const string& b) {
	
		return !(a==b);
	}



}