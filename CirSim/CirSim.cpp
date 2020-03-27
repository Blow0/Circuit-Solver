#include<iostream>
#include <string>
#include <memory.h>
#include "complex.h"
#include "Elements/resistor.h"
#include "Elements/capacitor.h"
#include "Elements/inductor.h"
#include "Elements//currentsource.h"
#include "Elements/voltagesource.h"
#include "Elements/vcvs.h"
#include "Elements/vccs.h"
#include "Elements/cccs.h"
#include "Elements/ccvs.h"


using namespace std;

//Solves system of linear equations
double* SolveSystem(double** matrix, unsigned int size);

int main()
{
	//Nodes 
	unsigned int gndNode = 0;

	//Element temporary variables
	string elementType;
	string posNode;
	string negNode;
	double elementVal;

	//Controlled Sources temporary variables
	string controlSource;
	string controlPosNode;
	string controlNegNode;
	double phase;
	Complex controlElement(.6,.8);
	//Take Input from user until he enters "end"
	while (1)
	{
		cin >> elementType;
		if (elementType == "end")
			break;
		cin >> posNode >> negNode >> elementVal;
		Node* elementPosNode = Node::createNode(posNode);
		Node* elementNegNode = Node::createNode(negNode);

		switch (elementType[0])
		{
		case 'r':
		case 'R':
		{
			Resistor * resistor = Resistor::createResistor(elementType, *elementPosNode, *elementNegNode, elementVal);
			break;
		}
		case 'L':
		case 'l':
		{
			Inductor* inductor = Inductor::createInductor(elementType, *elementPosNode, *elementNegNode, elementVal);
			break;
		}
		case 'C':
		case 'c':
		{
			if (elementType.size() > 2)
			{
				cin >> controlSource;
				Element* controlsource = Element::getElement(controlSource);
				switch (elementType[2])
				{
				case 'V': //CCVS
				case 'v':
				{
					CCVS* ccvs = CCVS::createCCVS(elementType, *elementPosNode, *elementNegNode, elementVal, controlsource, 0.0);
					break;
				}
				case 'C':
				case 'c':
				{

					CCCS* cccs = CCCS::createCCCS(elementType, *elementPosNode, *elementNegNode, elementVal, controlsource, 0.0);
					break;
				}
				}
			}	
			else
			{
				switch (elementType[1])
				{
				case 'S':
				case 's':
				{
					cin >>phase;
					controlElement.setPolar(elementVal, phase);
					CurrentSource* currentsource = CurrentSource::createCurrentSource(elementType, *elementPosNode, *elementNegNode, controlElement);
					break;
				}
				default:
				{
				Capacitor* capacitor = Capacitor::createCapacitor(elementType, *elementPosNode, *elementNegNode, elementVal);
				break;
				}
				}
			}
		}
		case 'V':
		case 'v':
		{
			if (elementType.size() > 2)
			{
				cin >> controlPosNode >> controlNegNode;
				Node* controlposnode = Node::getNode(controlPosNode);
				Node* controlnegnode = Node::getNode(controlNegNode);
				switch (elementType[2])
				{
				case 'V': // VCVS
				case 'v':
				{
					VCVS* vcvs = VCVS::createVCVS(elementType, *elementPosNode, *elementNegNode, elementVal, controlposnode, controlnegnode, 0.0);
					break;
				}
				case 'C':
				case 'c':
				{
					VCCS* vccs = VCCS::createVCCS(elementType, *elementPosNode, *elementNegNode, elementVal, controlposnode, controlnegnode, 0.0);
					break;
				}
				}
			}
				
			else
			{
				switch (elementType[1])
				{
				case 'S': //VS voltage source
				case 's':
					cin >> phase;
					controlElement.setPolar(elementVal, phase);
					VoltageSource* voltagesource = VoltageSource::createVoltageSource(elementType, *elementPosNode, *elementNegNode, controlElement);
					break;
				}
			}
		}
		}
	}

	//Get size of Augmented matrix
	unsigned int matrixSize = Node::getNodesCount() + VoltageSource::getVoltageSrcsCount();
								//Height     *     Width
	Complex* matrix = new Complex[matrixSize * (matrixSize + 1)];
	// [i*width + j]  /2 * 3 = 6 -> j 0 1 2, 3 4 5 j < width, i< height

	
	delete[] matrix;
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