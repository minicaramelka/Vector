#include <iostream>
#include "MyVector.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

MyVector::MyVector(size_t size, ResizeStrategy ResizeStrategy, float coef) {
	_size = size;
	_capacity = 1;
	_data = nullptr;
	_str = ResizeStrategy;
	if (_str == ResizeStrategy::Additive) {
		_delta = coef;
		_capacity = _size + _delta;
		_data = new ValueType[_capacity];
		for (int i = 0; i < _size; i++) {
			_data[i] = ValueType();
		}
	}
	if (_str == ResizeStrategy::Multiplicative) {
		_coef = coef;
		if (size != 0) {
			_capacity = _size * _coef;
		}
		_data = new ValueType[_capacity];
		for (int i = 0; i < _size; i++) {
			_data[i] = ValueType();
		}
	}
}

MyVector::MyVector(size_t size, ValueType value, ResizeStrategy ResizeStrategy, float coef) {
	_size = size;
	_str = ResizeStrategy;
	if (_str == ResizeStrategy::Additive) {
		_delta = coef;
		_capacity = _size + _delta;
	}
	if (_str == ResizeStrategy::Multiplicative) {
		_coef = coef;
		_capacity = _size * _coef;
	}
	_data = new ValueType[_capacity];
	for (int i = 0; i < _size; i++) {
		_data[i] = value;
	}
}

MyVector::MyVector(const MyVector& copy) {
	_size = copy._size;
	_capacity = copy._capacity;
	_str = copy._str;
	if (_str == ResizeStrategy::Additive) {
		_delta = copy._delta;
	}
	if (_str == ResizeStrategy::Multiplicative) {
		_coef = copy._coef;
	}
	_data = new ValueType[_capacity];
	for (int i = 0; i < _size; i++) {
		_data[i] = copy._data[i];
	}
}

MyVector::~MyVector() {
	delete[] _data;
	_data = nullptr;
	_size = 0;
	_capacity = 0;
}

MyVector& MyVector::operator=(const MyVector& copy) {
	if (this != &copy) {
		_size = copy._size;
		_capacity = copy._capacity;
		_str = copy._str;
		if (_str == ResizeStrategy::Additive) {
			_delta = copy._delta;
		}
		if (_str == ResizeStrategy::Multiplicative) {
			_coef = copy._coef;
		}
		delete[] _data;
		_data = new ValueType[_capacity];
		for (int i = 0; i < _size; i++) {
			_data[i] = copy._data[i];
		}
		return *this;
	}
	else
		return *this;
}

ValueType& MyVector::operator[](const size_t i) const {
	return _data[i];
}

size_t MyVector::size() const {
	return _size;
}

size_t MyVector::capacity() const {
	return _capacity;
}

float MyVector::loadFactor() {
	return ((float)(_size) / (float)(_capacity));
}

void MyVector::pushBack(const ValueType& value) {

	if (_size + 1 <= _capacity) {
		_size++;
		_data[_size - 1] = value;
	}
	else {
		resize(_size + 1);
		_data[_size - 1] = value;
	}
	/*
	if (_capacity > _size) {
	_size++;
	_data[_size - 1] = value;
	}
	else {
	resize(_size + 1);
	_data[_size - 1] = value;
	}*/
}

void MyVector::popBack() {
	if (_size > 0) {
		_data[_size - 1] = 0;
		_size--;
		while (loadFactor() <= (1 / (_coef * _coef))) {
			if (_str == ResizeStrategy::Multiplicative) {
				_capacity = _capacity / _coef;
			}
			else {
				_capacity = _capacity - _coef;
			}
		}
	}
	else
		throw out_of_range("size < 0");
}

void MyVector::insert(const size_t i, const ValueType& value) {
	size_t size = _size;
	if (i == 0) {
		ValueType* bufArray = new ValueType[_size + 1];
		for (int j = 0; j < _size; j++) {
			bufArray[j + 1] = _data[j];
		}
		bufArray[0] = value;
		resize(_size + 1);
		for (int k = 0; k < _size; k++) {
			_data[k] = bufArray[k];
		}
		delete[] bufArray;
		bufArray = nullptr;
	}
	if (i == size) {
		pushBack(value);
	}
	if (i > 0 && i < size) {
		ValueType* bufArray = new ValueType[_size + 1];
		for (int m = i; m < _size; m++) {
			bufArray[m + 1] = _data[m];
		}
		for (int v = 0; v < i; v++) {
			bufArray[v] = _data[v];
		}
		bufArray[i] = value;
		resize(_size + 1);
		for (int c = 0; c < _size; c++) {
			_data[c] = bufArray[c];
		}
		delete[] bufArray;
		bufArray = nullptr;
	}
}

void MyVector::insert(const size_t i, const MyVector& value) {
	size_t size = _size;
	if (i == 0)
	{
		ValueType* bufArray = new ValueType[_size + value.size()];
		for (int m = 0; m < _size; m++)
		{
			bufArray[m + value.size()] = _data[m];
		}
		for (int j = 0; j < value.size(); j++)
		{
			bufArray[j] = value[j];
		}
		resize(_size + value.size());
		for (int k = 0; k < _size; k++) {
			_data[k] = bufArray[k];
		}
		delete[] bufArray;
		bufArray = nullptr;
	}
	if (i == size) {
		for (int i = 0; i < value.size(); i++) {
			pushBack(value[i]);
		}
	}
	if (i > 0 && i < size) {
		ValueType* bufArray = new ValueType[_size + value.size()];
		for (int j = 0; j < i; j++) {
			bufArray[j] = _data[j];
		}
		for (int k = i; k < _size; k++) {
			bufArray[k + value.size()] = _data[k];
		}
		for (int c = 0; c < value.size(); c++) {
			bufArray[c + i] = value[c];
		}
		resize(_size + value.size());
		for (int m = 0; m < _size; m++) {
			_data[m] = bufArray[m];
		}
		delete[] bufArray;
		bufArray = nullptr;
	}
}

void MyVector::reserve(const size_t capacity) {
	_capacity = capacity;
	ValueType* bufArray = new ValueType[_size];
	for (int i = 0; i < _size; i++) {
		bufArray[i] = _data[i];
	}
	delete[] _data;
	_data = nullptr;
	_data = new ValueType[_capacity];
	if (_capacity < _size) {
		for (int j = 0; j < _capacity; j++) {
			_data[j] = bufArray[j];
		}
		_size = _capacity;
	}
	if (_capacity>_size) {
		for (int j = 0; j < _capacity; j++) {
			_data[j] = bufArray[j];
		}
	}
	delete[] bufArray;
	bufArray = nullptr;
}
size_t MyVector::loadFactorForResizeMore() {
	if (_str == ResizeStrategy::Additive) {
		while (loadFactor()>1) {
			_capacity = _capacity + _delta;
		}

	}
	if (_str == ResizeStrategy::Multiplicative) {
		while (loadFactor() > 1) {
			_capacity = ceil((float)_capacity * (float)_coef);
		}
	}
	return _capacity;
}

size_t MyVector::loadFactorForResizeFew() {
	if (_str == ResizeStrategy::Additive) {
		while (_capacity - _size > _delta) {
			_capacity = _capacity - _delta;
		}
	}
	if (_str == ResizeStrategy::Multiplicative) {
		while (loadFactor() <= (1 / (_coef * _coef))) {
			_capacity = _capacity * (1 / _coef);
		}
	}
	return _capacity;
}

void MyVector::resize(const size_t size, const ValueType value) {
	if (_size == 0) {
		_size = size;
		if (_str == ResizeStrategy::Multiplicative)
			_capacity = _size * _coef;
		else
			_capacity = _size + _coef;
		delete[] _data;
		_data = nullptr;
		_data = new ValueType[_capacity];
		for (int i = 0; i < _size; i++) {
			_data[i] = value;
		}
	}
	else {
		ValueType* bufArray = new ValueType[size];
		if (size > _size) {
			for (int i = 0; i < _size; i++) {
				bufArray[i] = _data[i];
			}
			for (int j = _size; j < size; j++) {
				bufArray[j] = value;
			}
			delete[] _data;
			_data = nullptr;
			_size = size;
			loadFactorForResizeMore();
			_data = new ValueType[_capacity];
			for (int k = 0; k < size; k++) {
				_data[k] = bufArray[k];
			}
			delete[] bufArray;
			bufArray = nullptr;
		}
		else {
			for (int i = 0; i < size; i++) {
				bufArray[i] = _data[i];
			}
			delete[] _data;
			_data = nullptr;
			_size = size;
			loadFactorForResizeFew();
			_data = new ValueType[_capacity];
			for (int k = 0; k < size; k++) {
				_data[k] = bufArray[k];
			}
			delete[] bufArray;
			bufArray = nullptr;
		}
	}
}

void MyVector::erase(const size_t i) {
	ValueType* bufArray = new ValueType[_size - 1];
	for (int j = 0; j < i; j++) {
		bufArray[j] = _data[j];
	}
	for (int k = i + 1; k < _size; k++) {
		bufArray[k - 1] = _data[k];
	}
	resize(_size - 1);
	for (int m = 0; m < _size; m++) {
		_data[m] = bufArray[m];
	}
	delete[] bufArray;
	bufArray = nullptr;
	if (_capacity == 0)
		reserve(1);
}

void MyVector::erase(const size_t i, const size_t len) {
	ValueType* bufArray = new ValueType[_size - len];
	for (int j = 0; j < i; j++) {
		bufArray[j] = _data[j];

	}
	for (int k = i + len; k < _size; k++) {

		bufArray[k - len] = _data[k];
	}
	resize(_size - len);
	for (int m = 0; m < _size; m++) {
		_data[m] = bufArray[m];
	}
	delete[] bufArray;
	bufArray = nullptr;
	if (_capacity == 0)
		reserve(1);
}

void MyVector::clear() {
	for (int i = 0; i < _size; i++) {
		_data[i] = 0;
	}
	_size = 0;
}

ValueType* MyVector::begin() {
	return _data;
}

ValueType* MyVector::end() {
	return _data + _size;
}

long long int MyVector::find(const ValueType& value, bool isBegin) const {
	int a = 0;
	if (isBegin == true) {
		for (int i = 0; i < _size; i++) {
			if (_data[i] == value) {
				a = 1;
				return i;
			}
		}
	}
	if (isBegin == false) {
		for (int i = _size; i > 0; --i) {
			if (_data[i] == value) {
				a = 1;
				return i;
			}
		}
	}
	if (a == 0) {
		return -1;
	}
}

MyVector sortedSquares(const MyVector& vec, SortedStrategy strategy) {
	MyVector bufArr(vec);
	MyVector bufBrr(vec);
	for (int i = 0; i < vec.size(); i++) {
		bufArr[i] = bufArr[i] * bufArr[i];
	}
	int j = bufArr.size() - 1;
	int i = 0;
	while (i <= j) {
		if (bufArr[i] > bufArr[j]) {
			if (strategy == SortedStrategy::Up) {
				bufBrr[j - i] = bufArr[i];
			}
			else {
				bufBrr[bufArr.size() - j + i - 1] = bufArr[i];
			}
			i++;
		}
		else {
			if (strategy == SortedStrategy::Up) {
				bufBrr[j - i] = bufArr[j];
			}
			else {
				bufBrr[bufArr.size() - j + i - 1] = bufArr[j];
			}
			j--;
		}
	}
	return bufBrr;
}
