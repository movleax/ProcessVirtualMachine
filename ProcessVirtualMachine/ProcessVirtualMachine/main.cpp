#include <iostream>
#include <vector>
using namespace std;

/**
* HALT = 0
* LOAD = 1
* ADD = 2
* SUB = 3
* MULT = 4
* DIV = 5
* CMP = 6
* JMP = 7
* JE / JZ	= 8			Jump Equal or Jump Zero							ZF
* JNE / JNZ = 9			Jump not Equal or Jump Not Zero					ZF
* JG / JNLE = 10		Jump Greater or Jump Not Less / Equal		OF, SF, ZF
* JGE / JNL = 11		Jump Greater / Equal or Jump Not Less		OF, SF
* JL / JNGE = 12		Jump Less or Jump Not Greater / Equal		OF, SF
* JLE / JNG = 13		Jump Less / Equal or Jump Not Greater		OF, SF, ZF
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
Instruction instructions[] = { { (char)0x01, (short)0x8000, 0x00001000 }, { (char)0x01, (short)0x4000, 0x00000100 }, { (char)0x02, (short)0x8480, 0x00000000 },{ (char)0x06, (short)0x8480, 0x00000001 }, { (char)0x00, (short)0x0000, 0x00000000 } };
unsigned currInst = -1;
unsigned numberOfInstructions = 5;


std::vector<Instruction> memory;
std::vector<int> variables;


char opCode = 0;
int reg1 = 0;
int reg2 = 0;
int reg3 = 0;
int reg4 = 0;
int regSlot[4] = { 0 };
int imm = 0;

bool OF = false;
bool CF = false;
bool ZF = false;
bool SF = false;


void fetch()
{
	if (currInst == numberOfInstructions)
	{
		cout << "End of instructions\n";
		running = false;
		return;
	}

	currInst++;
}

void decode()
{
	if (currInst < 0)
	{
		cout << "Incorrect instruction index\n";
		running = false;
		return;
	}

	opCode = instructions[currInst].opCode;

	regSlot[0] = (instructions[currInst].registers & 0xF000) >> 12;
	regSlot[1] = (instructions[currInst].registers & 0x0F00) >> 8;
	regSlot[2] = (instructions[currInst].registers & 0x00F0) >> 4;
	regSlot[3] = (instructions[currInst].registers & 0x000F);

	/*for (int i = 0; i < 4; i++)
	{
		if (reg1 == 0) reg1 = (regSlot[i] & 8) >> 3;
		if (reg2 == 0) reg2 = (regSlot[i] & 4) >> 2;
		if (reg3 == 0) reg3 = (regSlot[i] & 2) >> 1;
		if (reg4 == 0) reg4 = (regSlot[i] & 1);
	}*/
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
	default: cout << "Invalid register number\n"; running = false;
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
		running = false;
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
		running = false;
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
		running = false;
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
		running = false;
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
		running = false;
		return;
	}

	if (*selectedRegisterB == 0)
	{
		cout << "Cannot divide by zero\n";
		running = false;
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

void jmp()
{
	// TODO check for out of bounds of program's alloted memory and its memory that should not be accessible
	if (imm < 0)
	{
		cout << "Cannot jump out of bounds of alotted program memory\n";
		running = false;
		return;
	}

	currInst = imm;
}

// http://unixwiz.net/techtips/x86-jumps.html
void cmp()
{
	// what is the first selected register
	int *selectedRegisterA = SelectRegister(regSlot[0]);
	int *selectedRegisterB = SelectRegister(regSlot[1]);
	int *selectedRegisterC = SelectRegister(regSlot[2]);

	if (selectedRegisterA == 0 || selectedRegisterB == 0 || selectedRegisterC == 0)
	{
		cout << "Cannot load value into null register\n";
		running = false;
		return;
	}

	// JE / JZ		Jump Equal or Jump Zero							ZF
	if (*selectedRegisterA == *selectedRegisterB)
	{
		ZF = true;
		cout << "*selectedRegisterA == *selectedRegisterB\n";
	}
	// JNE / JNZ		Jump not Equal or Jump Not Zero					ZF
	else
	{
		ZF = false;
		cout << "*selectedRegisterA != *selectedRegisterB\n";
	}
	
	// JG / JNLE		Jump Greater or Jump Not Less / Equal		OF, SF, ZF
	if (*selectedRegisterA > *selectedRegisterB)
	{ 
		OF = false;
		SF = false;
		// ZF == False -- handled in JE / JNE if statement
		cout << "*selectedRegisterA > *selectedRegisterB\n";
	}
	// JGE / JNL		Jump Greater / Equal or Jump Not Less		OF, SF
	else
	{
		OF = true;
		SF = true;
		cout << "*selectedRegisterA <= *selectedRegisterB\n";
	}

	// JL / JNGE		Jump Less or Jump Not Greater / Equal		OF, SF
	if (*selectedRegisterA < *selectedRegisterB)
	{
		OF = true;
		SF = true;
		cout << "*selectedRegisterA < *selectedRegisterB\n";
	}

	// JLE / JNG		Jump Less / Equal or Jump Not Greater		OF, SF, ZF
	else
	{
		OF = false;
		SF = false;
		// ZF == True -- handled in JE / JNE if statement
		cout << "*selectedRegisterA >= *selectedRegisterB\n";
	}

	//*selectedRegisterC = *selectedRegisterA + *selectedRegisterB;
}

void JE_JZ()
{
	if (ZF == false)
		return;
	

}

void JNE_JNZ()
{
	if (ZF == true)
		return;
}

void JG_JNLE()
{
	if (!(OF == SF && ZF == false))
		return;
}

void JGE_JNL()
{
	if (!(OF == SF))
		return;
}

void JL_JNGE()
{
	if (!(OF != SF))
		return;
}

void JLE_JNG()
{
	if (!(OF != SF && ZF == true))
		return;
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
	case 0x06: cmp(); break;
	case 0x07: jmp(); break;
	case 0x08: JE_JZ(); break;
	case 0x09: JNE_JNZ(); break;
	case 0x0A: JG_JNLE(); break;
	case 0x0B: JGE_JNL(); break;
	case 0x0C: JL_JNGE(); break;
	case 0x0D: JLE_JNG(); break;
	}
}

int main()
{
	for (int i = 0; i < numberOfInstructions; i++)
	{
		memory.push_back(instructions[i]);
	}

	int nop = 0;

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