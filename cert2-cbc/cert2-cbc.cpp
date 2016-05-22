// Реализация алгоритма криптографического преобразования ГОСТ 28147-89
// Режим простой замены с зацеплением
// Файл gost-cbc.cpp

#include <iostream>
#include <iomanip>
#include <opencl/gost.h>

using namespace std;
using namespace OpenCL;

const u32bit KL = 32; 
const u32bit BS = 8;
const u32bit CT2 = 2;
const u32bit CT32 = 32;

void main(void)
{
	GOST g1;
	void cbc_run2test(GOST, byte [CT2][BS], byte [BS], byte [KL]);
	void cbc_run32test(GOST, byte [CT32][BS], byte [BS], byte [KL]);

	// Set test key 1 - KZU-1
	byte kzu1[KL] =	{	0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff };

	// Set test key 2 - KZU-2
	byte kzu2[KL] = {	0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00 };
				
	// Set test key 3 - KZU-3	
	byte kzu3[KL] = {	0x55, 0x55, 0x55, 0x55,
						0xaa, 0xaa, 0xaa, 0xaa,
						0x55, 0x55, 0x55, 0x55,
						0xaa, 0xaa, 0xaa, 0xaa,
						0x55, 0x55, 0x55, 0x55,
						0xaa, 0xaa, 0xaa, 0xaa,
						0x55, 0x55, 0x55, 0x55,
						0xaa, 0xaa, 0xaa, 0xaa };

	// Set test key 4 - KZU-4	
	byte kzu4[KL] = {	0xaa, 0xaa, 0xaa, 0xaa,
						0x55, 0x55, 0x55, 0x55,
						0xaa, 0xaa, 0xaa, 0xaa,
						0x55, 0x55, 0x55, 0x55,
						0xaa, 0xaa, 0xaa, 0xaa,
						0x55, 0x55, 0x55, 0x55,
						0xaa, 0xaa, 0xaa, 0xaa, 
						0x55, 0x55, 0x55, 0x55 };

	// Set test key 5 - KZU-5
	// Attention! In reverse order! Low-endian
	byte kzu5[KL] = {	0x04, 0x75, 0xf6, 0xe0,
						0x50, 0x38, 0xfb, 0xfa,
						0xd2, 0xc7, 0xc3, 0x90,
						0xed, 0xb3, 0xca, 0x3d,
						0x15, 0x47, 0x12, 0x42,
						0x91, 0xae, 0x1e, 0x8a,
						0x2f, 0x79, 0xcd, 0x9e,
						0xd2, 0xbc, 0xef, 0xbd };
	
	// Prepare S-Box
	g1.generate_sbox();

	// Set open data To-1
	// Attention! In reverse order! Low-endian
	byte to1[CT2][BS] = {{0xcc, 0xcc, 0xcc, 0xcc,
						0x33, 0x33, 0x33, 0x33},
						{0x33, 0x33, 0x33, 0x33,
						0xcc, 0xcc, 0xcc, 0xcc} };				
	
	// Set open data To-2
	// Attention! In reverse order! Low-endian
	byte to2[CT32][BS];
	for (u32bit j = 0; j < CT32; j++)
		for (u32bit i = 0; i < BS; i++)
			to2[j][i] = j*8 + (7-i);
	
	// Init vector
	// Attention! In reverse order! Low-endian
	byte s1[BS] = {		0x2a, 0x80, 0xa7, 0xc3,
						0xff, 0xa8, 0xe3, 0x47 };
	byte s2[BS] = {		0x73, 0xcf, 0x64, 0xe6,
						0x7d, 0x4b, 0x42, 0xe6 };	

	cout << "GOST 28147-89. CBC mode." << endl;

	// Run test with KZU1
	cout << "\nTEST 1\n"; 
	cbc_run2test(g1, to1, s1, kzu1);
	// Must be
	// Ts1  1a 07 eb 50 6f 2f 01 65 
	// Ts2  61 02 51 e9 a7 a2 f3 18 

	// Run test with KZU2
	cout << "\nTEST 2\n"; 
	cbc_run2test(g1, to1, s1, kzu2);
	// Must be
	// Ts1  3c 3e 65 e4 23 9a cf 2f 
	// Ts2  f5 d8 d1 f2 14 69 ea d6

	// Run test with KZU3
	cout << "\nTEST 3\n"; 
	cbc_run2test(g1, to1, s1, kzu3);
	// Must be
	// Ts1  ac 64 ad 6d 46 b2 ac 0b 
	// Ts2  c4 ca 34 4c 11 a8 95 6d 

	// Run test with KZU4
	cout << "\nTEST 4\n"; 
	cbc_run2test(g1, to1, s1, kzu4);
	// Must be
	// Ts1  2b 49 58 7a 4f 15 9c 13 
	// Ts2  ca da 76 9a a9 89 cd 26 

	// Run test with KZU5
	cout << "\nTEST 5\n"; 
	cbc_run2test(g1, to1, s1, kzu5);
	// Must be
	// Ts1  a6 c4 65 2a 0c 87 90 e8 
	// Ts2  dd 52 0e 73 c1 65 4b 14

	// Run test with To-2 and KZU5
	cout << "\nTEST 6\n"; 
	cbc_run32test(g1, to2, s2, kzu5);

}

void cbc_run2test(GOST gg, byte to[CT2][BS], byte iv[BS], byte key[KL])
{
	byte ts[CT2][BS];
	byte td[CT2][BS];
	byte tt[BS];
	u32bit i, j;

	cout << "Test key:" << endl;
	for (i = 0; i < 8; i++)
	{
		cout << "X" << i << "  " 
			<< hex << setfill('0') << setw(2)
			<< (int)key[4*i+3] << "  "
			<< hex << setfill('0') << setw(2)
			<< (int)key[4*i+2] << "  "
			<< hex << setfill('0') << setw(2)
			<< (int)key[4*i+1] << "  "
			<< hex << setfill('0') << setw(2)
			<< (int)key[4*i]
			<< endl;
	}	
	cout << endl;
		
	gg.set_key(key, KL);

	// Output init vector
	cout << "Init vector:" << endl;
	cout << "S1   ";
	for (i = 8; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)iv[i-1] << " ";
	cout << endl << endl;

	// Output open data
	cout << "Open data:" << endl;
	for ( j = 0; j < CT2; j++)
	{
		cout << "To" << j+1 << "  ";
		for (i = 8; i > 0; i--)
			cout << hex << setfill('0') << setw(2) 
				 << (int)to[j][i-1] << " ";
		cout << endl;
	}
	cout << endl;
	
	// Encrypt data in CBC mode
	xor_buf(tt, to[0], iv, BS);
	gg.encrypt(tt, ts[0]);
	xor_buf(tt, to[1], ts[0], BS);
	gg.encrypt(tt, ts[1]);

	// Output encrypted data
	cout << "Encrypted data:" << endl;
	for ( j = 0; j < CT2; j++)
	{
		cout << "Ts" << j+1 << "  ";
		for (i = 8; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)ts[j][i-1] << " ";
		cout << endl;
	}
	cout << endl;

	// Decrypt data in CBC mode
	gg.decrypt(ts[1], tt);
	xor_buf(td[1], tt, ts[0], BS);
	gg.decrypt(ts[0], tt);
	xor_buf(td[0], tt, iv, BS);

	// Output decrypted data
	cout << "Decrypted data:" << endl;
	for ( j = 0; j < CT2; j++)	
	{
		cout << "Td" << j+1 << "  ";
		for (i = 8; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)td[j][i-1] << " ";
		cout << endl;
	}
	cout << endl;
}

void cbc_run32test(GOST gg, byte to[CT32][BS], byte iv[BS], byte key[KL])
{
	byte ts[CT32][BS];
	byte tt[BS];
	u32bit i, j;

	cout << "Test key:" << endl;
	for (i = 0; i < 8; i++)
	{
		cout << "X" << i << "  " 
			<< hex << setfill('0') << setw(2)
			<< (int)key[4*i+3] << "  "
			<< hex << setfill('0') << setw(2)
			<< (int)key[4*i+2] << "  "
			<< hex << setfill('0') << setw(2)
			<< (int)key[4*i+1] << "  "
			<< hex << setfill('0') << setw(2)
			<< (int)key[4*i]
			<< endl;
	}	
	cout << endl;
		
	gg.set_key(key, KL);

	// Output init vector
	cout << "Init vector:" << endl;
	cout << "S1   ";
	for (i = 8; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)iv[i-1] << " ";
	cout << endl << endl;

	// Output open data
	cout << "Open data:" << endl;
	for ( j = 0; j < CT32; j++)
	{
		cout << "To" << dec << setfill('0') << setw(2) << j+1 << "  ";
		for (i = 8; i > 0; i--)
			cout << hex << setfill('0') << setw(2) 
				 << (int)to[j][i-1] << " ";
		cout << endl;
	}
	cout << endl;
	
	// Encrypt data in CBC mode
	xor_buf(tt, to[0], iv, BS);
	gg.encrypt(tt, ts[0]);
	for (j = 1; j < CT32; j++)
	{
		xor_buf(tt, to[j], ts[j-1], BS);
		gg.encrypt(tt, ts[j]);
	}

	// Output encrypted data
	cout << "Encrypted data:" << endl;
	for ( j = 0; j < CT32; j++)
	{
		cout << "Ts" << dec << setfill('0') << setw(2) << j+1 << "  ";
		for (i = 8; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)ts[j][i-1] << " ";
		cout << endl;
	}
	cout << endl;
}