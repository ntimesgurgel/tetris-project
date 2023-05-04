#define HEIGHT 20
#define WIDTH 10

int Score = 0;

int field[HEIGHT][WIDTH] = {0};

struct Point
{int x,y;} current[4], next[4];

const int figures[7][4] =
{
    1,3,5,7,
    0,2,3,5,
    1,3,2,4,
    1,3,2,5,
    0,1,3,5,
    1,3,5,4,
    0,1,2,3,
};
