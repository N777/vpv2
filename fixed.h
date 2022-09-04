//**************************************************************************************
// Пример исследования реализации функции sin(x)/x = 1 - x^2/3! + x^4/5! - x^6/7! + ...
// sinx_divx: март 2020:: nvnulstu@gmail.com 
//**************************************************************************************
#pragma once

#include "vpv-lab2.h"
#include "string.h"
#include <sstream>

using namespace std;

//#define DEBUG // раскомментировать, если нужно бороться с ошибкми бесцикловой реализация схемы Горнера

FixPoint fixCoef[LEN_POLINOM] = {DIV1_F0FP, DIV1_F1FP, DIV1_F2FP, DIV1_F3FP, DIV1_F4FP, DIV1_F5FP, DIV1_F6FP, DIV1_F7FP,
                                 DIV1_F8FP, DIV1_F9FP, DIV1_F10FP, DIV1_F11FP, DIV1_F12FP, DIV1_F13FP, DIV1_F14FP,
                                 DIV1_F15FP, DIV1_F16FP, DIV1_F17FP, DIV1_F18FP, DIV1_F19FP, DIV1_F20FP, DIV1_F21FP,
                                 DIV1_F22FP, DIV1_F23FP, DIV1_F24FP, DIV1_F25FP, DIV1_F26FP, DIV1_F27FP, DIV1_F28FP,
                                 DIV1_F29FP,};

void printHex(FixPoint x) {
	cout << hex << uppercase << setfill('0') << setw(8) << x << endl;
}
// Формирование строки двочиного представления FixPoint 32 бита с lenFP разрядами дробной части
string fixPointToStr(FixPoint fx, int lenFP) {
	string str = "";
	for (int count = 0; count < 32; count++) {
		if (count == lenFP)
			str.insert(0, 1, '.');
		str.insert(0, 1, (fx & 1) ? '1' : '0');
		fx >>= 1;
	}
	return str;
}
// Формат фиксированных для sin(x)/x: знак, один бит целой части, 30 бит дробной части
// т.е  w[31]: знак, w[30]: 1, w[29]: 2^(-1), .. w[0]: 2^(-30)
bool testFixOperations(Config config) {
	char strbuf[34]; // буфер двоичного числа
	// Контрольные числа: 1, 2^(-1), 2^(-2), 2^(-3), 2^(-30), -2^(-1), -2^(-2), -2^(-3), -2^(-30)
	float flo[] = {2., 1., 0.5, 0.25, 0.125, 0, -0.5, -0.25, -0.125, 0 };
	// Те же самые числа, что в массиве flo, представляются в массиве fix в формате FixPoint
	FixPoint fix[] = {
		0x02000000, // 01.000000..0 : 2
		0x01000000, // 00.100000..0 : 1
		0x00800000, // 00.010000..0 : 0.5
		0x00400000, // 00.001000..0 : 0.25
		0x00200000, // 00.001000..0 : 0.125
		0x00000000, // 00.0000..001 : 0
		// для отрицательных код дополнительный
		-0x00800000, // 11.100000..0 : -0.5
		-0x00400000, // 11.110000..0 : -0.25
		-0x00200000, // 11.111000..0 : -0.125
		-0x00000000, // 11.1111..111 : 0
	};
	int sz = sizeof(fix) / sizeof(FixPoint);
	// Контроль FLOAT2FIX путем сопоставления fix[i] и FLOAT2FIX(flo[i]) 
	bool ok1 = true;
	for (int n = 0; n < sz; n++) { 
		FixPoint fx = FLOAT2FIX(flo[n]);
#ifdef DEBUG
		printHex(fx);
#endif
		if ( fx != fix[n]) { // 
			cout	<< "FLOAT2FIX(" << flo[n] << "):" << ": " << fixPointToStr(fx, FRACT_PART)
					<< " != " << fixPointToStr(fix[n], FRACT_PART) << endl;
			ok1 = false;
		}
	}
	if (ok1 && config.lenPrintLog > 0) 
		cout << "FLOAT2FIX(x) - OK" << endl;
	
	// Контроль FIX2FLOAT для каждого fix[i] получить float и сравнивнить с flo[i]
	bool ok2 = true;
	for (int n = 0; n < sz; n++) {
		float fl = FIX2FLOAT(fix[n]);
		if (fl != flo[n]) {
			cout	<< "FIX2FLOAT(" << fixPointToStr(fix[n], FRACT_PART) << "):: " << fl
					<< " != " << flo[n] << endl;
			ok2 = false;
		}
	}
	if (ok2 && config.lenPrintLog > 0) 
		cout << "FIX2FLOAT(x) - OK" << endl;
	// Контроль FIXMUL - всемозможные сочетания элементов fix умножаются и результат считается равным эталоному,
	// если он по модулю не больше X_STEP 
	bool ok3 = true;
	for (int n = 0; n < sz; n++) {
		for (int k = 0; k < sz; k++) {
			FixPoint mfix = FIXMUL(fix[n], fix[k]);
			float etalon = flo[n] * flo[k];
			if (fabs(FIX2FLOAT(mfix) - etalon) > X_STEP) {
				cout	<< "FIXMUL(" << fixPointToStr(fix[n], FRACT_PART) << ", "
						<< fixPointToStr(fix[k], FRACT_PART) << ") = "
						<< fixPointToStr(mfix, FRACT_PART) << " != " << etalon << endl;
				ok3 = false;
			}
		}
	}
	if (ok3 && config.lenPrintLog > 0) 
		cout << "FIXMUL(x, y) - OK" << endl;
	return ok1 && ok2 && ok3;
}

// Цикл схемы Горнера
// Директивы условной трасляции DEBUG вставлены для облегчения отладки бесцикловой реализации
// Когда цикловая реализация отлажена
FixPoint fxCycleGorn(FixPoint x) {
	FixPoint x2 = FIXMUL(x,x), sum = fixCoef[LEN_POLINOM - 1];
#ifdef DEBUG
	// В if должно быть значение, на котором спотыкается проверка бесцикловой реализации
	// Если это случиться, то придется отрессировать бесцикловую реализацию, 
	//	сравнивая значения с теми, что получаются в цикловой
	FixPoint val = 0x30BDF580;
	if (x == val) {
		cout << FIX2FLOAT(x);
		cout << " = x = ";
        printHex(x);
		cout << ":: x2 = ";
		printHex(x2);
		cout << ":: sum("; 
		printHex(x);
		cout << "): ";
	}
#endif
	for (int n = LEN_POLINOM-1; n > 0; n--) {
		sum = FIXMUL(sum, x2) + fixCoef[n - 1];
#ifdef DEBUG
		if (x == val) {
			float fpsum = FIX2FLOAT(sum);
			printHex(sum);
			cout << " " << endl;
		}
#endif
	}
	sum = FIXMUL(sum, x);
#ifdef DEBUG
	if (x == val) {
		float fpsum = FIX2FLOAT(sum);
		printHex(sum);
		cout << " " << fpsum << endl;
	}
#endif
    sum = FIXMUL(sum, TWOFP);
#ifdef DEBUG
	if (x == val) {
		printHex(DIV1_FACT1FP);
		cout << " ";
		float two = FIX2FLOAT(TWOFP);
		printHex(TWOFP);
		cout << " ";
		printHex(sum);
		float fpsum = FIX2FLOAT(sum), flsum = flCycleGorn(FIX2FLOAT(x));
		cout << " " << fpsum << "?= " << flsum << endl;
	}
#endif
	return sum;
}

// Бесцикловая схема Горнера (массив коэффициентов)
FixPoint fxNoCyGornArr(FixPoint x) {
	FixPoint x2 = FIXMUL(x, x); // за скобки выносится x^2
	FixPoint sum = FIXMUL(fixCoef[29], x2) + fixCoef[28];
	sum = FIXMUL(sum, x2) + fixCoef[27];
    sum = FIXMUL(sum, x2) + fixCoef[27];
    sum = FIXMUL(sum, x2) + fixCoef[26];
    sum = FIXMUL(sum, x2) + fixCoef[25];
    sum = FIXMUL(sum, x2) + fixCoef[24];
    sum = FIXMUL(sum, x2) + fixCoef[23];
    sum = FIXMUL(sum, x2) + fixCoef[22];
    sum = FIXMUL(sum, x2) + fixCoef[21];
    sum = FIXMUL(sum, x2) + fixCoef[20];
    sum = FIXMUL(sum, x2) + fixCoef[19];
    sum = FIXMUL(sum, x2) + fixCoef[18];
    sum = FIXMUL(sum, x2) + fixCoef[17];
    sum = FIXMUL(sum, x2) + fixCoef[16];
    sum = FIXMUL(sum, x2) + fixCoef[15];
    sum = FIXMUL(sum, x2) + fixCoef[14];
    sum = FIXMUL(sum, x2) + fixCoef[13];
    sum = FIXMUL(sum, x2) + fixCoef[12];
    sum = FIXMUL(sum, x2) + fixCoef[11];
    sum = FIXMUL(sum, x2) + fixCoef[10];
    sum = FIXMUL(sum, x2) + fixCoef[9];
    sum = FIXMUL(sum, x2) + fixCoef[8];
    sum = FIXMUL(sum, x2) + fixCoef[7];
    sum = FIXMUL(sum, x2) + fixCoef[6];
    sum = FIXMUL(sum, x2) + fixCoef[5];
    sum = FIXMUL(sum, x2) + fixCoef[4];
    sum = FIXMUL(sum, x2) + fixCoef[3];
    sum = FIXMUL(sum, x2) + fixCoef[2];
    sum = FIXMUL(sum, x2) + fixCoef[1];
    sum = FIXMUL(sum, x2) + fixCoef[0];
    sum = FIXMUL(sum, x);
    sum = FIXMUL(sum, TWOFP);
	return sum;
}
FixPoint fxNoCyGornConst(FixPoint x) { // Бесцикловая схема Горнера с константами
	FixPoint x2 = FIXMUL(x, x);
	FixPoint sum = FIXMUL(DIV1_F29FP, x2) + DIV1_F28FP;
    sum = FIXMUL(sum, x2) + DIV1_F27FP;
    sum = FIXMUL(sum, x2) + DIV1_F26FP;
    sum = FIXMUL(sum, x2) + DIV1_F25FP;
    sum = FIXMUL(sum, x2) + DIV1_F24FP;
    sum = FIXMUL(sum, x2) + DIV1_F23FP;
    sum = FIXMUL(sum, x2) + DIV1_F22FP;
    sum = FIXMUL(sum, x2) + DIV1_F21FP;
    sum = FIXMUL(sum, x2) + DIV1_F20FP;
    sum = FIXMUL(sum, x2) + DIV1_F19FP;
    sum = FIXMUL(sum, x2) + DIV1_F18FP;
    sum = FIXMUL(sum, x2) + DIV1_F17FP;
    sum = FIXMUL(sum, x2) + DIV1_F16FP;
    sum = FIXMUL(sum, x2) + DIV1_F15FP;
    sum = FIXMUL(sum, x2) + DIV1_F14FP;
    sum = FIXMUL(sum, x2) + DIV1_F13FP;
    sum = FIXMUL(sum, x2) + DIV1_F12FP;
    sum = FIXMUL(sum, x2) + DIV1_F11FP;
    sum = FIXMUL(sum, x2) + DIV1_F10FP;
    sum = FIXMUL(sum, x2) + DIV1_F9FP;
    sum = FIXMUL(sum, x2) + DIV1_F8FP;
    sum = FIXMUL(sum, x2) + DIV1_F7FP;
    sum = FIXMUL(sum, x2) + DIV1_F6FP;
    sum = FIXMUL(sum, x2) + DIV1_F5FP;
    sum = FIXMUL(sum, x2) + DIV1_F4FP;
    sum = FIXMUL(sum, x2) + DIV1_F3FP;
    sum = FIXMUL(sum, x2) + DIV1_F2FP;
    sum = FIXMUL(sum, x2) + DIV1_F1FP;
    sum = FIXMUL(sum, x2) + DIV1_F0FP;
    sum = FIXMUL(sum, x);
    sum = FIXMUL(sum, TWOFP);
	return sum;
}
FixPoint fxNoCyGornAsm(FixPoint x) { // Бесцикловая схема Горнера asm
    FixPoint test1, test2;
	FixPoint sum;
	if (x == 0x00B50E40)
		sum = 5;
	_asm
	{
		; eax = x * x
		MOV		EAX, x
		MOV		ECX, EAX
		IMUL	ECX
        mov     test2, eax
		MOV		EAX, EDX
        mov     test1, eax
		;SAL		EAX, 2
        mov     test1, eax
		; ecx = x2 = x * 2
		MOV		ECX, EAX
		; EAX = a[4] * x2 + a[3]
		MOV		EAX, 000456C7h; DIV1_FACT9FP
		IMUL	ECX
		MOV		EAX, EDX
        MOV		sum,	EAX
		;SAL		EAX, 2
        ADD		EAX, 00047DC1h; -DIV1_FACT7FP

		; EAX = a[4] * x2
        mov test2, ECX
        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 0004A790H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 0004D487H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00050505H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00053978H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00057262H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 0005B05BH

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 0005F417H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00063E70H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00069069H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 0006EB3EH

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00075075H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 0007C1F0H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00084210H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 0008D3DCH

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00097B42H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 000A3D70H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 000B2164H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 000C30C3H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 000D7943H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 000F0F0FH

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00111111H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 0013B13BH

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 001745D1H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 001C71C7H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00249249H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00333333H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 00555555H

        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        ADD		EAX, 01000000H


        MOV     ECX, x
        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2

        MOV     ECX, 2
        IMUL	ECX
        MOV		EAX, EDX
        ;SAL		EAX, 2
        
        MOV		sum,	EAX
	}
	return sum;
}

