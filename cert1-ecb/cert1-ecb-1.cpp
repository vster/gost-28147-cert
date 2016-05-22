// Реализация алгоритма криптографического преобразования ГОСТ 28147-89
// Режим простой замены
// Файл gost-ecb.cpp

#include <iostream>
#include <iomanip>
#include <opencl/gost.h>

using namespace std;
using namespace OpenCL;

const u32bit KL = 32; 
const u32bit BS = 8;
const u32bit CT2 = 2;


void main(void)
{
	GOST g1;
	void ecb_run2test(GOST, byte [CT2][BS], byte [KL]);

	// Set test key 1 - KZU-1
	byte kzu1[KL] =	{	0x11, 0x11, 0x11, 0x11,
						0x22, 0x22, 0x22, 0x22,
						0x33, 0x33, 0x33, 0x33,
						0x44, 0x44, 0x44, 0x44,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff };

			
	// Prepare S-Box
	g1.generate_sbox();

	// Set open data To-1
	// Attention! In reverse order! Low-endian
	byte to1[CT2][BS] = {	{0x55, 0x55, 0x55, 0x55, 
							 0xaa, 0xaa, 0xaa, 0xaa},
							{0xaa, 0xaa, 0xaa, 0xaa,
							 0x55, 0x55, 0x55, 0x55 } };				
		
	cout << "GOST 28147-89. ECB mode." << endl;

	// Run test with KZU1
	cout << "\nTEST 1\n"; 
	ecb_run2test (g1, to1, kzu1);
}

void ecb_run2test(GOST gg, byte to[CT2][BS], byte key[KL])
{
	byte ts[CT2][BS];
	byte td[CT2][BS];
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
			
	// Encrypt data in ECB mode
	for (j = 0; j < CT2; j++)
		gg.encrypt(to[j], ts[j]);

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

	// Decrypt data in ECB mode
	for (j = 0; j < CT2; j++)
		gg.decrypt(ts[j], td[j]);
	
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

