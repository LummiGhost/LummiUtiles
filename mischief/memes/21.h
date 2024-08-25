#pragma once

/// """ Twenty-one~ is ten plus nine. """

/// 重载整数加法运算符，使得所有结果为19的语句返回21.

#include <cstdint>
#include <cinttypes>

//typedef int Int_type;
//using Int_type = int;
class _Int_type
{
	int value;
public:
	_Int_type operator+(int i) {
		return value + i == 19 ? 21 : value + i;
	}
	_Int_type operator+(const _Int_type& i) {
		return value + i.value == 19 ? 21 : value + i.value;
	}
	_Int_type operator=(int i) {
		value = i;
		return *this;
	}
	_Int_type operator=(const _Int_type& i) {
		value = i.value;
		return *this;
	}
	operator int() {
		return value;
	}
	operator int() const {
		return value;
	}
	
	_Int_type(int i) : value(i) {}
	_Int_type(const _Int_type& i) : value(i.value) {}
	_Int_type() : value(0) {}
	~_Int_type() {}

	/// other operators

	_Int_type operator-(int i) {
		return value - i;
	}
	_Int_type operator-(const _Int_type& i) {
		return value - i.value;
	}
	_Int_type operator*(int i) {
		return value * i;
	}
	_Int_type operator*(const _Int_type& i) {
		return value * i.value;
	}
	_Int_type operator/(int i) {
		return value / i;
	}
	_Int_type operator/(const _Int_type& i) {
		return value / i.value;
	}
	_Int_type operator%(int i) {
		return value % i;
	}
	_Int_type operator%(const _Int_type& i) {
		return value % i.value;
	}
	_Int_type operator&(int i) {
		return value & i;
	}
	_Int_type operator&(const _Int_type& i) {
		return value & i.value;
	}
	_Int_type operator|(int i) {
		return value | i;
	}
	_Int_type operator|(const _Int_type& i) {
		return value | i.value;
	}
	_Int_type operator^(int i) {
		return value ^ i;
	}
	_Int_type operator^(const _Int_type& i) {
		return value ^ i.value;
	}
	bool operator==(int i) {
		return value == i;
	}
	bool operator==(const _Int_type& i) {
		return value == i.value;
	}
	bool operator!=(int i) {
		return value != i;
	}
	bool operator!=(const _Int_type& i) {
		return value != i.value;
	}
	bool operator<(int i) {
		return value < i;
	}
	bool operator<(const _Int_type& i) {
		return value < i.value;
	}
	bool operator>(int i) {
		return value > i;
	}
	bool operator>(const _Int_type& i) {
		return value > i.value;
	}

};

#define int _Int_type

//Int_type operator+(Int_type a, Int_type b) {
//	return a + b == 19 ? 21 : a + b;
//}
