#ifndef _SURFACE_H
#define _SURFACE_H

#include "universal.h"

class Surface {
public:
	int get(int i, int j);
	void set(int i, int j, int x);
	void fill(int x);
	void readFrom3_3(int c[3][3]);
	void leftRotate();
	void rightRotate();
	void print();
	void samplePrint();
	void read();
	bool checkV();
	bool operator == (const Surface &B) const {
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				if (c[i][j] != B.c[i][j])
					return false;
		return true;
	}
	bool operator != (const Surface &B) const {
		return !(*this == B);
	}
private:
	int c[N][N];
};

void circleTransition(Surface &s0, Surface &s1, Surface &s2, Surface &s3, int x[4][3], int y[4][3]); 
#endif
