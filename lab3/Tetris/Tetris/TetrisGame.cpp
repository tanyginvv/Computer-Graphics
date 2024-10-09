#include "TetrisGame.h"

TetrisGame::TetrisGame() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 25; ++j) {
            board[i][j] = 0;
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 2; ++j) {
            curr_shape[i][j] = -1;
        }
    }

    score = 0;
    oldScore = 0;

    width = 480;
    height = 480;

    scoreStr = "Score: 0";
    highScoreStr = "High Score: 0";
    NewShape();
}

int TetrisGame::GetWidth() {
    return width;
}

int TetrisGame::GetHeight() {
    return height;
}

void TetrisGame::Display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GameFinishCheck();

    DisplayGameName();

    DisplayScore();

    MoveShapeDown();

    DrawBoxes();

    glFlush();
}

void TetrisGame::NonPrintableKeys(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN)
        glutPostRedisplay();
    else {
        if (key == GLUT_KEY_LEFT)
            MoveShapeLeft();
        else if (key == GLUT_KEY_RIGHT)
            MoveShapeRight();
        else if (key == GLUT_KEY_UP)
            RotateShape();
        DrawBoxes();
        glFlush();
    }
}


void TetrisGame:: DrawSquare(float x, float y, float size, int cIndex) {
    glBegin(GL_QUADS);
    glColor3f(colors[cIndex][0], colors[cIndex][1], colors[cIndex][2]);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glEnd();
}

void TetrisGame:: NewShape() {
    int randNum = rand() % 5;
    currShapeType = randNum;
    currShapeColor = rand() % 6 + 2;
    direction = 1;

    // straight line
    if (randNum == 0) {
        // x coordinates
        curr_shape[0][0] = rand() % 7;
        curr_shape[1][0] = curr_shape[0][0] + 1;
        curr_shape[2][0] = curr_shape[0][0] + 2;
        curr_shape[3][0] = curr_shape[0][0] + 3;
        // y coordinates
        curr_shape[0][1] = curr_shape[1][1] = curr_shape[2][1] = curr_shape[3][1] = 21;
    }

    // box shape
    else if (randNum == 1) {
        // x corrdinates
        curr_shape[0][0] = rand() % 9;
        curr_shape[1][0] = curr_shape[0][0] + 1;
        curr_shape[2][0] = curr_shape[0][0];
        curr_shape[3][0] = curr_shape[1][0];
        // y coordinates
        curr_shape[0][1] = curr_shape[1][1] = 22;
        curr_shape[2][1] = curr_shape[3][1] = 21;
    }

    // L shape
    else if (randNum == 2) {
        // x coordinates
        curr_shape[0][0] = rand() % 8;
        curr_shape[1][0] = curr_shape[0][0];
        curr_shape[2][0] = curr_shape[0][0] + 1;
        curr_shape[3][0] = curr_shape[1][0] + 2;
        // y coordinates
        curr_shape[0][1] = 22;
        curr_shape[1][1] = curr_shape[2][1] = curr_shape[3][1] = 21;
    }

    // S shape
    else if (randNum == 3) {
        //  x coordinates
        curr_shape[0][0] = rand() % 8 + 1;
        curr_shape[1][0] = curr_shape[0][0] + 1;
        curr_shape[2][0] = curr_shape[0][0];
        curr_shape[3][0] = curr_shape[0][0] - 1;
        // y coordinates
        curr_shape[0][1] = curr_shape[1][1] = 22;
        curr_shape[2][1] = curr_shape[3][1] = 21;
    }

    // T shape
    else if (randNum == 4) {
        // x coordinates
        curr_shape[0][0] = rand() % 8;
        curr_shape[1][0] = curr_shape[0][0] + 1;
        curr_shape[2][0] = curr_shape[0][0] + 2;
        curr_shape[3][0] = curr_shape[1][0];
        // y coordinates
        curr_shape[0][1] = curr_shape[1][1] = curr_shape[2][1] = 22;
        curr_shape[3][1] = 21;
    }
}
void TetrisGame:: ClearMovingShape() {
    for (int a = 0; a < 10; a++) {
        for (int b = 0; b < 20; b++) {
            if (board[a][b] == 1)
                board[a][b] = 0;
        }
    }
}

void TetrisGame:: MoveShapeLeft() {
    int canMove = 1;
    for (int a = 0; a < 4; a++) {
        if (curr_shape[a][0] == 0 || (board[curr_shape[a][0] - 1][curr_shape[a][1]] > 1))
            canMove = 0;
    }
    if (canMove) {
        for (int a = 0; a < 4; a++)
            board[curr_shape[a][0]][curr_shape[a][1]] = 0;
        for (int a = 0; a < 4; a++)
            curr_shape[a][0] -= 1;
    }
}

void TetrisGame::MoveShapeRight() {
    int canMove = 1;
    for (int a = 0; a < 4; a++) {
        if (curr_shape[a][0] == 9 || (board[curr_shape[a][0] + 1][curr_shape[a][1]] > 1))
            canMove = 0;
    }
    if (canMove) {
        for (int a = 0; a < 4; a++)
            board[curr_shape[a][0]][curr_shape[a][1]] = 0;
        for (int a = 0; a < 4; a++)
            curr_shape[a][0] += 1;
    }
}

void TetrisGame:: RotateLineShape() {
    if (direction == 1 || direction == 3) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[1][0] = tempPoints[2][0] = tempPoints[3][0] = tempPoints[0][0];
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[0][1] - 1;
        tempPoints[2][1] = tempPoints[0][1] - 2;
        tempPoints[3][1] = tempPoints[0][1] - 3;
    }
    else if (direction == 2 || direction == 4) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[1][0] = tempPoints[0][0] + 1;
        tempPoints[2][0] = tempPoints[0][0] + 2;
        tempPoints[3][0] = tempPoints[0][0] + 3;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[2][1] = tempPoints[3][1] = tempPoints[0][1];
    }
}

void TetrisGame::RotateSShape() {
    if (direction == 1 || direction == 3) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[0][0] = tempPoints[0][0] - 1;
        tempPoints[1][0] = tempPoints[0][0];
        tempPoints[2][0] = tempPoints[0][0] + 1;
        tempPoints[3][0] = tempPoints[0][0] + 1;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[2][1] = tempPoints[0][1] - 1;
        tempPoints[3][1] = tempPoints[0][1] - 2;
    }
    else if (direction == 2 || direction == 4) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[0][0] = tempPoints[0][0] + 1;
        tempPoints[1][0] = tempPoints[0][0] + 1;
        tempPoints[2][0] = tempPoints[0][0];
        tempPoints[3][0] = tempPoints[0][0] - 1;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[0][1];
        tempPoints[2][1] = tempPoints[0][1] - 1;
        tempPoints[3][1] = tempPoints[2][1];
    }
}
void TetrisGame:: RotateLShape() {
    if (direction == 1) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[1][0] = tempPoints[0][0] + 1;
        tempPoints[2][0] = tempPoints[0][0];
        tempPoints[3][0] = tempPoints[0][0];
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[0][1];
        tempPoints[2][1] = tempPoints[0][1] - 1;
        tempPoints[3][1] = tempPoints[0][1] - 2;
    }
    else if (direction == 2) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[1][0] = tempPoints[0][0] + 1;
        tempPoints[2][0] = tempPoints[0][0] + 2;
        tempPoints[3][0] = tempPoints[0][0] + 2;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[2][1] = tempPoints[0][1];
        tempPoints[3][1] = tempPoints[2][1] - 1;
    }
    else if (direction == 3) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[0][0] = tempPoints[0][0] + 1;
        tempPoints[1][0] = tempPoints[2][0] = tempPoints[0][0];
        tempPoints[3][0] = tempPoints[0][0] - 1;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[0][1] - 1;
        tempPoints[2][1] = tempPoints[0][1] - 2;
        tempPoints[3][1] = tempPoints[0][1] - 2;
    }
    else if (direction == 4) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[0][0] = tempPoints[0][0] - 1;
        tempPoints[1][0] = tempPoints[0][0];
        tempPoints[2][0] = tempPoints[0][0] + 1;
        tempPoints[3][0] = tempPoints[0][0] + 2;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[2][1] = tempPoints[3][1] = tempPoints[0][1] - 1;
    }
}

void TetrisGame::RotateTShape() {
    if (direction == 1) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[0][0] = tempPoints[0][0] + 1;
        tempPoints[1][0] = tempPoints[2][0] = tempPoints[0][0];
        tempPoints[3][0] = tempPoints[0][0] - 1;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[0][1] - 1;
        tempPoints[2][1] = tempPoints[0][1] - 2;
        tempPoints[3][1] = tempPoints[0][1] - 1;
    }
    else if (direction == 2) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[1][0] = tempPoints[0][0] - 1;
        tempPoints[2][0] = tempPoints[0][0];
        tempPoints[3][0] = tempPoints[0][0] + 1;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[2][1] = tempPoints[3][1] = tempPoints[0][1] - 1;
    }
    else if (direction == 3) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[0][0] = tempPoints[0][0] - 1;
        tempPoints[1][0] = tempPoints[2][0] = tempPoints[0][0];
        tempPoints[3][0] = tempPoints[0][0] + 1;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[0][1] - 1;
        tempPoints[2][1] = tempPoints[0][1] - 2;
        tempPoints[3][1] = tempPoints[0][1] - 1;
    }
    else if (direction == 4) {
        // new x coordinates
        tempPoints[0][0] = curr_shape[0][0];
        tempPoints[1][0] = tempPoints[0][0] + 1;
        tempPoints[2][0] = tempPoints[0][0] + 2;
        tempPoints[3][0] = tempPoints[0][0] + 1;
        // new y coordinates
        tempPoints[0][1] = curr_shape[0][1];
        tempPoints[1][1] = tempPoints[2][1] = tempPoints[0][1];
        tempPoints[3][1] = tempPoints[0][1] - 1;
    }
}
//збавиться от констант 
//разбить на классы визуализации и логики
void TetrisGame::RemoveCompleteLines() {
    int count = 0;
    for (int a = 0; a < 20; a++) {
        count = 0;
        for (int b = 0; b < 10; b++)
        {
            if ((board[b][a] != 1) && (board[b][a] > 1)) {
                count++;
            }
        }
        if (count == 10) {
            for (int i = a; i < 20; i++) {
                for (int b = 0; b < 10; b++) {
                    if (board[b][i + 1] != 1)
                        board[b][i] = board[b][i + 1];
                    else
                        board[b][i] = 0;
                }
            }
            a--;
            score += 100;
        }
    }
}
void TetrisGame::DrawBoxes() {
    ClearMovingShape();

    for (int a = 0; a < 4; a++) {
        board[curr_shape[a][0]][curr_shape[a][1]] = 1;
    }

    for (int a = 0; a < 10; a++) {
        for (int b = 0; b < 20; b++) {
            DrawSquare(a * 24, b * 24, 23, board[a][b]);
            if (board[a][b] == 1) {
                DrawSquare(a * 24, b * 24, 23, currShapeColor);
            }
        }
    }
}

void TetrisGame::RotateShape() {
    if (currShapeType == 1) {
    }
    else {
        if (currShapeType == 0)
            RotateLineShape();
        else if (currShapeType == 2)
            RotateLShape();
        else if (currShapeType == 3)
            RotateSShape();
        else if (currShapeType == 4)
            RotateTShape();
        RotationUpdate();
    }
}
void TetrisGame::MoveShapeDown() {
    int stop = 0;
    for (int a = 0; a < 4; a++) {
        if (curr_shape[a][1] == 0 || (board[curr_shape[a][0]][curr_shape[a][1] - 1] > 1))
            stop = 1;
    }

    if (stop == 0) {
        for (int a = 0; a < 4; a++) {
            curr_shape[a][1] -= 1;
        }
    }

    else {
        for (int a = 0; a < 4; a++) {
            board[curr_shape[a][0]][curr_shape[a][1]] = currShapeColor;
        }
        RemoveCompleteLines();
        NewShape();
    }
}

void TetrisGame::RotationUpdate() {
    int otherShape = 0;
    int i = -1, j = -1;
    for (int a = 0; a < 4; a++) {
        i = tempPoints[a][0];
        j = tempPoints[a][1];
        if (i >= 0 && i <= 9 && j >= 0)
        {
            if (board[tempPoints[a][0]][tempPoints[a][1]] > 1)
                otherShape = 1;
        }
        else
            otherShape = 1;

    }
    if (otherShape == 0) {
        for (int a = 0; a < 4; a++) {
            curr_shape[a][0] = tempPoints[a][0];
            curr_shape[a][1] = tempPoints[a][1];
        }
        if (direction >= 4)
            direction = 1;
        else
            direction++;
    }
}

void TetrisGame::GameFinishCheck() {
    int complete = 0;
    for (int a = 0; a < 10; a++) {
        if (board[a][19] > 1)
            complete = 1;
    }
    if (complete) {
        score = 0;
        for (int a = 0; a < 10; a++) {
            for (int b = 0; b < 25; b++) {
                board[a][b] = 0;
            }
        }
        NewShape();
    }
}
void TetrisGame:: DisplayGameName() {
    DisplayText(320, 400, "Tetris");
}


void TetrisGame:: DisplayScore() {
    if (score != oldScore) {
        oldScore = score;
        std::stringstream s;
        s << score;
        std::string tmpStr = "";
        s >> tmpStr;
        scoreStr = "Score: " + tmpStr;
    }
    DisplayText(300, 350, scoreStr);
}

void TetrisGame:: DisplayText(int x, int y, const std::string& str)
{
    GLvoid* font_style = GLUT_BITMAP_TIMES_ROMAN_24;
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(x, y, 1);
    for (int a = 0; a < str.size(); a++)
        glutBitmapCharacter(font_style, str[a]);
}