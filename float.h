//**************************************************************************************
// Пример исследования реализации функции sin(x)/x = 1 - x^2/3! + x^4/5! - x^6/7! + x^8/9!...
// sinx_divx: март 2020:: nvnulstu@gmail.com 
//**************************************************************************************
#pragma once
#include "vpv-lab2.h"

// массив коэффициентов
float flCoef[LEN_POLINOM] = {DIV1_F0, DIV1_F1, DIV1_F2, DIV1_F3, DIV1_F4, DIV1_F5, DIV1_F6,
                             DIV1_F7, DIV1_F8, DIV1_F9, DIV1_F10, DIV1_F11, DIV1_F12, DIV1_F13, DIV1_F14, DIV1_F15,
                             DIV1_F16, DIV1_F17, DIV1_F18, DIV1_F19, DIV1_F20, DIV1_F21, DIV1_F22, DIV1_F23, DIV1_F24,
                             DIV1_F25, DIV1_F26, DIV1_F27, DIV1_F28, DIV1_F29,};

// Факториал для int - не может иметь n > 12
int factorial(int n) {
	int fact = n;
	while (--n > 0) {
		fact *= n;
	}
	return fact;
}

// Библиотечная реализация функции на основе библиотеки math.h для измерения
float flMathFunc(float x) {
	return (float)log((1 + x) / (1 - x));;
}

// Цикл формулы ряда 1 - x^2/3! + x^4/5! - x^6/7! + x^8/9! через цикл sum += x^2n/(2n+1)!
float flCyNoGorn(float x) {
	float sum = 0;
	for (int n = 0; n < LEN_POLINOM; n++) {
        sum += (float)(pow(x, 2 * n + 1) / (2 * n + 1)) * 2;
	}
	return sum;
}

// Непосредственная реализация формулы 1 - x^2/3! + x^4/5! - x^6/7! + x^8/9!
float flNoCyNoGorn(float x) {
    return (x + powf(x, 3) / 3 + powf(x, 5) / 5 + powf(x, 7) / 7 + powf(x, 9) / 9 +
            powf(x, 11) / 11 + powf(x, 13) / 13 + powf(x, 15) / 15 + powf(x, 17) / 17 +
            powf(x, 19) / 19 + powf(x, 21) / 21 + powf(x, 23) / 23 + powf(x, 25) / 25 +
            powf(x, 27) / 27 + powf(x, 29) / 29 + powf(x, 31) / 31 + powf(x, 33) / 33 +
            powf(x, 35) / 35 + powf(x, 37) / 37 + powf(x, 39) / 39 + powf(x, 41) / 41 +
            powf(x, 43) / 43 + powf(x, 45) / 45 + powf(x, 47) / 47 + powf(x, 49) / 49 +
            powf(x, 51) / 51 + powf(x, 53) / 53 + powf(x, 55) / 55 + powf(x, 57) / 57 +
            powf(x, 59) / 59 + powf(x, 61) / 61 + powf(x, 63) / 63) * 2;
}
// Цикл схемы Горнера
float flCycleGorn(float x) {
	float x2 = x * x, sum = flCoef[LEN_POLINOM - 1];
	for (int n = LEN_POLINOM-1; n > 0; n--)
		sum = sum * x2 + flCoef[n-1];
	sum = sum * x;
	return sum*2;
}


//qwe = 'flCoef[29]'
//for i in range(28, -1, -1) :
//	qwe = f"({qwe} * x2 + flCoef[{i}])"
//	print(qwe + " * x * 2")

// Бесцикловая схема Горнера классическая sin(x)/x = (((a[4]*x^2 + a[3])*x^2 + a[2])*x^2 + a[1])*x^2 + a[0]
float flNoCyGornArr(float x) {
	float x2 = x * x; // за скобки выносится x^2
    return (((((((((((((((((((((((((((((flCoef[29] * x2 + flCoef[28]) * x2 + flCoef[27]) * x2 + flCoef[26]) * x2 +
                                     flCoef[25]) * x2 + flCoef[24]) * x2 + flCoef[23]) * x2 + flCoef[22]) * x2 +
                                 flCoef[21]) * x2 + flCoef[20]) * x2 + flCoef[19]) * x2 + flCoef[18]) * x2 +
                             flCoef[17]) * x2 + flCoef[16]) * x2 + flCoef[15]) * x2 + flCoef[14]) * x2 + flCoef[13]) *
                        x2 + flCoef[12]) * x2 + flCoef[11]) * x2 + flCoef[10]) * x2 + flCoef[9]) * x2 + flCoef[8]) *
                   x2 + flCoef[7]) * x2 + flCoef[6]) * x2 + flCoef[5]) * x2 + flCoef[4]) * x2 + flCoef[3]) * x2 +
              flCoef[2]) * x2 + flCoef[1]) * x2 + flCoef[0]) * x * 2;
}

// Бесцикловая схема Горнера - константы вместо элементов массива
float flNoCyGornConst(float x) {
	float x2 = x * x;
    return (((((((((((((((((((((((((((((DIV1_F29 * x2 + DIV1_F28) * x2 + DIV1_F27) * x2 + DIV1_F26) * x2 + DIV1_F25) *
                                    x2 + DIV1_F24) * x2 + DIV1_F23) * x2 + DIV1_F22) * x2 + DIV1_F21) * x2 + DIV1_F20) *
                               x2 + DIV1_F19) * x2 + DIV1_F18) * x2 + DIV1_F17) * x2 + DIV1_F16) * x2 + DIV1_F15) * x2 +
                          DIV1_F14) * x2 + DIV1_F13) * x2 + DIV1_F12) * x2 + DIV1_F11) * x2 + DIV1_F10) * x2 +
                     DIV1_F9) * x2 + DIV1_F8) * x2 + DIV1_F7) * x2 + DIV1_F6) * x2 + DIV1_F5) * x2 + DIV1_F4) * x2 +
               DIV1_F3) * x2 + DIV1_F2) * x2 + DIV1_F1) * x2 + DIV1_F0) * x * 2;
}

