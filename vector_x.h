#pragma once
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <new>

namespace XJB {
	
template <typename T>
class vector {
public:
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using iterator = value_type*;
	using const_iterator = const value_type*;

private:
	T* m_start;
	T* m_finish;
	T* e_end;

	//分配内存
	T* allocate(size_type n) {
		if (n == 0) {
			return nullptr;
		}
		return static_cast<T*>(operator new(n * sizeof(value_type)));
	}
	//释放内存
	void deallocate(T* p) {
		operator delete(p);
	}

	void destroy(iterator first, iterator last) {
		for (; first != last; ++first) {
			first->~T();
		}
	}
public:
	//1.打印功能
	void show() const { //这里的const可以保证函数的只读，并且使常量对象也可以调用函数
		for (const T& val : *this) {
			std::cout << val << " ";
		}
		std::cout << "\n";
		std::cout << "size=" << size() << ",capacity=" << capacity() << "\n";
	}

	//2.容器中实际存在的元素数量
	size_type size() const {
		return m_finish - m_start;
	}

	//3.容器中可以存放元素的数量
	size_type capacity() const {
		return e_end - m_start;
	}
	
	//4.构造函数（这样写更简洁一点）
	vector()
		:m_start(nullptr), m_finish(nullptr), e_end(nullptr) {}

	//初始化函数
	

	//5.初始化一个含有n个T类型val的vector
	vector(std::size_t n, const T& val = T()) {
		m_start = allocate(n);
		e_end = m_start + n;
		m_finish = m_start;
		for (std::size_t i = 0; i < n; i++) {
			new (m_finish) T(val);
			m_finish++;
		}

	}

	//6.感觉迭代器来初始化函数，简单来说就是复制过来一份已知元素
	template<class InputIt>
	vector(InputIt first, InputIt last) {
		std::size_t n = std::distance(first, last);
		m_start = allocate(n);
		std::uninitialized_copy(first, last, m_start);
		m_finish = e_end = m_start + n;
	}

	//7.拷贝构造
	vector(const vector& other) {
		size_type n = other.size();
		size_type cap = other._capacity();

		m_start = allocate(cap);
		e_end = m_start + n;
		m_finish = m_start;

		for (const T& val : other) {
			new(m_finish) T(val);
			++m_finish;
		}
	}

	//8.移动构造
	vector(vector&& other) noexcept {    //&&是右值引用，不是复制的
		m_start = other.m_start;
		m_finish = other.m_finish;
		e_end = other.e_end;
		other.m_start = other.m_finish = other.e_end = nullptr;
	}

	//9.析构函数
	~vector() {
		T* cur = m_start;
		while (cur != m_finish) {
			cur -> ~T();
			cur++;
		}

		deallocate(m_start);
	}

	//10.交换两容器元素滴函数
	void swap(vector& other) {
		std::swap(m_start, other.m_start);
		std::swap(m_finish, other.m_finish);
		std::swap(e_end, other.e_end);
	}

	//11.提前分配内存
	void reserve(size_type n) {
		if (capacity() >= n) {
			return;
		}
		T* new_start = allocate(n);

		T* new_finish=std::uninitialized_copy(begin(), end(), new_start);
		T* cur = m_start;
		destroy(begin(), end());
		deallocate(m_start);
		m_start = new_start;
		m_finish = new_finish;
		e_end = m_start + n;
	}
	
	//12.拷贝赋值函数
	vector& operator=(const vector& other) {
		if (this == &other) {
			return *this;
		}
		if (other.size() > capacity()) { //使用tmp当做中间者来将other中的值给到*this中去
			vector tmp(other);
			tmp.swap(*this);
		}
		else if(other.size()>size()){
			std::copy(other.begin(), other.begin() + size(), m_start);
			m_finish=std::uninitialized_copy(other.begin() + size(), other.end(), m_finish);

		}
		else {
			T* new_finish=std::copy(other.begin(), other.end(), m_start);
			destroy(new_finish, end());
			m_finish = new_finish;
		}
		return *this;
	}

	//13.移动赋值(顾名思义就是，把这一坨移动到另一坨)
	vector& operator=(vector&& other) noexcept {
		if (this == &other) {
			return *this;
		}
		destroy(begin(), end());
		deallocate(m_start);
		m_start = other.m_start;
		m_finish = other.m_finish;
		e_end = other.e_end;
		other.m_start = other.m_finish = other.e_end = nullptr;
		return *this;
	}
	
	//14.方括号运算符
	T& operator[](size_type n) {
		return *(begin() + n);
	}
	const T& operator[](size_type n) const {
		return *(begin() + n);
	}

	//15.返回首元素

	T& front() {
		return *begin();
	}
	const T& front() const{
		return *begin();
	}
	
	//16.返回最后一个元素

	T& back() {
		return *(end() - 1);
	}
	const T& back() const {
		return *(end() - 1);
	}

	//17.返回首元素的指针值
	
	T* data() {
		return begin();
	}
	const T* data() const{
		return begin();
	}

	// 为了实现迭代创建分别指向开头与末尾的指针

	T* begin(){
		return m_start;
	}
	const T* begin() const{
		return m_start;
	}
	T* end() {
		return m_finish;
	}
	const T* end() const {
		return m_finish;
	}

	//18.Empty函数:判断是否为空容器
	bool empty() const {
		return begin() == end();
	}

	//19.clear:清空所有元素，但是不会释放空间
	void clear() {
		destroy(begin(), end());
		m_finish = m_start;
	}

	//20(1)Insert:在指定pos插入n个元素
	T* insert(T* pos, size_type n, const T& val) {
		if (size_type(e_end - m_finish) >= n) {
			size_type elems_after = m_finish - pos;
			if (n < elems_after) {
				std::uninitialized_move(m_finish - n, m_finish, m_finish);
				T* cur = m_finish-1-n;
				while (cur >= pos) {
					new(cur + n)T(std::move(*cur));
					cur->~T();
					--cur;
				}
				for (size_type i = 0; i < n; ++i) {
					new(pos + i)T(val);
				}
				m_finish += n;
				return pos;
			}
			else {
				std::uninitialized_move(pos, m_finish, pos + n);
				for (T* cur = pos; cur < m_finish; cur++) {
					*cur = val;
				}
				for (size_type i = 0; i < n - (m_finish - pos); i++) {
					new(m_finish + i)T(val);
				}
				m_finish += n;
				return pos;
			}
		}
		else {
			size_type new_size = std::max(2 * size(), size() + n);
			T* new_start = allocate(new_size);
			T* new_finish = std::uninitialized_move(m_start, pos, new_start);
			T* ret = new_finish;
			new_finish = std::uninitialized_fill_n(new_finish, n, val);
			new_finish=std::uninitialized_move(pos, m_finish, new_finish);
			destroy(begin(), end());
			m_finish = new_finish;
			m_start = new_start;
			e_end = m_start + new_size;
			return ret;
		}
	}

	//20(2)Insert:在指定位置插入单个value(右值)
	T* insert(T* pos, size_type n, T&& val) {
		if (m_finish != e_end) {
			if (m_finish != pos) {
				//std::construct_at(m_finish,std::move(*(m_finish-1)));
				new(static_cast<void*>(m_finish))T(std::move(*(m_finish - 1)));
				T* cur = m_finish - 1;
				while (cur >= pos) {
					new(cur + 1)T(std::move(*cur));
					cur->~T();
					--cur;
				}
				*pos = std::move(val);
				++m_finish;
				return pos;
			}
			else {
				new(static_cast<void*>(m_finish))T(std::move(val));
				++m_finish;
				return pos;
			}
		}
		else {
			size_type new_size = std::max(2 * size(), size() + 1);
			T* new_start=allocate(new_size);
			T* new_finish = std::uninitialized_move(m_start, pos, new_start);
			T* ret = new_finish;
			new(static_cast<void*>(new_finish))T(std::move(val));
			++new_finish;
			new_finish=std::uninitialized_move(pos, m_finish, new_finish);
			destroy(begin(), end());
			m_finish = new_finish;
			m_start = new_start;
			e_end = m_start + new_size;
			return ret;

		}
	}

	//20 (3). Insert
	T* insert(T* pos, size_type n, T& val) {
		if (m_finish != e_end) { 
			if (m_finish != pos) {
				//std::construct_at(m_finish,std::move(*(m_finish-1)));
				new(static_cast<void*>(m_finish))T(*(m_finish - 1));
				T* cur = m_finish - 1;
				while (cur >= pos) {
					new(cur + 1)T(std::move(*cur));
					cur->~T();
					--cur;
				}
				*pos = std::move(val);
				++m_finish;
				return pos;
			}
			else {
				new(static_cast<void*>(m_finish))T(val);
				++m_finish;
				return pos;
			}
		}
		else {
			size_type new_size = std::max(2 * size(), size() + 1);
			T* new_start=allocate(new_size);
			T* new_finish = std::uninitialized_move(m_start, pos, new_start);
			T* ret = new_finish;
			new(static_cast<void*>(new_finish))T(val);
			++new_finish;
			new_finish=std::uninitialized_move(pos, m_finish, new_finish);
			destroy(begin(), end());
			m_finish = new_finish;
			m_start = new_start;
			e_end = m_start + new_size;
			return ret;

		}
	}

	//21.erase
	T* erase(T* first, T* last) {
		T* new_finish = std::move(last, m_finish, first);
		destroy(new_finish, m_finish);
		m_finish = new_finish;
	}
	T* erase(T* pos) {
		return erase(pos, pos + 1);
	}

	//22.resize函数

	void resize(size_type n, const T& val) {
		if (n < size()) {
			destroy(begin() + n, end());
			m_finish = begin() + n;
		}
		else if (n > size()) {
			reserve(n);
			for (size_type i = size(); i < n; ++i) {
				new (m_finish) T(val); 
				++m_finish;
			}
		}
	}
	//
	void resize(size_type n) {
		resize(n, value_type());
	}

	// 23.在末尾添加一个元素的函数
	void push_back(const T& val) {
		if (m_finish == e_end) { 
			reserve(std::max(size() + 1, 2 * size()));
		}
		new (m_finish) T(val); 
		++m_finish;
	}

	// 24.在末尾添加一个元素移动tianjia
	void push_back(T&& val) {
		if (m_finish == e_end) 
		{
			reserve(std::max(size() + 1, 2 * size()));
		}
		new (m_finish) T(std::move(val)); // 移动构造
		++m_finish;
	}

	// 25.移除末尾元素
	void pop_back() {
		if (m_finish > m_start) {
			--m_finish;
			destroy(m_finish);
		}
	}
};
}

