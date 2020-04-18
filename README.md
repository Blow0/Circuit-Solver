
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

Before you insert the circuit you wish to solve, make sure that you name every element in the circuit.
**Note:** Names are ***case-insensitive*** and no two elements can have the same name unless they have different types.

### 3. Circuit Insertion

**Circuit-Solver** stores the circuit in the form of a net list of elements.

***3. 1. Insertion Format***
**Circuit-Solver** allows you to edit the stored net list using three operations and has this general input format:
```
OperationType ElementType ElementName [ElementData]
```

***3. 2. Insertion Operation***
**Circuit-Solver** have three insertion-related operations:
- **add:** Adds a new element into the circuit net list.
- **rmv:** Removes an existing element in the circuit net list.
- **edt:**  Edits an existing element in the circuit net list.
- **end:** Ends the insertion and saves the net list.

***3. 3. Element Input***
**Circuit-Solver** have a lot of elements but one thing in common any element input starts with a string indicating its type and then is followed by the name of the element and then the element data which depends on the type of element and operation type.
**Note:** Element type and name are converted to lowercase letters.

***Insertion Input Example*** 
```
add cs I1 1 4 20K<1.02rad
edt cs I1 1 4 30+50iK
add r R1 1 4 8
add r R2 1 2 4
add r R3 2 3 5
add r R4 3 4 6
rmv r R1
end
```

#### Notes
- You must end your input with **end** operation.
- For more details on each operation input check the documentation of the program.
- For details on each element input form check the documentation of the program.
- You aren't required to input nodes individually, you can input only elements and **Circuit-Solver** will create the needed nodes for you.

## Documentation

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
pico   | p            |	$10^{-12}$
nano   | n            |	$10^{-9}$
micro  | u            |	$10^{-6}$
milli  | m            |	$10^{-3}$
kilo   | K            |	$10^{3}$
mega   | M            |	$10^{6}$
giga   | G            |	$10^{9}$
tera   | T            |	$10^{12}$

**Note:** kilo, mega, giga, and tera have capical abbreviations.
