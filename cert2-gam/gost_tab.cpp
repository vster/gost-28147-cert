/*************************************************
* S-Box Tables for GOST                          *
* (C) 1999-2001 The OpenCL Project               *
*************************************************/

#include <opencl/gost.h>

namespace OpenCL {

/*
	// S-����� �� ����������� ������ ���� 28147-89	
	const byte GOST::K1[16]=
	{0x4,0x2,0xf,0x5,0x9,0x1,0x0,0x8,0xe,0x3,0xb,0xc,0xd,0x7,0xa,0x6};		//1
	const byte GOST::K2[16]=	
	{0xc,0x9,0xf,0xe,0x8,0x1,0x3,0xa,0x2,0x7,0x4,0xd,0x6,0x0,0xb,0x5};		//2		
	const byte GOST::K3[16]=
	{0xd,0x8,0xe,0xc,0x7,0x3,0x9,0xa,0x1,0x5,0x2,0x4,0x6,0xf,0x0,0xb};		//3
	const byte GOST::K4[16]=
	{0xe,0x9,0xb,0x2,0x5,0xf,0x7,0x1,0x0,0xd,0xc,0x6,0xa,0x4,0x3,0x8};		//4
	const byte GOST::K5[16]=	
	{0x3,0xe,0x5,0x9,0x6,0x8,0x0,0xd,0xa,0xb,0x7,0xc,0x2,0x1,0xf,0x4};		//5
	const byte GOST::K6[16]=
	{0x8,0xf,0x6,0xb,0x1,0x9,0xc,0x5,0xd,0x3,0x7,0xa,0x0,0xe,0x2,0x4};		//6
	const byte GOST::K7[16]=
	{0x9,0xb,0xc,0x0,0x3,0x6,0x7,0x5,0x4,0x8,0xe,0xf,0x1,0xa,0x2,0xd};		//7
	const byte GOST::K8[16]=
	{0xc,0x6,0x5,0x2,0xb,0x0,0x9,0xd,0x3,0xe,0x7,0xa,0xf,0x4,0x1,0x8};		//8
*/	

	// ����� ����� ������-������������
	const byte GOST::K1[16]=
	{0xB,0xA,0xF,0x5,0x0,0xC,0xE,0x8,0x6,0x2,0x3,0x9,0x1,0x7,0xD,0x4};		//1
	const byte GOST::K2[16]=    
	{0x1,0xD,0x2,0x9,0x7,0xA,0x6,0x0,0x8,0xC,0x4,0x5,0xF,0x3,0xB,0xE};		//2
	const byte GOST::K3[16]=
	{0x3,0xA,0xD,0xC,0x1,0x2,0x0,0xB,0x7,0x5,0x9,0x4,0x8,0xF,0xE,0x6};		//3
	const byte GOST::K4[16]=
	{0xB,0x5,0x1,0x9,0x8,0xD,0xF,0x0,0xE,0x4,0x2,0x3,0xC,0x7,0xA,0x6};		//4
	const byte GOST::K5[16]=
	{0xE,0x7,0xA,0xC,0xD,0x1,0x3,0x9,0x0,0x2,0xB,0x4,0xF,0x8,0x5,0x6};		//5
	const byte GOST::K6[16]=
	{0xE,0x4,0x6,0x2,0xB,0x3,0xD,0x8,0xC,0xF,0x5,0xA,0x0,0x7,0x1,0x9};		//6
	const byte GOST::K7[16]=
	{0x3,0x7,0xE,0x9,0x8,0xA,0xF,0x0,0x5,0x2,0x6,0xC,0xB,0x4,0xD,0x1};		//7
	const byte GOST::K8[16]=
	{0x9,0x6,0x3,0x2,0x8,0xB,0x1,0x7,0xA,0x4,0xE,0xF,0xC,0x0,0xD,0x5};		//8

	u32bit GOST::SBOX1[256];
	u32bit GOST::SBOX2[256];
	u32bit GOST::SBOX3[256];
	u32bit GOST::SBOX4[256];
}
