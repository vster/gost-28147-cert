// Реализация алгоритма криптографического преобразования ГОСТ 28147-89
// Режим гаммирования
// Файл gost-gam.cpp

#include <iostream>
#include <iomanip>
#include <opencl/gost.h>

using namespace std;
using namespace OpenCL;

const u32bit KL = 32; 
const u32bit BS = 8;
const u32bit CT2 = 2;
const u32bit CT32 = 32;
const u32bit C1 = 0x01010104;
const u32bit C2 = 0x01010101;
const u32bit FF = 0xffffffff;

int main(void)
{
	GOST g1;
	void gam_run2test(GOST, byte [CT2][BS], byte [BS], byte [KL]);
	void gam_run32test(GOST, byte [CT32][BS], byte [BS], byte [KL]);

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
	
	cout << "GOST 28147-89. Gamming mode." << endl;
	
	// Run test with KZU1
	cout << "\nTEST 1\n"; 
	gam_run2test(g1, to1, s1, kzu1);
	// Must be
	// Ts1  e3 60 72 e8 1b 59 2b e7 
	// Ts2  ca 92 d0 5b 6f 03 b8 69 

	// Run test with KZU2
	cout << "\nTEST 2\n"; 
	gam_run2test(g1, to1, s1, kzu2);
	// Must be
	// Ts1  3e 32 a7 64 17 a1 82 f5 
	// Ts2  8e 33 c9 7d a5 3b 44 55 

	// Run test with KZU3
	cout << "\nTEST 3\n"; 
	gam_run2test(g1, to1, s1, kzu3);
	// Must be
	// Ts1  5c b6 b2 12 26 d5 9c f4 
	// Ts2  34 fa 55 9b ea 4b 3b e1 

	// Run test with KZU4
	cout << "\nTEST 4\n"; 
	gam_run2test(g1, to1, s1, kzu4);
	// Must be
	// Ts1  23 18 40 f6 96 04 6d 08 
	// Ts2  be c5 a6 41 55 ec 47 32 

	// Run test with KZU5
	cout << "\nTEST 5\n"; 
	gam_run2test(g1, to1, s1, kzu5);
	// Must be
	// Ts1  2f 6f 31 dd 74 f2 49 0e 
	// Ts2  a6 7b 24 d5 a7 50 97 b1 

	// Run test with To-1 and KZU5
	cout << "\nTEST 6\n"; 
	gam_run32test(g1, to2, s2, kzu5);

}

void gam_run2test(GOST gg, byte to[CT2][BS], byte iv[BS], byte key[KL])
{
	byte ts[CT2][BS];
	byte td[CT2][BS];
	byte tt1[BS], gamma[BS];
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
	
	// Encryption in gamma mode
	// Initial filling of N3, N4
	gg.encrypt(iv,tt1);
	u32bit	N3 = make_u32bit(tt1[3], tt1[2], tt1[1], tt1[0]),
			N4 = make_u32bit(tt1[7], tt1[6], tt1[5], tt1[4]);

	for (j = 0; j < CT2; j++)
	{
		// Add constants C1 and C2
		N3 = N3+C2;
		N4 = ((u64bit)N4+(u64bit)C1)%(u64bit)FF;

		tt1[0] = get_byte(3, N3); tt1[1] = get_byte(2, N3);
		tt1[2] = get_byte(1, N3); tt1[3] = get_byte(0, N3);
		tt1[4] = get_byte(3, N4); tt1[5] = get_byte(2, N4);
		tt1[6] = get_byte(1, N4); tt1[7] = get_byte(0, N4);

		gg.encrypt(tt1, gamma);
		xor_buf(ts[j], to[j], gamma, BS);
	}

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

	// Decryption in gamma mode
	// Initial filling of N3, N4
	gg.encrypt(iv,tt1);
	N3 = make_u32bit(tt1[3], tt1[2], tt1[1], tt1[0]),
	N4 = make_u32bit(tt1[7], tt1[6], tt1[5], tt1[4]);

	for (j = 0; j < CT2; j++)
	{
		// Add constants C1 and C2
		N3 = N3+C2;
		N4 = ((u64bit)N4+(u64bit)C1)%(u64bit)FF;
	
		tt1[0] = get_byte(3, N3); tt1[1] = get_byte(2, N3);
		tt1[2] = get_byte(1, N3); tt1[3] = get_byte(0, N3);
		tt1[4] = get_byte(3, N4); tt1[5] = get_byte(2, N4);
		tt1[6] = get_byte(1, N4); tt1[7] = get_byte(0, N4);

		gg.encrypt(tt1, gamma);
		xor_buf(td[j], ts[j], gamma, BS);
	}

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

void gam_run32test(GOST gg, byte to[CT32][BS], byte iv[BS], byte key[KL])
{
	byte ts[CT32][BS];
	byte td[CT32][BS];
	byte tt1[BS], gamma[BS];
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
	cout << "S2   ";
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
	
	// Initial filling of N3, N4
	gg.encrypt(iv,tt1);
	u32bit	N3 = make_u32bit(tt1[3], tt1[2], tt1[1], tt1[0]),
			N4 = make_u32bit(tt1[7], tt1[6], tt1[5], tt1[4]);

	for (j = 0; j < CT32; j++)
	{
		// Add constants C1 and C2
		N3 = N3+C2;
		N4 = ((u64bit)N4+(u64bit)C1)%(u64bit)FF;
	
		tt1[0] = get_byte(3, N3); tt1[1] = get_byte(2, N3);
		tt1[2] = get_byte(1, N3); tt1[3] = get_byte(0, N3);
		tt1[4] = get_byte(3, N4); tt1[5] = get_byte(2, N4);
		tt1[6] = get_byte(1, N4); tt1[7] = get_byte(0, N4);

		gg.encrypt(tt1, gamma);
		xor_buf(ts[j], to[j], gamma, BS);
	}

	// Output encrypted data
	cout << "Encrypted data:" << endl;
	for ( j = 0; j < CT32; j++)
	{
		cout << "Ts" << dec << setfill('0') << setw(2)<< j+1 << "  ";
		for (i = 8; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)ts[j][i-1] << " ";
		cout << endl;
	}
	cout << endl;
}

