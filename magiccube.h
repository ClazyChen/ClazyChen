#ifndef _MAGICCUBE_H
#define _MAGICCUBE_H

#include "universal.h"
#include "surface.h"
#include <vector>
#include <string>

class MagicCube {

public:
	MagicCube();
	bool operator == (const MagicCube &B) const{
		if (front != B.front)
			return false;
		if (back != B.back)
			return false;
		if (left != B.left)
			return false;
		if (right != B.right)
			return false;
		if (up != B.up)
			return false;
		if (down != B.down)
			return false;
		return true;
	}
	void readStatus();
	void readFrom6_3_3(int c[6][3][3]);
	void clearPath();
	void addPath(std::string s);
	int getSteps();
	void leftRoll();
	void rightRoll();
	void frontRoll();
	void backRoll();
	void leftRotate();
	void rightRotate();
	void L();
	void LL();
	void R();
	void RR();
	void U();
	void UU();
	void D();
	void DD();
	void F();
	void FF();
	void B();
	void BB();
	void rotate(std::string s);
	void randomRotate();
	void printPath();
	void printVisualStatus();
	void setUp(char c);
	void whiteFlower();
	bool checkWhiteFlower();
	void whiteSurface();
	bool checkAllWhite();
	bool checkWhiteSurface();
	void firstTwoLayers();
	bool checkFirstTwoLayers();
	void yellowFlower();
	bool checkYellowFlower();
	void yellowSurface();
	bool checkYellowSurface();
	void noNameStep();
	void recover();
	bool checkRecover();
	std::vector<std::string> path;
	void solve();
	void getPathWithoutRoll();
	void getReducedPath();
	Surface front, back, left, right, up, down;


private:
		std::vector<int> permutation;
};
#endif
