#include <iostream>
#include <fstream>
using namespace std;

typedef struct
{
	char instruction;
	char constant;
}VMCommand;

int main()
{
	ifstream decryptor("decryptor.bin", ios::in|ios::binary);
	ifstream input("q1_encr.txt");
	ofstream result("decrypted.txt");
	unsigned char regs[16];
	char prog_mem[256];
	int register1, register2;
	bool eofflag = false;
	bool halt = false;
	bool zero_flag = false;
	for (int i = 0; i < 256; i++)
	{
		decryptor >> noskipws >> prog_mem[i];
		if (decryptor.eof())
			break;
	}
	decryptor.close();
	cout << endl;
	VMCommand* command_ptr = (VMCommand*)prog_mem;
	while (!halt)
	{
		register1 = command_ptr->constant & 0x0F;
		register2 = (command_ptr->constant & 0xF0) >> 4;
		switch (command_ptr->instruction)
		{
		case 0x01:
			regs[register1]++;
			break;
		case 0x02:
			regs[register1]--;
			break;
		case 0x03:
			regs[register1] = regs[register2];
			break;
		case 0x04:
			regs[register1] = command_ptr->constant;
			break;
		case 0x05:
			regs[register1] = regs[register1] << 1;
			break;
		case 0x06:
			regs[register1] = regs[register1] >> 1;
			break;
		case 0x07:
			command_ptr = (VMCommand*)(command_ptr->constant + (char*)command_ptr);
			command_ptr--;
			break;
		case 0x08:
			if (!zero_flag)
			{
				command_ptr = (VMCommand*)(command_ptr->constant + (char*)command_ptr);
				command_ptr--;
			}
			break;
		case 0x09:
			if (zero_flag)
			{
				command_ptr = (VMCommand*)(command_ptr->constant + command_ptr);
				command_ptr--;
			}
			break;
		case 0x0A:
			if (eofflag)
			{
				command_ptr = (VMCommand*)(command_ptr->constant + (char*)command_ptr);
				command_ptr--;
			}
			break;
		case 0x0B:
			halt = true;
			input.close();
			break;
		case 0x0C:
			regs[register1] += regs[register2];
			if (regs[register1] == 0) zero_flag = true;
			else zero_flag = false;
			break;
		case 0x0D:
			regs[register1] -= regs[register2];
			if (regs[register1] == 0) zero_flag = true;
			else zero_flag = false;
			break;
		case 0x0E:
			regs[register1] = regs[register1] ^ regs[register2];
			break;
		case 0x0F:
			regs[register1] = regs[register1] | regs[register2];
			break;
		case 0x10:
			input >> regs[register1];
			if (input.eof()) eofflag = true;
			break;
		case 0x11:
			result << (char) regs[register1];
			break;
		}
		command_ptr++;
	}
	decryptor.close();
	return 0;
}
