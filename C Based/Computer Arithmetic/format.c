/*
 * format.c
 *
 *  Created on: Mar 6, 2016
 *      Author: pawel
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int BinToD(char * bin){
	int ret = 0; // return value
	int cur = 1; // current value in binary
	int i;
	for(i = strlen(bin) -1; i >= 0; i--){
		if(bin[i] == '1')
			ret += cur;
		else if(bin[i] > '1'){
			printf("ERROR: The number given is in an invalid BINARY format!\n");
			return -1;
		}
		cur *= 2;
	}

	return ret;
}
char * ReverseTwo(char * two){
	char * cpy = (char *)malloc(strlen(two));
	int i;
	int carry = 0;

	for(i = 0; i < strlen(two); i++){
		if(two[i] == '0')
			cpy[i] = '1';
		else if(two[i] == '1')
			cpy[i] = '0';
		else{
			printf("ERROR: The number given is in an invalid BINARY format!\n");
			return "2";
		}
	}

	do{
		i--;
		if(cpy[i] == '0'){
			cpy[i] = '1';
			carry = 0;
		}else{
			cpy[i] = '0';
			carry = 1;
		}

	}while(carry != 0);

	cpy[strlen(two)] = '\0';
	return cpy;
}
double MantToDec(char * mant){
	double ret = 1; // return value
	double cur = 0.5; // current value in binary
	int i;
	for(i = 0; i < strlen(mant); i++){
		if(mant[i] == '1')
			ret += cur;
		else if(mant[i] > '1'){
			printf("ERROR: The mantissa given is in an invalid BINARY format!\n");
			return -1;
		}
		cur /= 2;
	}

	return ret;
}
int getExpo(double res){// find the exponent so that the result is represented in scientific format
	int result = 0;
	int isNeg = 0;
	int small = 0;
	double tmp = res;
	if(tmp < 0){
		tmp *= -1;
		isNeg = 1;
	}
	if(tmp < 1 && res > 0)
		small = 1;
	if(small == 1){
		while(tmp < 1 && tmp > 0){
			tmp *= 10;
			result--;
		}
	}else{
		while(tmp >= 10){
			tmp /= 10;
			result++;
		}
	}
	return result;
}
int main(int argc, char **argv){

	int result;
	if(strcmp(argv[2], "int") == 0){
		if(argv[1][0] == '1'){
			result = BinToD(ReverseTwo(argv[1]));
			if(result == -1)// -1 received from BinToD, indicating an error
				return 1;
			result *= -1;
			printf("-%d", result); // if -- pops out, max integer limit was reached
		}else{
			result = BinToD(argv[1]);
			printf("%d", result);
		}
	}else if(strcmp(argv[2], "float") == 0){
		int isNeg = 0;

		int length = strlen(argv[1]); // This variable, along with the next 3, is a length. In this case, the # of bits.
		int expo; // Will turn into 8, 11, or 15 depending on length
		int mant; // 23, 52, or 112
		int bias = 0; // 127, 1023, or 16383
		int expoA; // Holds the actual exponent
		if(length == 32){
			expo = 8;
			mant = 23;
			bias = 127;
		}else if(length == 64){
			expo = 11;
			mant = 52;
			bias = 1023;
		}else if(length == 128){
			expo = 15;
			mant = 112;
			bias = 16383;
		}else{
			printf("ERROR: Input sequence is not in 32, 64, or 128 bit format! Try again, or select output as int.");
			return 1;
		}
		if(argv[1][0] == '1')
			isNeg = 1;

		char * tmp = (char *) malloc(expo + 1);
		tmp = strncpy(tmp, argv[1]+1, expo);
		tmp[expo] = '\0';
		expoA = BinToD(tmp); // Turns the exponent into a decimal number from binary
		expoA -= bias; // Turns the exponent into the correct exponent

		tmp = (char *)realloc(tmp,mant + 1);
		tmp = strncpy(tmp, argv[1]+1+expo, mant);
		tmp[mant] = '\0';
		double manti = 0;
		manti = MantToDec(tmp);
		if(manti == -1)
			return -1;

		if(expoA == bias * -1){ // Compensation for de-normalization
			expoA += 1;
			manti -= 1;
		}else if(expoA == 128){ // Checks if exponent is infinity, and if the mantissa is a number other than zero
			if(manti != 0.0 && manti != 0)
				printf("NaN");
			else{
				if(isNeg == 1)
					printf("ninf");
				else
					printf("pinf");
			}
			return 0;
		}

		double res = 1;
		if(expoA > 0){
			for(length = 1; length <= expoA; length++ ){
				res *= 2;
			}
		}else{
			for(length = -1; length >= expoA; length-- ){
				res /= 2;
			}
		} // At this point, result contains 2 ^ expo.

		res *= manti; // Now result is the result, ready to be printed depending on isNeg.
		expoA = getExpo(res);
		if(expoA < 0)
			for(expo = expoA; expo != 0; expo++)
				res *= 10;
		else
			for(expo = expoA; expo != 0; expo--)
				res /= 10;
		if(isNeg == 1)
			printf("-%f e %d", res, expoA);
		else
			printf("%f e %d", res, expoA);

	}else{
		printf("ERROR: Something that wasn't 'int' or 'float' was typed in! Program IS case-sensitive!");
		return 1;
	}

	return 0;
}

