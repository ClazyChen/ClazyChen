#include "magiccube.h"
#include "universal.h"
#include "surface.h"
#include <cstdio>
#include <vector>
#include <string>
#include <iostream>

MagicCube::MagicCube() {
	path.clear();
	up.fill(RED);
	down.fill(ORANGE);
	front.fill(YELLOW);
	back.fill(WHITE);
	left.fill(GREEN);
	right.fill(BLUE);
}

void MagicCube::readStatus() {
	path.clear();
	front.read();
	back.read();
	left.read();
	right.read();
	up.read();
	down.read();
}

void MagicCube::readFrom6_3_3(int c[6][3][3]) {
    path.clear();
	for (int i = 0; i < 6; ++i) {
		if (c[i][1][1] == YELLOW)
			front.readFrom3_3(c[i]);
		if (c[i][1][1] == WHITE)
			back.readFrom3_3(c[i]);
		if (c[i][1][1] == GREEN)
			left.readFrom3_3(c[i]);
		if (c[i][1][1] == BLUE)
			right.readFrom3_3(c[i]);
		if (c[i][1][1] == RED)
			up.readFrom3_3(c[i]);
		if (c[i][1][1] == ORANGE)
			down.readFrom3_3(c[i]);
	}
}

void MagicCube::clearPath() {
	path.clear();
}

void MagicCube::addPath(std::string s) {
	path.push_back(s);
}

int MagicCube::getSteps() {
	return path.size();
}

void MagicCube::leftRoll() {
	static Surface temp;
	temp = up;
	up = right;
	right = down;
	down = left;
	left = temp;
	up.leftRotate();
	left.leftRotate();
	down.leftRotate();
	right.leftRotate();
	front.leftRotate();
	back.rightRotate();
	addPath(std::string("LRoll"));
}

void MagicCube::rightRoll() {
	static Surface temp;
	temp = up;
	up = left;
	left = down;
	down = right;
	right = temp;
	up.rightRotate();
	right.rightRotate();
	down.rightRotate();
	left.rightRotate();
	front.rightRotate();
	back.leftRotate();
	addPath(std::string("RRoll"));
}

void MagicCube::frontRoll() {
	static Surface temp;
	temp = up;
	up = back;
	up.rightRotate(), up.rightRotate();
	back = down;
	back.rightRotate(), back.rightRotate();
	down = front;
	front = temp;
	left.rightRotate();
	right.leftRotate();
	addPath(std::string("FRoll"));
}

void MagicCube::backRoll() {
	static Surface temp;
	temp = up;
	up = front;
	front = down;
	down = back;
	down.rightRotate(), down.rightRotate();
	back = temp;
	back.rightRotate(), back.rightRotate();
	left.leftRotate();
	right.rightRotate();
	addPath(std::string("BRoll"));
}

void MagicCube::leftRotate() {
	static Surface temp;
	temp = front;
	front = left;
	left = back;
	back = right;
	right = temp;
	up.leftRotate();
	down.rightRotate();
	addPath(std::string("LRot"));
}

void MagicCube::rightRotate() {
	static Surface temp;
	temp = front;
	front = right;
	right = back;
	back = left;
	left = temp;
	up.rightRotate();
	down.leftRotate();
	addPath(std::string("RRot"));
}

void MagicCube::L() {
	static int x[4][3] = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {2, 1, 0}};
	static int y[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2, 2, 2}};
	circleTransition(up, front, down, back, x, y);
	left.rightRotate();	
	addPath(std::string("L"));
}

void MagicCube::LL() {
	static int x[4][3] = {{2, 1, 0}, {0, 1, 2}, {2, 1, 0}, {2, 1, 0}};
	static int y[4][3] = {{0, 0, 0}, {2, 2, 2}, {0, 0, 0}, {0, 0, 0}};
	circleTransition(up, back, down, front, x, y);
	left.leftRotate();
	addPath(std::string("LL"));
}

void MagicCube::R() {
	static int x[4][3] = {{2, 1, 0}, {0, 1, 2}, {2, 1, 0}, {2, 1, 0}};
	static int y[4][3] = {{2, 2, 2}, {0, 0, 0}, {2, 2, 2}, {2, 2, 2}};
	circleTransition(up, back, down, front, x, y);
	right.rightRotate();
	addPath(std::string("R"));
}

void MagicCube::RR() {
	static int x[4][3] = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {2, 1, 0}};
	static int y[4][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}, {0, 0, 0}};
	circleTransition(up, front, down, back, x, y);
	right.leftRotate();
	addPath(std::string("RR"));
}

void MagicCube::U() {
	static int x[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	static int y[4][3] = {{2, 1, 0}, {2, 1, 0}, {2, 1, 0}, {2, 1, 0}};
	circleTransition(back, right, front, left, x, y);
	up.rightRotate();
	addPath(std::string("U"));
}

void MagicCube::UU() {
	
	static int x[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	static int y[4][3] = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2}};
	circleTransition(back, left, front, right, x, y);
	up.leftRotate();
	addPath(std::string("UU"));
}

void MagicCube::D() {
	
	static int x[4][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}, {2, 2, 2}};
	static int y[4][3] = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2}};
	circleTransition(front, right, back, left, x, y);
	down.rightRotate();
	addPath(std::string("D"));
}

void MagicCube::DD() {

	static int x[4][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}, {2, 2, 2}};
	static int y[4][3] = {{2, 1, 0}, {2, 1, 0}, {2, 1, 0}, {2, 1, 0}};
	circleTransition(front, left, back, right, x, y);
	down.leftRotate();
	addPath(std::string("DD"));
}

void MagicCube::F() {
	static int x[4][3] = {{2, 2, 2}, {0, 1, 2}, {0, 0, 0}, {2, 1, 0}};
	static int y[4][3] = {{0, 1, 2}, {0, 0, 0}, {2, 1, 0}, {2, 2, 2}};
	circleTransition(up, right, down, left, x, y);
	front.rightRotate();
	addPath(std::string("F"));
}

void MagicCube::FF() {
	static int x[4][3] = {{2, 2, 2}, {0, 1, 2}, {0, 0, 0}, {2, 1, 0}};
	static int y[4][3] = {{2, 1, 0}, {2, 2, 2}, {0, 1, 2}, {0, 0, 0}};
	circleTransition(up, left, down, right, x, y);
	front.leftRotate();
	addPath(std::string("FF"));
}

void MagicCube::B() {
	static int x[4][3] = {{0, 0, 0}, {0, 1, 2}, {2, 2, 2}, {2, 1, 0}};
	static int y[4][3] = {{2, 1, 0}, {0, 0, 0}, {0, 1, 2}, {2, 2, 2}};
	circleTransition(up, left, down, right, x, y);
	back.rightRotate();
	addPath(std::string("B"));
}

void MagicCube::BB() {
	static int x[4][3] = {{0, 0, 0}, {0, 1, 2}, {2, 2, 2}, {2, 1, 0}};
	static int y[4][3] = {{0, 1, 2}, {2, 2, 2}, {2, 1, 0}, {0, 0, 0}};
	circleTransition(up, right, down, left, x, y);
	back.leftRotate();
	addPath(std::string("BB"));
}

void MagicCube::rotate(std::string s) {
	if (s == "L")
		L();
	if (s == "LL")
		LL();
	if (s == "R")
		R();
	if (s == "RR")
		RR();
	if (s == "F")
		F();
	if (s == "FF")
		FF();
	if (s == "B")
		B();
	if (s == "BB")
		BB();
	if (s == "U")
		U();
	if (s == "UU")
		UU();
	if (s == "D")
		D();
	if (s == "DD")
		DD();
}


void MagicCube::randomRotate() {
	int r = getRandom();
	if (r == 0)
		L();
	if (r == 1)
		LL();
	if (r == 2)
		R();
	if (r == 3)
		RR();
	if (r == 4)
		U();
	if (r == 5)
		UU();
	if (r == 6)
		D();
	if (r == 7)
		DD();
	if (r == 8)
		F();
	if (r == 9)
		FF();
	if (r == 10)
		B();
	if (r == 11)
		BB();
	if (r == 12)
		leftRoll();
	if (r == 13)
		rightRoll();
	if (r == 14)
		backRoll();
	if (r == 15)
		frontRoll();
	if (r == 16)
		leftRotate();
	if (r == 17)
		rightRotate();
}


void MagicCube::printPath() {
	printf("Total %d operates.\n", (int)path.size());
	for (int i = 0; i < (int)path.size(); ++i)
		std::cout << path[i] << " ";
	std::cout << std::endl;
}

void MagicCube::printVisualStatus() {
	static char s[12][9];
	static int x, y, i, j;
	for (i = 0; i < 12; ++i)
		for (j = 0; j < 9; ++j)
			s[i][j] = ' ';
	for (x = 0, i = 2; i >= 0; --i, ++x)
		for (y = 3, j = 2; j >= 0; --j, ++y)
			s[x][y] = numberToCol(back.get(i, j));
	for (x = 3, j = 0; j < 3; ++j, ++x)
		for (y = 0, i = 2; i >= 0; --i,  ++y)
			s[x][y] = numberToCol(left.get(i, j));
	for (x = 3, i = 0; i < 3; ++i, ++x)
		for (y = 3, j = 0; j < 3; ++j, ++y)
			s[x][y] = numberToCol(up.get(i, j)); 
	for (x = 3, j = 2; j >= 0; --j, ++x)
		for (y = 6, i = 0; i < 3; ++i,  ++y)
			s[x][y] = numberToCol(right.get(i, j)); 
	for (x = 6, i = 0; i < 3; ++i, ++x)
		for (y = 3, j = 0; j < 3; ++j, ++y)
			s[x][y] = numberToCol(front.get(i, j)); 
	for (x = 9, i = 0; i < 3; ++i, ++x)
		for (y = 3, j = 0; j < 3; ++j, ++y)
			s[x][y] = numberToCol(down.get(i, j)); 
	for (i = 0; i < 12; ++i) {
		for (j = 0; j < 9; ++j)
			putchar(s[i][j]);
		puts("");
	}
}

void MagicCube::setUp(char c) {
	int x = colorToNumber(c);
	if (front.get(1, 1) == x)
		backRoll();
	else if (back.get(1, 1) == x)
		frontRoll();
	else if (left.get(1, 1) == x)
		rightRoll();
	else if (right.get(1, 1) == x)
		leftRoll();
	else if (down.get(1, 1) == x)
		leftRoll(), leftRoll();
}

void MagicCube::whiteFlower() {
	setUp('Y');
	while (!checkWhiteFlower()) {
		if (front.get(1, 0) == WHITE) {
			while (up.get(1, 0) == WHITE)
				U();
			LL();
			continue;
		}
		if (front.get(1, 2) == WHITE) {
			while (up.get(1, 2) == WHITE)
				U();
			R();
			continue;
		}
		if (left.get(1, 0) == WHITE) {
			while (up.get(0, 1) == WHITE)
				U();
			BB();
			continue;
		}
		if (left.get(1, 2) == WHITE) {
			while (up.get(2, 1) == WHITE)
				U();
			F();
			continue;
		}
		if (right.get(1, 0) == WHITE) {
			while (up.get(2, 1) == WHITE)
				U();
			FF();
		}
		if (right.get(1, 2) == WHITE) {
			while (up.get(0, 1) == WHITE)
				U();
			B();
		}
		if (back.get(1, 0) == WHITE) {
			while (up.get(1, 2) == WHITE)
				U();
			RR();
			continue;
		}
		if (back.get(1, 2) == WHITE) {
			while (up.get(1, 0) == WHITE)
				U();
			L();
			continue;
		}
		if (down.get(0, 1) == WHITE) {
			while (up.get(2, 1) == WHITE)
				U();
			F(), F();
			continue;
		}
		if (down.get(1, 0) == WHITE) {
			while (up.get(1, 0) == WHITE)
				U();
			L(), L();
			continue;
		}
		if (down.get(1, 2) == WHITE) {
			while (up.get(1, 2) == WHITE)
				U();
			R(), R();
			continue;
		}
		if (down.get(2, 1) == WHITE) {
			while (up.get(0, 1) == WHITE)
				U();
			B(), B();
			continue;
		}
		if (front.get(0, 1) == WHITE || front.get(2, 1) == WHITE) {
			while (up.get(2, 1) == WHITE)
				U();
			FF();
			continue;
		}
		if (left.get(0, 1) == WHITE || left.get(2, 1) == WHITE) {
			while (up.get(1, 0) == WHITE)
				U();
			LL();
			continue;
		}
		if (right.get(0, 1) == WHITE || right.get(2, 1) == WHITE) {
			while (up.get(1, 2) == WHITE)
				U();
			RR();
			continue;
		}
		if (back.get(0, 1) == WHITE || back.get(2, 1) == WHITE) {
			while (up.get(0, 1) == WHITE)
				U();
			BB();
			continue;
		}
	}
}

bool MagicCube::checkWhiteFlower() {
	if (up.get(1, 1) != YELLOW)
		return false;
	if (up.get(0, 1) != WHITE || up.get(1, 0) != WHITE || up.get(1, 2) != WHITE || up.get(2, 1) != WHITE)
		return false;
	return true;
}

void MagicCube::whiteSurface() {
	while (front.get(0, 1) != front.get(1, 1))
		U(), leftRotate();
	F(), F();
	while (right.get(0, 1) != right.get(1, 1))
		U(), leftRotate();
	R(), R();
	while (back.get(0, 1) != back.get(1, 1))
		U(), leftRotate();
	B(), B();
	while (left.get(0, 1) != left.get(1, 1))
		U(), leftRotate();
	L(), L();
	setUp('W');

	while (!checkWhiteSurface()) {
		if (checkAllWhite()) {
			if (!(front.get(0, 2) == front.get(1, 1) && right.get(0, 0) == right.get(1, 1))) {
				F(), D(), FF();
				continue;
			}
			if (!(front.get(0, 0) == front.get(1, 1) && left.get(0, 2) == left.get(1, 1))) {
				FF(), D(), F();
				continue;
			}
			if (!(right.get(1, 1) == right.get(0, 2) && back.get(1, 1) == back.get(0, 0))) {
				BB(), D(), B();
				continue;
			}
			if (!(left.get(1, 1) == left.get(0, 0) && back.get(1, 1) == back.get(0, 2))) {
				B(), D(), BB();
				continue;
			}
		}
		if (front.get(2, 0) == WHITE) {
			while (!checkSame(front.get(1, 1), left.get(1, 1), left.get(2, 2), down.get(0, 0)))
				DD(), leftRotate();
			FF(), DD(), F(); 
			continue;
		}
		if (front.get(2, 2) == WHITE) {
			while (!checkSame(front.get(1, 1), right.get(1, 1), right.get(2, 0), down.get(0, 2)))
				D(), rightRotate();
			F(), D(), FF();
			continue;
		}
		if (right.get(2, 0) == WHITE || right.get(2, 2) == WHITE) {
			rightRotate();
			continue;
		}
		if (left.get(2, 0) == WHITE || left.get(2, 2) == WHITE) {
			leftRotate();
			continue;
		}
		if (back.get(2, 0) == WHITE || back.get(2, 2) == WHITE) {
			leftRotate(), leftRotate();
			continue;
		}

		if (front.get(0, 0) == WHITE) {
			FF(), DD(), F();
			continue;
		}
		if (front.get(0, 2) == WHITE) {
			F(), D(), FF();
			continue;
		}
		if (right.get(0, 0) == WHITE || right.get(0, 2) == WHITE) {
			rightRotate();
			continue;
		}
		if (left.get(0, 0) == WHITE || left.get(0, 2) == WHITE) {
			leftRotate();
			continue;
		}
		if (back.get(0, 0) == WHITE || back.get(0, 2) == WHITE) {
			leftRotate(), leftRotate();
			continue;
		}

		if (down.get(0, 0) == WHITE) {
			L(), D(), D(), LL();
			continue;
		}
		if (down.get(0, 2) == WHITE) {
			F(), D(), D(), FF();
			continue;
		}
		if (down.get(2, 0) == WHITE) {
			LL(), D(), D(), L();
			continue;
		}
		if (down.get(2, 2) == WHITE) {
			R(), D(), D(), RR();
			continue;
		}
	}
}

bool MagicCube::checkAllWhite() {
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (up.get(i, j) != WHITE)
				return false;
	return true;
}

bool MagicCube::checkWhiteSurface() {
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (up.get(i, j) != WHITE)
				return false;
	for (int j = 0; j < N; ++j)
		if (front.get(0, j) != front.get(1, 1))
			return false;
	for (int j = 0; j < N; ++j)
		if (left.get(0, j) != left.get(1, 1))
			return false;
	for (int j = 0; j < N; ++j)
		if (right.get(0, j) != right.get(1, 1))
			return false;
	for (int j = 0; j < N; ++j)
		if (back.get(0, j) != back.get(1, 1))
			return false;
	return true;
}

void MagicCube::firstTwoLayers() {
	setUp('Y');
	while (!checkFirstTwoLayers()) {
		if (front.get(0, 1) != YELLOW && up.get(2, 1) != YELLOW) {
			while (front.get(0, 1) != front.get(1, 1))
				U(), leftRotate();
			if (up.get(2, 1) == right.get(1, 1)) {
				U(), R(), UU(), RR();
				UU(), FF(), U(), F();
			}
			else {
				UU(), LL(), U(), L();
				U(), F(), UU(), FF();
			}
			continue;
		}
		if (left.get(0, 1) != YELLOW && up.get(1, 0) != YELLOW) {
			leftRotate();
			continue;
		}
		if (right.get(0, 1) != YELLOW && up.get(1, 2) != YELLOW) {
			rightRotate();
			continue;
		}
		if (back.get(0, 1) != YELLOW && up.get(0, 1) != YELLOW) {
			leftRotate(), leftRotate();
			continue;
		}
	
		bool find = 0;
		for (int i = 0; i < 4; ++i, leftRotate()) {
			if (!(front.get(1, 1) == front.get(1, 2) && right.get(1, 1) == right.get(1, 0))) {
				find = 1;
				U(), R(), UU(), RR();
				UU(), FF(), U(), F();
				break;
			}
		}
	}
}

bool MagicCube::checkFirstTwoLayers() {
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (down.get(i, j) != WHITE)
				return false;
	for (int i = 1; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (front.get(i, j) != front.get(1, 1))
				return false;
	for (int i = 1; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (left.get(i, j) != left.get(1, 1))
				return false;
	for (int i = 1; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (right.get(i, j) != right.get(1, 1))
				return false;
	for (int i = 1; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (back.get(i, j) != back.get(1, 1))
				return false;
	return true;
}

void MagicCube::yellowFlower() {
	static bool find;
	while (!checkYellowFlower()) {
		find = 0;
		for (int i = 0; i < 4; ++i, leftRotate()) {
			if (up.get(1, 1) == YELLOW && up.get(1, 2) == YELLOW && up.get(2, 1) == YELLOW)
				find = 1;
			if (up.get(1, 0) == YELLOW && up.get(1, 1) == YELLOW && up.get(1, 2) == YELLOW)
				find = 1;
			if (find)
				break;
		}
		F(), R(), U(), RR(), UU(), FF();
	}
}

bool MagicCube::checkYellowFlower() {
	for (int j = 0; j < N; ++j)
		if (up.get(1, j) != YELLOW)
			return false;
	if (up.get(0, 1) != YELLOW)
		return false;
	if (up.get(2, 1) != YELLOW)
		return false;
	return true;
}

void MagicCube::yellowSurface() {
	if (checkYellowSurface())
		return;
	static int non_yellow;
	non_yellow = 0;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			non_yellow += up.get(i, j) != YELLOW;
	if (non_yellow == 2) {
		while (!(up.get(0, 0) != YELLOW && back.get(0, 2) == YELLOW))
			leftRotate();
		RR(), UU(), R(), UU(), RR(), UU(), UU(), R();
	}
	else if (non_yellow == 4) {
		while (!(up.get(0, 0) != YELLOW && left.get(0, 0) == YELLOW))
			leftRotate();
		RR(), UU(), R(), UU(), RR(), UU(), UU(), R();
	}

	while (!(up.get(0, 0) == YELLOW && up.get(0, 1) == YELLOW && up.get(1, 0) == YELLOW && up.get(1, 1) == YELLOW && up.get(1, 2) == YELLOW && up.get(2, 1) == YELLOW))
		leftRotate();
	if (left.get(0, 2) == YELLOW)
		F(), U(), FF(), U(), F(), U(), U(), FF();
	else
		RR(), UU(), R(), UU(), RR(), UU(), UU(), R();
	if (!checkYellowSurface())
		while (1);
}


bool MagicCube::checkYellowSurface() {
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (up.get(i, j) != YELLOW)
				return false;
	return true;
}

void MagicCube::noNameStep() {
	static bool find;
	while (1) {
		find = 0;
		for (int i = 0; i < 4; ++i, U()) {
			if (front.checkV() && left.checkV() && right.checkV() && back.checkV()) {
				find = 1;
				break;
			}
		}
		if (find)
			break;

		if (front.get(0, 0) != front.get(0, 2) && left.get(0, 0) != left.get(0, 2) && right.get(0, 0) != right.get(0, 2) && back.get(0, 0) != back.get(0, 2)) {
			R(), R(), F(), F(), RR(), BB(), R(), FF(), FF(), RR(), B(), RR();
			continue;
		}

		while (right.get(0, 0) != right.get(0, 2))
			leftRotate();
		R(), R(), F(), F(), RR(), BB(), R(), FF(), FF(), RR(), B(), RR();
	}
}

void MagicCube::recover() {
	if (checkRecover())
		return;
	static bool check;
	check = 0;
	if (front.get(0, 1) == back.get(1, 1) && back.get(0, 1) == front.get(1, 1) && left.get(0, 1) == right.get(1, 1) && right.get(0, 1) == left.get(1, 1))
		check = 1;
	if (front.get(0, 1) == right.get(1, 1) && right.get(0, 1) == front.get(1, 1) && left.get(0, 1) == back.get(1, 1) && back.get(0, 1) == left.get(1, 1))
		check = 1;
	if (front.get(0, 1) == left.get(1, 1) && left.get(0, 1) == front.get(1, 1) && right.get(0, 1) == back.get(1, 1) && back.get(0, 1) == right.get(1, 1))
		check = 1;
	if (check) {
		F(), U(), FF(), U(), F(), U(), U(), FF();
		leftRotate(), leftRotate();
		RR(), UU(), R(), UU(), RR(), UU(), UU(), R();
	}
	while (!(right.get(0, 0) == right.get(0, 1) && right.get(0, 2) == right.get(0, 1)))
		leftRotate();
	if (left.get(0, 1) == front.get(1, 1)) {
		rightRotate();
		RR(), UU(), R(), UU(), RR(), UU(), UU(), R();
		leftRotate(), leftRotate();
		F(), U(), FF(), U(), F(), U(), U(), FF();
	}
	else {
		F(), U(), FF(), U(), F(), U(), U(), FF();
		leftRotate(), leftRotate();
		RR(), UU(), R(), UU(), RR(), UU(), UU(), R();
	}

	if (!checkRecover())
		while (1);
}

bool MagicCube::checkRecover() {
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (front.get(i, j) != front.get(1, 1))
				return false;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (left.get(i, j) != left.get(1, 1))
				return false;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (right.get(i, j) != right.get(1, 1))
				return false;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (back.get(i, j) != back.get(1, 1))
				return false;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (up.get(i, j) != up.get(1, 1))
				return false;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (down.get(i, j) != down.get(1, 1))
				return false;
	return true;
}

void MagicCube::solve() {
	whiteFlower();
    whiteSurface();
    firstTwoLayers();
    yellowFlower();
    yellowSurface();
    noNameStep();
    recover();
}

void MagicCube::getPathWithoutRoll() {
	std::vector<std::string> realPath;
	stringMap p = initial();
	std::string s;
	for (int i = 0; i < (int)path.size(); ++i) {
		s = path[i];
		if (s == "LRoll")
			p = add(p, LRollMap());
		if (s == "RRoll")
			p = add(p, RRollMap());
		if (s == "FRoll")
			p = add(p, FRollMap());
		if (s == "BRoll")
			p = add(p, BRollMap());
		if (s == "LRot")
			p = add(p, LRotMap());
		if (s == "RRot")
			p = add(p, RRotMap());
		if (s.length() <= 2)
			realPath.push_back(p[s]);
	}
	path.clear();
	for (int i = 0; i < (int)realPath.size(); ++i)
		path.push_back(realPath[i]);
}

void MagicCube::getReducedPath() {
	std::vector<std::string> newPath;
    std::pair<int, int> p;
    MagicCube a[10], b[10];
    bool match;
	while (1) {
		int mx = -1;
		for (int i = 0; i < path.size(); ++i) {
            for (int k = 0; k < 10; ++k) {
                a[k] = MagicCube();
                for (int d = 0; d < 100; ++d)
                    a[k].randomRotate();
                b[k] = a[k];
            }
			for (int j = i; j < path.size(); ++j) {
                for (int k = 0; k < 10; ++k)
                    b[k].rotate(path[j]);
                match = 1;
                for (int k = 0; k < 10; ++k)
                    if (!(a[k] == b[k]))
                        match = 0;
                if (match) {
					if (mx < j - i + 1) {
						mx = j - i + 1;
                        p = std::make_pair(i, j);
					}
				}
			}
		}
		if (mx != -1) {
			newPath.clear();
			for (int i = 0; i < p.first; ++i)
				newPath.push_back(path[i]);
			for (int i = p.second + 1; i < path.size(); ++i)
				newPath.push_back(path[i]);
			path.clear();
			for (int i = 0; i < newPath.size(); ++i)
				path.push_back(newPath[i]);
		}
		else
			break;
	}
}
