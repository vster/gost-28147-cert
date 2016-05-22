#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "gost.h"
#include "gmodes.h"

using namespace std;

const int MAXFN = 20;

extern int SBoxStat[8][16];

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

// Run one test
void run_one_test( Mode mode, 
				   char tofname[], 
				   char ivfname[], 
				   char keyfname[])
{
	
	int oidnum = 1;		// Номер OID
	byte to[MAXT][BS];	// Открытый текст
	byte ts[MAXT][BS];	// Зашифрованный текст
	byte td[MAXT][BS];	// Расшифрованный текст
	byte iv[BS];		// Синхропосылка
	byte key[KL];		// Ключ
	u32bit n;			// Кол-во блоков
	byte ip[BS];		// Имитоприставка

	generate_sbox( oidnum );

	// Читаем ключ
	ifstream keyfile;
	keyfile.open(keyfname,ios::binary);
	keyfile.read(reinterpret_cast<char*>(key),KL);
	keyfile.close();

	set_key( key );

	output_key ( key );

	// Читаем файл синхропосылки iv
	ifstream ivfile;
	ivfile.open(ivfname,ios::binary);
	ivfile.read(reinterpret_cast<char*>(iv),BS);
	ivfile.close();
	
	// Output init vector
	if ( (mode != ECB) && (mode != IMT)  )
		output_iv ( iv );

	// Читаем входной файл
	ifstream tofile;
	tofile.open(tofname,ios::binary);

	// get size of file
	tofile.seekg (0,tofile.end);
	long size = tofile.tellg();
	tofile.seekg (0);

	// allocate memory for file content
	byte* buffer = new byte[size];

	// read content of  open text file
	tofile.read (reinterpret_cast<char*>(buffer),size);
	tofile.close();

	// Число блоков
	n = size/BS;

	for ( int i = 0; i < size; i++)
		to[i/BS][i%BS] = buffer[i];
	
	// При некратном блоку BS размера входных данных забиваем остаток нулями
	if ( size%BS > 0 )
	{
		n++;
		for ( int i = size%BS; i < BS; i++ )
			to[n-1][i] = 0x0;	
	}

	cout << "Open data:" << endl;
	output_data ( to, n, "To" );

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

	cout << "Encrypted data:" << endl;
	output_data ( ts, n, "Ts" );

	// Decrypt
	switch (mode)
	{
		case ECB: 		ecb_decrypt ( ts, td, n);		break;
		case CBC:		cbc_decrypt ( ts, td, iv, n);	break;
		case OFB:		ofb_decrypt ( ts, td, iv, n);	break;
		case CFB:		cfb_decrypt ( ts, td, iv, n);	break;
	}

	cout << "Decrypted data:" << endl;
	output_data ( td, n, "Td" );

end: {}
}

// Main module
int main(int argc, char* argv[])
{
	Mode mode = CBC;
	char tofile[20], ivfile[20], keyfile[20];

	int tonum = 1;	
    sprintf(tofile, "to%d.dat", tonum);
	
	int snum = 1;
    sprintf(ivfile, "s%d.dat", snum);
	
	int kzunum;
	for ( kzunum = 1; kzunum <= 5; kzunum++ )
	{
        sprintf(keyfile, "kzu%d.dat", kzunum);
	
		cout << endl << keyfile << endl;

		run_one_test( mode, tofile, ivfile, keyfile );
	}

	tonum = 2;	
    sprintf(tofile, "to%d.dat", tonum);

	snum = 2;
    sprintf(ivfile, "s%d.dat", snum);

	kzunum = 5;
    sprintf(keyfile, "kzu%d.dat", kzunum);

	run_one_test( mode, tofile, ivfile, keyfile );

	for ( int i = 0; i < 16; i++)
	{
		for ( int j = 0; j < 8; j++ )
			cout << dec << setw(4) << setfill(' ') << SBoxStat[j][i] << " ";
		cout << endl;
	}

	return 0;
}
