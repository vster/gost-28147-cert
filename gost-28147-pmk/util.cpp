#include <iostream>
#include <string>
#include "util.h"

using namespace std;

/*************************************************
* XOR arrays together                            *
*************************************************/
void xor_buf(byte data[], const byte mask[], u32bit length)
   {
   while(length >= 8)
      { 
		for ( u32bit j = 0; j < 8; j++ )
			data[j] ^= mask[j];

		data += 8; mask += 8; length -= 8;
      }
   for(u32bit j = 0; j != length; j++)
      data[j] ^= mask[j];
   }

void xor_buf(byte out[], const byte in[], const byte mask[], u32bit length)
   {
   while(length >= 8)
      {

	  for ( u32bit j = 0; j < 8; j++ )
			out[j] = in[j] ^ mask[j];     
	  in += 8; out += 8; mask += 8; length -= 8;
      }
   for(u32bit j = 0; j != length; j++)
      out[j] = in[j] ^ mask[j];
   }

/*************************************************
* Byte Reversal Functions                        *
*************************************************/
u16bit reverse_bytes(u16bit input)
   {
   return rotate_left(input, 8);
   }

u32bit reverse_bytes(u32bit input)
   {
   input = ((input & 0xFF00FF00) >> 8) | ((input & 0x00FF00FF) << 8);
   return rotate_left(input, 16);
   }

u64bit reverse_bytes(u64bit input)
   {
   input = ((input & 0xFF00FF00FF00FF00) >>  8) |
           ((input & 0x00FF00FF00FF00FF) <<  8);
   input = ((input & 0xFFFF0000FFFF0000) >> 16) |
           ((input & 0x0000FFFF0000FFFF) << 16);
   return rotate_left(input, 32);
   }

/*************************************************
* Convert an integer into a string               *
*************************************************/
std::string to_string(u32bit n)
   {
   if(n)
      {
      std::string lenstr;
      while(n > 0)
         {
         lenstr = (char)('0' + n % 10) + lenstr;
         n /= 10;
         }
      return lenstr;
      }
   else
      return "zero";
   }
