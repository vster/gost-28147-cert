// Реализация алгоритма криптографического преобразования ГОСТ 28147-89

#include <iostream>
#include <iomanip>
#include <opencl/gost.h>
#include "gmodes.h"

using namespace std;
using namespace OpenCL;

// Main module
int main(int argc, char* argv[])
{
	Mode mode;
	GOST g1;

	// Prepare S-Box
	g1.generate_sbox();
	
	// Output OID table
	cout << "\nOID 1.2.643.2.2.31.4\n\n";
	g1.output_oid();

	if (argc > 1)
	{
		if (strcmp (argv[1], "ECB") == 0 )
			mode = ECB;
		else if (strcmp (argv[1], "CBC") == 0 )
			mode = CBC;
		else if (strcmp (argv[1], "OFB") == 0 )
			mode = OFB;
		else if (strcmp (argv[1], "CFB") == 0 )
			mode = CFB;
		else if (strcmp (argv[1], "IMT") == 0 )
			mode = IMT;	
		else if (strcmp (argv[1], "ALL") == 0 )
		{
			run_test_block ( g1, ECB );
			run_test_block ( g1, CBC );
			run_test_block ( g1, OFB );
			run_test_block ( g1, CFB );
			run_test_block ( g1, IMT );
			return 0;
		}
		else
		{
			cout << "\nUnknown mode\n";
			return 1;
		}
	}
	else
	{
	     std::cout << "Usage: " << argv[0] << " mode\n"
						"mode:    ECB, CBC, OFB, CFB, IMT, ALL\n";                
			return 1;
	}

//	run_test_block ( g1, ECB );
//	run_test_block ( g1, CBC );
//	run_test_block ( g1, OFB );
// 	run_test_block ( g1, CFB );
//	run_test_block ( g1, IMT );
	run_test_block ( g1, mode );

	return 0;
}



