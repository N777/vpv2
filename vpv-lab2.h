//**************************************************************************************
// ������ ������������ ���������� ������� sin(x)/x = 1 - x^2/3! + x^4/5! - x^6/7! + ...
// vpv-lab2: ���� 2020:: nvnulstu@gmail.com 
//**************************************************************************************
#pragma once
#include "stdlib.h"
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

#define LEN_RES 19  // ����������� ���������� �������, � ������� ���� �������� ����� ���� ������
#define MAX_ERR (float)(1.0/(1 << LEN_RES)) // �������� ����������� ����� 2^(-LEN_RES)
#define LEN_POLINOM 30 // ���������� ����� 5 ������ ���� 
#define X_STEP ((float)1.0/(float)(1 << 24)) // 2^(-24) - ��� �������� ������� �������� float

// �������� ����������� 
#define FACT3 (float)(2 * 3)
#define FACT5 (float)(2 * 3 * 4 * 5)
#define FACT7 (float)(2 * 3 * 4 * 5 * 6 * 7)
#define FACT9 (float)(2 * 3 * 4 * 5 * 6 * 7 * 8 * 9)

#define F0 (float)(2 * 0 + 1)
#define F1 (float)(2 * 1 + 1)
#define F2 (float)(2 * 2 + 1)
#define F3 (float)(2 * 3 + 1)
#define F4 (float)(2 * 4 + 1)
#define F5 (float)(2 * 5 + 1)
#define F6 (float)(2 * 6 + 1)
#define F7 (float)(2 * 7 + 1)
#define F8 (float)(2 * 8 + 1)
#define F9 (float)(2 * 9 + 1)
#define F10 (float)(2 * 10 + 1)
#define F11 (float)(2 * 11 + 1)
#define F12 (float)(2 * 12 + 1)
#define F13 (float)(2 * 13 + 1)
#define F14 (float)(2 * 14 + 1)
#define F15 (float)(2 * 15 + 1)
#define F16 (float)(2 * 16 + 1)
#define F17 (float)(2 * 17 + 1)
#define F18 (float)(2 * 18 + 1)
#define F19 (float)(2 * 19 + 1)
#define F20 (float)(2 * 20 + 1)
#define F21 (float)(2 * 21 + 1)
#define F22 (float)(2 * 22 + 1)
#define F23 (float)(2 * 23 + 1)
#define F24 (float)(2 * 24 + 1)
#define F25 (float)(2 * 25 + 1)
#define F26 (float)(2 * 26 + 1)
#define F27 (float)(2 * 27 + 1)
#define F28 (float)(2 * 28 + 1)
#define F29 (float)(2 * 29 + 1)
#define F30 (float)(2 * 30 + 1)

// �������� �������� ������� ����������� (float)
#define DIV1_FACT3 (1./FACT3)
#define DIV1_FACT5 (1./FACT5)
#define DIV1_FACT7 (1./FACT7)
#define DIV1_FACT9 (1./FACT9)

#define DIV1_F0 (1./F0)
#define DIV1_F1 (1./F1)
#define DIV1_F2 (1./F2)
#define DIV1_F3 (1./F3)
#define DIV1_F4 (1./F4)
#define DIV1_F5 (1./F5)
#define DIV1_F6 (1./F6)
#define DIV1_F7 (1./F7)
#define DIV1_F8 (1./F8)
#define DIV1_F9 (1./F9)
#define DIV1_F10 (1./F10)
#define DIV1_F11 (1./F11)
#define DIV1_F12 (1./F12)
#define DIV1_F13 (1./F13)
#define DIV1_F14 (1./F14)
#define DIV1_F15 (1./F15)
#define DIV1_F16 (1./F16)
#define DIV1_F17 (1./F17)
#define DIV1_F18 (1./F18)
#define DIV1_F19 (1./F19)
#define DIV1_F20 (1./F20)
#define DIV1_F21 (1./F21)
#define DIV1_F22 (1./F22)
#define DIV1_F23 (1./F23)
#define DIV1_F24 (1./F24)
#define DIV1_F25 (1./F25)
#define DIV1_F26 (1./F26)
#define DIV1_F27 (1./F27)
#define DIV1_F28 (1./F28)
#define DIV1_F29 (1./F29)
#define DIV1_F30 (1./F30)

typedef float(*FloatFunc)(float); // ��������� �� ������� ������������ ��������

typedef long FixPoint;

// � ������� ����� �������������� ����� 30 ���
#define FRACT_PART 27
// 2^31 - ��������� ��� ��������� 31 ���� �������� � long, ����������� ����� � ������������� ������  
#define FACTOR (1 << FRACT_PART) 
#define FLOAT2FIX(x) (FixPoint)(x * FACTOR) // �������������� float � Fixed
#define FIX2FLOAT(x) ((float)x / FACTOR)   // �������������� Fixed �� float
#define FIXMUL(x,y) ((FixPoint)((__int64)x * (__int64)y >> FRACT_PART))     // ��������� ������������� � �++ ������� ������� ��������
// �������� �������� ������� ����������� (FixPointt)
#define DIV1_FACT1FP (FixPoint)FLOAT2FIX(1.0) // 1 � ������� FixPoint
#define MAXXFP (FixPoint)FLOAT2FIX(0.973695)
#define M1FP (FixPoint)FLOAT2FIX(-1.0) // 1 � ������� FixPoint
#define TWOFP (FixPoint)FLOAT2FIX(2.0) // 2 � ������� FixPoint
#define DIV1_FACT3FP (FixPoint)FLOAT2FIX(DIV1_FACT3)
#define DIV1_FACT5FP (FixPoint)FLOAT2FIX(DIV1_FACT5)
#define DIV1_FACT7FP (FixPoint)FLOAT2FIX(DIV1_FACT7)
#define DIV1_FACT9FP (FixPoint)FLOAT2FIX(DIV1_FACT9)

#define DIV1_F0FP (FixPoint)FLOAT2FIX(DIV1_F0)
#define DIV1_F1FP (FixPoint)FLOAT2FIX(DIV1_F1)
#define DIV1_F2FP (FixPoint)FLOAT2FIX(DIV1_F2)
#define DIV1_F3FP (FixPoint)FLOAT2FIX(DIV1_F3)
#define DIV1_F4FP (FixPoint)FLOAT2FIX(DIV1_F4)
#define DIV1_F5FP (FixPoint)FLOAT2FIX(DIV1_F5)
#define DIV1_F6FP (FixPoint)FLOAT2FIX(DIV1_F6)
#define DIV1_F7FP (FixPoint)FLOAT2FIX(DIV1_F7)
#define DIV1_F8FP (FixPoint)FLOAT2FIX(DIV1_F8)
#define DIV1_F9FP (FixPoint)FLOAT2FIX(DIV1_F9)
#define DIV1_F10FP (FixPoint)FLOAT2FIX(DIV1_F10)
#define DIV1_F11FP (FixPoint)FLOAT2FIX(DIV1_F11)
#define DIV1_F12FP (FixPoint)FLOAT2FIX(DIV1_F12)
#define DIV1_F13FP (FixPoint)FLOAT2FIX(DIV1_F13)
#define DIV1_F14FP (FixPoint)FLOAT2FIX(DIV1_F14)
#define DIV1_F15FP (FixPoint)FLOAT2FIX(DIV1_F15)
#define DIV1_F16FP (FixPoint)FLOAT2FIX(DIV1_F16)
#define DIV1_F17FP (FixPoint)FLOAT2FIX(DIV1_F17)
#define DIV1_F18FP (FixPoint)FLOAT2FIX(DIV1_F18)
#define DIV1_F19FP (FixPoint)FLOAT2FIX(DIV1_F19)
#define DIV1_F20FP (FixPoint)FLOAT2FIX(DIV1_F20)
#define DIV1_F21FP (FixPoint)FLOAT2FIX(DIV1_F21)
#define DIV1_F22FP (FixPoint)FLOAT2FIX(DIV1_F22)
#define DIV1_F23FP (FixPoint)FLOAT2FIX(DIV1_F23)
#define DIV1_F24FP (FixPoint)FLOAT2FIX(DIV1_F24)
#define DIV1_F25FP (FixPoint)FLOAT2FIX(DIV1_F25)
#define DIV1_F26FP (FixPoint)FLOAT2FIX(DIV1_F26)
#define DIV1_F27FP (FixPoint)FLOAT2FIX(DIV1_F27)
#define DIV1_F28FP (FixPoint)FLOAT2FIX(DIV1_F28)
#define DIV1_F29FP (FixPoint)FLOAT2FIX(DIV1_F29)
#define DIV1_F30FP (FixPoint)FLOAT2FIX(DIV1_F30)

#define TAB "\t"

// ��������� ������� � ������ �� ����� "������ - ������ TSC"
#define CPUID_RDTSC(t) \
	__asm xor eax, eax\
	__asm cpuid __asm rdtsc\
	__asm mov DWORD PTR[t], eax\
	__asm mov DWORD PTR[t + 4], edx

typedef FixPoint(*FixedFunc)(FixPoint); // ��������� �� ������� ������������ ����������

vector<float> arrX; // ������ ���������� ��� ����� ���������

float flMathFunc(float);
// ��������� ����� ��������� � ���������� �������������� �������
struct Result {
	string shortname;	// �������� ��� ������ ������ ������
	string name;		// ������������ ������ �������
	__int64 min, max;	// ������� � �������� ����� ������ ������ ������� (�� ����������)
	double avg, dev;	// ������� � ���
	double factor;		// ��������� avg ������� ������ � avg ������ �������� ������
	Result(string sn, string nm, __int64 minim, __int64 maxim, double average, double deviation) :
		shortname(sn), name(nm), min(minim), max(maxim), avg(average), dev(deviation) {};
};

class Report; // �����

struct Config {
	int count;			// ����� ������� � ����� �����
	int delMax, delMin;	// ����� ��������� ������������ � ����������� ��������� ��� ���������� ����������� ����� ���������
	int lenPrintLog;	// ����� ������ ����������� ����� ���������, ��������� �� �������
	int pass;			// ����� �������� ��� ������� �������������
	Report * rep;		// ��������� �� �����
	float maxErr;		// ����������� ���������� �����������
	float xStepVerify;	// ��� ������ �������� ��������� ��� ����������� float
	__int64 overhead;	// ������ ��������� �������� �� ��������� ������� � ������
	Config(int cnt, int maxd, int mind, int nPrint, int npass, Report * report, float err, float step, __int64 over)
		  :	count(cnt), delMax(maxd), delMin(mind), lenPrintLog(nPrint), pass(npass), rep(report), maxErr(err), 
			xStepVerify(step), overhead(over) {};
};