
# Circuit-Solver
## About

This application solves  electrical circuits using **AC sinusoidal frequency domain analysis**. 
which means it calculates:
- **Nodal voltages** for every node.
- **Voltage difference** on each element.
-  **Current** through each element .
- **Power supplied** and/or **power dissipated** for each element in the circuit.

## Usage

### 1. Circuit Building Block

**Circuit-Solver** teats the circuit as a net list of elements *(resistors, voltage sources, etc.)* interconnected with each other.
#### Element Types
- Node
- Resistor
- Capacitor
- Inductor 
- Voltage Source
- Current Source
- Voltage Controlled Voltage Source
- Voltage Controlled Current Source
- Current Controlled Voltage Source
- Current Controlled Current Source

### 2. Circuit Naming

Before you insert the circuit you wish to solve, make sure that you name every element in the circuit.<br/>
**Note:** Names are ***case-insensitive*** and no two elements can have the same name unless they have different types.

### 3. Circuit Insertion

**Circuit-Solver** stores the circuit in the form of a net list of elements.

***3. 1. Insertion Format***<br/>
**Circuit-Solver** allows you to edit the stored net list using three operations and has this general input format:
```
OperationType ElementType ElementName [ElementData]
```

***3. 2. Insertion Operation***<br/>
**Circuit-Solver** have three insertion-related operations:
- **add:** Adds a new element into the circuit net list.
- **rmv:** Removes an existing element in the circuit net list.
- **edt:**  Edits an existing element in the circuit net list.
- **end:** Ends the insertion and saves the net list.

***3. 3. Element Input***<br/>
**Circuit-Solver** have a lot of elements but one thing in common any element input starts with a string indicating its type and then is followed by the name of the element and then the element data which depends on the type of element and operation type.<br/>
**Note:** Element type and name are converted to lowercase letters.

**Insertion Input Example**
```
add cs I1 1 4 20K<1.02rad
edt cs I1 1 4 30+50iK
add r R1 1 4 8
add r R2 1 2 4
add r R3 2 3 5
add r R4 3 4 6
rmv r R1
edt r R2 1 2 5
end
```

#### Notes
- You must end your input with **end** operation.
- For more details on each operation input check the documentation of the program.
- For details on each element input form check the documentation of the program.
- You aren't required to input nodes individually, you can input only elements and **Circuit-Solver** will create the needed nodes for you.

## Documentation

### Operations

#### 1. Add
Operation String:
```
"ADD" or "add"
```
Input Form:
```
add elementType elementName [elementData]
```
Input Example:
```
add C C1 1 2 20u
```
#### 2. Edit
Operation String:
```
"EDT" or "edt"
```
Input Form:
```
edt elementType elementName [elementNewData]
```
Input Example:
```
edt C C1 1 2 40u
```
or
```
edt C C1 2 3 40u
```
**Note:** You can change any element data (e.g: PositiveNode, elementValue, controlElement..etc) by entering new data.

#### 3. Remove
Operation String:
```
"RMV" or "rmv"
```
Input Form:
```
rmv elementType elementName
```
Input Example:
```
rmv C C1
```
### Elements 

#### 1. Node
Element Type String:
```
"N" or "n"
```
Element Input Format:
```
n Name
```
Input Example:
```
n Node1
```

#### 2. Resistor
Type String:
```
"R" or "r"
```
Input Format:
```
r Name PositiveNode NegativeNode Resistance
```
Input Example:
```
r Resistor1 Node1 Ground 20K 
```
#### 3. Capacitor
Type String:
```
"C" or "c"
```
Input Format:
```
c Name PositiveNode NegativeNode Capcitance
```
Input Example:
```
c Capacitor1 Node1 Ground 20u
```
#### 4. Inductor
Type String:
```
"L" or "l"
```
Input Format:
```
L Name PositiveNode NegativeNode Inductance
```
Input Example:
```
L Inductor1 Node1 Ground 20m
```
#### 5. Voltage Source
Type String:
```
"VS", "Vs", "vS", or "vs"
```
Input Format:
```
vs Name PositiveNode NegativeNode SupplyVoltage [InternalImpedance = 0]
```
Input Example:
```
vs Battery Positive Ground 9 
```
#### 6. Current Source
Type String:
```
"CS", "Cs", "cS", or "cs"
```
Input Format:
```
cs Name PositiveNode NegativeNode SupplyCurrent [InternalAdmittance = 0]
```
Input Example:
```
cs currentSource Positive Ground 5 
```
#### 7. Voltage Controlled Voltage Source
Type String:
```
"VCVS", "vcvs", "VCvs", or "vcVS", ..etc
```
Input Format:
```
vcvs Name PositiveNode NegativeNode voltageFactor controlPositiveNode controlNegativeNode [InternalImpedance = 0]
```
Input Example:
```
vcvs controlled1 Positive Ground 5 Positive1 Negative1  
```
#### 8. Voltage Controlled Current Source
Type String:
```
"VCCS", "vccs", "VCcs", or "vcCS", ..etc
```
Input Format:
```
vccs Name PositiveNode NegativeNode currentFactor controlPositiveNode controlNegativeNode [InternalAdmittance = 0]
```
Input Example:
```
vccs controlled1 Positive Ground 5 Positive1 Negative1  
```
#### 9. Current Controlled Voltage Source
Type String:
```
"CCVS", "ccvs", "CCvs", or "ccVS", ..etc
```
Input Format:
```
ccvs Name PositiveNode NegativeNode voltageFactor controlElementType controlElementName [InternalImpedance = 0]
```
Input Example:
```
vcvs controlled1 Positive Ground 5 vs Battery   
```
#### 10. Current Controlled Current Source
Type String:
```
"CCCS", "cccs", "CCcs", or "ccCS", ..etc
```
Input Format:
```
cccs Name PositiveNode NegativeNode currentFactor controlElementType controlElementName [InternalAdmittance = 0]
```
Input Example:
```
vccs controlled1 Positive Ground 5 vs Battery   
```

---

### Elements Values (Complex Values)

**Circuit-Solver** lets you input complex or real values in one of two ways:

**1. Cartesian Format**
Allowed Formats:
```
(real)[prefix]
(imag)i[prefix]
(real)+(imag)i[prefix]
(real)-(imag)i[prefix]
(imag)i+(real)[prefix]
(imag)i-(real)[prefix]
```
Input Example:
```
50
30i
50+30iK
```
**Note:** You put the prefix at the end of the Cartesian form input.

**2. Polar Format**
Allowed Formats:
```
(mag)[prefix]<(angle)		//degrees
(mag)[prefix]<(angle)rad	//radians
(mag)[prefix]<(angle)deg	//degrees
```
Input Example:
```
10<20rad
10K<20deg
```
**Note:** You put the prefix after magnitude input in the Polar form input.

---

### Elements Values Prefixes

**Circuit-Solver** lets you specify a prefix to scale values.

#### Allowed Prefixes

Prefix | Abbreviation | Scale
------ | ------------ | -----
pico   | p            |	<img src="https://render.githubusercontent.com/render/math?math=10^{-12}">
nano   | n            |	<img src="https://render.githubusercontent.com/render/math?math=10^{-9}">
micro  | u            |	<img src="https://render.githubusercontent.com/render/math?math=10^{-6}">
milli  | m            |	<img src="https://render.githubusercontent.com/render/math?math=10^{-3}">
kilo   | K            |	<img src="https://render.githubusercontent.com/render/math?math=10^{3}">
mega   | M            |	<img src="https://render.githubusercontent.com/render/math?math=10^{6}">
giga   | G            |	<img src="https://render.githubusercontent.com/render/math?math=10^{9}">
tera   | T            |	<img src="https://render.githubusercontent.com/render/math?math=10^{12}">

**Note:** kilo, mega, giga, and tera have capital abbreviations.
