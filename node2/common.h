/*------------------------------------------------------
* Common.h file is used from other source files
* bit manipulation
------------------------------------------------------*/
#ifndef COMMON_H
#define COMMON_H 

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))
#define wait_until_bit_is_set( reg, bit ) while( !test_bit( reg, bit ) )
#define wait_until_bit_is_clear( reg, bit) while( test_bit( reg, bit ) )

#endif