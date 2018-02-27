#include <iostream>

using namespace std;

/**
* HALT = 0
* LOAD = 1
* ADD = 2
* SUB = 3
* MULT = 4
* DIV = 5
*
*
*/

/**
*
* Reg		0000	0001	0010	0011	0100	0101	0110	0111	1000	1001	1010	1011	1100	1101	1110	1111
* Value		 0	     1	     2       3	     4	     5	     6	     7	     8	     9	     A	     B	     C       D       E	     F
*
* register 1 == 8
* register 2 == 4
* registers 1 and 2 == C
*/
struct Instruction
{
	char opCode;
	short registers;
	int value;

	Instruction(const char &oc, const short &reg, const int &v)
	{
		opCode = oc;
		registers = reg;
		value = v;
	}

	~Instruction()
	{

	}

private:
	Instruction()
	{
		// no private construcor available
	}
};

bool running = true;
Instruction instructions[] = { { (char)0x01, (short)0x8000, 0x00001000 },{ (char)0x01, (short)0x4000, 0x00000100 },{ (char)0x02, (short)0x8420, 0x00000000 },{ (char)0x00, (short)0x0000, 0x00000000 } };
unsigned currInst = -1;
unsigned numberOfInstructions = 4;

char opCode = 0;
int reg1 = 0;
int reg2 = 0;
int reg3 = 0;
int reg4 = 0;
int regSlot[4] = { 0 };
int imm = 0;


void fetch()
{
	if (currInst == numberOfInstructions)
	{
		cout << "End of instructions\n";
		return;
	}

	currInst++;
}

void decode()
{
	if (currInst < 0)
	{
		cout << "Incorrect instruction index\n";
		return;
	}

	opCode = instructions[currInst].opCode;

	regSlot[0] = (instructions[currInst].registers & 0xF000) >> 12;
	regSlot[1] = (instructions[currInst].registers & 0x0F00) >> 8;
	regSlot[2] = (instructions[currInst].registers & 0x00F0) >> 4;
	regSlot[3] = (instructions[currInst].registers & 0x000F);

	for (int i = 0; i < 4; i++)
	{
		if (reg1 == 0) reg1 = (regSlot[i] & 8) >> 3;
		if (reg2 == 0) reg2 = (regSlot[i] & 4) >> 2;
		if (reg3 == 0) reg3 = (regSlot[i] & 2) >> 1;
		if (reg4 == 0) reg4 = (regSlot[i] & 1);
	}
	imm = instructions[currInst].value;
}

int *SelectRegister(const int &r)
{
	int *ret = 0;

	switch (r)
	{
	case 8: ret = &reg1; break;
	case 4: ret = &reg2; break;
	case 2: ret = &reg3; break;
	case 1: ret = &reg4; break;
	default: cout << "Invalid register number\n";
	}

	return ret;
}

void load()
{
	// what is the first selected register
	int *selectedRegister = SelectRegister(regSlot[0]);

	if (selectedRegister == 0)
	{
		cout << "Cannot load value into null register\n";
		return;
	}

	*selectedRegister = instructions[currInst].value;
}

void add()
{
	// what is the first selected register
	int *selectedRegisterA = SelectRegister(regSlot[0]);
	int *selectedRegisterB = SelectRegister(regSlot[1]);
	int *selectedRegisterC = SelectRegister(regSlot[2]);

	if (selectedRegisterA == 0 || selectedRegisterB == 0 || selectedRegisterC == 0)
	{
		cout << "Cannot load value into null register\n";
		return;
	}

	*selectedRegisterC = *selectedRegisterA + *selectedRegisterB;
}

void sub()
{
	// what is the first selected register
	int *selectedRegisterA = SelectRegister(regSlot[0]);
	int *selectedRegisterB = SelectRegister(regSlot[1]);
	int *selectedRegisterC = SelectRegister(regSlot[2]);

	if (selectedRegisterA == 0 || selectedRegisterB == 0 || selectedRegisterC == 0)
	{
		cout << "Cannot load value into null register\n";
		return;
	}

	*selectedRegisterC = *selectedRegisterA - *selectedRegisterB;
}

void mult()
{
	// what is the first selected register
	int *selectedRegisterA = SelectRegister(regSlot[0]);
	int *selectedRegisterB = SelectRegister(regSlot[1]);
	int *selectedRegisterC = SelectRegister(regSlot[2]);

	if (selectedRegisterA == 0 || selectedRegisterB == 0 || selectedRegisterC == 0)
	{
		cout << "Cannot load value into null register\n";
		return;
	}

	*selectedRegisterC = *selectedRegisterA * *selectedRegisterB;
}

void div()
{
	// what is the first selected register
	int *selectedRegisterA = SelectRegister(regSlot[0]);
	int *selectedRegisterB = SelectRegister(regSlot[1]);
	int *selectedRegisterC = SelectRegister(regSlot[2]);

	if (selectedRegisterA == 0 || selectedRegisterB == 0 || selectedRegisterC == 0)
	{
		cout << "Cannot load value into null register\n";
		return;
	}

	if (*selectedRegisterB == 0)
	{
		cout << "Cannot divide by zero\n";
		return;
	}

	*selectedRegisterC = *selectedRegisterA / *selectedRegisterB;
}

void printRegisters()
{
	cout << "reg1 = " << std::hex << reg1 << "\n";
	cout << "reg2 = " << std::hex << reg2 << "\n";
	cout << "reg3 = " << std::hex << reg3 << "\n";
	cout << "reg4 = " << std::hex << reg4 << "\n";
	cout << "\n";
}

void execute()
{

	switch (opCode)
	{
	case 0x00: running = false; break;
	case 0x01: load(); break;
	case 0x02: add(); break;
	case 0x03: sub(); break;
	case 0x04: mult(); break;
	case 0x05: div(); break;
	}
}

int main()
{

	while (running)
	{
		fetch();
		decode();
		execute();
		printRegisters();
	}

	cin.get();

	return 0;
}