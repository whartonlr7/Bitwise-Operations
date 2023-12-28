/*
    bits.h
    Luke Wharton
*/

#include <stdbool.h>

/*
    Replaces each char in the specified null-terminated 
    string with its uppercase equivalent if it's a lowercase
    Latin letter (ASCII 97-122). Any other char remains
    unchanged. If s is NULL, to_upper does nothing.
    
    The trick, however, is that to_upper may only use
    bitwise operations on the chars in s. That is, when
    computing the new char for a particular location,
    you may only use >>, <<, &, |, ~, or ^. No +, no -, and
    no calling the toupper library function.
   
    (You may, however, test for whether a char is a letter by
    using <, <=, >, and >=, or even the library functions
    islower, isupper, and isalpha.)
   
    Hint: take a look at the binary ASCII values for corresponding
    uppercase and lowercase letters. Do they have a difference
    in common? (Spoiler: yes.) Can you exploit that difference in
    to_upper and to_lower? (Spoiler: also yes.)
*/
void to_upper(char *s);

/*
    Replaces each char in the specified null-terminated 
    string with its lowercase equivalent if it's an uppercase
    Latin letter (ASCII 65-90). Any other char remains
    unchanged. If s is NULL, to_upper does nothing.
    
    Same implementation restrictions as for to_upper.
*/
void to_lower(char *s);

/*
    In brief, middle_bits copies a range of bits from x,
    returning the integer represented by those bits.

    PRECONDITIONS:
        0 <= x
        0 <= low_bit_index <= 30
        0 <= high_bit_index <= 30

    For this function, bits are numbered starting at
    0 at the lowest-order bit up to 31 for the highest-
    order bit. So, for example, the 1 bit in the number
    shown below is located at bit index 23 (read the top
    two lines vertically as a single position each column).
   
              3322 2222 2222 1111 1111 1100 0000 0000
              1098 7654 3210 9876 5432 1098 7654 3210
   
      number: 0000 0000 1000 0000 0000 0000 0000 0000
    
    The range of bits specified by low_bit_index and
    high_bit_index is the sequence of bits whose bit numbers
    are between low_bit_index and high_bit_index, inclusive.
 
    For example, suppose:
        x:              0000 0000 0101 1001 1000 1111 1000 1111 (base 2)
        low_bit_index:  15 (base 10)
        high_bit_index: 20 (base 10)
   
    Then middle_bits should result in a return value of 51,
    because bits 20, 19, 18, 17, 16, and 15 are 110011:
    x:              0000 0000 0101 1001 1000 1111 1000 1111 (base 2)
                                 ^ ^^^^ ^
   
    If x, low_bit_index, and high_bit_index do not satisfy the
    preconditions specified above, middle_bits should return 0.

    Note: This may seem like a weird function. But it can be very
    handy when you're implementing to_utf8, described below.
*/
int middle_bits(int x, int low_bit_index, int high_bit_index);

/*
    Given a 32-bit integer codepoint between 0 and 0x10FFFF
    and a pointer to a char buffer containing enough
    space to contain at least 5 chars, to_utf8 fills
    the buffer with the bytes in the UTF-8 encoding of the
    codepoint followed by a null byte.
   
    For example, if the codepoint is 0xE9 (that is,
    the Unicode codepoint for é), the first three
    chars in utf8_buffer should be filled by:
   
         0xC3 0xA9 0x00
   
    If utf8_buffer is NULL, codepoint < 0, or
    codepoint > 0x10FFFF, to_utf8 returns false. Otherwise,
    it returns true.
   
    Suggestions:
    - Consult the chart at https://en.wikipedia.org/wiki/UTF-8#Encoding
    - Consider using your middle_bits function to help you
    - You might find this helpful: https://sandbox.jeffondich.com/encoder/
*/
bool to_utf8(int codepoint, char *utf8_buffer);

/*
    Given a char buffer containing the (null-terminated)
    bytes in the UTF-8 encoding of a codepoint between
    0 and 0x10FFFF, from_utf8 returns the codepoint.
   
    If utf8_buffer is NULL or if the bytes contained
    there do not correspond to a valid UTF-8 encoding,
    from_utf8 returns -1.
   
    For example, if utf8_buffer contains:
   
         0xC3 0xA9 0x00
   
    from_utf8 returns 0xE9 (also known as 233).
*/
int from_utf8(char *utf8_buffer);