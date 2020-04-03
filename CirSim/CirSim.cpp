#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "cirsimdef.h"

//Program data
double angularFrequency;

//Circuit data
Node* gndNode;
std::map<std::string, size_t> nodesIndexMap;
std::map<std::string, size_t> voltageSourcesIndexMap;

//Takes input from user.
//Builds the circuit graph.
void takeInputAndBuildCircuit();

//Takes input frequency from user.
void takeInputFrequency();

//Analyzes the circuit.
//Fills circuit data.
void analyzeCircuit();

//Searches all nodes for "gnd" node.
//If non found takes the first current source negative terminal as gnd.
//Returns gnd node or nullptr if non found.
Node* getGroundNode();

//solves the system of linear equations.
//[angularFrequency]: Angular frequency to solve the circuit at
//Outputs solutions of the circuit.
void solveCircuit();

//Solves Linear System of Equations using gaussian elimination.
//Returns array of complex solutions to the system.
Complex* SolveSystem(Complex* matrix, size_t height);

//Outputs the solutions of the circuit.
void showOutput();

//Clears the circuit from memory.
void clearCircuit();

//Clears the circuit analysis from memory.
void clearCircuitAnalysis();

//Splits string into substrings using a delimiter.
void splitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& strings);

int main()
{
	size_t choice = 0;
	while (choice != 1)
	{
		//Clear Screen
		system("ClS");

		//Give user options
		std::cout << "1. Create New Circuit" << std::endl;
		std::cout << "2. Exit" << std::endl;

		//Get user choice
		if (!(std::cin >> choice))
		{
			//Bad input
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		switch (choice)
		{
		case 1:
			//Take circuit elements from user
			takeInputAndBuildCircuit();
			//Analyze circuit
			analyzeCircuit();
			//Solve Circuit
			solveCircuit();
			break;
		case 2:
			//Exit
			return 0;
		}
	}

	while (true)
	{
		//Clear Screen
		system("ClS");

		//Give user options
		std::cout << "1. Create New Circuit" << std::endl;
		std::cout << "2. Edit Existing Circuit" << std::endl;
		std::cout << "3. Enter The Angular Frequency" << std::endl;
		std::cout << "4. Show Solution" << std::endl;
		std::cout << "5. Exit" << std::endl;

		//Get user choice
		if (!(std::cin >> choice))
		{
			//Bad input
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		switch (choice)
		{
		case 1:
			//Clear Circuit
			clearCircuit();
			//Take circuit elements from user
			takeInputAndBuildCircuit();
			//Analyze circuit
			analyzeCircuit();
			//Solve Circuit
			solveCircuit();
			break;
		case 2:
			//Clear Circuit Analysis
			clearCircuitAnalysis();
			//Take circuit elements from user
			takeInputAndBuildCircuit();
			//Analyze circuit
			analyzeCircuit();
			//Solve Circuit
			solveCircuit();
			break;
		case 3:
			//Take frequency from user
			takeInputFrequency();
			//Solve Circuit
			solveCircuit();
			break;
		case 4:
			//Show Output
			showOutput();
			break;
		case 5:
			//Exit
			return 0;
		}
	}
	return 0;
}

/*
cs I1 1 4 20<1.02rad
r R1 1 4 8
r R2 1 2 4
r R3 2 3 5
r R4 3 4 6
end
*/
void takeInputAndBuildCircuit()
{
	//Input temp. values
	std::string line;
	std::vector<std::string> tokens;

	//Clear Screen
	system("ClS");

	//Flush input stream
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');

	//Ask user to enter circuit netlist
	std::cout << "Enter the Circuit Netlist:" << std::endl;

	//Take Input lines from user until he enters "end"
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
void takeInputFrequency()
{
	size_t choice;
	while (true)
	{
		//Clear Screen
		system("ClS");

		//Ask user for choice
		std::cout << "1. Enter Angular Frequency (rad/sec)" << std::endl;
		std::cout << "2. Enter Frequency (Hz)" << std::endl;
		std::cout << "3. Back" << std::endl;
	
		//Get user choice
		if (!(std::cin >> choice))
		{
			//Bad input
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		switch (choice)
		{
		case 1:
		case 2:
			while (true)
			{
				//Clear Screen
				system("ClS");

				//Ask user to input angular frequency
				std::cout << "Enter b to go back." << std::endl;
				if (choice == 1)
					std::cout << "Enter Angular Frequency (rad/sec): ";
				else
					std::cout << "Enter Frequency (Hz): ";

				//Get user input
				std::string input;
				std::cin >> input;

				//Check for back
				if (input[0] == 'b' || input[0] == 'B')
				{
					//Back
					break;
				}
				else
				{
					try
					{
						angularFrequency = stod(input) * (choice == 2 ? 2.0 * PI : 1.0);
						return;
					}
					catch (const std::invalid_argument&)
					{
						continue;
					}
				}
			}
			break;
		case 3:
			return;
		}
	}
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

	for (std::list<VoltageSource*>::const_iterator it = voltageSourcesList.begin(); it != voltageSourcesList.end(); idx++)
		voltageSourcesIndexMap.emplace((*it)->getName(), idx);

	//Get Ground Node
	gndNode = getGroundNode();
	if (gndNode == nullptr)
		throw std::runtime_error("Couldn't find ground node.");
}
void solveCircuit()
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
	Element::loadElementsIntoMatrix(matrix, matrixWidth, nodesIndexMap, voltageSourcesIndexMap, angularFrequency);
	
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

	//Clear matrix from memory
	delete[] solutions;
	delete[] matrix;
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
			case ElementType::CCCS:
				return ((CCCS*)it->second)->getNegNode();
			case ElementType::CCVS:
				return ((CCVS*)it->second)->getNegNode();
			case ElementType::VCCS:
				return ((VCCS*)it->second)->getNegNode();
			case ElementType::VCVS:
				return ((VCVS*)it->second)->getNegNode();
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

void showOutput()
{
	//Clear Screen
	system("ClS");

	//Nodes and Elemnts Maps
	std::map<std::string, Node*> nodesMap = Node::getNodesMap();
	std::map<std::string, Element*>elementMap = Element::getElementsMap();

	std::cout << "Nodes:" << std::endl;
	std::cout << "-----------" << std::endl;

	//Iterate over every node
	for (std::map<std::string, Node*>::iterator it = nodesMap.begin(); it != nodesMap.end(); it++)
	{
		//Print node information
		std::cout << "Node[" << it->first << "]:" << std::endl;
		std::cout << "\tNodal_Voltage = " << Complex::complexToPolarString(it->second->getNodalVoltage(), true) << std::endl;
		std::cout << "\tFlow_Current = " << Complex::complexToPolarString(it->second->getFlowCurrent(angularFrequency), true) << std::endl << std::endl;
	}

	std::cout << "Elements:" << std::endl;
	std::cout << "-----------" << std::endl;

	//Iterate over every element
	for (std::map<std::string, Element*>::iterator it = elementMap.begin(); it != elementMap.end(); it++)
	{
		//Print element information
		switch (it->second->getType())
		{
			case ElementType::Resistor:
			{
				Resistor* resistor = dynamic_cast<Resistor*>(it->second);
				std::cout << "Resistor[" << resistor->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(resistor->getCurrent(), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(resistor->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Dissipated = " << Complex::complexToPolarString(resistor->getPowerDissipated(), true) << std::endl << std::endl;
				break;
			}
			case ElementType::Inductor:
			{	
				Inductor* inductor = dynamic_cast<Inductor*>(it->second);
				std::cout << "Capacitor[" << inductor->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(inductor->getCurrent(angularFrequency), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(inductor->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Stored = " << Complex::complexToPolarString(inductor->getPowerStored(angularFrequency), true) << std::endl << std::endl;
				break;
			}
			case ElementType::Capacitor:
			{
				Capacitor* capacitor = dynamic_cast<Capacitor*>(it->second);
				std::cout << "Capacitor[" << capacitor->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(capacitor->getCurrent(angularFrequency), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(capacitor->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Stored = " << Complex::complexToPolarString(capacitor->getPowerStored(angularFrequency), true) << std::endl << std::endl;
				break;
			}
			case ElementType::CS:
			{	
				CurrentSource* currentSource = dynamic_cast<CurrentSource*>(it->second);
				std::cout << "CurrentSource[" << currentSource->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(currentSource->getCurrent(), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(currentSource->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(currentSource->getTotalPowerSupplied(), true) << std::endl << std::endl;
				break;
			}
			case ElementType::VS:
			{	
				VoltageSource* voltageSource = dynamic_cast<VoltageSource*>(it->second);
				std::cout << "VoltageSource[" << voltageSource->getName() << "]:" << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(voltageSource->getVoltageDiff(), true) << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(voltageSource->getCurrent(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(voltageSource->getTotalPowerSupplied(), true) << std::endl << std::endl;
				break;
			}
			case ElementType::CCCS:
			{
				CCCS* cccs = dynamic_cast<CCCS*>(it->second);
				std::cout << "CCCS[" << cccs->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(cccs->getCurrent(angularFrequency), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(cccs->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(cccs->getTotalPowerSupplied(angularFrequency), true) << std::endl << std::endl;
				break;
			}
			case ElementType::CCVS:
			{
				CCVS* ccvs = dynamic_cast<CCVS*>(it->second);
				std::cout << "CCVS[" << ccvs->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(ccvs->getCurrent(), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(ccvs->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(ccvs->getTotalPowerSupplied(angularFrequency), true) << std::endl << std::endl;
				break;
			}
			case ElementType::VCCS:
			{
				VCCS* vccs = dynamic_cast<VCCS*>(it->second);
				std::cout << "VCCS[" << vccs->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(vccs->getCurrent(), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(vccs->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(vccs->getTotalPowerSupplied(), true) << std::endl << std::endl;
				break;
			}
			case ElementType::VCVS:
			{
				VCVS* vcvs = dynamic_cast<VCVS*>(it->second);
				std::cout << "VCVS[" << vcvs->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(vcvs->getCurrent(), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(vcvs->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(vcvs->getTotalPowerSupplied(), true) << std::endl << std::endl;
				break;
			}
		}
	}

	//Choice
	size_t choice = 0;

	//Give user options
	std::cout << "1. Back" << std::endl;

	while (choice != 1)
	{
		//Get user choice
		if (!(std::cin >> choice))
		{
			//Bad input
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}
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
void clearCircuit()
{
	clearCircuitAnalysis();
	Node::clearNodes();
	Element::clearElements();
	VoltageSource::clearVoltageSources();
}
void clearCircuitAnalysis()
{
	gndNode = nullptr;
	nodesIndexMap.clear();
	voltageSourcesIndexMap.clear();
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