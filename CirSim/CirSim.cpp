#include<iostream>
#include <string>
#include <vector>

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

//Solves system of linear equations
Complex* SolveSystem(Complex* matrix, unsigned int height);

void splitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& strings)
{
	size_t pos = -1;
	size_t nextPos = -1;
	while ((nextPos = str.find(delimiter, pos + 1)) != std::string::npos)
		strings.push_back(str.substr(pos + 1, nextPos - pos - 1));
}

int main()
{
	//Input
	std::string line;
	std::vector<std::string> tokens;

	//Take Input from user until he enters "end"
	while (1)
	{
		tokens.clear();
		std::getline(std::cin, line);
		splitString(line, " ", tokens);

		if (tokens[0] == "end")
			break;

		switch (tokens[0].at(0))
		{
			case 'r':
			case 'R':
				Resistor::createResistor(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), std::stod(tokens[4]));
				break;
			case 'L':
			case 'l':
				Inductor::createInductor(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), std::stod(tokens[4]));
				break;
			case 'C':
			case 'c':
			{
				if (tokens[0].size() > 1)
				{
					switch (tokens[0].at(1))
					{
					case 'V':
					case 'v':
						CCVS::createCCVS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
						break;
					case 'C':
					case 'c':
						CCVS::createCCVS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
						break;
					case 'S':
					case 's':
						CurrentSource::createCurrentSource(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), (tokens.size() > 5 ? Complex::stringToComplex(tokens[5]) : 0.0));
						break;
					}
				}
				else
					Capacitor::createCapacitor(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), std::stod(tokens[4]));
				break;
			}
			case 'V':
			case 'v':
			{
				if (tokens[0].size() > 1)
				{
					switch (tokens[0].at(1))
					{
					case 'V':
					case 'v':
						VCVS::createVCVS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], tokens[6], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
						break;
					case 'C':
					case 'c':
						VCVS::createVCVS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], tokens[6], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
						break;
					case 'S':
					case 's':
						VoltageSource::createVoltageSource(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), (tokens.size() > 5 ? Complex::stringToComplex(tokens[5]) : 0.0));
						break;
					}
				}
				break;
			}
		}
	}

	//Get size of Augmented matrix
	if (Node::getNodesCount() + VoltageSource::getVoltageSrcsCount() > 0)
	{
		std::string gndNode;
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
		size_t matrixSize = Node::getNodesCount() + VoltageSource::getVoltageSrcsCount();
		//Height     *     Width
		Complex* matrix = new Complex[matrixSize * (matrixSize + 1)];

		size_t gndNodeIdx = nodeIndexMap[gndNode];
		Element::LoadElementsIntoMatrix(matrix, matrixSize + 1, nodeIndexMap, voltageSourceIndexMap, 0);

		//Set Gnd Equation Vgnd = 0
		for (size_t i = 0; i < matrixSize + 1; i++)
		{
			if (i != gndNodeIdx)
				matrix[gndNodeIdx * (matrixSize + 1) + i] = 0.0;
			else
				matrix[gndNodeIdx * (matrixSize + 1) + i] = 1.0;
		}

		Complex* solutions = SolveSystem(matrix, matrixSize);

		for (size_t i = 0; i < Node::getNodesCount(); i++)
		{
			std::cout << "V[" << i + 1 << "]" << " = ";
			std::cout << solutions[i].getMagnitude() << "<" << solutions[i].getPhase() << std::endl;
		}

		delete[] solutions;
		delete[] matrix;
	}
	
	return 0;
}

Complex* SolveSystem(Complex* matrix, size_t height) //Height
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