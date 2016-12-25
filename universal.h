#ifndef _UNIVERSAL_H
#define _UNIVERSAL_H

#include <string>
#include <algorithm>
#include <map>

static const int N = 3;
static const int M = 12;
static const int WHITE   = 0;
static const int BLUE    = 1;
static const int ORANGE  = 2;
static const int RED     = 3;
static const int GREEN   = 4;
static const int YELLOW  = 5;

static const int BLACK   = 9;

const std::string rotate[M] = {"L", "LL", "R", "RR", "F", "FF", "B", "BB", "U", "UU", "D", "DD"};

typedef std::map<std::string, std::string> stringMap;

std::string numberToColor(int c);

char numberToCol(int c);

int colorToNumber(char c);

int getRandom();

bool checkSame(int a, int b, int c, int d);

bool checkIntersect(int a, int b, int c, int d);

stringMap initial();

stringMap LRollMap();
stringMap RRollMap();
stringMap BRollMap();
stringMap FRollMap();
stringMap LRotMap();
stringMap RRotMap();

stringMap add(stringMap a, stringMap b);

#endif
