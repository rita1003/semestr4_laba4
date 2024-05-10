/*
Шаблоны классов позволяют определить конструкции (функции, классы), которые используют определенные
типы, но на момент написания кода точно не известно, что это будут за типы.

Определение шаблона функции начинается с ключевого слова template, после которого в угловых скобках
идет слово typename и затем список параметров шаблона.

В данном случае после typename указан один параметр - T. Параметр шаблона представляет произвольный
идентификатор, в качестве которого, как правило, применяются заглавные буквы, например, T.

friend - ключевое слово, используется для предоставления доступа приватным или защищенным членам 
класса другим функциям или классам.

new - оператор, выделяет место в динамической памяти для объекта и возвращает указатель на этот 
объект.

Шаблон класса std::numeric_limits предоставляет стандартизированный способ запроса различных свойств
арифметических типов (например, максимально возможное значение для типа int
— std::numeric_limits<int>::max() ).
*/

#include <iostream>
#include <limits>
#include <ctime>

using namespace std;

//задание 1. шаблон поиска второго максимума в массиве

template <typename T>
int SecondMax(T* arr, int n) {

	T max1 = numeric_limits<T>::min(); //минимально возможное значение 
	T max2 = numeric_limits<T>::min();


	int max1_index = -1;
	int max2_index = -1;


	for (int i = 0; i < n; i++) {
		if (arr[i] > max1) {
			max2 = max1;
			max2_index = max1_index;

			max1 = arr[i];
			max1_index = i;
		}
		else if (arr[i] > max2 && arr[i] != max1) {
			max2 = arr[i];
			max2_index = i;
		}
	}


	return max2_index;
}

//задание 2. переработать класс Matrix из лабораторной 2 

template <typename T>
class Matrix { 
private:

	//в лабе 2 вместо строчки ниже была строчка int** data. Но так как мы реализуем шаблон, где
	//тип неизвестен, вместо типа у нас T.
	T** data;
	unsigned m; //строки
	unsigned n; //столбцы

public:

	Matrix(unsigned m, unsigned n); //конструктор
	~Matrix(); //деструктор

	T* operator[](int i); //перегрузка оператора []
	void fillRandom(); //заполнение произвольным образом

	friend ostream& operator<<(ostream& out, const Matrix<T>& matrix) {
		for (int i = 0; i < matrix.m; i++) {
			for (int j = 0; j < matrix.n; j++) {
				out << matrix.data[i][j] << " ";
			}
			out << endl;
		}
		return out;
	}

	//при работе с данным отрывком кодау меня возник вопрос: почему где-то после Matrix мы
	//ставим амперсанд, а где-то нет? Мой ответ такой: в случае, если переменная изменяется,
	//амперсанд ставить нужно. Поэтому при перегрузке операторов изменения: +=, -=, *= мы
	//и ставим амперсанд.

	//ссылка, кстати, по сути, "псевдоним" для переменной
	Matrix& operator+=(const Matrix& matrix);
	Matrix operator+(const Matrix& matrix) const;
	Matrix& operator-=(const Matrix& matrix);
	Matrix operator-(const Matrix& matrix) const;
	Matrix& operator*=(const Matrix& matrix);

	bool operator!= (const Matrix<T>& other) const {
		return !(*this == other);
	}

	bool operator== (const Matrix<T>& other) const {
		if (m != other.m || n != other.n) return false;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (data[i][j] != other.data[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	Matrix operator*(const Matrix<T>& other) const {
		Matrix result(m, other.n);
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < other.n; ++j) {
				result.data[i][j] = 0;
				for (int k = 0; k < n; ++k) {
					result.data[i][j] += data[i][k] * other.data[k][j];
				}
			}
		}
		return result;
	}
};

template <typename T>
Matrix<T>::Matrix(unsigned m, unsigned n) : m(m), n(n) {
	data = new T * [m];
	for (unsigned int i = 0; i < m; i++) {
		data[i] = new T[n];
	}
}

template <typename T>
Matrix<T>::~Matrix() {
	for (unsigned int i = 0; i < m; i++) {
		delete[] data[i];
	}
	delete[] data;
}

template <typename T>
T* Matrix<T>::operator[](int i) {
	return data[i];
}

template <typename T>
void Matrix<T>::fillRandom() {
	for (unsigned int i = 0; i < m; i++) {
		for (unsigned int j = 0; j < n; j++) {
			data[i][j] = rand() % 10;
		}
	}
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& matrix) {
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			data[i][j] += matrix.data[i][j];
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& matrix) const{
	Matrix<T> result(m, n);
	for (unsigned int i = 0; i < m; i++) {
		for (unsigned int j = 0; j < n; j++) {
			result.data[i][j] = data[i][j] + matrix.data[i][j];
		}
	}
	return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& matrix) {
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			data[i][j] -= matrix.data[i][j];
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& matrix) const {
	Matrix<T> result(m, n);
	result = *this;
	result -= matrix;
	return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& matrix) {
	Matrix<T> result(m, matrix.n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < matrix.n; j++) {
			result.data[i][j] = 0;
			for (int k = 0; k < n; k++) {
				result.data[i][j] += data[i][k] * matrix.data[k][j];
			}
		}
	}
	*this = result;
	return *this;
}


int main() {

	//проверка для задания 1

	cout << "Ex. 1" << endl << endl;

	int arr_int[] = { 54, 32, 12, 87, 98 }; //предполож. результат: 87 (index = 3)
	float arr_float[] = { 32.5, 76.9, 4.2, 76.6, 53.5 }; //предполож. результат: 76.6 (index = 3)
	double arr_double[] = { 54.424, 64.422, 76.436, 87.653 }; //предполож. результат: 76.436 (index = 2)
	char arr_char[] = {'r', 'i', 't', 'o', 'c', 'h', 'k', 'a' }; //предполож. результат: r (index = 0)

	cout << "For massiv integer second max has got index " << SecondMax(arr_int, 5) << endl;
	cout << "For massiv float second max has got index " << SecondMax(arr_float, 5) << endl;
	cout << "For massiv double second max has got index " << SecondMax(arr_double, 4) << endl;
	cout << "For massiv char second max has got index " << SecondMax(arr_char, 8) << endl;

	//проверка для задания 2 

	cout << endl <<"Ex. 2" << endl << endl;

	Matrix<int> matrix1(5, 5);
	matrix1.fillRandom();
	cout << "The matrix 1 is: " << endl << matrix1 << endl;

	Matrix<int> matrix2(5, 5);
	matrix2.fillRandom();
	cout << "The matrix 2 is: " << endl << matrix2 << endl;

	Matrix<int> matrix3(5, 5); //for +=
	matrix3.fillRandom();
	cout << "The matrix 3 (for +=) is: " << endl << matrix3 << endl;

	Matrix<int> matrix4(5, 5); //for -=
	matrix4.fillRandom();
	cout << "The matrix 4 (for -=) is: " << endl << matrix4 << endl;

	Matrix<int> matrix5(5, 5); //for *=
	matrix5.fillRandom();
	cout << "The matrix 5 (for *=) is: " << endl << matrix5 << endl;

	matrix3 += matrix1;
	matrix4 -= matrix1;
	matrix5 *= matrix1;

	cout << "Sum of matrix 1 and 2 is: " << endl << matrix1 + matrix2 << endl;
	cout << "The result of matrix3 += matrix1 is: " << endl << matrix3 << endl;
	cout << "The operation - for matrix 1 and 2: " << endl << matrix1 - matrix2 << endl;
	cout << "The result of matrix4 -= matrix1 is: " << endl << matrix4 << endl;
	cout << "The result of multiplication of matrix 2 and 3 is: " << endl << matrix2 * matrix3 << endl;
	cout << "The result of operation != for matrix 2 and matrix 3 is: " << endl << (matrix2 != matrix3) << endl;
	cout << "The result of operation == for matrix 2 and matrix 3 is: " << endl << (matrix2 == matrix3) << endl;

	return 0;
}

