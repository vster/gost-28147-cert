// ���������� ��������� ������������������ �������������� ���� 28147-89
// ����� ��������� ��������������
// ���� gost-ip.cpp

#include <iostream>
#include <iomanip>
#include <opencl/gost.h>

using namespace std;
using namespace OpenCL;

const u32bit KL = 32; 
const u32bit BS = 8;
const u32bit CT2 = 2;
const u32bit CT32 = 32;

int main(void)
{
	GOST g1;

	void ip_run2test(GOST, byte [CT2][BS], byte [KL]);
	void ip_run32test(GOST, byte [CT32][BS], byte [KL]);

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
	byte to1[CT2][BS] = {	{0xcc, 0xcc, 0xcc, 0xcc,
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
	byte s1[BS]	 = {	0x2a, 0x80, 0xa7, 0xc3,
						0xff, 0xa8, 0xe3, 0x47 };
	byte s2[BS]  = {	0x73, 0xcf, 0x64, 0xe6,
						0x7d, 0x4b, 0x42, 0xe6 };	
	
	cout << "GOST 28147-89. Imitopristavka (HMAC) mode." << endl;
	
	// Run test with KZU1
	cout << "\nTEST 1\n"; 
	ip_run2test (g1, to1, kzu1);

	// Run test with KZU2
	cout << "\nTEST 2\n"; 
	ip_run2test (g1, to1, kzu2);

	// Run test with KZU3
	cout << "\nTEST 3\n"; 
	ip_run2test (g1, to1, kzu3);

	// Run test with KZU4
	cout << "\nTEST 4\n"; 
	ip_run2test (g1, to1, kzu4);

	// Run test with KZU5
	cout << "\nTEST 5\n"; 
	ip_run2test (g1, to1, kzu5);

	// Run test with To-2 and KZU5
	cout << "\nTEST 6\n"; 
	ip_run32test (g1, to2, kzu5);
}

void ip_run2test(GOST gg, byte to[CT2][BS], byte key[KL])
{
	byte tt1[BS], tt2[BS];
	byte ip[4];
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

	// Make Imito-Pristavka
	gg.encrypt(to[0], tt1);
	xor_buf(tt2, tt1, to[1], BS);
	gg.encrypt(tt2, tt1);

	for (i = 0; i < 4; i++)
		ip[i] = tt1[i];

	cout << "I32  ";
	for (i = 4; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)ip[i-1] << " ";
	cout << endl;
}

void ip_run32test(GOST gg, byte to[CT32][BS], byte key[KL])
{
	byte tt1[BS], tt2[BS];
	byte ip[4];
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

	// Make Imito-Pristavka
	gg.encrypt(to[0], tt1);
	for (j = 1; j < CT32; j++)
	{
		xor_buf(tt2, tt1, to[j], BS);
		gg.encrypt(tt2, tt1);
	}

	for (i = 0; i < 4; i++)
		ip[i] = tt1[i];

	cout << "I32   ";
	for (i = 4; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)ip[i-1] << " ";
	cout << endl;
}

