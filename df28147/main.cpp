#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
#include <string.h>
#include "util.h"
#include "gost.h"
#include "gmodes.h"
// #include "vchtest.h"

using namespace std;

const int MAXFN = 20;
int oidnum;
Mode mode;
ofstream outfile;

void BEFromInt(byte buf[], int num)
{
	for (int i = 3; i>=0; i--)
	{
		buf[i] = num&0xFF;
		num >>= 8;
	}
}

void FnameFromNum (char filename[], int num)		
{
	char fext[4];
	char fname[MAXFN] = "test."; 
    sprintf(fext,"%03x",num);
	strcat_s(fname,fext);
	strcpy_s(filename, 20, fname);
}


void KeyFromNum (byte key[KL], int num)
{
	int i;
	byte BitArr[8] = {0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff};
	
	for (i = 0; i < num/8; i++)
		key[i] = 0xff;
	key[i] = BitArr[num%8];
}

void TOFromNum (byte to[BS], int num)
{
	byte BitArr[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	for (int i = 0; i < 8; i++)
		to[i] = 0x00;
	to[num/8] = BitArr[num%8];
}

void TOFromNum2 (byte to[BS], int num)
{
	byte BitArr[30];
	for (int i = 0; i < 15; i++)
	{
		BitArr[2*i] = i+1;
		BitArr[2*i+1] = (i+1)*0x10;
	}

	for (int i = 0; i < 8; i++)
		to[i] = 0x00;
	to[num%8] = BitArr[num/8];
}

void run_one_test( byte to[BS], 
				   byte iv[BS], 
				   byte key[KL],
				   int FitNum)
{
	byte ts[BS] = {0}; 
	set_key(key);
	
	// int n = 1;
	Encrypt( to, ts );
	// ecb_encrypt ( to, ts, n);	

	outfile << "ParamOid=" << OID[oidnum] << endl;
	outfile << "CipherText=";
	for (int i = 0; i < BS; i++ ) 
		outfile << hex << setw(2) << setfill('0') << (int)ts[i];
	outfile << endl;
	outfile << "InputText=";
	for (int i = 0; i < BS; i++ ) 
		outfile << hex << setw(2) << setfill('0') << (int)to[i];
	outfile << endl;
	outfile << "CipherMode=" << ModeName[mode] << endl;
	outfile << "SecretKeyValue=";
	for (int i = 0; i < KL; i++ ) 
		outfile << hex << setw(2) << setfill('0') << (int)key[i];
	outfile << endl;
	outfile << "FitNumber=" << dec << FitNum << endl;
	outfile << endl;
}

// Test N1
// Выполнение тестов с переменным открытым текстом
// AllText
void AllTextTest()
{
	byte key[KL] = {0};
	byte to[BS] = {0}; 
	byte iv[BS] = {0};
	
	generate_sbox(oidnum);

	char fname[MAXFN] = "AllText_";
	strcat_s(fname,OidExt[oidnum]);
	// strcat_s(fname,".dat");

	outfile.open(fname);
	
	for (int i = 0; i < 32; i++)
		key[i] = 0x05;
	for (int i = 0; i < 64; i++)
	{
		TOFromNum (to,i);
		run_one_test(to,iv,key,i);
	}
	outfile.close();
}

// Test N2
// Выполнение тестов с переменным ключом
// AllKey
void AllKeyTest()
{
	byte key[KL] = {0};
	byte to[BS] = {0}; 
	byte iv[BS] = {0};
	
	generate_sbox(oidnum);

	char fname[MAXFN] = "AllKey_";
	strcat_s(fname,OidExt[oidnum]);
	// strcat_s(fname,".dat");

	outfile.open(fname);
	
	for (int i = 0; i < 256; i++)
	{
		KeyFromNum(key,i);
		run_one_test(to,iv,key,i);
	}
	outfile.close();
}

// Test N3
// Тест корректности использования таблицы замен
// HalfByteTest
void HalfByteTest()
{
	byte key[KL] = {0};
	byte to[BS] = {0}; 
	byte iv[BS] = {0};
	
	generate_sbox(oidnum);

	char fname[MAXFN] = "HalfByteText_";
	strcat_s(fname,OidExt[oidnum]);
	// strcat_s(fname,".dat");

	outfile.open(fname);
	
	for (int i = 0; i < 32; i++)
		key[i] = 0x00;
	for (int i = 0; i < 240; i++)
	{
		TOFromNum2 (to,i);
		run_one_test(to,iv,key,i);
	}
	outfile.close();
}	

// Test N5
// Тест с использованием случайных данных
// RandByteTest
void RandByteTest()
{
	byte key[KL] = {0};
	byte to[BS] = {0};
	// byte to0[BS];
	byte ts[BS] = {0};
	byte iv[BS] = {0};
	byte gamma[KL];
	
	generate_sbox(oidnum);

	char fname[MAXFN] = "RandByteText_";
	strcat_s(fname,OidExt[oidnum]);
	// strcat_s(fname,".dat");

	outfile.open(fname);
	
	for ( int FitNum = 0; FitNum < 400; FitNum++)
	{
		// memcpy(to0,to,BS);
		set_key(key);
		for ( int j = 0; j < 9996; j++)
		{
			// set_key(key);
			Encrypt(to,ts);
			// for (int k = 0; k < BS; k++)
			//	to[k] = ts[k];

			memcpy(to,ts,BS);
		}
		// Последние 4 цикла в ГОСТе используем для генерации гаммы 
		Encrypt(to,ts);
		memcpy(to,ts,BS);
		memcpy(gamma,ts,BS);
		Encrypt(to,ts);
		memcpy(to,ts,BS);
		memcpy(gamma+8,ts,BS);
		Encrypt(to,ts);
		memcpy(to,ts,BS);
		memcpy(gamma+16,ts,BS);
		Encrypt(to,ts);

		outfile << "ParamOid=" << OID[oidnum] << endl;
		outfile << "CipherText=";
		for (int i = 0; i < BS; i++ ) 
			outfile << hex << setw(2) << setfill('0') << (int)ts[i];
		outfile << endl;
		outfile << "InputText=";
		for (int i = 0; i < BS; i++ ) 
			outfile << hex << setw(2) << setfill('0') << (int)to[i];
		outfile << endl;
		outfile << "CipherMode=" << ModeName[mode] << endl;
		outfile << "SecretKeyValue=";
		for (int i = 0; i < KL; i++ ) 
			outfile << hex << setw(2) << setfill('0') << (int)key[i];
		outfile << endl;
		outfile << "FitNumber=" << dec << FitNum << endl;
		outfile << endl;

		memcpy(to,ts,BS);
		memcpy(gamma+24,ts,BS);	

		xor_buf(key,gamma,KL);
	}

	outfile.close();
}	


// Main module
int main()
{
	mode = ECB;
	
/*
	for ( oidnum = 0; oidnum < 6; oidnum++)
		AllTextTest();

	for ( oidnum = 0; oidnum < 6; oidnum++)	
		AllKeyTest();

	for ( oidnum = 0; oidnum < 6; oidnum++)			
		HalfByteTest();
*/
	for ( oidnum = 0; oidnum < 6; oidnum++)	
		RandByteTest();

	return 0;
}
