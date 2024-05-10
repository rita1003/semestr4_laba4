/*
������� ������� ��������� ���������� ����������� (�������, ������), ������� ���������� ������������
����, �� �� ������ ��������� ���� ����� �� ��������, ��� ��� ����� �� ����.

����������� ������� ������� ���������� � ��������� ����� template, ����� �������� � ������� �������
���� ����� typename � ����� ������ ���������� �������.

� ������ ������ ����� typename ������ ���� �������� - T. �������� ������� ������������ ������������
�������������, � �������� ��������, ��� �������, ����������� ��������� �����, ��������, T.

friend - �������� �����, ������������ ��� �������������� ������� ��������� ��� ���������� ������ 
������ ������ �������� ��� �������.

new - ��������, �������� ����� � ������������ ������ ��� ������� � ���������� ��������� �� ���� 
������.

������ ������ std::numeric_limits ������������� ������������������� ������ ������� ��������� �������
�������������� ����� (��������, ����������� ��������� �������� ��� ���� int
� std::numeric_limits<int>::max() ).
*/

#include <iostream>
#include <limits>
#include <ctime>

using namespace std;

//������� 1. ������ ������ ������� ��������� � �������

template <typename T>
int SecondMax(T* arr, int n) {

	T max1 = numeric_limits<T>::min(); //���������� ��������� �������� 
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

//������� 2. ������������ ����� Matrix �� ������������ 2 

template <typename T>
class Matrix { 
private:

	//� ���� 2 ������ ������� ���� ���� ������� int** data. �� ��� ��� �� ��������� ������, ���
	//��� ����������, ������ ���� � ��� T.
	T** data;
	unsigned m; //������
	unsigned n; //�������

public:

	Matrix(unsigned m, unsigned n); //�����������
	~Matrix(); //����������

	T* operator[](int i); //���������� ��������� []
	void fillRandom(); //���������� ������������ �������

	friend ostream& operator<<(ostream& out, const Matrix<T>& matrix) {
		for (int i = 0; i < matrix.m; i++) {
			for (int j = 0; j < matrix.n; j++) {
				out << matrix.data[i][j] << " ";
			}
			out << endl;
		}
		return out;
	}

	//��� ������ � ������ �������� ����� ���� ������ ������: ������ ���-�� ����� Matrix ��
	//������ ���������, � ���-�� ���? ��� ����� �����: � ������, ���� ���������� ����������,
	//��������� ������� �����. ������� ��� ���������� ���������� ���������: +=, -=, *= ��
	//� ������ ���������.

	//������, ������, �� ����, "���������" ��� ����������
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

	//�������� ��� ������� 1

	cout << "Ex. 1" << endl << endl;

	int arr_int[] = { 54, 32, 12, 87, 98 }; //���������. ���������: 87 (index = 3)
	float arr_float[] = { 32.5, 76.9, 4.2, 76.6, 53.5 }; //���������. ���������: 76.6 (index = 3)
	double arr_double[] = { 54.424, 64.422, 76.436, 87.653 }; //���������. ���������: 76.436 (index = 2)
	char arr_char[] = {'r', 'i', 't', 'o', 'c', 'h', 'k', 'a' }; //���������. ���������: r (index = 0)

	cout << "For massiv integer second max has got index " << SecondMax(arr_int, 5) << endl;
	cout << "For massiv float second max has got index " << SecondMax(arr_float, 5) << endl;
	cout << "For massiv double second max has got index " << SecondMax(arr_double, 4) << endl;
	cout << "For massiv char second max has got index " << SecondMax(arr_char, 8) << endl;

	//�������� ��� ������� 2 

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

