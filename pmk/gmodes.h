// Encrypt/decrypt modes
#include "gost.h"

using namespace std;
using namespace OpenCL;

const u32bit KL = 32; 
const u32bit BS = 8;
const u32bit MAXT = 32;
const u32bit C1 = 0x01010104;
const u32bit C2 = 0x01010101;
const u32bit FF = 0xffffffff;
const u32bit STR = 10;
enum Mode { ECB, CBC, OFB, CFB, IMT };


void output_key( byte key[KL] );
void output_iv ( byte iv[BS] );
void output_data(byte tx[MAXT][BS], u32bit n, char* type);
void output_imit(byte ip[4]);
void ecb_encrypt (GOST gg, byte to[MAXT][BS], byte ts[MAXT][BS], u32bit n);
void ecb_decrypt (GOST gg, byte ts[MAXT][BS], byte td[MAXT][BS], u32bit n);
void cbc_encrypt (GOST gg, byte to[MAXT][BS], byte ts[MAXT][BS], byte iv[BS], u32bit n);
void cbc_decrypt (GOST gg, byte ts[MAXT][BS], byte td[MAXT][BS], byte iv[BS], u32bit n);
void ofb_encrypt (GOST gg, byte to[MAXT][BS], byte ts[MAXT][BS], byte iv[BS], u32bit n);
void ofb_decrypt (GOST gg, byte ts[MAXT][BS], byte td[MAXT][BS], byte iv[BS], u32bit n);
void cfb_encrypt (GOST gg, byte to[MAXT][BS], byte ts[MAXT][BS], byte iv[BS], u32bit n);
void cfb_decrypt (GOST gg, byte ts[MAXT][BS], byte td[MAXT][BS], byte iv[BS], u32bit n);
void imita (GOST gg, byte to[MAXT][BS], byte ip[4], u32bit n);
void run_one_test(GOST gg, Mode mode, byte to[MAXT][BS], byte iv[BS], byte key[KL], u32bit n);
void run_test_block ( GOST gg, Mode mode );
void gen_to2();
