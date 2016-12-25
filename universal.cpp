#include "universal.h"
#include <string>
#include <cstdlib>
#include <ctime>

std::string numberToColor(int c) {
	if (c == 0)
		return "WHITE ";
	if (c == 1)
		return "BLUE  ";
	if (c == 2)
		return "ORANGE";
	if (c == 3)
		return "RED   ";
	if (c == 4)
		return "GREEN ";
	if (c == 5)
		return "YELLOW";
	return "Wrong color number!";
}

char numberToCol(int c) {
	if (c == 0)
		return 'W';
	if (c == 1)
		return 'B';
	if (c == 2)
		return 'O';
	if (c == 3)
		return 'R';
	if (c == 4)
		return 'G';
	if (c == 5)
		return 'Y';
	return 'Q';
}

int colorToNumber(char c) {
	if (c == 'W')
		return 0;
	if (c == 'B')
		return 1;
	if (c == 'O')
		return 2;
	if (c == 'R')
		return 3;
	if (c == 'G')
		return 4;
	if (c == 'Y')
		return 5;
	return -1;
}

int getRandom() {
	return rand() % 18;
}

bool checkSame(int a, int b, int c, int d) {
	//check if {a, b} = {c, d}
	if (a > b)
		std::swap(a, b);
	if (c > d)
		std::swap(c, d);
	return a == c && b == d;
}

bool checkIntersect(int a, int b, int c, int d) {
	return a == c || a == d || b == c || b == d;
}

stringMap initial() {
	stringMap a;
	for (int i = 0; i < M; ++i)
		a[rotate[i]] = rotate[i];
	return a;
}

stringMap LRollMap() {
//L->D,D->R,R->U,U->L
	stringMap a = initial();
	a["D"] = "L";
	a["DD"] = "LL";
	a["R"] = "D";
	a["RR"] = "DD";
	a["U"] = "R";
	a["UU"] = "RR";
	a["L"] = "U";
	a["LL"] = "UU";
	return a;
}

stringMap RRollMap() {
//L->U,U->R,R->D,D->L
	stringMap a = initial();
	a["U"] = "L";
	a["UU"] = "LL";
	a["R"] = "U";
	a["RR"] = "UU";
	a["D"] = "R";
	a["DD"] = "RR";
	a["L"] = "D";
	a["LL"] = "DD";
	return a;
}

stringMap FRollMap() {
//U->F,F->D,D->B,B->U
	stringMap a = initial();
	a["F"] = "U";
	a["FF"] = "UU";
	a["D"] = "F";
	a["DD"] = "FF";
	a["B"] = "D";
	a["BB"] = "DD";
	a["U"] = "B";
	a["UU"] = "BB";
	return a;
}

stringMap BRollMap() {
//U->B,B->D,D->F,F->U
	stringMap a = initial();
	a["B"] = "U";
	a["BB"] = "UU";
	a["D"] = "B";
	a["DD"] = "BB";
	a["F"] = "D";
	a["FF"] = "DD";
	a["U"] = "F";
	a["UU"] = "FF";
	return a;
}
stringMap LRotMap() {
//F->R,R->B,B->L,L->F
	stringMap a = initial();
	a["R"] = "F";
	a["RR"] = "FF";
	a["B"] = "R";
	a["BB"] = "RR";
	a["L"] = "B";
	a["LL"] = "BB";
	a["F"] = "L";
	a["FF"] = "LL";
	return a;
}

stringMap RRotMap() {
//F->L,L->B,B->R,R->F
	stringMap a = initial();
	a["L"] = "F";
	a["LL"] = "FF";
	a["B"] = "L";
	a["BB"] = "LL";
	a["R"] = "B";
	a["RR"] = "BB";
	a["F"] = "R";
	a["FF"] = "RR";
	return a;
}

stringMap add(stringMap a, stringMap b) {
	stringMap c;
	for (stringMap::iterator it = b.begin(); it != b.end(); ++it)
		c[it->first] = a[it->second];
	return c;
}

