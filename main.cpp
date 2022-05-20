#include <iostream>
#include <fstream>
#include <math.h> // for fabs()

using namespace std;

enum class Mode {
	NoChoice,
	RowChoice,
	ColChoice,
	FullChoice
};

void read(string fileName, double**& A, int& size)
{
	ifstream source(fileName);
	if (!source.is_open())
	{
		cout << "The file has not been opened!" << endl;
		exit(EXIT_FAILURE);
	}

	source >> size;
	source.ignore(255, '\n');	// next line

	A = new double* [size + 1];
	for (int i = 0; i <= size; i++)
		A[i] = new double[size];

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			source >> A[x][y];
			source.ignore(1); // semicolon
		}
		source.ignore(1);
		source >> A[size][y];
	}
}

void rowTransform(double**& A, int size, int step, int*& history)
{
	double val = fabs(A[step][step]);
	int posX = step;

	for (int x = step; x < size; x++)
		if (val < fabs(A[x][step]))
			posX = x;

	if (posX != 0)
	{
		for (int y = step; y < size; y++)
			swap(A[step][y], A[posX][y]);

		swap(history[step], history[posX]);
	}
}

void colTransform(double**& A, int size, int step)
{
	double val = fabs(A[step][step]);
	int posY = step;

	for (int y = step; y < size; y++)
		if (val < fabs(A[step][y]))
			posY = y;

	if (posY != 0)
		for (int x = step; x <= size; x++)
			swap(A[x][step], A[x][posY]);
}

void fullTransform(double**& A, int size, int step, int* history)
{
	double val = fabs(A[step][step]);
	int posX = step, posY = step;

	for (int y = step; y < size; y++)
		for (int x = step; x < size; x++)
			if (val < fabs(A[x][y]))
			{
				posX = x;
				posY = y;
			}

	if (posX != 0)
	{
		for (int y = step; y < size; y++)
			swap(A[step][y], A[posX][y]);

		swap(history[step], history[posX]);
	}

	if (posY != 0)
		for (int x = step; x <= size; x++)
			swap(A[x][step], A[x][posY]);
}

void gaussStep(double**& A, int size, int step)
{
	for (int y = step + 1; y < size; y++)
	{
		double p = A[step][y] / A[step][step];

		for (int x = step; x <= size; x++)
			A[x][y] -= p * A[x][step];
	}
}

int* gaussAlghoritm(double**& A, int size, Mode mode)
{
	int* history = new int[size];
	for (int i = 0; i < size; i++)
		history[i] = i;

	for (int k = 0; k < size - 1; k++)
	{
		switch (mode)
		{
		case Mode::RowChoice:
			rowTransform(A, size, k, history);
			break;
		case Mode::ColChoice:
			colTransform(A, size, k);
			break;
		case Mode::FullChoice:
			fullTransform(A, size, k, history);
			break;
		}
		gaussStep(A, size, k);
	}
	return history;
}

double* determinants(double** A, int size)
{
	double* x0 = new double[size];

	for (int y = size - 1; y >= 0; y--)
	{
		x0[y] = A[size][y];

		for (int x = size - 1; x > y; x--)
			x0[y] -= A[x][y] * x0[x];

		x0[y] /= A[y][y];
	}
	return x0;
}

void sortDetermiants(int*& history, double*& x0, int size)
{
	bool isDone = false;

	while (!isDone)
	{
		isDone = true;
		for (int i = 0; i < size - 1; i++)
			if (history[i] > history[i + 1])
			{
				swap(history[i], history[i + 1]);
				swap(x0[i], x0[i + 1]);
				isDone = false;
			}
	}
}

void gauss(Mode mode, string fileName)
{
	double** A, * x0;
	int* history, size;

	read(fileName, A, size);

	history = gaussAlghoritm(A, size, mode);
	x0 = determinants(A, size);

	sortDetermiants(history, x0, size);

	for (int i = 0; i < size; i++)
		printf("x%1d= % 2.1f\n", i + 1, x0[i]);
	cout << endl;

	for (int i = 0; i <= size; i++)	// free memory
		delete[] A[i];
	delete[] A;
	delete[] x0;
	delete[] history;
}

void zadanie1(string fileName)
{
	cout << "Zadanie 1:" << endl;
	gauss(Mode::NoChoice, fileName);
}
void zadanie2(string fileName)
{
	cout << "Zadanie 2:" << endl;
	gauss(Mode::RowChoice, fileName);
}
void zadanie3(string fileName)
{
	cout << "Zadanie 3:" << endl;
	gauss(Mode::ColChoice, fileName);
}
void zadanie4(string fileName)
{
	cout << "Zadanie 4:" << endl;
	gauss(Mode::FullChoice, fileName);
}

int main()
{
	zadanie1("plik1.csv");
	zadanie2("plik2.csv");
	zadanie3("plik3.csv");
	zadanie4("plik4.csv");

	return 0;
}
