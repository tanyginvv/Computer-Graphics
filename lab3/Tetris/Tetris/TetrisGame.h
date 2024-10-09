#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "stdafx.h"

class TetrisGame {
public:
    TetrisGame();
    void DrawSquare(float x, float y, float size, int cIndex);
    void NewShape();
    void ClearMovingShape();
    void MoveShapeLeft();
    void MoveShapeRight();
    void RotateShape();
    void RotateLineShape();
    void RotateLShape();
    void RotateSShape();
    void RotateTShape();
    void RotationUpdate();
    void RemoveCompleteLines();
    void MoveShapeDown();
    void DisplayGameName();
    void DisplayScore();
    void DrawBoxes();
    void DisplayText(int x, int y, const std::string& str);
    void GameFinishCheck();
    int GetWidth();
    int GetHeight();
    void Display();
    void NonPrintableKeys(int key, int x, int y);
  
private:
    int board[10][25];
    int curr_shape[4][2];
    int currShapeType;
    int currShapeColor;
    int direction;
    int tempPoints[4][2];
    int score;
    int oldScore;
    int highScore;
    int width;
    int height;
    std::string scoreStr;
    std::string highScoreStr;

    float colors[8][3] = {
        {0.95f, 0.95f, 0.95f},    // gray
        {1.0f, 1.0f, 1.0f},       // white
        {1.0f, 0.0f, 0.0f},       // red
        {1.0f, 0.90f, 0.0f},      // yellow
        {0.2f, 0.6f, 1.0f},       // blue
        {0.0f, 0.8f, 0.4f},       // green
        {1.0f, 0.49f, 0.0f},      // orange
        {1.0f, 0.0f, 1.0f}        // magenta
    };
};

#endif // TETRIS_GAME_H