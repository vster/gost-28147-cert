#ifndef GOST_H__
#define GOST_H__

const u32bit KL = 32; 
const u32bit BS = 8;
const u32bit MAXT = 32;
const u32bit C1 = 0x01010104;
const u32bit C2 = 0x01010101;
const u32bit FF = 0xffffffff;
const u32bit STR = 10;
const int OIDNUM = 7;
enum Mode { ECB, CBC, CNT, CFB, IMT };

extern const char* OID[OIDNUM]; 

void generate_sbox ( int oidnum );
void set_key( const byte key[KL] );
void Encrypt(const byte in[BS], byte out[BS] );
void Decrypt(const byte in[BS], byte out[BS] );
void Encrypt16(const byte in[BS], byte out[BS] );
void output_oid(int oidnum);

#endif