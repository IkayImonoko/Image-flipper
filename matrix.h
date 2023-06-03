#ifndef MATRIX
#define MATRIX
#include <vector>
#include <stdexcept>
#include <iostream>

template <typename T>
class Matrix; 

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m);

template <typename T>
class Matrix {
	public:
		uint32_t dementions;
		std::vector<T> data;
		Matrix(uint32_t d);
		~Matrix();
		Matrix<T>& operator=(std::vector<T>& v);
		friend std::ostream& operator<<<>(std::ostream& os, const Matrix& m);
		std::vector<T>& operator*(std::vector<T>&);
};

template <typename T>
Matrix<T>::Matrix(uint32_t d) : dementions(d) {
}

template <typename T>
Matrix<T>::~Matrix() {
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(std::vector<T>& v) {
	if (v.size() != dementions * dementions) {
		throw std::runtime_error("Error! Unexeptebl vector size");
	}
	else {
		data = v;
		return *this;
	}
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
	for (int j = 0; j < m.dementions; ++j) {
		for (int i = 0; i < m.dementions; ++i) {
			os << m.data[i + j * m.dementions] << "   ";
		}
		os << std::endl;
	}
	return os;
}

template <typename T>
std::vector<T>& Matrix<T>::operator*(std::vector<T>& v) {
if (v.size() != this->dementions) {
	throw std::runtime_error("Error! Unacelible vector size.");
}
else {
	std::vector<T> temp{0};
	temp.resize(v.size());
	for (int j = 0; j < this->dementions; ++j) {
		for (int i = 0; i < this->dementions; ++i) {
			temp[j] += this->data[this->dementions * j + i] * v[i];
		}
	}
	return v = temp;
}
}
#endif
