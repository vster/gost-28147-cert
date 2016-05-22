/*************************************************
* GOST Source File                               *
* (C) 1999-2001 The OpenCL Project               *
*************************************************/

#include <iostream>
#include <iomanip>
#include "gost.h"

using namespace OpenCL;
using namespace std;

/*************************************************
* GOST Encryption                                *
*************************************************/
void GOST::encrypt(const byte in[BLOCKSIZE], byte out[BLOCKSIZE]) const
   {
   u32bit N1 = make_u32bit(in[3], in[2], in[1], in[0]),
          N2 = make_u32bit(in[7], in[6], in[5], in[4]);
   round(N2, N1 + EK[ 0]); round(N1, N2 + EK[ 1]);
   round(N2, N1 + EK[ 2]); round(N1, N2 + EK[ 3]);
   round(N2, N1 + EK[ 4]); round(N1, N2 + EK[ 5]);
   round(N2, N1 + EK[ 6]); round(N1, N2 + EK[ 7]);
   round(N2, N1 + EK[ 0]); round(N1, N2 + EK[ 1]);
   round(N2, N1 + EK[ 2]); round(N1, N2 + EK[ 3]);
   round(N2, N1 + EK[ 4]); round(N1, N2 + EK[ 5]);
   round(N2, N1 + EK[ 6]); round(N1, N2 + EK[ 7]);
   round(N2, N1 + EK[ 0]); round(N1, N2 + EK[ 1]);
   round(N2, N1 + EK[ 2]); round(N1, N2 + EK[ 3]);
   round(N2, N1 + EK[ 4]); round(N1, N2 + EK[ 5]);
   round(N2, N1 + EK[ 6]); round(N1, N2 + EK[ 7]);
   round(N2, N1 + EK[ 7]); round(N1, N2 + EK[ 6]);
   round(N2, N1 + EK[ 5]); round(N1, N2 + EK[ 4]);
   round(N2, N1 + EK[ 3]); round(N1, N2 + EK[ 2]);
   round(N2, N1 + EK[ 1]); round(N1, N2 + EK[ 0]);
   out[0] = get_byte(3, N2); out[1] = get_byte(2, N2);
   out[2] = get_byte(1, N2); out[3] = get_byte(0, N2);
   out[4] = get_byte(3, N1); out[5] = get_byte(2, N1);
   out[6] = get_byte(1, N1); out[7] = get_byte(0, N1);
   }

/*************************************************
* GOST Decryption                                *
*************************************************/
void GOST::decrypt(const byte in[BLOCKSIZE], byte out[BLOCKSIZE]) const
   {
   u32bit N1 = make_u32bit(in[3], in[2], in[1], in[0]),
          N2 = make_u32bit(in[7], in[6], in[5], in[4]);
   round(N2, N1 + EK[ 0]); round(N1, N2 + EK[ 1]);
   round(N2, N1 + EK[ 2]); round(N1, N2 + EK[ 3]);
   round(N2, N1 + EK[ 4]); round(N1, N2 + EK[ 5]);
   round(N2, N1 + EK[ 6]); round(N1, N2 + EK[ 7]);
   round(N2, N1 + EK[ 7]); round(N1, N2 + EK[ 6]);
   round(N2, N1 + EK[ 5]); round(N1, N2 + EK[ 4]);
   round(N2, N1 + EK[ 3]); round(N1, N2 + EK[ 2]);
   round(N2, N1 + EK[ 1]); round(N1, N2 + EK[ 0]);
   round(N2, N1 + EK[ 7]); round(N1, N2 + EK[ 6]);
   round(N2, N1 + EK[ 5]); round(N1, N2 + EK[ 4]);
   round(N2, N1 + EK[ 3]); round(N1, N2 + EK[ 2]);
   round(N2, N1 + EK[ 1]); round(N1, N2 + EK[ 0]);
   round(N2, N1 + EK[ 7]); round(N1, N2 + EK[ 6]);
   round(N2, N1 + EK[ 5]); round(N1, N2 + EK[ 4]);
   round(N2, N1 + EK[ 3]); round(N1, N2 + EK[ 2]);
   round(N2, N1 + EK[ 1]); round(N1, N2 + EK[ 0]);
   out[0] = get_byte(3, N2); out[1] = get_byte(2, N2);
   out[2] = get_byte(1, N2); out[3] = get_byte(0, N2);
   out[4] = get_byte(3, N1); out[5] = get_byte(2, N1);
   out[6] = get_byte(1, N1); out[7] = get_byte(0, N1);
   }

/*************************************************
* GOST Encryption for Imito-Pristavka 16 rounds                                *
*************************************************/
void GOST::encrypt16(const byte in[BLOCKSIZE], byte out[BLOCKSIZE]) const
   {
   u32bit N1 = make_u32bit(in[3], in[2], in[1], in[0]),
          N2 = make_u32bit(in[7], in[6], in[5], in[4]);
   round(N2, N1 + EK[ 0]); round(N1, N2 + EK[ 1]);
   round(N2, N1 + EK[ 2]); round(N1, N2 + EK[ 3]);
   round(N2, N1 + EK[ 4]); round(N1, N2 + EK[ 5]);
   round(N2, N1 + EK[ 6]); round(N1, N2 + EK[ 7]);
   round(N2, N1 + EK[ 0]); round(N1, N2 + EK[ 1]);
   round(N2, N1 + EK[ 2]); round(N1, N2 + EK[ 3]);
   round(N2, N1 + EK[ 4]); round(N1, N2 + EK[ 5]);
   round(N2, N1 + EK[ 6]); round(N1, N2 + EK[ 7]);
   out[0] = get_byte(3, N1); out[1] = get_byte(2, N1);
   out[2] = get_byte(1, N1); out[3] = get_byte(0, N1);
   out[4] = get_byte(3, N2); out[5] = get_byte(2, N2);
   out[6] = get_byte(1, N2); out[7] = get_byte(0, N2);
   }

/*************************************************
* GOST Decryption for Imito-Pristavka 16 rounds                               *
*************************************************/
void GOST::decrypt16(const byte in[BLOCKSIZE], byte out[BLOCKSIZE]) const
   {
   u32bit N1 = make_u32bit(in[3], in[2], in[1], in[0]),
          N2 = make_u32bit(in[7], in[6], in[5], in[4]);
   round(N2, N1 + EK[ 7]); round(N1, N2 + EK[ 6]);
   round(N2, N1 + EK[ 5]); round(N1, N2 + EK[ 4]);
   round(N2, N1 + EK[ 3]); round(N1, N2 + EK[ 2]);
   round(N2, N1 + EK[ 1]); round(N1, N2 + EK[ 0]);
   round(N2, N1 + EK[ 7]); round(N1, N2 + EK[ 6]);
   round(N2, N1 + EK[ 5]); round(N1, N2 + EK[ 4]);
   round(N2, N1 + EK[ 3]); round(N1, N2 + EK[ 2]);
   round(N2, N1 + EK[ 1]); round(N1, N2 + EK[ 0]);
   out[0] = get_byte(3, N2); out[1] = get_byte(2, N2);
   out[2] = get_byte(1, N2); out[3] = get_byte(0, N2);
   out[4] = get_byte(3, N1); out[5] = get_byte(2, N1);
   out[6] = get_byte(1, N1); out[7] = get_byte(0, N1);
   }




/*************************************************
* GOST Round                                     *
*************************************************/
void GOST::round(u32bit& left, u32bit right)
   {
   right = SBOX1[get_byte(0, right)] | SBOX2[get_byte(1, right)] |
           SBOX3[get_byte(2, right)] | SBOX4[get_byte(3, right)];
   left ^= rotate_left(right, 11);
   }

/*************************************************
* GOST Key Schedule                              *
*************************************************/
void GOST::set_key(const byte key[], u32bit length) throw(InvalidKeyLength)
   {
   if(!valid_keylength(length))
      throw InvalidKeyLength(name(), length);
   for(u32bit j = 0; j != 8; j++)
      EK[j] = make_u32bit(key[4*j+3], key[4*j+2], key[4*j+1], key[4*j]);
   }

/*************************************************
* GOST S-Box Generation                              *
*************************************************/
void GOST::generate_sbox( int oid )
	{
		int i, j;
		byte b0 = 0x00;
		for (i = 0; i < 16; i++)
			for (j = 0; j < 16; j++)
			{
				SBOX1[i*16+j] = make_u32bit((K[oid][7][i]*16+K[oid][6][j]),b0,b0,b0);
				SBOX2[i*16+j] = make_u32bit(b0,(K[oid][5][i]*16+K[oid][4][j]),b0,b0);
				SBOX3[i*16+j] = make_u32bit(b0,b0,(K[oid][3][i]*16+K[oid][2][j]),b0);
				SBOX4[i*16+j] = make_u32bit(b0,b0,b0,(K[oid][1][i]*16+K[oid][0][j]));
			}
	}

/*************************************************
* GOST Output OID                              *
*************************************************/
void GOST::output_oid(int oid)
 {
	int j;
	char chtab[17] = "0123456789ABCDEF";

	cout << GOST::OID[oid] << endl << endl;
	
	cout << "   K8 K7 K6 K5 K4 K3 K2 K1\n";
	for (j = 0; j < 16; j++)
	{
		cout	<< setfill('0') << setw(2) 
					<< j << "  ";
		for (int i = 7; i >= 0; i--)
			cout << chtab[ K[oid][i][j] ]  << "  ";
		cout << endl;
	}
 }

