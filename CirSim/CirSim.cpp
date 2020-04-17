#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "cirsimdef.h"

//Program data
double angularFrequency;

//Circuit data
Node* gndNode;
std::string gndNodeName = "gnd";
std::map<std::string, size_t> nodesIndexMap;
std::map<std::string, size_t> voltageSourcesIndexMap;
bool isCircuitSolved = false;
std::string errorMsg = "";

//Takes input from user.
//Builds the circuit graph.
void takeInputAndBuildCircuit();

//Takes input frequency from user.
void takeInputFrequency();

//Edits Existing Circuit
void editExistringCircuit();
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

//Make line all lowercase letters
std::string& stringToLower(std::string& str);

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
			try
			{
				//Analyze circuit
				analyzeCircuit();
				//Solve Circuit
				solveCircuit();
				//Mark Circuit as solved
				isCircuitSolved = true;
				errorMsg = "";
			}
			catch (const std::runtime_error & err)
			{
				//Mark Circuit as unsolved
				isCircuitSolved = false;
				errorMsg = err.what();
			}
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
			try
			{
				//Analyze circuit
				analyzeCircuit();
				//Solve Circuit
				solveCircuit();
				//Mark Circuit as solved
				isCircuitSolved = true;
				errorMsg = "";
			}
			catch (const std::runtime_error & err)
			{
				//Mark Circuit as unsolved
				isCircuitSolved = false;
				errorMsg = err.what();
			}
			break;
		case 2:
			//Edit Existring Circuit
			editExistringCircuit();
			try
			{
				//Analyze circuit
				analyzeCircuit();
				//Solve circuit
				solveCircuit();
				//Mark Circuit as solved
				isCircuitSolved = true;
				errorMsg = "";
			}
			catch (const std::runtime_error & err)
			{
				//Mark Circuit as unsolved
				isCircuitSolved = false;
				errorMsg = err.what();
			}
			break;
		case 3:
			//Take frequency from user
			takeInputFrequency();
			if (isCircuitSolved)
			{
				try
				{
					//Solve Circuit
					solveCircuit();
					//Mark Circuit as solved
					isCircuitSolved = true;
					errorMsg = "";
				}
				catch (const std::runtime_error & err)
				{
					//Mark Circuit as unsolved
					isCircuitSolved = false;
					errorMsg = err.what();
				}
			}
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
add cs I1 1 4 20<1.02rad
add r R1 1 4 8
add r R2 1 2 4
add r R3 2 3 5
add r R4 3 4 6
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

		//Convert operation and element type to lowercase
		stringToLower(tokens[0]);

		//Check operation
		if (tokens[0] == "add" || tokens[0] == "rmv" || tokens[0] == "edt")
		{
			//Check if tokens >= 3
			if (tokens.size() < 3)
			{
				std::cout << "Error: Bad Operation Input." << std::endl;
				continue;
			}

			//Element Details:
			//	tokens[0]: Operations Type
			//	tokens[1]: Element Type									add/rmv/edt
			//	tokens[2]: Element Name									add/rmv/edt
			//	tokens[3]: Element PosNode								add/edt
			//	tokens[4]: Element NegNode								add/edt
			//	tokens[5]: Element Value	/ CC-VC factor				add/edt
			//	tokens[6]: cs-vs internal / CC Element Type/ VC PosNode	add/edt
			//	tokens[7]: CC internal / VC NegNode/ CC Element			add/edt
			//	tokens[8]: VC internal									add/edt

			if (tokens[0] == "add") //Add Element
			{
				//Create element based on type(token[1])
				ElementType elementType = Element::stringToElementType(stringToLower(tokens[1]));
				switch (elementType)
				{
				case ElementType::None:
					std::cout << "Error: Bad Element Type." << std::endl;
					break;
				case ElementType::Resistor:
					if (tokens.size() < 6)
					{
						std::cout << "Error: Bad Resistor Input." << std::endl;
						break;
					}
					try
					{
						Resistor::createResistor(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), std::stod(tokens[5]));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad Resistor Input." << std::endl;
					}
					break;
				case ElementType::Inductor:
					if (tokens.size() < 6)
					{
						std::cout << "Error: Bad Inductor Input." << std::endl;
						break;
					}
					try
					{
						Inductor::createInductor(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), std::stod(tokens[5]));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad Inductor Input." << std::endl;
					}
					break;
				case ElementType::Capacitor:
					if (tokens.size() < 6)
					{
						std::cout << "Error: Bad Capacitor Input." << std::endl;
						break;
					}
					try
					{
						Capacitor::createCapacitor(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), std::stod(tokens[5]));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad Capacitor Input." << std::endl;
					}
					break;
				case ElementType::CS:
					if (tokens.size() < 6)
					{
						std::cout << "Error: Bad CS Input." << std::endl;
						break;
					}
					try
					{
						CurrentSource::createCurrentSource(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), Complex::stringToComplex(tokens[5]), (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad CS Input." << std::endl;
					}
					break;
				case ElementType::VS:
					if (tokens.size() < 6)
					{
						std::cout << "Error: Bad VS Input." << std::endl;
						break;
					}
					try
					{
						VoltageSource::createVoltageSource(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), Complex::stringToComplex(tokens[5]), (tokens.size() > 6 ? Complex::stringToComplex(tokens[6]) : 0.0));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad VS Input." << std::endl;
					}
					break;
				case ElementType::CCCS:
				{
					if (tokens.size() < 8)
					{
						std::cout << "Error: Bad CCCS Input." << std::endl;
						break;
					}
					ElementType controlType = Element::stringToElementType(tokens[6]);
					if (controlType == ElementType::None)
					{
						std::cout << "Error: Bad CCCS Input." << std::endl;
						break;
					}
					try
					{
						CCCS::createCCCS(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), Complex::stringToComplex(tokens[5]), controlType, stringToLower(tokens[7]), (tokens.size() > 8 ? Complex::stringToComplex(tokens[7]) : 0.0));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad CCCS Input." << std::endl;
					}
				}
				break;
				case ElementType::CCVS:
				{
					if (tokens.size() < 8)
					{
						std::cout << "Error: Bad CCVS Input." << std::endl;
						break;
					}
					ElementType controlType = Element::stringToElementType(tokens[6]);
					if (controlType == ElementType::None)
					{
						std::cout << "Error: Bad CCVS Input." << std::endl;
						break;
					}
					try
					{
						CCVS::createCCVS(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), Complex::stringToComplex(tokens[5]), controlType, stringToLower(tokens[7]), (tokens.size() > 8 ? Complex::stringToComplex(tokens[7]) : 0.0));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad CCVS Input." << std::endl;
					}
				}
				break;
				case ElementType::VCCS:
					if (tokens.size() < 8)
					{
						std::cout << "Error: Bad VCCS Input." << std::endl;
						break;
					}
					try
					{
						VCCS::createVCCS(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), Complex::stringToComplex(tokens[5]), stringToLower(tokens[6]), stringToLower(tokens[7]), (tokens.size() > 8 ? Complex::stringToComplex(tokens[8]) : 0.0));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad VCCS Input." << std::endl;
					}
					break;
				case ElementType::VCVS:
					if (tokens.size() < 8)
					{
						std::cout << "Error: Bad VCVS Input." << std::endl;
						break;
					}
					try
					{
						VCVS::createVCVS(stringToLower(tokens[2]), *Node::createNode(stringToLower(tokens[3])), *Node::createNode(stringToLower(tokens[4])), Complex::stringToComplex(tokens[5]), stringToLower(tokens[6]), stringToLower(tokens[7]), (tokens.size() > 8 ? Complex::stringToComplex(tokens[8]) : 0.0));
					}
					catch (const std::invalid_argument&)
					{
						std::cout << "Error: Bad VCVS Input." << std::endl;
					}
					break;
				}
			}
			else if (tokens[0] == "rmv") //Remove Element
			{
				Element* element = Element::getElement(Element::elementNameWithType(stringToLower(tokens[2]), Element::stringToElementType(stringToLower(tokens[1]))));
				if (element != nullptr)
					delete element;
				else
					std::cout << "Element Not Found." << std::endl;
			}
			else if (tokens[0] == "edt") //Edit Element
			{
				ElementType elementType = Element::stringToElementType(stringToLower(tokens[1]));
				Element* element = Element::getElement(Element::elementNameWithType(stringToLower(tokens[2]), elementType));

				if (element != nullptr)
				{
					switch (elementType)
					{
					case ElementType::None:
						std::cout << "Error: Bad Element Type." << std::endl;
						break;
					case ElementType::Resistor:
					{
						if (tokens.size() < 6)
						{
							std::cout << "Error : Bad Resistor Input." << std::endl;
							break;
						}

						Resistor* resistor = dynamic_cast<Resistor*>(element);
						resistor->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						resistor->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							resistor->setResistance(std::stod(tokens[5]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error: Bad Resistor Input." << std::endl;
						}
					}
					break;
					case ElementType::Inductor:
					{
						if (tokens.size() < 6)
						{
							std::cout << "Error : Bad Inductor Input." << std::endl;
							break;
						}

						Inductor* inductor = dynamic_cast<Inductor*>(element);
						inductor->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						inductor->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							inductor->setInductance(std::stod(tokens[5]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error: Bad Inductor Input." << std::endl;
						}
					}
					break;
					case ElementType::Capacitor:
					{
						if (tokens.size() < 6)
						{
							std::cout << "Error : Bad Capacitor Input." << std::endl;
							break;
						}

						Capacitor* capacitor = dynamic_cast<Capacitor*>(element);
						capacitor->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						capacitor->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							capacitor->setCapacitance(std::stod(tokens[5]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error: Bad Capacitor Input." << std::endl;
						}
					}
					break;
					case ElementType::CS:
					{
						if (tokens.size() < 6)
						{
							std::cout << "Error : Bad CS Input." << std::endl;
							break;
						}

						CurrentSource* cs = dynamic_cast<CurrentSource*>(element);
						cs->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						cs->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							cs->setSupplyCurrent(Complex::stringToComplex(tokens[5]));
							if (tokens.size() > 6)
								cs->setInternalAdmittance(Complex::stringToComplex(tokens[6]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error: Bad CS Input." << std::endl;
						}
					}
					break;
					case ElementType::VS:
					{
						if (tokens.size() < 6)
						{
							std::cout << "Error : Bad VS Input." << std::endl;
							break;
						}

						VoltageSource* vs = dynamic_cast<VoltageSource*>(element);
						vs->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						vs->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							vs->setSupplyVoltage(Complex::stringToComplex(tokens[5]));
							if (tokens.size() > 6)
								vs->setInternalImpedance(Complex::stringToComplex(tokens[6]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error: Bad VS Input." << std::endl;
						}
					}
					break;
					case ElementType::CCCS:
					{
						if (tokens.size() < 8)
						{
							std::cout << "Error : Bad CCCS Input." << std::endl;
							break;
						}

						CCCS* cccs = dynamic_cast<CCCS*>(element);
						cccs->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						cccs->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							cccs->setCurrentFactor(Complex::stringToComplex(tokens[5]));
							cccs->setControlElementType(Element::stringToElementType(stringToLower(tokens[6])));
							cccs->setControlElementName(stringToLower(tokens[7]));
							if (tokens.size() > 8)
								cccs->setInternalAdmittance(Complex::stringToComplex(tokens[8]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error: Bad CCCS Input." << std::endl;
						}
					}
					break;
					case ElementType::CCVS:
					{
						if (tokens.size() < 8)
						{
							std::cout << "Error : Bad CCVS Input." << std::endl;
							break;
						}

						CCVS* ccvs = dynamic_cast<CCVS*>(element);
						ccvs->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						ccvs->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							ccvs->setCurrentFactor(Complex::stringToComplex(tokens[5]));
							ccvs->setControlElementType(Element::stringToElementType(stringToLower(tokens[6])));
							ccvs->setControlElementName(stringToLower(tokens[7]));
							if (tokens.size() > 8)
								ccvs->setInternalImpedance(Complex::stringToComplex(tokens[8]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error: Bad CCVS Input." << std::endl;
						}
					}
					break;
					case ElementType::VCCS:
					{
						if (tokens.size() < 8)
						{
							std::cout << "Error : Bad VCCS Input." << std::endl;
							break;
						}

						VCCS* vccs = dynamic_cast<VCCS*>(element);
						vccs->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						vccs->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							vccs->setVoltageFactor(Complex::stringToComplex(tokens[5]));
							vccs->setControlPosNodeName(stringToLower(tokens[6]));
							vccs->setControlNegNodeName(stringToLower(tokens[7]));
							if (tokens.size() > 8)
								vccs->setInternalAdmittance(Complex::stringToComplex(tokens[8]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error: Bad VCCS Input." << std::endl;
						}
					}
					break;
					case ElementType::VCVS:
					{
						if (tokens.size() < 8)
						{
							std::cout << "Error : Bad VCVS Input." << std::endl;
							break;
						}

						VCVS* vcvs = dynamic_cast<VCVS*>(element);
						vcvs->setPosNode(*Node::createNode(stringToLower(tokens[3])));
						vcvs->setNegNode(*Node::createNode(stringToLower(tokens[4])));
						try
						{
							vcvs->setVoltageFactor(Complex::stringToComplex(tokens[5]));
							vcvs->setControlPosNodeName(stringToLower(tokens[6]));
							vcvs->setControlNegNodeName(stringToLower(tokens[7]));
							if (tokens.size() > 8)
								vcvs->setInternalImpedance(Complex::stringToComplex(tokens[8]));
						}
						catch (const std::invalid_argument&)
						{
							std::cout << "Error : Bad VCVS Input." << std::endl;
						}
					}
					break;
					}
				}
				else
					std::cout << "Element Not Found." << std::endl;
			}
		}
		else if (tokens[0] == "end") //Exit input stage if user entered "end" 
			break;
		else
			std::cout << "Error: Bad Operation Type." << std::endl;
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
						//Calculate angular frequency
						angularFrequency = stod(input) * (choice == 2 ? 2.0 * PI : 1.0);
						return;
					}
					catch (const std::invalid_argument&)
					{
						//Bad input
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
void editExistringCircuit()
{
	size_t choice;

	while (true)
	{
		//Clear Output
		system("CLS");

		//Give user Options
		std::cout << "1. Edit Ground Node" << std::endl;
		std::cout << "2. Edit Circuit Element(s)" << std::endl;
		std::cout << "3. Back" << std::endl;

		if (!(std::cin >> choice))
		{
			//Bad Input
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		switch (choice)
		{
		case 1:
			while (true)
			{
				//Clear Screen
				system("ClS");

				//Ask user to input gnd node
				std::cout << "Enter b to go back." << std::endl;
				std::cout << "Enter Ground Node Name: ";

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
					//Check if ground existed
					if (!Node::nodeExists(input))
					{
						std::cout << "Error Node not Found" << std::endl;
						continue;
					}

					//Set gndNodeName to new ground name
					gndNodeName = input;
					break;
				}
			}
			break;
		case 2:
			//Take circuit elements from user
			takeInputAndBuildCircuit();
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

	for (std::list<VoltageSource*>::const_iterator it = voltageSourcesList.begin(); it != voltageSourcesList.end(); it++, idx++)
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
	Element::loadAllElementsIntoMatrix(matrix, matrixWidth, nodesIndexMap, voltageSourcesIndexMap, angularFrequency);

	//Set Gnd Equation Vgnd = 0
	size_t gndNodeIdx = nodesIndexMap[gndNode->getName()];
	for (size_t i = 0; i < matrixSize + 1; i++)
		matrix[gndNodeIdx * (matrixSize + 1) + i] = 0.0;
	matrix[gndNodeIdx * matrixWidth + gndNodeIdx] = 1.0;

	//Get MNA Equations Solutions
	Complex* solutions = SolveSystem(matrix, matrixSize);

	//Fill nodes and voltage sources
	Node::fillAllNodesFromVector(solutions, nodesIndexMap);
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
			//Check for ground node
			if (it->second->getName() == gndNodeName)
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

	if (isCircuitSolved)
	{
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
			}
			break;
			case ElementType::Inductor:
			{
				Inductor* inductor = dynamic_cast<Inductor*>(it->second);
				std::cout << "Capacitor[" << inductor->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(inductor->getCurrent(angularFrequency), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(inductor->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Stored = " << Complex::complexToPolarString(inductor->getPowerStored(angularFrequency), true) << std::endl << std::endl;
			}
			break;
			case ElementType::Capacitor:
			{
				Capacitor* capacitor = dynamic_cast<Capacitor*>(it->second);
				std::cout << "Capacitor[" << capacitor->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(capacitor->getCurrent(angularFrequency), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(capacitor->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Stored = " << Complex::complexToPolarString(capacitor->getPowerStored(angularFrequency), true) << std::endl << std::endl;
			}
			break;
			case ElementType::CS:
			{
				CurrentSource* currentSource = dynamic_cast<CurrentSource*>(it->second);
				std::cout << "CurrentSource[" << currentSource->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(currentSource->getCurrent(), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(currentSource->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(currentSource->getTotalPowerSupplied(), true) << std::endl << std::endl;
			}
			break;
			case ElementType::VS:
			{
				VoltageSource* voltageSource = dynamic_cast<VoltageSource*>(it->second);
				std::cout << "VoltageSource[" << voltageSource->getName() << "]:" << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(voltageSource->getVoltageDiff(), true) << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(voltageSource->getCurrent(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(voltageSource->getTotalPowerSupplied(), true) << std::endl << std::endl;
			}
			break;
			case ElementType::CCCS:
			{
				CCCS* cccs = dynamic_cast<CCCS*>(it->second);
				std::cout << "CCCS[" << cccs->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(cccs->getCurrent(angularFrequency), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(cccs->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(cccs->getTotalPowerSupplied(angularFrequency), true) << std::endl << std::endl;
			}
			break;
			case ElementType::CCVS:
			{
				CCVS* ccvs = dynamic_cast<CCVS*>(it->second);
				std::cout << "CCVS[" << ccvs->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(ccvs->getCurrent(), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(ccvs->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(ccvs->getTotalPowerSupplied(angularFrequency), true) << std::endl << std::endl;
			}
			break;
			case ElementType::VCCS:
			{
				VCCS* vccs = dynamic_cast<VCCS*>(it->second);
				std::cout << "VCCS[" << vccs->getName() << "]:" << std::endl;
				std::cout << "\tCurrent = " << Complex::complexToPolarString(vccs->getCurrent(), true) << std::endl;
				std::cout << "\tVoltage_Difference = " << Complex::complexToPolarString(vccs->getVoltageDiff(), true) << std::endl;
				std::cout << "\tPower_Supplied = " << Complex::complexToPolarString(vccs->getTotalPowerSupplied(), true) << std::endl << std::endl;
			}
			break;
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
	}
	else
		std::cout << "Error: " << errorMsg << std::endl;

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
			if (matrix[i * width + i] < matrix[j * width + i])
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
	Element::deleteAllElements();
	Node::deleteAllNodes();
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

std::string& stringToLower(std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]))
			str[i] = tolower(str[i]);
	}
	return str;
}