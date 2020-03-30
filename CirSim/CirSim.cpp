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
Complex* SolveSystem(Complex* matrix, unsigned int height);

int main()
{
	//Nodes 
	string gndNode;

	//Element temporary variables
	string elementType;
	string posNode;
	string negNode;
	double elementVal;
	double phase;
	double internalResistance;
	//Controlled Sources temporary variables
	string controlSource;
	string controlPosNode;
	string controlNegNode;
	Complex controlElement(0.8, 0.6);
	//Take Input from user until he enters "end"
	while (1)
	{
		std::cin >> elementType;
		if (elementType == "end")
			break;

		std::cin >> posNode >> negNode >> elementVal;
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
				switch (elementType[2])
				{
				case 'V': //CCVS
				case 'v':
				{
				std::cin >> controlSource;
				Element* controlsource = Element::getElement(controlSource);
					CCVS* ccvs = CCVS::createCCVS(elementType, *elementPosNode, *elementNegNode, elementVal, controlsource, 0.0);
					break;
				}
				case 'C':
				case 'c':
				{
					std::cin >> controlSource;
					Element* controlsource = Element::getElement(controlSource);
					CCCS* cccs = CCCS::createCCCS(elementType, *elementPosNode, *elementNegNode, elementVal, controlsource, 0.0);
					break;
				}
				}
				switch (elementType[1])
				{
				case 'S':
				case 's':
				{
					std::cin >>phase>>internalResistance;
					controlElement.setPolar(elementVal, phase);
					CurrentSource* currentsource = CurrentSource::createCurrentSource(elementType, *elementPosNode, *elementNegNode, controlElement, internalResistance);
					break;
				}
				default:
				{
				Capacitor* capacitor = Capacitor::createCapacitor(elementType, *elementPosNode, *elementNegNode, elementVal);
				break;
				}
				}
			break;
		}
		case 'V':
		case 'v':
		{
				switch (elementType[2])
				{
				case 'V': // VCVS
				case 'v':
				{
					std::cin >> controlPosNode >> controlNegNode;
					Node* controlposnode = Node::getNode(controlPosNode);
					Node* controlnegnode = Node::getNode(controlNegNode);
					VCVS* vcvs = VCVS::createVCVS(elementType, *elementPosNode, *elementNegNode, elementVal, controlposnode, controlnegnode, 0.0);
					break;
				}
				case 'C':
				case 'c':
				{
					std::cin >> controlPosNode >> controlNegNode;
					Node* controlposnode = Node::getNode(controlPosNode);
					Node* controlnegnode = Node::getNode(controlNegNode);
					VCCS* vccs = VCCS::createVCCS(elementType, *elementPosNode, *elementNegNode, elementVal, controlposnode, controlnegnode, 0.0);
					break;
				}
				}
				switch (elementType[1])
				{
				case 'S': //VS voltage source
				case 's':
					std::cin >> phase>>internalResistance;
					controlElement.setPolar(elementVal, phase);
					VoltageSource* voltagesource = VoltageSource::createVoltageSource(elementType, *elementPosNode, *elementNegNode, controlElement, internalResistance);
					break;
				}
			break;
		}
		}
	}

	//Get size of Augmented matrix
	if (Node::getNodesCount() + VoltageSource::getVoltageSrcsCount() > 0)
	{

		std::cout << "Enter Ground Node ";
		std::cin >> gndNode;

		//IndexMap
		std::map<std::string, size_t> nodeIndexMap;
		std::map<std::string, size_t> voltageSourceIndexMap;
		std::map<std::string, Node*> nodesMap = Node::getNodesMap();
		std::list<VoltageSource*> voltageSourceList = VoltageSource::getVoltageSourceList();
		size_t idx = 0;
		for (std::map<std::string, Node*>::iterator it = nodesMap.begin(); it != nodesMap.end(); it++)
		{
			nodeIndexMap.emplace(it->first, idx);
			idx++;
		}
		for (std::list<VoltageSource*>::iterator it = voltageSourceList.begin(); it != voltageSourceList.end(); it++)
		{
			voltageSourceIndexMap.emplace((*it)->getName(), idx);
			idx++;
		}
		unsigned int matrixHeight = Node::getNodesCount() + VoltageSource::getVoltageSrcsCount();
		unsigned int matrixWidth = matrixHeight + 1;
		//Height     *     Width
		Complex* matrix = new Complex[matrixHeight * (matrixWidth)];
		size_t gndNodeIdx = nodeIndexMap[gndNode];
		Element::InjectMatrix(matrix, matrixWidth, nodeIndexMap, voltageSourceIndexMap, 0);

		//Set Gnd Equation Vgnd = 0
		for (unsigned int i = 0; i < matrixWidth; i++)
		{
			if (i != gndNodeIdx)
				matrix[gndNodeIdx * (matrixWidth) + i] = 0.0;
			else
				matrix[gndNodeIdx * (matrixWidth) + i] = 1.0;
		}

		for (int i = 0; i < matrixHeight; i++)
		{
			for (int j = 0; j < matrixWidth; j++)
			{
				cout << matrix[i * matrixWidth + j].getReal()<<" ";
			}
			cout << endl;
		}

		Complex* solutions = SolveSystem(matrix, matrixHeight);

		size_t i = 0;
		for (i = 0; i < Node::getNodesCount(); i++)
		{
			std::cout << "V[" << i + 1 << "]" << " = ";
			std::cout << solutions[i].getMagnitude() << "<" << solutions[i].getPhase() << endl;
		}
		for (size_t j = i; j < matrixHeight; j++)
		{
			cout << "I[" << i << "]" << " = ";
			cout << solutions[i].getMagnitude() << "<" << solutions[i].getPhase() << endl;
		}

		delete[] solutions;
		delete[] matrix;
	}
	
	return 0;
}

Complex* SolveSystem(Complex* matrix, unsigned int height) //Height
{
	Complex tempFactor;
	unsigned int i, j, k;
	unsigned int width = height + 1;
	for (i = 0; i < height; i++)
	{
		for (j = i + 1; j < height; j++)
		{
			if (matrix[i * width + j ].getMagnitude() < matrix[j * width + i].getMagnitude())
			{
				for (k = 0; k <= height; k++)
				{
					tempFactor = matrix[i * width + k];
					matrix[i * width + k] = matrix[j * width + k];
					matrix[j * width + k] = tempFactor;
				}
			}
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < height+1; j++)
		{
			cout << matrix[i * (height+1) + j].getReal() << " ";
		}
		cout << endl;
	}

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < height; j++)
		{
			if (j != i)
			{
				tempFactor = matrix[j * width + i] / matrix[i * width + i];
				for (k = i; k < height + 1; k++)
					matrix[j * width + k] = matrix[j * width + k] - tempFactor * matrix[i * width + k];
			}
		}
	}

	Complex* solutions = new Complex[height];
	for (i = 0; i < height; i++)
		solutions[i] = matrix[i * width + height] / matrix[i * width + i];
	return solutions;
}