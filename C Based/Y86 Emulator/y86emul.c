/*
 * y86emul.c
 *
 *  Created on: Apr 15, 2016
 *      Author: pawel
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

//Start of global variables (registers, flags, etc.)


int reg[10]; //eax = 0, ecx = 1, edx = 2, ebx = 3, esp = 4, ebp = 5, esi = 6, edi = 7, and the last two 'registers', reg[8] is the program counter and reg[9] is the point at where the program counter should stop
// Note: User should not know about the extra "registers" thus, the user can modify all the registers without having adverse affects
struct Flag{
	unsigned int isActive : 1;
}OF, ZF, SF, text; // text added for a boolean to see if .text directive was found

unsigned int size;// size of memory block

typedef enum{AOK, HLT, ADR, INS} progStatus;
progStatus status = AOK; // Program status implemented as prescribed. Initial status is always AOK.

unsigned char * mem; //virtual memory for the program. Unsigned char pointer due to no padding present

union{
	char hex[4];
	int decimal;
}converter; // converts decimal to hex, used when long directive is found

int checkAddr(int addr){// Checks if addr is out of memory limit
	if(addr < 0 || addr >= size){
		status = ADR;
		return 1;
	}else
		return 0;
}

int getInt(int addr){
	unsigned int i;
	for(i = 0; i < 4; i++){
		converter.hex[i] = mem[addr+i];
	}
	return converter.decimal;
}
void placeInt(int addr, int deci){
	converter.decimal = deci;
	unsigned int i;
	for(i = 0; i < 4; i++){
		mem[addr+i] = converter.hex[i];
	}
}

// Next 2 functions are implemented to create a working stack, push and pop. However, the following variable makes sure the stack doesn't go too high and overwrite information
unsigned int highaddr = 0; // highest addr found while putting information into mem.

int pop(){
	int data= getInt(reg[4]);
	reg[4]= reg[4]+4;
	return data;
}
int push(int data){

	reg[4]= reg[4]-4; //register 4 is ESP, which always contains the address of a location in the stack

	if(reg[4]<=highaddr){ // The highest address in normal data must be lower than the lowest address in the stack
		printf("ERROR: Stack overflowed!\n");
		status = INS;
		return 1;
	}

	placeInt(reg[4], data);

	return 0;
}

void FlagSetter(int first, int second, int result, int op){// OP is the operation code, to go into in-depth flag setting. First and second are the respective values from that op.
	// Flags are reset upon each call to this function, due to a new OP call
	OF.isActive = 0;
	ZF.isActive = 0;
	SF.isActive = 0;

	if(result == 0)
		ZF.isActive = 1;
	else if(result < 0)
		SF.isActive = 1;

	if(op == 0){// ADDL

		if( (( first > 0) == (second > 0 )) && (( result > 0) != (first > 0) )) // First and second value agree in sign, but result does not agree with that sign
			OF.isActive = 1;

	}else if(op == 1){// SUBL

		if( (( first > 0) == (second < 0 )) && (( result > 0) != (first > 0) )) // First and second don't agree in sign, thus adding. Result must agree with sign of the first and opposite of second.
			OF.isActive = 1;

	}else if(op == 4){// MULL

		if(result / second != first) // F * S = R -> R / S = F.
			OF.isActive = 1;
	}

}

int DecodeExecute(){//returns 0 upon successful decode and execute, 1 otherwise

	unsigned int comm1, comm2; // these two ints hold the command when put side by side. I.E. comm1 = 0 and comm2 = 0 means the command is NOP.
	unsigned int rA, rB; // represents the values being used in the command
	unsigned int disp; //displacement

	while(reg[8] != reg[9]){

		if(checkAddr(reg[8]) == 1){
			printf("ERROR: Program counter went out of bounds!");
			status = ADR;
			return 1;
		}
		comm2 = mem[reg[8]] % 16; //Standard way of converting decimal to hex
		comm1 = (mem[reg[8]]/16) % 16; //Can only have 2 bits

		if(comm1 == 0){//NOP case
			if(comm2 != 0){//if comm2 is not zero, the instruction is not NOP
				printf("ERROR: Incomplete NOP encountered.\n");
				status = INS;
				return 1;
			}
			reg[8]++;// And NOP does nothing, so move forward a byte
		}else if(comm1 == 1){// HLT is input, as only HLT starts with a 1.
			if(comm2 != 0){
				printf("ERROR: Incomplete HALT encountered.\n");
				status = INS;
				return 1;
			}
			printf("HALT encountered. Terminating program.\n");
			status = HLT;
			return 0;
		}else if(comm1 == 2){// RRMOVL
			if(comm2 != 0){
				printf("ERROR: Incomplete RRMOVL encountered.\n");
				status = INS;
				return 1;
			}

			reg[8]++;
			rB = mem[reg[8]] % 16;
			rA = (mem[reg[8]] /16) % 16;
			reg[rB] = reg[rA];

			reg[8]++;
		}else if(comm1 == 3){// IRMOVL
			if(comm2 != 0){
				printf("ERROR: Incomplete IRMOVL encountered.\n");
				status = INS;
				return 1;
			}

			reg[8]++;
			rB = mem[reg[8]] % 16;

			//Have to put a long into rB
			reg[rB] = getInt(reg[8]+1);

			reg[8]+=5;
		}else if(comm1 == 4){//RMMOVL - questionable
			if(comm2 != 0){
				printf("ERROR: Incomplete RMMOVL encountered.\n");
				status = INS;
				return 1;
			}

			reg[8]++;
			rB = mem[reg[8]] % 16;
			rA = (mem[reg[8]] /16) % 16;

			disp = getInt(reg[8]+1);
			disp += reg[rB]; // disp acts as an address now

			if(checkAddr(disp)==1){
				printf("ERROR: Address went out of bounds during RMMOVL.\n");
				status = ADR;
				return 1;
			}

			placeInt(disp, reg[rA]);
			reg[8]+=5;
		}else if(comm1 == 5){//MRMOVL
			if(comm2 != 0){
				printf("ERROR: Incomplete MRMOVL encountered.\n");
				status = INS;
				return 1;
			}

			reg[8]++;
			rB = mem[reg[8]] % 16;
			rA = (mem[reg[8]] /16) % 16;

			disp = getInt(reg[8]+1);
			disp += reg[rB]; // disp acts as an address now

			if(checkAddr(disp+3)==1){
				printf("ERROR: Address went out of bounds during MRMOVL.\n");
				status = ADR;
				return 1;
			}

			reg[rA] = getInt(disp);

			reg[8] +=5;
		}else if(comm1 == 6){// Start of OP calls, operations.
			int res;

			reg[8]++;
			rB = mem[reg[8]] % 16;
			rA = (mem[reg[8]] /16) % 16;

			if(comm2 == 0){//ADDL
				res = reg[rA] + reg[rB];

				FlagSetter(reg[rA], reg[rB], res, 0);

				reg[rB] = res;
			}else if(comm2 == 1 || comm2 == 5){//SUBL and CMPL -> CMPL does implicit subtraction, only difference is that the result isn't saved
				res = reg[rB] - reg[rA];
				FlagSetter(reg[rB], reg[rA], res, 1);

				if(comm2 != 5){
					reg[rB] = res;
				}

			}else if(comm2 == 2){//ANDL
				res = reg[rB] & reg[rA];
				FlagSetter(reg[rB], reg[rA], res, 2);
				reg[rB] = res;

			}else if(comm2 == 3){//XORL
				res = reg[rB] ^ reg[rA];
				FlagSetter(reg[rB], reg[rA], res, 3);
				reg[rB] = res;

			}else if(comm2 == 4){//MULL
				res = reg[rB] * reg[rA];
				FlagSetter(reg[rB], reg[rA], res, 4);
				reg[rB] = res;

			}else{
				printf("ERROR: Incomplete OP command encountered. \n");
				status = INS;
				return 1;
			}

			reg[8]++;

		}else if(comm1 == 7){// Start of jump commands

			reg[8]++;

			int dest = getInt(reg[8]);

			if(checkAddr(dest) == 1 || checkAddr(reg[8] + 3) == 1){
				printf("ERROR: Address went out of bounds during a jump call.\n");
				status = ADR;
				return 1;
			}

			if(comm2 == 0){// JMP -> may or may not need to add continues.
				reg[8] = dest;
				continue;
			}else if(comm2 == 1){// JLE
				if(ZF.isActive == 1 || SF.isActive != OF.isActive){
					reg[8] = dest;
					continue;
				}
			}else if(comm2 == 2){// JL
				if(SF.isActive != OF.isActive){
					reg[8] = dest;
					continue;
				}
			}else if(comm2 == 3){// JE
				if(ZF.isActive == 1){
					reg[8] = dest;
					continue;
				}
			}else if(comm2 == 4){// JNE
				if(ZF.isActive == 0){
					reg[8] = dest;
					continue;
				}
			}else if(comm2 == 5){// JGE
				if(SF.isActive == OF.isActive){
					reg[8] = dest;
					continue;
				}
			}else if(comm2 == 6){// JG
				if(ZF.isActive == 0 && SF.isActive == OF.isActive){
					reg[8] = dest;
					continue;
				}
			}

			reg[8]+=4;

		}else if(comm1 == 8){// CALL

			if(checkAddr(reg[8]+5) == 1){
				printf("ERROR: Address went out of bounds during a call... call?\n");
				status = ADR;
				return 1;
			}

			if(push(reg[8]+5) == 1){
				return 1;
			}

			reg[8] = getInt(reg[8]+1);

		}else if(comm1 == 9){// RET
			reg[8] = pop();
		}else if(comm1 == 10){// PUSHL
			reg[8]++;
			rA = (mem[reg[8]] /16) % 16;

			if(push(reg[rA]) == 1){
				return 1;
			}

			reg[8]++;
		}else if(comm1 == 11){// POPL
			reg[8]++;
			rA = (mem[reg[8]] /16) % 16;

			reg[rA] = pop();

			reg[8]++;

		}else if(comm1 == 12){// start of read commands
			OF.isActive = 0;
			SF.isActive = 0;// Read only sets ZF, but all get reset.
			ZF.isActive = 0;

			reg[8]++;
			rA = (mem[reg[8]] /16) % 16;

			reg[8]++;

			disp = getInt(reg[8])+reg[rA];

			if(checkAddr(disp) == 1){
				printf("ERROR: Destination for read call is out of bounds.\n");
				status = ADR;
				return 1;
			}

			int fileStatus;

			if(comm2 == 0){// READB
				char byte;
				fileStatus = scanf("%c", &byte);

				if(fileStatus == EOF){
					ZF.isActive = 1;
				}else{
					mem[disp] = byte;
				}

			}else if(comm2 == 1){// READL
				int data;
				fileStatus = scanf("%d", &data);

				if(fileStatus == EOF){
					ZF.isActive = 1;
				}else{
					placeInt(disp, data);
				}
			}else{
				printf("ERROR: Incomplete READ command encountered. \n");
				status = INS;
				return 1;
			}
			reg[8]+=4;
		}else if(comm1 == 13){// start of write commands

			reg[8]++;
			rA = (mem[reg[8]] /16) % 16;

			reg[8]++;
			disp = getInt(reg[8])+reg[rA];

			if(checkAddr(disp) == 1){
				printf("ERROR: Source for write call is out of bounds.\n");
				status = ADR;
				return 1;
			}

			if(comm2 == 0){// WRITEB
				printf("%c", mem[disp]);
			}else if(comm2 == 1){// WRITEL
				printf("%d", getInt(disp));
			}else{
				printf("ERROR: Incomplete WRITE command encountered. \n");
				status = INS;
				return 1;
			}

			reg[8]+=4;
		}else if(comm1 == 14){// MOVSBL
			if(comm2 != 0){
				printf("ERROR: Incomplete MOVSBL command encountered. \n");
				status = INS;
				return 1;
			}

			reg[8]++;
			rB = mem[reg[8]] % 16;
			rA = (mem[reg[8]] /16) % 16;

			reg[8]++;
			disp = getInt(reg[8]) + reg[rB];

			if(checkAddr(disp) == 1){
				printf("ERROR: Invalid address accessed for MOVSBL.\n");
				status = ADR;
				return 1;
			}

			if((mem[disp] & 0x80) == 0){
				reg[rA] = mem[disp] | 0x00000000;
			}else{
				reg[rA] = mem[disp] | 0xFFFFFF00;
			}

			reg[8]+=4;

		}else{
			printf("ERROR: Unknown command call.\n");
			status = INS;
			return 1;
		}


	}
	return 0;
}


void printStatus(){// Prints the status of the program at the time called.
	if(status == AOK){
		printf("Program Status: AOK.\n");
	}else if(status == HLT){
		printf("Program Status: HLT.\n");
	}else if(status == ADR){
		printf("Program Status: ADR.\n");
	}else if(status == INS){
		printf("Program Status: INS.\n");
	}else{
		printf("How on Earth did you get here? This wasn't supposed to happen...\n");
	}
}


int main(int argc, char ** argv){//performs fetch

	if(strcmp(argv[1], "-h") == 0){
		printf("To use this program, enter the path for the file to be read. I.E. \"./Desktop/prog1.y86\" \n");
		return 0;
	}
	// Initialize global vars
	OF.isActive = 0;
	ZF.isActive = 0;
	SF.isActive = 0;
	text.isActive = 0;
	FILE * fp = fopen(argv[1], "r");

	if(fp == NULL || argc < 2){
		printf("ERROR: The file you provided does not exist.\n");
		return 0;
	}
	// Create and initialize local vars
	char * directive = (char *) malloc(8); //contains the directive, .string is 7 chars, 8 for null byte.
	int temp = fscanf(fp, "%s\t%x", directive, &size); //temp is created to satisfy compiler for unused result, along with EOF checking
	unsigned int addr, info; //unsigned ints to hold the address and bytes respectively. Unsigned, since addr nor data can be negative

	if(temp == EOF){
		printf("ERROR: File is rather empty!\n");
		printf("Program Status: INS \n");
		return 1;
	}

	if(strcmp(directive, ".size")!=0){
		printf("ERROR: The first directive found was not the .size directive! \n");
		printf("Program Status: INS \n");
		return 1;
	}else
		mem = (unsigned char *)malloc(size);


	while(fscanf(fp, "%s", directive) != EOF){//start of directive scanning and sorting

		if(strcmp(directive, ".text") == 0){

			if(text.isActive == 1){
				printf("ERROR: More than one text directive found!\n");
				printf("Program Status: INS \n");
				return 1;
			}else
				text.isActive = 1;

			temp = fscanf(fp, "\t %x \t", &addr);
			reg[8] = addr;//register 8 will hold the start addr of the commands
			char * splice = (char *)malloc(3);
			splice = fgets(splice, 3, fp);
			int counter = addr; // counter will hold the end addr of the commands
			while( splice[0] != '\0' && splice[0] != '\n' && splice != NULL){ // Each splice is a byte of the command line.
				if(checkAddr(counter) == 1){
					printf("ERROR: Instruction set from the text directive reached outside of memory limit. \n");
					printf("Program Status: ADR \n");
					return 1;
				}
				sscanf(splice, "%x", &info); //does a nice job of converting ascii into hex, then to a decimal value
				mem[counter]=info;
				splice = fgets(splice, 3, fp);
				counter++;
			}
			if(highaddr < counter)
				highaddr = counter;
			reg[9] = counter;

		}else if(strcmp(directive, ".byte") == 0){
			temp = fscanf(fp, "\t %x \t %x", &addr, &info);
			if(checkAddr(addr) == 1){
				printf("ERROR: An address of one of the byte directives is out of bounds.\n");
				printf("Program Status: ADR \n");
				return 1;
			}
			mem[addr] = info;
			if(highaddr < addr)
				highaddr = addr;
		}else if(strcmp(directive, ".string") == 0){

			temp = fscanf(fp, "\t %x \t", &addr);
			char c = fgetc(fp);//Grabs the string by going through a char at a time. First char should be an open quote, thus avoided.

			while((c = fgetc(fp)) != '\"'){
				if(checkAddr(addr) == 1){
					printf("ERROR: A string directive reached outside of memory limit. \n");
					printf("Program Status: ADR \n");
					return 1;
				}
				mem[addr]=c;
				addr++;
			}
			if(highaddr < addr)
				highaddr = addr;

		}else if(strcmp(directive, ".long") == 0){
			temp = fscanf(fp, "\t %x \t %d", &addr, &converter.decimal);

			unsigned int i;
			for(i = 0; i < 4; i++){
				if(checkAddr(addr) == 1){
					printf("ERROR: A long directive reached outside of memory limit. \n");
					printf("Program Status: ADR \n");
					return 1;
				}
				mem[addr+i] = converter.hex[i];
			}
			if(highaddr < addr)
				highaddr = addr;
		}

	}

	if(DecodeExecute() == 1){//something went awry
		printStatus();
		return 1;
	}

	printStatus();
	return 0;
}
