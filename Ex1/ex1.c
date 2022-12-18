#include "ex1.h"

//Created by: Shlomo Shatz, 316093202

//A function to test if this computer's CPU architecture Big Endian or Little Endian
int is_big_endian() {
    //Declares an unsigned int that equals 1, and in binary will be 00000000 00000000 00000000 00000001(for 4 bytes)
    unsigned int testInteger = 1;
    //Casts a char pointer to point at the integer. Since char only uses 1 byte, it will point to the first byte
    char *testChar = (char*)&testInteger;
    //If the char pointer points to 1, the first byte is 1 and the architecture is Little Endian
    if (*testChar) return 0;
    //If the char pointer points to 0, the first byte is 0 and the architecture is Big Endian
    else return 1;
}

//A function to convert an array of bools to the integer number it represents in sign magnitude
int get_sign_magnitude(bool bitArray[8]) {
    //Declares two variables to be used later
    int temp = 1, sum = 0, i;
    //A loop that calculates the binary number(without the MSB)
    for (i = 0; i < 7; i++) {
        sum += temp * bitArray[i];
        temp *= 2;
    }
    //Checks the MSB, if it is 1 - the number is negative, otherwise - non-negative
    if (bitArray[7]) return (-1 * sum);
    return sum;
}

//A function to convert an array of bools to the integer number it represents in two's complement
int get_two_comp(bool bitArray[8]) {
    //Declares two variables to be used later
    int temp = 1, sum = 0;
    //If the MSB is 0, the number is non-negative, and the number is calculated same as the sign magnitude
    if(!bitArray[7]) {
        return get_sign_magnitude(bitArray);
    } else {
        //If the MSB is 1, the number is negative, and it is calculated as such: first, reverse the bits
        int i;
        for (i = 0; i < 8; i++) {
            bitArray[i] = !bitArray[i];
        }
        //Second, calculate the binary number as it is
        sum = get_sign_magnitude(bitArray);
        //Add 1
        sum += 1;
        //Return the negative form
        return -1 * sum;
    }
}
