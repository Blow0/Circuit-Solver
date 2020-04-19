#ifndef _SHORT_H
#define _SHORT_H

#include "voltagesource.h"
#include "../Node/node.h"

class Short : public VoltageSource
{
public: //Static Current controlled Voltage Source creation
	static Short* createShort(const std::string& shortName, Node& posNode, Node& negNode);

private: //Constructors
	Short(const std::string& shortName, Node& posNode, Node& negNode);

public: //Destructor
	~Short();

private: //Blocked Setters
	inline void setSupplyVoltage(Complex supplyVoltage) { }

	Short(const Short&) = delete;
	void operator=(const Short&) = delete;
};

#endif //_SHORT_H