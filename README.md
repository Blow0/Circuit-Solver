# Circuit-Solver

## Overview

This application solves **AC&DC circuits**, calculates nodal voltages, voltage difference,
current through, and power supplied/dissipated for **each element** in the circuit.
  
## Usage
  
This application uses **Netlist** to desribe the circuit.

### Input Format:

*elementType    elementName   positiveNode   negativeNode   elementValue    phase   internalResistance*

for ex.:
 *R A1 1 2 50*
 
**Note** 

* *Element Types*: 
  * R Resistance 
  * C Capacitor 
  * L Inductor 
  * VS Voltage Source
  * CS Current Source
  * VCVS Voltage Controlled Voltage Source
  * VCCS Voltage Controlled Current Source
  * CCVS Current Controleed Voltage Source
  * CCCS Current Controlled Current Source

* *phase& InternalResistance* is optional for Active elements (the App. assumes the phase is zero if not entered).

**For Voltage Dep. Sources:**

*elementType    elementName   positiveNode   negativeNode   Factor controlPositiveNode controlNegativeNode internalResistance*

**For Current Dep. Sources:**

same as Voltage Dep. sources but instead of the controlling nodes, just enter the controlling Element's name.

