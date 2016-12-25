#include "rubiccubesolver.h"

void RubicCubeSolver::read(int c[6][3][3]) {
	cube.readFrom6_3_3(c);
}

std::vector<std::string> RubicCubeSolver::method() {
	cube.solve();
	cube.getPathWithoutRoll();
    cube.getReducedPath();

	std::vector<std::string> ans;
	std::string s;
    for (int i = 0; i < (int) cube.path.size(); ++i) {
		s = cube.path[i];
		if (s == "F")
			ans.push_back("Y");
		if (s == "FF")
			ans.push_back("YY");
		if (s == "B")
			ans.push_back("W");
		if (s == "BB")
			ans.push_back("WW");
		if (s == "U")
            ans.push_back("R");
		if (s == "UU")
            ans.push_back("RR");
		if (s == "D")
            ans.push_back("O");
		if (s == "DD")
            ans.push_back("OO");
		if (s == "L")
            ans.push_back("G");
		if (s == "LL")
            ans.push_back("GG");
		if (s == "R")
            ans.push_back("B");
		if (s == "RR")
            ans.push_back("BB");
	}
	return ans;
}

