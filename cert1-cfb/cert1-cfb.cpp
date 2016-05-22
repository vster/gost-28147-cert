// Реализация алгоритма криптографического преобразования ГОСТ 28147-89
// Режим гаммирования с обратной связью
// Файл gost-cfb.cpp

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
	void cfb_run2test(GOST, byte [CT2][BS], byte [BS], byte [KL]);
	void cfb_run32test(GOST, byte [CT32][BS], byte [BS], byte [KL]);

	// Set test key 1 - KZU-1
	byte kzu1[KL] =	{	0x01, 0x00, 0x00, 0x00,
						0x01, 0x00, 0x00, 0x00,
						0x01, 0x00, 0x00, 0x00,
						0x01, 0x00, 0x00, 0x00,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff,
						0xff, 0xff, 0xff, 0xff };
	
	// Prepare S-Box
	g1.generate_sbox();

	// Set open data To-1
	// Attention! In reverse order! Low-endian
	byte to1[CT2][BS] = {{0x55, 0x55, 0x55, 0x55,
						0xaa, 0xaa, 0xaa, 0xaa},
						{0x00, 0x00, 0x00, 0x00,
						0xcc, 0xcc, 0xcc, 0xcc} };				
	
	// Init vector
	// Attention! In reverse order! Low-endian
	byte s1[BS] = {		0x2a, 0x80, 0xa7, 0xc3,
						0xff, 0xa8, 0xe3, 0x47 };
	
	
	cout << "GOST 28147-89. CFB mode." << endl;
	
	// Run test with KZU1
	cout << "\nTEST 1\n"; 
	cfb_run2test(g1, to1, s1, kzu1);
	
}

void cfb_run2test(GOST gg, byte to[CT2][BS], byte iv[BS], byte key[KL])
{
	byte ts[CT2][BS];
	byte td[CT2][BS];
	byte gamma[BS];
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
	
	// Encrypt in CFB mode
	gg.encrypt(iv, gamma);
	xor_buf(ts[0], to[0], gamma, BS);
	
	gg.encrypt(ts[0], gamma);
	xor_buf(ts[1], to[1], gamma, BS);

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

	// Decrypt in CFB mode
	gg.encrypt(iv, gamma);
	xor_buf(td[0], ts[0], gamma, BS);

	gg.encrypt(ts[0], gamma);
	xor_buf(td[1], ts[1], gamma, BS);

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

void cfb_run32test(GOST gg, byte to[CT32][BS], byte iv[BS], byte key[KL])
{
	byte ts[CT32][BS];
	byte td[CT32][BS];
	byte gamma[BS];
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
		cout << "To"  << dec << setfill('0') << setw(2) << j+1 << "  ";
		for (i = 8; i > 0; i--)
			cout << hex << setfill('0') << setw(2) 
				 << (int)to[j][i-1] << " ";
		cout << endl;
	}
	cout << endl;
	
	// Encrypt in CFB mode
	gg.encrypt(iv, gamma);
	xor_buf(ts[0], to[0], gamma, BS);
	
	for (j = 1; j < CT32; j++)
	{
		gg.encrypt(ts[j-1], gamma);
		xor_buf(ts[j], to[j], gamma, BS);
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

	// Decrypt in CFB mode
	gg.encrypt(iv, gamma);
	xor_buf(td[0], ts[0], gamma, BS);

	for (j = 1; j < CT32; j++)
	{
		gg.encrypt(ts[j-1], gamma);
		xor_buf(td[j], ts[j], gamma, BS);
	}
}
