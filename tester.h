#ifndef TESTER_H
#define TESTER_H

#include "magiccube.h"
#include "surface.h"
#include <vector>
#include <string>

class Tester {
public:
    Tester();
    void generateTestInput();
    /*
     * Generate a random rubiccube and output it to input.txt in the same directory as the program.
     */
    bool runCheck();
    /*
     * Use it to check if the plan in the answer.txt can solve the rubicube in the input.txt.
     */

private:
    MagicCube testRubicCube;
};

#endif
