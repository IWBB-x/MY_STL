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

	//�����ڴ�
	T* allocate(size_type n) {
		if (n == 0) {
			return nullptr;
		}
		return static_cast<T*>(operator new(n * sizeof(value_type)));
	}
	//�ͷ��ڴ�
	void deallocate(T* p) {
		operator delete(p);
	}

	void destroy(iterator first, iterator last) {
		for (; first != last; ++first) {
			first->~T();
		}
	}
public:
	//1.��ӡ����
	void show() const { //�����const���Ա�֤������ֻ��������ʹ��������Ҳ���Ե��ú���
		for (const T& val : *this) {
			std::cout << val << " ";
		}
		std::cout << "\n";
		std::cout << "size=" << size() << ",capacity=" << capacity() << "\n";
	}

	//2.������ʵ�ʴ��ڵ�Ԫ������
	size_type size() const {
		return m_finish - m_start;
	}

	//3.�����п��Դ��Ԫ�ص�����
	size_type capacity() const {
		return e_end - m_start;
	}
	
	//4.���캯��������д�����һ�㣩
	vector()
		:m_start(nullptr), m_finish(nullptr), e_end(nullptr) {}

	//��ʼ������
	

	//5.��ʼ��һ������n��T����val��vector
	vector(std::size_t n, const T& val = T()) {
		m_start = allocate(n);
		e_end = m_start + n;
		m_finish = m_start;
		for (std::size_t i = 0; i < n; i++) {
			new (m_finish) T(val);
			m_finish++;
		}

	}

	//6.�о�����������ʼ������������˵���Ǹ��ƹ���һ����֪Ԫ��
	template<class InputIt>
	vector(InputIt first, InputIt last) {
		std::size_t n = std::distance(first, last);
		m_start = allocate(n);
		std::uninitialized_copy(first, last, m_start);
		m_finish = e_end = m_start + n;
	}

	//7.��������
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

	//8.�ƶ�����
	vector(vector&& other) noexcept {    //&&����ֵ���ã����Ǹ��Ƶ�
		m_start = other.m_start;
		m_finish = other.m_finish;
		e_end = other.e_end;
		other.m_start = other.m_finish = other.e_end = nullptr;
	}

	//9.��������
	~vector() {
		T* cur = m_start;
		while (cur != m_finish) {
			cur -> ~T();
			cur++;
		}

		deallocate(m_start);
	}

	//10.����������Ԫ�صκ���
	void swap(vector& other) {
		std::swap(m_start, other.m_start);
		std::swap(m_finish, other.m_finish);
		std::swap(e_end, other.e_end);
	}

	//11.��ǰ�����ڴ�
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
	
	//12.������ֵ����
	vector& operator=(const vector& other) {
		if (this == &other) {
			return *this;
		}
		if (other.size() > capacity()) { //ʹ��tmp�����м�������other�е�ֵ����*this��ȥ
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

	//13.�ƶ���ֵ(����˼����ǣ�����һ���ƶ�����һ��)
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
	
	//14.�����������
	T& operator[](size_type n) {
		return *(begin() + n);
	}
	const T& operator[](size_type n) const {
		return *(begin() + n);
	}

	//15.������Ԫ��

	T& front() {
		return *begin();
	}
	const T& front() const{
		return *begin();
	}
	
	//16.�������һ��Ԫ��

	T& back() {
		return *(end() - 1);
	}
	const T& back() const {
		return *(end() - 1);
	}

	//17.������Ԫ�ص�ָ��ֵ
	
	T* data() {
		return begin();
	}
	const T* data() const{
		return begin();
	}

	// Ϊ��ʵ�ֵ��������ֱ�ָ��ͷ��ĩβ��ָ��

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

	//18.Empty����:�ж��Ƿ�Ϊ������
	bool empty() const {
		return begin() == end();
	}

	//19.clear:�������Ԫ�أ����ǲ����ͷſռ�
	void clear() {
		destroy(begin(), end());
		m_finish = m_start;
	}

	//20(1)Insert:��ָ��pos����n��Ԫ��
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

	//20(2)Insert:��ָ��λ�ò��뵥��value(��ֵ)
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

	//22.resize����

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

	// 23.��ĩβ���һ��Ԫ�صĺ���
	void push_back(const T& val) {
		if (m_finish == e_end) { 
			reserve(std::max(size() + 1, 2 * size()));
		}
		new (m_finish) T(val); 
		++m_finish;
	}

	// 24.��ĩβ���һ��Ԫ���ƶ�tianjia
	void push_back(T&& val) {
		if (m_finish == e_end) 
		{
			reserve(std::max(size() + 1, 2 * size()));
		}
		new (m_finish) T(std::move(val)); // �ƶ�����
		++m_finish;
	}

	// 25.�Ƴ�ĩβԪ��
	void pop_back() {
		if (m_finish > m_start) {
			--m_finish;
			destroy(m_finish);
		}
	}
};
}

