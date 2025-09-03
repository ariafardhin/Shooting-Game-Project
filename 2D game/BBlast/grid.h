#ifndef GRID_H
#define GRID_H

#include "bubble.h"

class Grid {
private:
    static const int MAX_ROWS = 12;
    static const int MAX_COLS = 8;
    static const int BUBBLE_RADIUS = 10;
    Bubble* bubbles[MAX_ROWS][MAX_COLS];
    int score;
    int width, height;
    int rows, cols;
    int **grid;

public:
    inline Grid();
    inline Grid(int rows, int cols);
    inline ~Grid();
    inline void initialize();
    inline void update();
    inline void draw();
    inline void drawGrid();
    inline bool checkMatch(int row, int col);
    inline void popConnectedBubbles(int row, int col, int color);
    inline int getScore() const { return score; }
    inline bool isGameOver() const;
    inline void addPoints(int points) { score += points; };
    inline void markConnectedBubbles(int row, int col, bool visited[MAX_ROWS][MAX_COLS]);
};

#endif