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

enum class ElementType { Resistor, Capacitor, Inductor, VS, CS, VCVS, VCCS, CCVS, CCCS };

class CCVS;
class CCCS;

class Element
{
protected: //Elements Map
	static std::map<std::string, Element*> elementsMap;

protected: //Members
	std::string m_name;
	ElementType m_type;
	
protected: //Constructors
	Element(const std::string& name, ElementType type);
	virtual ~Element();

public: //Static Elements Map Methods
	static inline const std::map<std::string, Element*>& getElementsMap() { return elementsMap; }
	static inline Element* getElement(const std::string& elementName) { return elementExists(elementName) ? elementsMap[elementName] : nullptr;}
	static inline bool elementExists(const std::string& elementName) { return (elementsMap.find(elementName) != elementsMap.end()); }
	static inline size_t getElementsCount() { return elementsMap.size(); }
<<<<<<< HEAD
	static inline void clearElements() { elementsMap.clear(); }
	static void deleteAllElements();
	static void loadElementsIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency);

=======
	static inline const std::map<std::string, Element*>& getElementsMap() { return elementsMap; }
	static void LoadElementsIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency);
	static inline void clearElements() { elementsMap.clear(); }
	static void deleteAllElements();
>>>>>>> 5ebc3ecc373446cdd456cba3ab9f3cb89a6312c9
public: //Matrix Operations
	virtual void injectIntoMatrix(Complex* matrix, size_t matrixWidth, std::map<std::string, size_t>& nodeIndexMap, std::map<std::string, size_t>& voltageIndexMap, double angularFrequency = 0.0) = 0;
	virtual void injectVSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCVS* ccvs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0) = 0;
	virtual void injectCSCurrentControlIntoMatrix(Complex* matrix, size_t matrixWidth, CCCS* cccs, Complex totalCurrentFactor, std::map<std::string, size_t> nodeIndexMap, std::map<std::string, size_t> voltageIndexMap, double angularFrequency = 0.0) = 0;

public: //Logic
	inline bool operator==(const Element& rhs) const { return rhs.m_name == m_name; }
	
public: //Setters
	inline void setName(const std::string& name) { elementsMap.erase(m_name); m_name = name; elementsMap.emplace(m_name, this); }

protected: //Setters
	inline void setType(const ElementType type) { m_type = type; }

public: //Getters
	inline const std::string& getName() const { return m_name; }
	inline ElementType getType() const { return m_type; }

	Element(const Element&) = delete;
	void operator=(const Element&) = delete;
};

#endif //_ELEMENT_H