// Реализация алгоритма криптографического преобразования ГОСТ 28147-89

#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
#include "gost.h"
#include "gmodes.h"

#include "vchtest.h"

using namespace std;
using namespace OpenCL;

const int MAXFN = 20;

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
    strcat(fname,fext);
    strcpy(filename, fname);
}

// Make one test file
void make_one_test(	char fname[], 
					Mode mode, 
					int oidnum, 
					byte to[MAXT][BS], 
					byte iv[BS], 
					byte key[KL], 
					u32bit n)
{
	GOST g1;
	byte ts[MAXT][BS];
	byte ip[4];
	byte buf4b[4];
	int blen = n*BS;	//	Размер входных/выходных данных в байтах

	g1.generate_sbox(oidnum);

	g1.set_key(key, KL);

	// Make test file
	ofstream testfile;
	testfile.open(fname,ios::binary);
	
	// Output to file OID
	testfile.write((char*)GOST::OID[oidnum], strlen(GOST::OID[oidnum])+1 );
	
	// Output to file key
	testfile.write(reinterpret_cast<char*>(key), KL);
	
	int mode1 = 0;
	switch (mode)
	{
		case CBC:		mode1 = 1;	break;
		case OFB:		mode1 = 3;	break;
		case CFB:		mode1 = 4;	break;
	}

	// Запись в файл режим шифрования и синхропосылки
	if ( (mode != ECB) && (mode != IMT) )
	{
		BEFromInt(buf4b, mode1);	
		testfile.write(reinterpret_cast<char*>(buf4b), sizeof(int));
		testfile.write(reinterpret_cast<char*>(iv), BS);	
	}
	// Output to file length of test data 
	BEFromInt(buf4b, blen);
	testfile.write(reinterpret_cast<char*>(buf4b), sizeof(int));

	// Output to file open data
	testfile.write(reinterpret_cast<char*>(to), blen);

	// Encrypt 
	switch (mode)
	{
		case ECB: 		ecb_encrypt (g1, to, ts, n);		break;
		case CBC:		cbc_encrypt (g1, to, ts, iv, n);	break;
		case OFB:		ofb_encrypt (g1, to, ts, iv, n);	break;
		case CFB:		cfb_encrypt (g1, to, ts, iv, n);	break;
		case IMT:		
			{
				imita(g1, to, ip, n);
				testfile.write(reinterpret_cast<char*>(ip), 4);
				goto end;	
			}
	}
	// Output encrypted data
	testfile.write(reinterpret_cast<char*>(ts), blen);	
end: 
	testfile.close();
}
// Генерация группы тестовых файлов для заданного режима
void make_test_group( Mode mode )
{
	int oidnum;
	int testnum;
	int keynum;
	char fname[MAXFN]; 

	// Make To2
	gen_to2();
	
	ofstream logfile;
	logfile.open("pmkinfo.txt");

	logfile << "Контрольные тесты ГОСТ 28147-89.\n";
	logfile << "Сформированы с помощью программной модели криптоалгоритма.\n\n";

	switch (mode)
	{
	case ECB:
		{ 		
			cout << "Generation of test files in ECB mode.\n";
			logfile << "Режим простой замены. \nECB Mode.\n\n";		
			logfile << "Формат:\n";
			logfile << "  OID - ASCIIZ строка;\n";
			logfile << "  32 байта - ключ в чистом виде;\n";
			logfile << "  4 байта - размер исходных данных;\n";
			logfile << "  data_length - исходные данные;\n";
			logfile << "  data_length - зашифрованные данные.\n\n";
			break;
		}	
	case CBC:
		{
			cout << "Generation of test files in CBC mode.\n";		
			logfile << "Режим простой замены с зацеплением. \nCBC Mode.\n\n";
			logfile << "Формат:\n";
			logfile << "  OID - ASCIIZ строка;\n";
			logfile << "  32 байта - ключ в чистом виде;\n";
			logfile << "  4 байта - режим шифрования (CBC - 1, OFB - 3, CFB - 4);\n";
			logfile << "  8 байта - SV;\n";
			logfile << "  4 байта - размер исходных данных;\n";
			logfile << "  data_length - исходные данные;\n";
			logfile << "  data_length - зашифрованные данные.\n\n";
			break;		
		}		
	case OFB:
		{
			cout << "Generation of test files in OFB mode.\n";			
			logfile << "Режим гаммирования. \nOFB Mode.\n\n";
			logfile << "Формат:\n";
			logfile << "  OID - ASCIIZ строка;\n";
			logfile << "  32 байта - ключ в чистом виде;\n";
			logfile << "  4 байта - режим шифрования (CBC - 1, OFB - 3, CFB - 4);\n";
			logfile << "  8 байта - SV;\n";
			logfile << "  4 байта - размер исходных данных;\n";
			logfile << "  data_length - исходные данные;\n";
			logfile << "  data_length - зашифрованные данные.\n\n";					
			break;		
		}
	case CFB:
		{
			cout << "Generation of test files in CFB mode.\n";			
			logfile << "Режим гаммирования с обратной связью. \nCFB Mode.\n\n";
			logfile << "Формат:\n";
			logfile << "  OID - ASCIIZ строка;\n";
			logfile << "  32 байта - ключ в чистом виде;\n";
			logfile << "  4 байта - режим шифрования (CBC - 1, OFB - 3, CFB - 4);\n";
			logfile << "  8 байта - SV;\n";
			logfile << "  4 байта - размер исходных данных;\n";
			logfile << "  data_length - исходные данные;\n";
			logfile << "  data_length - зашифрованные данные.\n\n";							
			break;			
		}		
	case IMT:
		{
			cout << "Generation of test files in IMT mode.\n";				
			logfile << "Режим выработки имитоприставки. \nIMIT Mode.\n\n";
			logfile << "Формат:\n";
			logfile << "  OID - ASCIIZ строка;\n";
			logfile << "  32 байта - ключ в чистом виде;\n";			
			logfile << "  4 байта - размер исходных данных;\n";
			logfile << "  data_length - исходные данные;\n";			
			logfile << "  4 байта - имитовставка.\n\n";						
			break;			
		}
	}
	logfile << "Список тестов:\n";

	for (oidnum = 0, testnum = 1; oidnum < 6; oidnum++)
	{
		for (keynum = 0; keynum < 5; keynum++)
		{
			FnameFromNum ( fname, testnum );

			logfile << fname << " - " 
					<< "KZU" << keynum+1 
					<< ", TO1";
			if ( (mode != ECB) && (mode != IMT) )
				logfile << ", S1";
			logfile << ", " << GOST::OID[oidnum] << endl;

			make_one_test ( fname, mode, oidnum, to1, s1, kzu[keynum], 2);
			testnum++;
		}
		FnameFromNum ( fname, testnum );
		
		logfile << fname << " - " 
					<< "KZU5" 
					<< ", TO2";
		if ( (mode != ECB) && (mode != IMT) )
			logfile << ", S2";
		logfile << ", " << GOST::OID[oidnum] << endl;
		
		make_one_test( fname, mode, oidnum, to2, s2, kzu[4], 32);
		testnum++;
	}

	logfile.close();
}

// Main module
int main(int argc, char* argv[])
{
	Mode mode;

	if (argc == 1)
	{
	     std::cout << "Usage: " << argv[0] << " mode\n"
						"mode:    ecb, cbc, ofb, cfb, imt\n";                
		return 1;
	}

	if (strcmp (argv[1], "ecb") == 0 )
		mode = ECB;
	else if (strcmp (argv[1], "cbc") == 0 )
		mode = CBC;
	else if (strcmp (argv[1], "ofb") == 0 )
		mode = OFB;
	else if (strcmp (argv[1], "cfb") == 0 )
		mode = CFB;
	else if (strcmp (argv[1], "imt") == 0 )
		mode = IMT;	
	else
	{
		cout << "\nUnknown mode\n";
		return 1;		
	}

	make_test_group( mode );

	return 0;
}



