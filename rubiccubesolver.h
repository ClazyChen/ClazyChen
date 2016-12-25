#ifndef _RUBIC_CUBE_SOLVER_H
#define _RUBIC_CUBE_SOLVER_H
/*
static const int WHITE   = 0;
static const int BLUE    = 1;
static const int ORANGE  = 2;
static const int RED     = 3;
static const int GREEN   = 4;
static const int YELLOW  = 5;
*/
#include "magiccube.h"
#include <vector>
#include <string>

#define TESTING

class RubicCubeSolver {

public:
    void read(int c[6][3][3]);
    /*
     * six surfaces, each 3 * 3
     * every entry is in the range [0,5]
     */
    std::vector<std::string> method();
    /*
     * "Y" means the yellow surface clockwise rotate and "YY" means it's counterclockwise rotate
     * also "W", "WW"(white), "B", "BB"(blue), "G", "GG"(green), "O", "OO"(orange), "R", "RR"(red)
     */

    std::vector<int> method_for_Czk;
    /*
     * use Czk's style to save rotates
     *
     */
#ifndef TESTING
private:
#endif
    MagicCube cube;

};

#endif
