#include "grid.h"
#include "bubble.h"
#include <iostream>
#include <graphics.h>

#define MAX_ROWS 12
#define MAX_COLS 8


Grid::Grid(int rows, int cols) : rows(rows), cols(cols) {
    initialize();
}

Grid::~Grid() {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            delete bubbles[i][j]; // Free memory for each bubble
        }
    }
}

void Grid::initialize() {
    // Fill top 6 rows with random colored bubbles
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            bubbles[i][j] = new Bubble(j * (BUBBLE_RADIUS * 2 + 5) + BUBBLE_RADIUS,
                                     i * (BUBBLE_RADIUS * 2 + 5) + BUBBLE_RADIUS);
        }
    }
    
    // Initialize remaining rows as empty
    for (int i = 6; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            bubbles[i][j] = nullptr;
        }
    }
}

void Grid::update() {
    // Check for floating bubbles after pops
    bool visited[MAX_ROWS][MAX_COLS] = {{false}};
    
    // Mark all bubbles connected to top row
    for (int j = 0; j < MAX_COLS; j++) {
        if (bubbles[0][j] && !bubbles[0][j]->isPopped) {
            markConnectedBubbles(0, j, visited);
        }
    }
    
    // Remove unconnected bubbles
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            if (bubbles[i][j] && !bubbles[i][j]->isPopped && !visited[i][j]) {
                delete bubbles[i][j];
                bubbles[i][j] = nullptr;
                score += 5; // Bonus points for floating bubbles
            }
        }
    }
}


void Grid::drawGrid() {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            if (bubbles[i][j] && !bubbles[i][j]->isPopped) {
                bubbles[i][j]->draw();
            }
        }
    }
}

void Grid::popConnectedBubbles(int row, int col, int color) {
    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) return;
    if (!bubbles[row][col] || bubbles[row][col]->isPopped) return;
    if (bubbles[row][col]->color != color) return;

    bubbles[row][col]->pop();
    score += 10;

    // Check all 6 directions (hexagonal grid)
    const int dr[] = {-1, -1, 0, 1, 1, 0};
    const int dc[] = {0, 1, 1, 0, -1, -1};
    
    for (int i = 0; i < 6; i++) {
        popConnectedBubbles(row + dr[i], col + dc[i], color);
    }
}

void Grid::markConnectedBubbles(int row, int col, bool visited[MAX_ROWS][MAX_COLS]) {
    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) return;
    if (!bubbles[row][col] || bubbles[row][col]->isPopped || visited[row][col]) return;
    
    visited[row][col] = true;
    
    // Check all 6 directions (hexagonal grid)
    const int dr[] = {-1, -1, 0, 1, 1, 0};
    const int dc[] = {0, 1, 1, 0, -1, -1};
    
    for (int i = 0; i < 6; i++) {
        markConnectedBubbles(row + dr[i], col + dc[i], visited);
    }
}

// Implement Grid::isGameOver() const
bool Grid::isGameOver() const {
    // Game is over if bubbles reach bottom row
    for (int j = 0; j < MAX_COLS; j++) {
        if (bubbles[MAX_ROWS-1][j] && !bubbles[MAX_ROWS-1][j]->isPopped) {
            return true;
        }
    }
    
    // Or if all bubbles are popped
    bool anyBubblesLeft = false;
    for (int i = 0; i < MAX_ROWS && !anyBubblesLeft; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            if (bubbles[i][j] && !bubbles[i][j]->isPopped) {
                anyBubblesLeft = true;
                break;
            }
        }
    }
    
    return !anyBubblesLeft;
}
