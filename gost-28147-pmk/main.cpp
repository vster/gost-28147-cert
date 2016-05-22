#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
#include "util.h"
#include "gost.h"
#include "gmodes.h"
#include "vchtest.h"

using namespace std;

const int MAXFN = 20;

void BEFromInt(byte buf[], int num)
{
	for (int i = 3; i>=0; i--)
	{
		buf[i] = num&0xFF;
		num >>= 8;
	}
}

// Main module
int main(int argc, char* argv[])
{
	Mode mode;
	int oidnum = 0;

	if (argc < 3)
	{
	     std::cout << "Usage: " << argv[0] << " mode OID\n"
						"\nMode:    ecb, cbc, cnt, cfb, imt\n"
						<< "\nOID:\n";              
		for ( int i = 0; i < 7; i++ )
			cout << i <<"\t" << OID[i] << endl;
		return 1;
	}

	if (strcmp (argv[1], "ecb") == 0 )
		mode = ECB;
	else if (strcmp (argv[1], "cbc") == 0 )
		mode = CBC;
	else if (strcmp (argv[1], "cnt") == 0 )
		mode = CNT;
	else if (strcmp (argv[1], "cfb") == 0 )
		mode = CFB;
	else if (strcmp (argv[1], "imt") == 0 )
		mode = IMT;	
	else
	{
		cout << "\nUnknown mode\n";
		return 1;		
	}
	
	oidnum = atoi(argv[2]);
	if ( oidnum < 0 || oidnum >= OIDNUM )
	{
		cout << "\nIncorrect OID\n";
		return 1;		
	}

	run_test_block ( mode, oidnum );
	//	make_test_group( mode );

	return 0;
}
