#include "universal.h"
#include "surface.h"
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>

int Surface::get(int i, int j) {
    return c[i][j];
}

void Surface::set(int i, int j, int x) {
    c[i][j] = x;
}

void Surface::fill(int x) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
                c[i][j] = x;
}

void Surface::readFrom3_3(int c[3][3]) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            set(i, j, c[i][j]);
}

void Surface::leftRotate() {
    static int cc[N][N];
    cc[0][0] = c[0][2];
    cc[2][0] = c[0][0];
    cc[2][2] = c[2][0];
    cc[0][2] = c[2][2];
    cc[0][1] = c[1][2];
    cc[1][0] = c[0][1];
    cc[2][1] = c[1][0];
    cc[1][2] = c[2][1];
    cc[1][1] = c[1][1];
    memcpy(c, cc, sizeof cc);
}

void Surface::rightRotate() {
    static int cc[N][N];
    cc[0][2] = c[0][0];
    cc[2][2] = c[0][2];
    cc[2][0] = c[2][2];
    cc[0][0] = c[2][0];
    cc[0][1] = c[1][0];
    cc[1][2] = c[0][1];
    cc[2][1] = c[1][2];
    cc[1][0] = c[2][1];
    cc[1][1] = c[1][1];
    memcpy(c, cc, sizeof cc);
}

void Surface::print() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            std::cout << numberToColor(c[i][j]) << " ";
        std::cout << std::endl;
    }
}

void Surface::sampleprint(FILE *fout = stdout) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            //putchar(numberToCol(c[i][j]));
            fprintf(fout, "%c", numberToCol(c[i][j]));
    //puts("");
    fprintf(fout, "\n");
}

void Surface::read() {
    std::string s;
    std::cin >> s;
    int i, j, k = 0;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            c[i][j] = colorToNumber(s[k++]);
}

bool Surface::checkV() {
    return get(0, 0) == get(1, 1) && get(0, 2) == get(1, 1);
}

void circleTransition(Surface &s0, Surface &s1, Surface &s2, Surface &s3, int x[4][3], int y[4][3]) {
    static Surface s[4], temp;
    s[0] = s0, s[1] = s1, s[2] = s2, s[3] = temp = s3;
    for (int i = N; i >= 1; --i)
        for (int j = 0; j < N; ++j)
            s[i].set(x[i][j], y[i][j], s[i - 1].get(x[i - 1][j], y[i - 1][j]));
    for (int j = 0; j < N; ++j)
        s[0].set(x[0][j], y[0][j], temp.get(x[3][j], y[3][j]));
    s0 = s[0], s1 = s[1], s2 = s[2], s3 = s[3];
}
