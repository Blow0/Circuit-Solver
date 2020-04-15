#ifndef _ELEMENT_H
#define _ELEMENT_H

#define SMALL_RES 0.0001
#define small_or_zero(x)  ((x) >= DBL_EPSILON ? 0.0 : 0.0001)
#define check_not_zero(x) ((x) >= DBL_EPSILON ? (x) : 0.0001)

#include <map>
#include <list>
#include <string>
#include <exception>
#include <stdexcept>
#include "../Math/complex.h"

enum class ElementType { None, Resistor, Inductor, Capacitor, CS, VS, CCCS, CCVS, VCCS, VCVS };

class CCVS;
class CCCS;

class Element
{
private: //Elements Map
	static std::map<std::string, Element*> elementsMap;

private: //Members
	std::string m_name;
	ElementType m_type;
	
protected: //Constructors
	Element(const std::string& elementName, ElementType type);

public: //Destructor
	virtual ~Element();

public: //Static Elements Map Methods
	static inline const std::map<std::string, Element*>& getElementsMap() { return elementsMap; }
	static inline bool elementExists(const std::string& elementName) { return (elementsMap.find(elementName) != elementsMap.end()); }
	static inline Element* getElement(const std::string& elementName) { return (elementExists(elementName) ? elementsMap[elementName] : nullptr); }
	static inline size_t getElementsCount() { return elementsMap.size(); }
	static void deleteAllElements();
	static void loadAllElementsIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0);

public: //Static Element Type Methods
	static std::string elementTypeToString(ElementType type);
	static ElementType stringToElementType(const std::string& typeStr);
	static std::string elementNameWithType(const std::string& name, ElementType type) { return elementTypeToString(type) + name; }

public: //Matrix Operations
	virtual void injectIntoMatrix(Complex* matrix, size_t matrixWidth, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0) = 0;
	virtual void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0) = 0;
	virtual void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, const std::map<std::string, size_t>& nodeIndexMap, const std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0) = 0;

public: //Logic
	inline bool operator==(const Element& rhs) const { return (rhs.m_name == m_name); }
	
protected: //Setters
	inline void setName(const std::string& elementName) { std::string type = elementTypeToString(m_type); elementsMap.erase(type + m_name); m_name = elementName; elementsMap.emplace(type + m_name, this); }

public: //Getters
	inline const std::string& getName() const { return m_name; }
	inline ElementType getType() const { return m_type; }

	Element(const Element&) = delete;
	void operator=(const Element&) = delete;
};

#endif //_ELEMENT_H