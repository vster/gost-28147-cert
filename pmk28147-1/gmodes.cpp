#include <iostream>
#include <iomanip>
#include "util.h"
#include "gost.h"
// #include "vchtest.h"

using namespace std;

extern byte kzu[5][KL];
extern byte to1[2][BS];
extern byte to2[2][BS];
extern byte s1[BS];
extern byte s2[BS];

// const int CT2 = 2;
// const int CT32 = 32;

// Output data
void output_data(byte tx[MAXT][BS], u32bit n, char* type)
{
	u32bit j, i;
	for ( j = 0; j < n; j++)
	{
		cout << type 
			 << dec << setfill('0') << setw(2) 
			 << j+1 << "  ";
		for (i = 8; i > 0; i--)
			cout << hex << setfill('0') << setw(2) 
				 << (int)tx[j][i-1] << " ";
		cout << endl;
	}
	cout << endl;
}

// Output init vector
void output_iv ( byte iv[BS] )
{
	int i;
	cout << "Init vector:" << endl;
	cout << "S  ";
	for (i = 8; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)iv[i-1] << " ";
	cout << endl << endl;
}

// Output key
void output_key( byte key[KL] )
{
	int i;
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
}	

// Output imito-pristavka
void output_imit(byte ip[4])
{
	u32bit i;
	cout << "I32  ";
	for (i = 4; i > 0; i--)
		cout << hex << setfill('0') << setw(2) 
			 << (int)ip[i-1] << " ";
	cout << endl;
}


// Encrypt data in ECB mode
void ecb_encrypt( byte to[MAXT][BS], 
				  byte ts[MAXT][BS], 
				  u32bit n )
{
	for (u32bit j = 0; j < n; j++)
		Encrypt( to[j], ts[j] );
}

// Decrypt data in ECB mode
void ecb_decrypt( byte ts[MAXT][BS], 
				  byte td[MAXT][BS], 
				  u32bit n)
{
	for (u32bit j = 0; j < n; j++)
		Decrypt( ts[j], td[j] );
}	

// Encrypt data in CBC mode
void cbc_encrypt( byte to[MAXT][BS], 
				  byte ts[MAXT][BS], 
				  byte iv[BS], 
				  u32bit n)
{
	u32bit j;
	byte tt[BS];

	xor_buf(tt, to[0], iv, BS);
	Encrypt( tt, ts[0] );
	// gg.encrypt(tt, ts[0]);
	for (j = 1; j < n; j++)
	{
		xor_buf(tt, to[j], ts[j-1], BS);
		Encrypt( tt, ts[j] );		
		// Encrypt(tt, ts[j]);
	}
}

// Decrypt data in CBC mode
void cbc_decrypt ( byte ts[MAXT][BS], 
				   byte td[MAXT][BS], 
				   byte iv[BS], 
				   u32bit n)
{
	u32bit j;
	byte tt[BS];

	for (j = n-1; j >= 1; j--)
	{
		Decrypt(ts[j], tt);		
		// gg.decrypt(ts[j], tt);
		xor_buf(td[j], tt, ts[j-1], BS);
	}
	Decrypt(ts[0], tt);
	xor_buf(td[0], tt, iv, BS);
}

// Encrypt in OFB mode (gamma)
void ofb_encrypt ( byte to[MAXT][BS], 
				   byte ts[MAXT][BS], 
				   byte iv[BS], 
				   u32bit n)
{	
	byte tt1[BS], gamma[BS];
	u32bit j;

	// Initial filling of N3, N4
	Encrypt(iv,tt1);
	u32bit	N3 = make_u32bit(tt1[3], tt1[2], tt1[1], tt1[0]),
			N4 = make_u32bit(tt1[7], tt1[6], tt1[5], tt1[4]);

	for (j = 0; j < n; j++)
	{
		// Add constants C1 and C2
		N3 = N3+C2;
		N4 = ((u64bit)N4+(u64bit)C1)%(u64bit)FF;

		tt1[0] = get_byte(3, N3); tt1[1] = get_byte(2, N3);
		tt1[2] = get_byte(1, N3); tt1[3] = get_byte(0, N3);
		tt1[4] = get_byte(3, N4); tt1[5] = get_byte(2, N4);
		tt1[6] = get_byte(1, N4); tt1[7] = get_byte(0, N4);

		Encrypt(tt1, gamma);
		xor_buf(ts[j], to[j], gamma, BS);
	}
}

// Decrypt data in OFB mode (gamma)
void ofb_decrypt( byte ts[MAXT][BS], 
				  byte td[MAXT][BS], 
				  byte iv[BS], 
				  u32bit n)
{
	byte tt1[BS], gamma[BS];
	u32bit j;

	// Initial filling of N3, N4
	Encrypt(iv,tt1);
	u32bit	N3 = make_u32bit(tt1[3], tt1[2], tt1[1], tt1[0]),
			N4 = make_u32bit(tt1[7], tt1[6], tt1[5], tt1[4]);

	for (j = 0; j < n; j++)
	{
		// Add constants C1 and C2
		N3 = N3+C2;
		N4 = ((u64bit)N4+(u64bit)C1)%(u64bit)FF;
	
		tt1[0] = get_byte(3, N3); tt1[1] = get_byte(2, N3);
		tt1[2] = get_byte(1, N3); tt1[3] = get_byte(0, N3);
		tt1[4] = get_byte(3, N4); tt1[5] = get_byte(2, N4);
		tt1[6] = get_byte(1, N4); tt1[7] = get_byte(0, N4);

		Encrypt(tt1, gamma);
		xor_buf(td[j], ts[j], gamma, BS);
	}
}

// Encrypt data in CFB mode
void cfb_encrypt( byte to[MAXT][BS], 
				  byte ts[MAXT][BS], 
				  byte iv[BS], 
				  u32bit n)
{
	byte gamma[BS];
	u32bit j;

	// Encrypt in CFB mode
	Encrypt(iv, gamma);
	xor_buf(ts[0], to[0], gamma, BS);
	
	for (j = 1; j < n; j++)
	{
		Encrypt(ts[j-1], gamma);
		xor_buf(ts[j], to[j], gamma, BS);
	}
}

// Decrypt data in CFB mode
void cfb_decrypt( byte ts[MAXT][BS], 
				  byte td[MAXT][BS], 
				  byte iv[BS], 
				  u32bit n)
{
	byte gamma[BS];
	u32bit j;

	Encrypt(iv, gamma);
	xor_buf(td[0], ts[0], gamma, BS);

	for (j = 1; j < n; j++)
	{
		Encrypt(ts[j-1], gamma);
		xor_buf(td[j], ts[j], gamma, BS);
	}
}

// Imita
void imita( byte to[MAXT][BS], 
		    byte ip[4], 
			u32bit n)
{
	byte tt1[BS], tt2[BS];
	u32bit i, j;

	// Make Imito-Pristavka
	Encrypt16(to[0], tt1);
	for (j = 1; j < n; j++)
	{
		xor_buf(tt2, tt1, to[j], BS);
		Encrypt16(tt2, tt1);
	}

	for (i = 0; i < 4; i++)
		ip[i] = tt1[i];
}

// Run one test
void run_one_test( Mode mode, 
				   byte to[MAXT][BS], 
				   byte iv[BS], 
				   byte key[KL], 
				   u32bit n)
{
	byte ts[MAXT][BS];
	byte td[MAXT][BS];
	byte ip[4];

	// Output key
	output_key ( key );
	set_key( key );

	// Output init vector
	if ( (mode != ECB) && (mode != IMT)  )
		output_iv ( iv );

	// Output open data
	cout << "Open data:" << endl;
	output_data(to, n, "To");

	// Encrypt 
	switch (mode)
	{
		case ECB: 		ecb_encrypt ( to, ts, n);		break;
		case CBC:		cbc_encrypt ( to, ts, iv, n);	break;
		case OFB:		ofb_encrypt ( to, ts, iv, n);	break;
		case CFB:		cfb_encrypt ( to, ts, iv, n);	break;
		case IMT:		
			{
				imita( to, ip, n);
				output_imit( ip );
				goto end;	
			}
	}

	// Output encrypted data
	cout << "Encrypted data:" << endl;
	output_data(ts, n, "Ts");

	// Decrypt
	switch (mode)
	{
		case ECB: 		ecb_decrypt ( ts, td, n);		break;
		case CBC:		cbc_decrypt ( ts, td, iv, n);	break;
		case OFB:		ofb_decrypt ( ts, td, iv, n);	break;
		case CFB:		cfb_decrypt ( ts, td, iv, n);	break;
	}

	// Output decrypted data
	cout << "Decrypted data:" << endl;
	output_data(td, n, "Td");

end: {}
}

// Run test block (6 tests)
void run_test_block ( Mode mode )
{
	cout << "\n\nGOST 28147-89." << endl;
	switch (mode)
	{
		case ECB: 	cout << "ECB";	break;
		case CBC:	cout << "CBC";	break;
		case OFB:	cout << "OFB";	break;
		case CFB:	cout << "CFB";	break;
		case IMT:	cout << "IMT";	break;
	}
	cout << " mode." << endl;
	
	for (int k = 0; k < 5; k++)
	{
		cout << "\nTEST " << k+1 << endl;
		run_one_test( mode, to1, s1, kzu[k], 2);
	}

	cout << "\nTEST 6\n"; 
	run_one_test( mode, to2, s2, kzu[4], 32);
}
