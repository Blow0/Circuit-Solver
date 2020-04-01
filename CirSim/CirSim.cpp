#include <iostream>
#include <string>
#include <vector>

#include "cirsimdef.h"

//Circuit data
Node* gndNode;
std::map<std::string, size_t> nodesIndexMap;
std::map<std::string, size_t> voltageSourcesIndexMap;

//Takes input from user.
//Builds the circuit graph.
void takeInputAndBuildCircuit();

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
void solveCircuit(double angularFrequency);

//Solves Linear System of Equations using gaussian elimination.
//Returns array of complex solutions to the system.
Complex* SolveSystem(Complex* matrix, size_t height);

//Outputs the solutions of the circuit.
void showOutput(Complex* solutions, std::map<std::string, size_t> nodeIndexMap, double angularFrequency);

//Clears the circuit from memory.
void clearCircuit();

//Splits string into substrings using a delimiter.
void splitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& strings);

int main()
{
	/*std::cout << "1. Create New Circuit." << std::endl;
	std::cout << "2. Enter Frequency." << std::endl;
	std::cout << "3. Exit." << std::endl;
	unsigned int input;
	std::cin >> input;
	std::string detectedGnd;
	double angularFrequency;
	while (input != 3)
	{
		switch (input)
		{
		case 1: //Create the circuit
			detectedGnd = takeInputAndBuildCircuit();
			break;
		case 2:
			std::cout << "Please Enter the Frequency: ";
			std::cin >> angularFrequency;
			analyzeCircuitAndGiveOutput(detectedGnd, angularFrequency);
			break;
		case 3:
			break;
		}
		std::cin >> input;
	}*/
	return 0;
}

/*
cs I1 1 4 20K<1.02rad
r R1 1   4 8
r R2 1 2 4
r R3 2 3 5
r R4 3   4 6
end
*/

void takeInputAndBuildCircuit()
{
	//Input temp. values
	std::string line;
	std::vector<std::string> tokens;

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
	Node* gndNode = getGroundNode();
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

	//Show Output
	showOutput(solutions, nodesIndexMap, angularFrequency);

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
void showOutput(Complex* solutions, std::map<std::string, size_t> nodeIndexMap, double angularFrequency)
{
	/*std::map<std::string, size_t>::iterator it;
	//"Node["<NodeName>"]:      Nodal_Voltage = <NodeVoltage>"
	for (it = nodeIndexMap.begin(); it != nodeIndexMap.end(); it++)
	{
		std::cout << "Node[<" << it->first << ">]" << ":   " << "Nodal_Voltage = ";
		std::cout << solutions[it->second].getMagnitude() << "<" << solutions[it->second].getPhase() << std::endl;
	}
	//"<ElementType>[" < ElementName > "]:      Voltage_Difference = <VoltageDiff>      Current = <Current>"
		//you can output based on the element: powe dissipated or power supplied or power stored
	const std::map<std::string, Element*>& elementMap = Element::getElementMap();
	std::map<std::string, Element*>::iterator it1;
	bool Dissipating;
	double power;
	for (it1 = elementMap.begin(); it1 != elementMap.end(); it1++)
	{
		Dissipating = false;
		std::cout << "<";
		switch (it1->second->getType())
		{
		case ElementType::Resistor:
			std::cout << "R";
			Dissipating = true;
			break;
		case ElementType::Capacitor:
			std::cout << "C";
			Dissipating = true;
			break;
		case ElementType::Inductor:
			std::cout << "L";
			Dissipating = true;
			break;
		case ElementType::CS:
			std::cout << "CS";
			break;
		case ElementType::CCCS:
			std::cout << "CCCS";
			break;
		case ElementType::CCVS:
			std::cout << "CCVS";
			break;
		case ElementType::VS:
			std::cout << "VS";
			break;
		case ElementType::VCCS:
			std::cout << "VCCS";
			break;
		case ElementType::VCVS:
			std::cout << "VCVS";
			break;
		}
		std::cout << ">[" << it1->first << "]:   ";
		std::cout << "Voltage_Difference = " << it1->second->getVoltageDiff().getMagnitude() << "<"
			<< it1->second->getVoltageDiff().getPhase() << "      ";
		std::cout << "Current = " << it1->second->getCurrent(angularFrequency).getMagnitude() << "<"
			<< it1->second->getCurrent(angularFrequency).getPhase();
		std::cout << "Power = ";
		if (Dissipating)
			power = it1->second->getPowerDissipated(angularFrequency).getReal();
		else
			power = it1->second->getTotalPowerSupplied().getReal();
		std::cout << power << std::endl;
	}*/
}
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