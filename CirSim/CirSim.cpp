#include<iostream>
#include <string>
#include <memory.h>
#include "complex.h"

using namespace std;

//Solves system of linear equations
double* SolveSystem(double** matrix, unsigned int size);

int main()
{
	//Nodes and elements count
	unsigned int nodes;
	unsigned int voltageSrc = 0, vSrc = 0;
	unsigned int elements;
	unsigned int gndNode;

	//Element temporary variables
	string elementType;
	unsigned int elementPosNode;
	unsigned int elementNegNode;
	double elementVal;

	//Controlled Sources temporary variables
	string controlType;
	unsigned int controlPosNode;
	unsigned int controlNegNode;

	//Decrement gndNode by 1
//	gndNode--;
	/*
	
	//Create matrix (Two dimensional array)
	double** matrix = new double*[nodes + voltageSrc];
	for (unsigned int i = 0; i < nodes + voltageSrc; i++)
	{
		matrix[i] = new double[nodes + voltageSrc + 1];
		memset(matrix[i], 0, (nodes + voltageSrc + 1) * sizeof(double));
	}

	//Ask user to enter elements
	cout << "Enter elements data:" << endl;

	//Loop over elements count
	for (unsigned int i = elements; i > 0; i--)
	{
		//Get element data from input
		cin >> elementType >> elementPosNode >> elementNegNode >> elementVal;
		elementPosNode--;
		elementNegNode--;

		//Inject element into matrix based on its type
		switch (elementType[0])
		{
			case 'R':
			case 'r':
				elementVal = 1 / elementVal;
				matrix[elementPosNode][elementPosNode] += elementVal;
				matrix[elementPosNode][elementNegNode] -= elementVal;
				matrix[elementNegNode][elementPosNode] -= elementVal;
				matrix[elementNegNode][elementNegNode] += elementVal;
				break;
			case 'I':
			case 'i':
				matrix[elementPosNode][nodes + voltageSrc] += elementVal;
				matrix[elementNegNode][nodes + voltageSrc] -= elementVal;
				break;
			case 'V':
			case 'v':
				if (vSrc <= voltageSrc)
				{
				vSrc++;
				matrix[nodes + vSrc - 1][elementPosNode] =  1;
				matrix[elementPosNode][nodes + vSrc - 1] = -1;
				matrix[nodes + vSrc - 1][elementNegNode] = -1;
				matrix[elementNegNode][nodes + vSrc - 1] =  1;
				matrix[nodes + vSrc - 1][nodes + voltageSrc] = elementVal;
				}
				break;
			case 'C': //Controlled Source 
			case 'c': 
				cin >> controlType >> controlPosNode >> controlNegNode;

				controlPosNode--;
				controlNegNode--;

				switch (controlType[0])
				{
				case 'V': //Voltage controls the source
				case 'v':
					switch (elementType[1]) 
					{
					case 'I': //Current source
					case 'i':
						matrix[elementPosNode][controlPosNode] += -elementVal;
						matrix[elementPosNode][controlNegNode] += elementVal;
						matrix[elementNegNode][controlPosNode] += elementVal;
						matrix[elementNegNode][controlNegNode] += -elementVal;
						break;
					case 'V': //Voltage source
					case 'v':
						if (vSrc <= voltageSrc)
						{
							vSrc++;
							matrix[nodes + vSrc - 1][elementPosNode] =  1;
							matrix[elementPosNode][nodes + vSrc - 1] = -1;
							matrix[nodes + vSrc - 1][elementNegNode] = -1;
							matrix[elementNegNode][nodes + vSrc - 1] =  1;
							matrix[nodes + vSrc - 1][controlPosNode] = -elementVal;
							matrix[nodes + vSrc - 1][controlNegNode] =  elementVal;
						}
						break;
					}
					break;
				
				}

		}

	}

	//Set the row (equation) related to gndNode [VGND = 0]
	for (unsigned int j = 0; j < nodes + voltageSrc + 1; j++)
		matrix[gndNode][j] = 0;
	matrix[gndNode][gndNode] = 1;

	//Get solutions
	double* solutions = SolveSystem(matrix, nodes + voltageSrc);

	//Print solutions
	for (unsigned int i = 0; i < nodes; i++)
		cout << "V(" << i + 1 << ") = " << solutions[i] << endl;
	for (unsigned int i = nodes; i < nodes + voltageSrc; i++)
	{
		cout << "I(" << i + 1 << ") = " << solutions[i] << endl;
	}
	//Pause program until user presses a key
	system("pause");
	
	//Delete solutions
	delete[] solutions;

	//Delete matrix
	for (unsigned int i = 0; i < nodes; i++)
		delete[] matrix[i];
	delete[] matrix;
	*/
	return 0;
}

double* SolveSystem(double** matrix, unsigned int size)
{
	double tempFactor;
	unsigned int i, j, k;

	for (i = 0; i < size; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			if (abs(matrix[i][i]) < abs(matrix[j][i]))
			{
				for (k = 0; k <= size; k++)
				{
					tempFactor = matrix[i][k];
					matrix[i][k] = matrix[j][k];
					matrix[j][k] = tempFactor;
				}
			}
		}
	}

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (j != i)
			{
				tempFactor = matrix[j][i] / matrix[i][i];
				for (k = i; k < size + 1; k++)
					matrix[j][k] = matrix[j][k] - tempFactor * matrix[i][k];
			}
		}
	}

	double* solutions = new double[size];
	for (i = 0; i < size; i++)
		solutions[i] = matrix[i][size] / matrix[i][i];
	return solutions;
}