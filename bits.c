/*
    bits.c
    Assignment 3
    Luke Wharton

    This file is an assignment attempting to gain the basic ideas behind bitwise operators. 
    In doing so, I implemented methods to change letters from lowercase to uppercase and vice versa
    In addition, this file includes encoding and decoding UTF8 to/from their corresponding codepoints
    and the ability to get bits by index in a 8 byte sequence.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "bits.h"

/*
    Replaces each char in the specified null-terminated 
    string with its uppercase equivalent if it's a lowercase
    Latin letter (ASCII 97-122). Any other char remains
    unchanged. If s is NULL, to_upper does nothing.
*/
void to_upper(char *s)
{
    if (s){

        char mask = 0xdf;
        for (int i = 0; i < strlen(s); i++){

            if (islower(s[i])){

                // Use mask to change bit values to corresponding uppercase letter
                s[i] = s[i] & mask;
            }
        }
    }
    return;
}

/*
    Same as to_upper but replacces uppercase characters with 
    lowercase equivalents
*/
void to_lower(char *s) {

    if (s){

        char mask = 0x20;
        for (int i = 0; i < strlen(s); i++){

            if (isupper(s[i])){

                // Use mask to change bit values to corresponding lowercase letter
                s[i] = s[i] ^ mask;
            }
        }
    }
    return;
}

/*
    middle_bits copies a range of bits from x,
    returning the integer represented by those bits. The 
    range is specified by low_bit_index and high_bit_index.
    If x is null or the bit index overlap, returns 0
*/
int middle_bits(int x, int low_bit_index, int high_bit_index)
{
    // Error checking
    if (x && high_bit_index >= low_bit_index && high_bit_index < 32 && low_bit_index >= 0) {

        unsigned result;
        //Shift bits left than right, getting value needed padded with 0s
        result = x << (31-high_bit_index);
        result = result >> (31-high_bit_index+low_bit_index);
        return result;
    }
    return 0;
}

/*
    Given a 32-bit integer codepoint between 0 and 0x10FFFF
    and a pointer to a char buffer containing enough
    space to contain at least 5 chars, to_utf8 fills
    the buffer with the bytes in the UTF-8 encoding of the
    codepoint followed by a null byte.

    If utf8_buffer is NULL, codepoint < 0, or
    codepoint > 0x10FFFF, to_utf8 returns false. Otherwise,
    it returns true.
*/
bool to_utf8(int codepoint, char *utf8_buffer)
{
    if (!utf8_buffer || codepoint < 0){

        return false;
    }

    // For utf8 encoding in 1 byte
    if (codepoint <= 0x7f){

        //Stores in char array
        utf8_buffer[0] = codepoint;
        utf8_buffer[1] = '\0';

        return true;
    }

    // For utf8 encoding in 2 bytes
    else if (codepoint <= 0x7ff){

        int splitOne = middle_bits(codepoint, 0, 5);
        int splitTwo = middle_bits(codepoint, 6, 10);

        // Encodes the 2 different bytes
        splitOne = splitOne | 0b10000000;
        splitTwo = splitTwo | 0b11000000;

        // Stores accordingly
        utf8_buffer[0] = splitTwo | utf8_buffer[0];
        utf8_buffer[1] = splitOne | utf8_buffer[1];
        utf8_buffer[2] = '\0';

        return true;
    }

    // For utf8 encoding in 3 bytes
    else if (codepoint <= 0xffff){

        int splitOne = middle_bits(codepoint, 0, 5);
        int splitTwo = middle_bits(codepoint, 6, 11);
        int splitThree = middle_bits(codepoint, 12, 15);

        // Encodes the 3 different bytes
        splitOne = splitOne | 0b10000000;
        splitTwo = splitTwo | 0b10000000;
        splitThree = splitThree | 0b11100000;

        // Stores accordingly
        utf8_buffer[0] = splitThree;
        utf8_buffer[1] = splitTwo;
        utf8_buffer[2] = splitOne;
        utf8_buffer[3] = '\0';

        return true;
    } 

    // For utf8 encoding in 4 bytes
    else if (codepoint <= 0x10ffff){

        int splitOne = middle_bits(codepoint, 0, 5);
        int splitTwo = middle_bits(codepoint, 6, 11);
        int splitThree = middle_bits(codepoint, 12, 17);
        int splitFour = middle_bits(codepoint, 18, 20);

        // Encodes the 4 different bytes
        splitOne = splitOne | 0b10000000;
        splitTwo = splitTwo | 0b10000000;
        splitThree = splitThree | 0b10000000;
        splitFour = splitFour | 0b11110000;

        // Stores accordingly
        utf8_buffer[0] = splitFour;
        utf8_buffer[1] = splitThree;
        utf8_buffer[2] = splitTwo;
        utf8_buffer[3] = splitOne;
        utf8_buffer[4] = '\0';

        return true;
    }

    return false;
}


/*
    Given a char buffer containing the (null-terminated)
    bytes in the UTF-8 encoding of a codepoint between
    0 and 0x10FFFF, from_utf8 returns the codepoint.
   
    If utf8_buffer is NULL or if the bytes contained
    there do not correspond to a valid UTF-8 encoding,
    from_utf8 returns -1.
*/
int from_utf8(char *utf8_buffer) {

    // Error checking
    if(!utf8_buffer){

        return -1;
    }

    int length = strlen(utf8_buffer);
    
    // Case One: length is 0 (getting unicode value 0, return 0)
    if (length == 0){

        return 0;
    }
    // Case Two: 1 byte
    if (length == 1 ){

        //Checks to make sure valid utf8
        if ((unsigned char) utf8_buffer[0] > 127){

            return -1;
        }

        int value = middle_bits(utf8_buffer[0], 0, 6);
        
        return value;

    // Case Two: 2 bytes
    } else if (length == 2){

        //Checks to make sure valid utf8
        if (   (unsigned char) utf8_buffer[0] < 192 || (unsigned char) utf8_buffer[0] > 223
            || (unsigned char) utf8_buffer[1] < 128 || (unsigned char) utf8_buffer[1] > 191) {

            return -1;
        }

        // Splits and combines correct parts of codepoint
        int value;
        int byteOne = middle_bits(utf8_buffer[0], 0, 4);
        int byteTwo = middle_bits(utf8_buffer[1], 0, 5);

        value = byteOne << 6;
        value = value | byteTwo;

        return value;

    // Case Three: 3 bytes
    } else if (length == 3){

        //Checks to make sure valid utf8
         if (  (unsigned char) utf8_buffer[0] < 223 || (unsigned char) utf8_buffer[0] > 239    
            || (unsigned char) utf8_buffer[1] < 128 || (unsigned char) utf8_buffer[1] > 191
            || (unsigned char) utf8_buffer[2] < 128 || (unsigned char) utf8_buffer[2] > 191) {

            return -1;
        }

        // Splits and combines correct parts of codepoint
        int value;
        int byteOne = middle_bits(utf8_buffer[0], 0, 3);
        int byteTwo = middle_bits(utf8_buffer[1], 0, 5);
        int byteThree = middle_bits(utf8_buffer[2], 0, 5);

        value = byteOne << 12;
        byteTwo = byteTwo << 6;
        value = value | byteTwo;
        value = value | byteThree;
        printf("BEFORE__%x__AFTER__%x\n", (unsigned int) byteThree, (unsigned int) value);

        return value;

    // Case Four: 4 bytes
    } else if (length == 4){

        //Checks to make sure valid utf8
        if (   (unsigned char) utf8_buffer[0] < 240 || (unsigned char) utf8_buffer[0] > 247
            || (unsigned char) utf8_buffer[1] < 128 || (unsigned char) utf8_buffer[1] > 191    
            || (unsigned char) utf8_buffer[2] < 128 || (unsigned char) utf8_buffer[2] > 191
            || (unsigned char) utf8_buffer[3] < 128 || (unsigned char) utf8_buffer[3] > 191) {

            return -1;
        }

        // Splits and combines correct parts of codepoint
        int value;
        int byteOne = middle_bits(utf8_buffer[0], 0, 2);
        int byteTwo = middle_bits(utf8_buffer[1], 0, 5);
        int byteThree = middle_bits(utf8_buffer[2], 0, 5);
        int byteFour = middle_bits(utf8_buffer[3], 0, 5);

        value = byteOne << 18;
        byteTwo = byteTwo << 12;
        byteThree = byteThree << 6;
        value = value | byteTwo;
        value = value | byteThree;
        value = value | byteFour;

        return value;
    }
    return -1;
}