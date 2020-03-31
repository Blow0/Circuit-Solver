#include <iostream>
#include <string>
#include <vector>

#include "cirsimdef.h"

//Declarations
std::string takeInputAndBuildCircuit();
std::string detectGroundNode(std::string elementType, std::string node);
void showOutput(Complex* solutions, std::map<std::string, size_t> nodeIndexMap, double angularFrequency);
void analyzeCircuitAndGiveOutput(std::string detectedGnd, double angularFrequency);
void splitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& strings);
Complex* SolveSystem(Complex* matrix, size_t height);

int main()
{
	std::cout << "1. Create New Circuit." << std::endl;
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
	}
	return 0;
}

/*
TODO:
	Toqa
	----
	1. Auto detect for ground. //Done
		Check if any of the nodes has the name "ground" or "gnd" or "0" whoever comes first is the gnd
		If non found then take the first Source (current or voltage) negative terminal to be gnd
		If no sources found take the first node to be gnd

	2. Decent Input.
		1. Ask for circuit elements and create circuit //Already done
		2. Ask for frequency //Done
		3. Solve Circuit    
		4. Clear Matrix
		5. GoTo 2 or 1 based on user choice 
			[don't forget to delete all elements then nodes before asking for another circuit]
			[no need to delete anything otherthan clearing matrix if just the frequency has changed]

	3. Decent Output on the form:
		For each node:
			"Node["<NodeName>"]:      Nodal_Voltage = <NodeVoltage>"
		For each element:
			"<ElementType>["<ElementName>"]:      Voltage_Difference = <VoltageDiff>      Current = <Current>"
			//you can output based on the element: powe dissipated or power supplied or power stored


	Adham
	-----
	1. Add Kilo-Mega-.. to input structure.
	2. Add Comments.
*/

//Definitions
//Program Flow
std::string takeInputAndBuildCircuit()
{
	std::string line;
	std::vector<std::string> tokens;
	std::string gndNode = "-1";

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
	std::cout << "Enter the Circuit Netlist:" << std::endl;
	//Take Input from user until he enters "end"
	while (1)
	{
		tokens.clear();

		std::getline(std::cin, line);
		splitString(line, " ", tokens);

		if (tokens[0] == "end")
			break;

		std::string detectedGnd = detectGroundNode(tokens[0],tokens[3]);
		if (detectedGnd != "-1")
			gndNode = detectedGnd;

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
					CCCS::createCCCS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
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
		case 'V':
		case 'v':
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
					VCCS::createVCCS(tokens[1], *Node::createNode(tokens[2]), *Node::createNode(tokens[3]), Complex::stringToComplex(tokens[4]), tokens[5], tokens[6], (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
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
	return gndNode;
}
std::string detectGroundNode(std::string elementType,std::string node)
{
	static bool firstTime = 0;
	static bool firstSource = 1;
	//Case1 
	if (node == "gnd" || node == "ground" || node == "0" && !firstTime)
	{
		return node;
		firstTime = 1;
	}
	//Case2
	else if(elementType.at(0) == 'V' || (elementType.at(0) == 'C' && elementType.size() > 1) && firstSource)
	{
		return node; //return Negative node of this Source
		firstSource = 0;
	}
	else
		return "-1";
}

void analyzeCircuitAndGiveOutput(std::string detectedGnd, double angularFrequency)
{
	//Get size of Augmented matrix
	if (Node::getNodesCount() + VoltageSource::getVoltageSrcsCount() > 0)
	{
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
		//Set Ground Node
		std::string gndNode;
		if (detectedGnd == "-1")
		{
			std::map<std::string, size_t>::iterator it = nodeIndexMap.begin();
			gndNode = it->first;
		}
		else
			gndNode = detectedGnd;

		size_t matrixSize = Node::getNodesCount() + VoltageSource::getVoltageSrcsCount();
		//Height     *     Width
		Complex* matrix = new Complex[matrixSize * (matrixSize + 1)];

		size_t gndNodeIdx = nodeIndexMap[gndNode];
		Element::LoadElementsIntoMatrix(matrix, matrixSize + 1, nodeIndexMap, voltageSourceIndexMap, angularFrequency);

		//Set Gnd Equation Vgnd = 0
		for (size_t i = 0; i < matrixSize + 1; i++)
		{
			if (i != gndNodeIdx)
				matrix[gndNodeIdx * (matrixSize + 1) + i] = 0.0;
			else
				matrix[gndNodeIdx * (matrixSize + 1) + i] = 1.0;
		}

		Complex* solutions = SolveSystem(matrix, matrixSize);
		//Output
		showOutput(solutions, nodeIndexMap, angularFrequency);
		delete[] solutions;
		delete[] matrix;
	}
}
void showOutput(Complex* solutions, std::map<std::string, size_t> nodeIndexMap, double angularFrequency)
{
	std::map<std::string, size_t>::iterator it;
	//"Node["<NodeName>"]:      Nodal_Voltage = <NodeVoltage>"
	for (it = nodeIndexMap.begin();it != nodeIndexMap.end(); it++)
	{
		std::cout << "Node[<" << it->first << ">]" << ":   "<<"Nodal_Voltage = ";
		std::cout <<solutions[it->second].getMagnitude() << "<" << solutions[it->second].getPhase() << std::endl;
	}
	//"<ElementType>[" < ElementName > "]:      Voltage_Difference = <VoltageDiff>      Current = <Current>"
		//you can output based on the element: powe dissipated or power supplied or power stored
	std::map<std::string, Element*> elementMap = Element::getElementMap();
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
			<< it1->second->getVoltageDiff().getPhase()<<"      ";
		std::cout << "Current = " << it1->second->getCurrent(angularFrequency).getMagnitude() <<"<"
			<< it1->second->getCurrent(angularFrequency).getPhase();
		std::cout << "Power = ";
		if (Dissipating)
			power = it1->second->getPowerDissipated(angularFrequency).getReal();
		else
			power = it1->second->getTotalPowerSupplied().getReal();
		std::cout << power<<std::endl;
	}
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
void splitString(const std::string& str, const std::string& delimiter, std::vector<std::string>& strings)
{
	size_t pos = 0;
	size_t nextPos = 0;
	while ((nextPos = str.find(delimiter, pos)) != std::string::npos)
	{
		strings.push_back(str.substr(pos, nextPos - pos));
		pos = nextPos + 1;
	}
	if (pos != str.length())
		strings.push_back(str.substr(pos));
}