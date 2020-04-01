#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "cirsimdef.h"
//Circuit data
Node* gndNode;
std::map<std::string, size_t> nodesIndexMap;
std::map<std::string, size_t> voltageSourcesIndexMap;

//Declarations
void takeInputAndBuildCircuit();
void analyzeCircuit();
void solveCircuit(double angularFrequency);
void clearCircuit();

Node* getGroundNode();
void showOutput(double angularFrequency);
void splitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& strings);
Complex* SolveSystem(Complex* matrix, size_t height);

int main()
{
	unsigned int choice = 0;
	double frequency;
	while (choice != 3)
	{
		system("ClS");
		std::cout << "1. Create New Circuit\n2. Enter the Angular Frequency\n3.Exit\n";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			takeInputAndBuildCircuit();
			analyzeCircuit();
			break;
		case 2:
			system("ClS");
			std::cout << "Enter Frequency: ";
			std::cin >> frequency;
			solveCircuit(frequency);
			break;
		case 3:
			return 0;
		}
	}
	system("PAUSE");
	return 0;
}

/*
TODO:
	Toqa
	----
	2. Decent Input.
		1. Ask for circuit elements and create circuit //Already done
		2. Ask for frequency //Done
		3. Solve Circuit    
		4. Clear Matrix
		5. GoTo 2 or 1 based on user choice 
			[don't forget to delete all elements then nodes before asking for another circuit]
			[no need to delete anything otherthan clearing matrix if just the frequency has changed]
*/

//Definitions
//Program Flow
void takeInputAndBuildCircuit()
{
	system("ClS");
	clearCircuit();

	std::string line;
	std::vector<std::string> tokens;

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
	std::cout << "Enter the Circuit Netlist:" << std::endl;
	//Take Input from user until he enters "end"
	while (true)
	{
		//Clear last input tokens
		tokens.clear();

		//Get line from user input
		std::getline(std::cin, line);

		//Sperate input line into tokens
		splitString(line, " ", tokens);

		//Make tokens[0] all lower case letters
		for (size_t i = 0; i < tokens[0].length(); i++)
			tokens[0][i] = tolower(tokens[0][i]);

		//Exit input stage if user entered "end" 
		if (tokens[0] == "end")
			break;

		//Create element based on type(token[0])
		switch (tokens[0].at(0))
		{
		case 'r':
			Resistor::createResistor(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), std::stod(tokens[4]));
			break;
		case 'l':
			Inductor::createInductor(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), std::stod(tokens[4]));
			break;
		case 'c':
			if (tokens[0].size() > 1)
			{
				switch (tokens[0].at(1))
				{
				case 'v':
					CCVS::createCCVS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
					break;
				case 'c':
					CCCS::createCCCS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
					break;
				case 's':
					CurrentSource::createCurrentSource(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), (tokens.size() > 5 ? Complex::stringToComplex(tokens[5]) : 0.0));
					break;
				}
			}
			else
				Capacitor::createCapacitor(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), std::stod(tokens[4]));
			break;
		case 'v':
			if (tokens[0].size() > 1)
			{
				switch (tokens[0].at(1))
				{
				case 'v':
					VCVS::createVCVS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], tokens[6], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
					break;
				case 'c':
					VCCS::createVCCS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], tokens[6], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
					break;
				case 's':
					VoltageSource::createVoltageSource(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), (tokens.size() > 5 ? Complex::stringToComplex(tokens[5]) : 0.0));
					break;
				}
			}
			break;
		}
	}

}
Node* getGroundNode()
{
	const std::map<std::string, Node*>& nodesMap = Node::getNodesMap();
	const std::map<std::string, Element*>& elementsMap = Element::getElementsMap();

	//Iterate over every node
	for (std::map<std::string, Node*>::const_iterator it = nodesMap.begin(); it != nodesMap.end(); it++)
	{
		if (it->second != nullptr)
		{
			//Get lower case node Name
			std::string nodeName = it->second->getName();
		for (size_t i = 0; i < nodeName.length(); i++)
			nodeName[i] = tolower(nodeName[i]);
		//Check for ground node
		if (nodeName == "gnd" || nodeName == "ground")
			return it->second;
		}
	}
	//Iterate over every element
	for (std::map<std::string, Element*>::const_iterator it = elementsMap.begin(); it != elementsMap.end(); it++)
	{
		if (it->second != nullptr)
		{
			//Check if element is a source and return its negative terminal
			switch (it->second->getType())
			{
			case ElementType::CS:
				return ((CurrentSource*)it->second)->getNegNode();
			case ElementType::VS:
				return ((VoltageSource*)it->second)->getNegNode();
			case ElementType::VCCS:
				return ((VCCS*)it->second)->getNegNode();
			case ElementType::VCVS:
				return ((VCVS*)it->second)->getNegNode();
			case ElementType::CCCS:
				return ((CCCS*)it->second)->getNegNode();
			case ElementType::CCVS:
				return ((CCVS*)it->second)->getNegNode();
			}
		}
	}
	//Iterate over every node
	for (std::map<std::string, Node*>::const_iterator it = nodesMap.begin(); it != nodesMap.end(); it++)
	{
		//Return first node as gnd node
		if (it->second != nullptr)
			return it->second;
	}
	return nullptr;
}

void analyzeCircuit()
{
	//Counter variable
	size_t idx = 0;

	//Create Nodes Index Map
	const std::map<std::string, Node*>& nodesMap = Node::getNodesMap();

	for (std::map<std::string, Node*>::const_iterator it = nodesMap.begin(); it != nodesMap.end(); it++, idx++)
		nodesIndexMap.emplace(it->first, idx);

	//Create Voltage Sources Index Map
	const std::list<VoltageSource*>& voltageSourcesList = VoltageSource::getVoltageSourceList();

	for (std::list<VoltageSource*>::const_iterator it = voltageSourcesList.begin(); it != voltageSourcesList.end(); it++, idx++)
		voltageSourcesIndexMap.emplace((*it)->getName(), idx);

	//Get Ground Node
	gndNode = getGroundNode();
	if (gndNode == nullptr)
		throw std::runtime_error("Couldn't find ground node.");
}
void solveCircuit(double angularFrequency)
{
	//Check for circuit validity
	size_t matrixSize = Node::getNodesCount() + VoltageSource::getVoltageSrcsCount();
	if (matrixSize == 0)
		throw std::runtime_error("Couldn't find any nodes nor voltage sources.");

	//Calculate matrix width and height
	size_t matrixWidth = matrixSize + 1;
	size_t matrixHeight = matrixSize;

	//Crate MNA Equations Matrix
	Complex* matrix = new Complex[matrixWidth * matrixHeight];
	//Load Each Element into MNA Matrix
	Element::LoadElementsIntoMatrix(matrix, matrixWidth, nodesIndexMap, voltageSourcesIndexMap, angularFrequency);

	//Set Gnd Equation Vgnd = 0
	size_t gndNodeIdx = nodesIndexMap[gndNode->getName()];
	for (size_t i = 0; i < matrixSize + 1; i++)
		matrix[gndNodeIdx * (matrixSize + 1) + i] = 0.0;
	matrix[gndNodeIdx * matrixWidth + gndNodeIdx] = 1.0;
	//Get MNA Equations Solutions
	Complex* solutions = SolveSystem(matrix, matrixSize);

	//Fill nodes and voltage sources
	Node::fillNodesFromVector(solutions, nodesIndexMap);
	VoltageSource::fillVoltageSourcesFromVector(solutions, voltageSourcesIndexMap);

	//Show Output
	showOutput(angularFrequency);

	//Clear matrix from memory
	delete[] solutions;
	delete[] matrix;
}

void showOutput(double angularFrequency)
{
	system("CLS");
	std::map<std::string, Node*> nodesMap = Node::getNodesMap();
	for (std::map<std::string, Node*>::iterator it = nodesMap.begin(); it != nodesMap.end(); it++)
	{
		std::cout << "Node[" << it->first << "]:	" << "Nodal_Voltage = "
			<< Complex::complexToPolarString(it->second->getNodalVoltage(), true) << std::endl;
	}
	std::map<std::string, Element*>elementMap = Element::getElementsMap();
	for (std::map<std::string, Element*>::iterator it = elementMap.begin(); it != elementMap.end(); it++)
	{
		//Element type to string

		Complex voltageDiff, current, power;
		std::string type;
		bool dissipating = false;
		switch (it->second->getType())
		{
		case ElementType::Resistor:
		{
			type = "R";
			Resistor* resistor = static_cast<Resistor*> (it->second);
			voltageDiff = resistor->getVoltageDiff();
			current = resistor->getCurrent();
			power = resistor->getPowerDissipated();
			dissipating = true;
			break;
		}
		case ElementType::Capacitor:
		{
			type = "C";
			Capacitor* capacitor = static_cast<Capacitor*> (it->second);
			voltageDiff = capacitor->getVoltageDiff();
			current = capacitor->getCurrent(angularFrequency);
			power = capacitor->getPowerDissipated(angularFrequency);
			dissipating = true;
			break;
		}
		case ElementType::Inductor:
		{	
			type = "L";
			Inductor* inductor = static_cast<Inductor*> (it->second);
			voltageDiff = inductor->getVoltageDiff();
			current = inductor->getCurrent(angularFrequency);
			power = inductor->getPowerDissipated(angularFrequency);
			dissipating = true;
			break;
		}
		case ElementType::CS:
		{	
			type = "CS";
			CurrentSource* cs = static_cast<CurrentSource*> (it->second);
			voltageDiff = cs->getVoltageDiff();
			current = cs->getCurrent();
			power = cs->getTotalPowerSupplied();
			break;
		}
		case ElementType::VS:
		{	
			type = "VS";
			VoltageSource* vs = static_cast<VoltageSource*> (it->second);
			voltageDiff = vs->getVoltageDiff();
			current = vs->getCurrent();
			power = vs->getTotalPowerSupplied();
			break;
		}
		case ElementType::CCCS:
		{
			type = "CCCS";
			CCCS* cccs = static_cast<CCCS*> (it->second);
			voltageDiff = cccs->getVoltageDiff();
			current = cccs->getCurrent();
			power = cccs->getTotalPowerSupplied();
			break;
		}
		case ElementType::VCCS:
		{
			type = "VCCS";
			VCCS* vccs = static_cast<VCCS*> (it->second);
			voltageDiff = vccs->getVoltageDiff();
			current = vccs->getCurrent();
			power = vccs->getTotalPowerSupplied();
			break;
		}
		case ElementType::CCVS:
		{
			type = "CCVS";
			CCVS* ccvs = static_cast<CCVS*> (it->second);
			voltageDiff = ccvs->getVoltageDiff();
			current = ccvs->getCurrent();
			power = ccvs->getTotalPowerSupplied();
			break;
		}
		case ElementType::VCVS:
		{
			type = "VCVS";
			VCVS* vcvs = static_cast<VCVS*> (it->second);
			voltageDiff = vcvs->getVoltageDiff();
			current = vcvs->getCurrent();
			power = vcvs->getTotalPowerSupplied();
			break;
		}
		}
		//Determine if source is supplying or dissipating
		(power.getReal() < 0.0 && !dissipating)? dissipating = false : dissipating = true;

		std::cout << type <<"[" << it->first << "]:	";
		std::cout << "Voltage_Difference = " << Complex::complexToPolarString(voltageDiff, true)
			<< "\t Current = " << Complex::complexToPolarString(current, true)
			<< "\n Power " << (dissipating ? "Dissipated" : "Supplied") << " = ";
			std::cout<< Complex::complexToPolarString(power, true)<<std::endl;

	}
	system("PAUSE");
}

//Solve Linear System of Equations
Complex* SolveSystem(Complex* matrix, size_t height)
{
	Complex tempFactor;
	size_t i, j, k;
	size_t width = height + 1;
	for (i = 0; i < height; i++)
	{
		for (j = i + 1; j < height; j++)
		{
			if (matrix[i * width + i].getMagnitude() < matrix[j * width + i].getMagnitude())
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
//Helpers
void clearCircuit()
{
	gndNode = nullptr;
	nodesIndexMap.clear();
	voltageSourcesIndexMap.clear();
	Node::clearNodes();
	Element::clearElements();
	VoltageSource::clearVoltageSources();
}
void splitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& strings)
{
	size_t pos = 0;
	size_t nextPos = 0;
	while ((nextPos = str.find(delimiter, pos)) != std::string::npos)
	{
		std::string sub = str.substr(pos, nextPos - pos);
		if (!sub.empty())
			strings.push_back(sub);
		pos = nextPos + 1;
	}
	if (pos != str.length())
	{
		std::string sub = str.substr(pos);
		if (!sub.empty())
			strings.push_back(sub);
	}
}