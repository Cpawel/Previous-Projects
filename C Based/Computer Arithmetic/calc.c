/*
 * calc.c
 *
 *  Created on: Mar 3, 2016
 *      Author: pawel
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int BinToDec(char * bin){
	int ret = 0; // return value
	int cur = 1; // current value in binary
	int i;
	for(i = strlen(bin) -1; i >= 0; i--){
		if(bin[i] == '1')
			ret += cur;
		else if(bin[i] > '1'){
			printf("ERROR: A number given is in an invalid BINARY format!\n");
			return -1;
		}
		cur *= 2;
	}
	return ret;
}

int OctToDec(char * oct){
	int ret = 0; // return value
	int cur = 1; // current value in octal
	int i;
	for(i = strlen(oct) -1; i >= 0; i--){
		if(oct[i] == '0')
			ret += 0;
		else if(oct[i] == '1')
			ret += cur;
		else if(oct[i] == '2')
			ret += 2*cur;
		else if(oct[i] == '3')
			ret += 3*cur;
		else if(oct[i] == '4')
			ret += 4*cur;
		else if(oct[i] == '5')
			ret += 5*cur;
		else if(oct[i] == '6')
			ret += 6*cur;
		else if(oct[i] == '7')
			ret += 7*cur;
		else{
			printf("ERROR: A number given is in an invalid OCTAL format!\n");
			return -1;
		}
		cur *= 8;
	}
	return ret;
}

int DeciToDec(char * deci){
	int ret = 0; // return value
	int cur = 1; // current value in decimal
	int i;
	for(i = strlen(deci) -1; i >= 0; i--){
		if(deci[i] == '0')
			ret += 0;
		else if(deci[i] == '1')
			ret += cur;
		else if(deci[i] == '2')
			ret += 2*cur;
		else if(deci[i] == '3')
			ret += 3*cur;
		else if(deci[i] == '4')
			ret += 4*cur;
		else if(deci[i] == '5')
			ret += 5*cur;
		else if(deci[i] == '6')
			ret += 6*cur;
		else if(deci[i] == '7')
			ret += 7*cur;
		else if(deci[i] == '8')
			ret += 8*cur;
		else if(deci[i] == '9')
			ret += 9*cur;
		else{
			printf("ERROR: A number given is in an invalid DECIMAL format!\n");
			return -1;
		}
		cur *= 10;
	}
	return ret;
}

int HexToDec(char * hex){
	int ret = 0; // return value
	int cur = 1; // current value in hexadecimal
	int i;
	for(i = strlen(hex) -1; i >= 0; i--){
		if(hex[i] == '0')
			ret += 0;
		else if(hex[i] == '1')
			ret += cur;
		else if(hex[i] == '2')
			ret += 2*cur;
		else if(hex[i] == '3')
			ret += 3*cur;
		else if(hex[i] == '4')
			ret += 4*cur;
		else if(hex[i] == '5')
			ret += 5*cur;
		else if(hex[i] == '6')
			ret += 6*cur;
		else if(hex[i] == '7')
			ret += 7*cur;
		else if(hex[i] == '8')
			ret += 8*cur;
		else if(hex[i] == '9')
			ret += 9*cur;
		else if(hex[i] == 'A' || hex[i] == 'a')
			ret += 10*cur;
		else if(hex[i] == 'B' || hex[i] == 'b')
			ret +=11*cur;
		else if(hex[i] == 'C' || hex[i] == 'c')
			ret += 12*cur;
		else if(hex[i] == 'D' || hex[i] == 'd')
			ret += 13*cur;
		else if(hex[i] == 'E' || hex[i] == 'e')
			ret += 14*cur;
		else if(hex[i] == 'F' || hex[i] == 'f')
			ret += 15*cur;
		else{
			printf("ERROR: A number given is in an invalid HEX format!\n");
			return -1;
		}
		cur *= 16;
	}
	return ret;
}

int DeciToBin(int * num){
	int isNeg = 0;
	if(*num < 0){
		isNeg = 1;
		*num *= -1;
	}
	int size = 0;
	int tmp = *num; // used as a holder for num at first, and then a counter for ret.
	while(tmp > 0){
		tmp /= 2;
		size++;
	}
	tmp = 0;
	char * ret = (char *) malloc(size+1);
	while(num > 0){
		if(*num % 16 == 0)
			ret[tmp] = '0';
		else if(*num % 16 == 1)
			ret[tmp] = '1';
		*num /= 2;
		tmp++;
	}
	ret[size] = '\0';
	int start = 0;
	int end = size -1;
	char holder;
	while(start < end){
		holder = ret[start];
		ret[start] = ret[end];
		ret[end] = holder;
		end--;
		start ++;
	}
	if(isNeg == 0)
		printf("b%s\n", ret);
	else
		printf("-b%s\n", ret);
	return 0;
}

int DeciToOct(int * num){
	int isNeg = 0;
	if(*num < 0){
		isNeg = 1;
		*num *= -1;
	}
	int size = 0;
	int tmp = *num; // used as a holder for num at first, and then a counter for ret.
	while(tmp > 0){
		tmp /= 8;
		size++;
	}
	tmp = 0;
	char * ret = (char *) malloc(size+1);
	while(num > 0){
		if(*num % 16 == 0)
			ret[tmp] = '0';
		else if(*num % 16 == 1)
			ret[tmp] = '1';
		else if(*num % 16 == 2)
			ret[tmp] = '2';
		else if(*num % 16 == 3)
			ret[tmp] = '3';
		else if(*num % 16 == 4)
			ret[tmp] = '4';
		else if(*num % 16 == 5)
			ret[tmp] = '5';
		else if(*num % 16 == 6)
			ret[tmp] = '6';
		else if(*num % 16 == 7)
			ret[tmp] = '7';
		*num /= 8;
		tmp++;
	}
	ret[size+1] = '\0';
	int start = 0;
	int end = size -1;
	char holder;
	while(start < end){
		holder = ret[start];
		ret[start] = ret[end];
		ret[end] = holder;
		end--;
		start ++;
	}
	if(isNeg == 0)
		printf("o%s\n", ret);
	else
		printf("-o%s\n", ret);
	return 0;
}

int DeciToHex(int * num){
	int isNeg = 0;
	if(*num < 0){
		isNeg = 1;
		*num *= -1;
	}
	int size = 0;
	int tmp = *num; // used as a holder for num at first, and then a counter for ret.
	while(tmp > 0){
		tmp /= 16;
		size++;
	}
	tmp = 0;
	char * ret = (char *) malloc(size+1);
	while(*num > 0){
		if(*num % 16 == 0)
			ret[tmp] = '0';
		else if(*num % 16 == 1)
			ret[tmp] = '1';
		else if(*num % 16 == 2)
			ret[tmp] = '2';
		else if(*num % 16 == 3)
			ret[tmp] = '3';
		else if(*num % 16 == 4)
			ret[tmp] = '4';
		else if(*num % 16 == 5)
			ret[tmp] = '5';
		else if(*num % 16 == 6)
			ret[tmp] = '6';
		else if(*num % 16 == 7)
			ret[tmp] = '7';
		else if(*num % 16 == 8)
			ret[tmp] = '8';
		else if(*num % 16 == 9)
			ret[tmp] = '9';
		else if(*num % 16 == 10)
			ret[tmp] = 'A';
		else if(*num % 16 == 11)
			ret[tmp] = 'B';
		else if(*num % 16 == 12)
			ret[tmp] = 'C';
		else if(*num % 16 == 13)
			ret[tmp] = 'D';
		else if(*num % 16 == 14)
			ret[tmp] = 'E';
		else if(*num % 16 == 15)
			ret[tmp] = 'F';
		*num /= 16;
		tmp++;
	}
	ret[size] = '\0';
	int start = 0;
	int end = size -1;
	char holder;
	while(start < end){
		holder = ret[start];
		ret[start] = ret[end];
		ret[end] = holder;
		end--;
		start ++;
	}
	if(isNeg == 0)
		printf("x%s\n", ret);
	else
		printf("-x%s\n", ret);
	return 0;
}

int main(int argc, char **argv){

	//extract necessary information and preallocate memory.
	if(strlen(argv[1]) > 1){
		printf("ERROR: Multiple operations given! Limit to 1 arithmetical operation!\n");
		return 1;
	}
	char * op = argv[1];
	char state1 = argv[2][0];// State of the input number, checks if these are actually minus signs later
	char state2 = argv[3][0]; 
	int jmp1 = 0; // Jumps, in the case one of the numbers is negative
	int jmp2 = 0; // Also act as isNeg, with 1 being true.
	if(state1 == '-'){
		state1 = argv[2][1];
		jmp1++;
	}
	if(state2 == '-'){
		state2 = argv[3][1];
		jmp2++;
	}
	char* num1 = (char *)malloc(strlen(argv[2]) - jmp1); // subtracting so there is only enough space for the number
	char* num2 = (char *)malloc(strlen(argv[3]) - jmp2);


	//assign num1 and num2 to given args
	int i;
	for(i = 0; i < strlen(argv[2]); i++)
		num1[i] = argv[2][i+1+jmp1];
	for(i = 0; i < strlen(argv[3]); i++)
		num2[i] = argv[3][i+1+jmp2];

	char type = argv[4][0];
	if(strlen(argv[4]) > 1){
		printf("ERROR: Return type is not of format 'b', 'd', 'o' or 'x' !\n");
		return 1;
	}

	int x;
	int y;

	if(state1 == 'b')
		x = BinToDec(num1);
	else if(state1 == 'd')
		x = DeciToDec(num1);
	else if(state1 == 'o')
		x = OctToDec(num1);
	else if(state1 == 'x')
		x = HexToDec(num1);
	else{
		printf("ERROR: Number type of the FIRST number is not of format 'b', 'd', 'o' or 'x' !\n");
		return 1;
	}
	if(state2 == 'b')
		y = BinToDec(num2);
	else if(state2 == 'd')
		y = DeciToDec(num2);
	else if(state2 == 'o')
		y = OctToDec(num2);
	else if(state2 == 'x')
		y = HexToDec(num2);
	else{
		printf("ERROR: Number type of the SECOND number is not of format 'b', 'd', 'o' or 'x' !\n");
		return 1;
	}

	//update numbers to negatives
	if(jmp1 == 1)
		x *= -1;
	if(jmp2 == 1)
		y*= -1;
	int * result = (int*) malloc(sizeof(x) + sizeof(y));

	if(x == -1 || y == -1) // From the error checks in the converters, x and/or y was incorrectly formatted
		return 1;

	//determine the operation requested by the user
	if(strcmp(op, "+") == 0)
		*result = x+y;
	else if(strcmp(op, "-") == 0)
		*result = x-y;
	else if(strcmp(op, "*") == 0)
		*result = x*y;
	else
		printf("ERROR: Operation type is not of format '+', '-' or '*' !\n");

	if(type == 'b'){
		DeciToBin(result);
	}else if(type == 'o'){
		DeciToOct(result);
	}else if(type == 'x'){
		DeciToHex(result);
	}else if(type == 'd'){
		if(*result < 0){
			*result *= -1;
			printf("-d%d\n", *result);
		}else
			printf("d%d\n", *result);
	}else{
		printf("ERROR: Return type is not of format 'b', 'd', 'o' or 'x' !\n");
		return 1;
	}
	return 0;
}

