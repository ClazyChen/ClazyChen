#include "tester.h"
#include "magiccube.h"
#include "surface.h"
#include <vector>
#include <string>

Tester::Tester()
{
}

void Tester::generateTestInput() {
    static MagicCube a;
    for (int i = 0; i < 1; ++i)
        a.randomRotate();
    FILE *fout = fopen("input.txt", "w");
    a.front.sampleprint(fout);
    a.back.sampleprint(fout);
    a.right.sampleprint(fout);
    a.left.sampleprint(fout);
    a.up.sampleprint(fout);
    a.down.sampleprint(fout);
    fclose(fout);
    testRubicCube = a;
}

bool Tester::runCheck() {
    FILE *fin = fopen("answer.txt", "r");
    static char s[100010];
    fscanf(fin, "%s", s);
    fclose(fin);
    int i = 0;
    while ((s[i] == 'L' || s[i] == 'R' || s[i] == 'U' || s[i] == 'D' || s[i] == 'F' || s[i] == 'B')) {
        if (s[i + 1] == 'i')
            testRubicCube.rotate(std::string(1, s[i]) + std::string(1, s[i])), i += 2;
        else
            testRubicCube.rotate(std::string(1, s[i])), ++i;
    }
    return testRubicCube.checkRecover();
}
