#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#define WIDTH 80
#define HIGHT 25
#define KEY_DOWN_1 'z'
#define KEY_DOWN_2 'm'
#define KEY_UP_1 'a'
#define KEY_UP_2 'k'
#define KEY_SKIP ' '
#define PLAYER1_X 1
#define PLAYER2_X (WIDTH - 2)
#define FINALLPOINT 21
#define BORDER 35
#define SPEED 100000

int draw(int player1Y, int player2Y, int player1Points, int player2Points, int ballX, int ballY);
int scanPlayer(int player1Y, char key);
int flags(int x_ball, int y_ball, int y_rocket_1, int y_rocket_2, int player1_x, int player2_x);
int ball_x_(int x_b, int v_x);
int ball_y_(int y_b, int v_y);
int ball_v_x(int v_x, int flag);
int ball_v_y(int v_y, int flag);
int p10(int i);
int getDigit(int num, int pos);
int main() {
    char c;
    int player1Y = HIGHT / 2;
    int player2Y = HIGHT / 2;
    int ballX = WIDTH / 2;
    int ballY = HIGHT / 2;
    int v_x = 1;
    int v_y = 1;
    int player1Points = 0;
    int player2Points = 0;
    int flag = 0;
    nodelay(initscr(), 1);
    noecho();
    draw(player1Y, player2Y, player1Points, player2Points, ballX, ballY);
    while (1) {
        if ((player1Points == FINALLPOINT) || (player2Points == FINALLPOINT)) {
            break;
        }
        clear();
        draw(player1Y, player2Y, player1Points, player2Points, ballX, ballY);
        ballX = ball_x_(ballX, v_x);
        ballY = ball_y_(ballY, v_y);

        c = getch();
        switch (c) {
            case KEY_DOWN_1:
            case KEY_UP_1:
                player1Y = scanPlayer(player1Y, c);
                break;
            case KEY_DOWN_2:
            case KEY_UP_2:
                player2Y = scanPlayer(player2Y, c);
                break;
        }
        flag = flags(ballX, ballY, player1Y, player2Y, PLAYER1_X, PLAYER2_X);
        switch (flag) {
            case 1:
                player1Points++;
                ballX = WIDTH / 2;
                ballY = HIGHT / 2;
                player1Y = HIGHT / 2;
                player2Y = HIGHT / 2;

                break;
            case 2:
                player2Points++;
                ballX = WIDTH / 2;
                ballY = HIGHT / 2;
                player1Y = HIGHT / 2;
                player2Y = HIGHT / 2;
                break;
        }
        refresh();
        usleep(SPEED);
        v_x = ball_v_x(v_x, flag);
        v_y = ball_v_y(v_y, flag);
    }
    clear();
    draw(player1Y, player2Y, player1Points, player2Points, ballX, ballY);
    printw("Player %d won!\n", flag);
    refresh();
    scanf("%c", &c);
    endwin();
    return 0;
}

int p10(int i) {
    if (i == 0) {
        return 1;
    } else {
        return 10 * p10(i - 1);
    }
}

int countDigits(int num) {
    if (num / 10 == 0) {
        return 1;
    } else {
        return 1 + countDigits(num / 10);
    }
}

int getDigit(int num, int pos) {
    if (pos == 1) {
        return num % 10;
    } else {
        return num / p10(pos - 1) % 10;
    }
}

int draw(int player1Y, int player2Y, int player1Points, int player2Points, int ballX, int ballY) {
    // 1st line 1st half
    for (int i = WIDTH / 2 - 1; i > 0; i--) {
        if (i > countDigits(player1Points)) {
            printw("_");
        } else {
            printw("%d", getDigit(player1Points, i));
        }
    }

    printw("||");

    // 1st line 2nd half
    for (int i = 1; i <= WIDTH / 2 - 1; i++) {
        if (i <= countDigits(player2Points)) {
            printw("%d", getDigit(player2Points, countDigits(player2Points) + 1 - i));
        } else {
            printw("_");
        }
    }
    //  player2Points += 1;
    printw("\n");

    // top border
    for (int i = 0; i <= WIDTH - 1; i++) {
        printw("%c", BORDER);
    }

    // main field draw
    printw("\n");
    for (int y = HIGHT - 1; y >= 1; y--) {
        for (int x = 0; x <= WIDTH - 1; x++) {
            switch (x) {
                case 0:  // left border
                    // printw("|%d%d|", y/10, y%10);
                    printw("|");
                    break;
                case PLAYER1_X:  // 1st player racket
                    if ((y <= player1Y + 1) && (y >= player1Y - 1)) {
                        printw("]");
                    } else {
                        printw(" ");
                    }
                    break;
                case PLAYER2_X:  // 2nd player racket
                    if ((y <= player2Y + 1) && (y >= player2Y - 1)) {
                        printw("[");
                    } else {
                        printw(" ");
                    }
                    break;
                case WIDTH - 1:  // right border
                                 //                    printw("|%d%d|", y/10, y%10);
                    printw("|");
                    break;
                default:
                    if ((x == ballX) && (y == ballY)) {
                        printw("o");
                    } else if ((x == WIDTH / 2 - 1) || x == WIDTH / 2) {
                        printw(".");  // center
                    } else {
                        printw(" ");
                    }
                    break;
            }
        }
        printw("\n");
    }

    // last line - border
    for (int i = 0; i <= WIDTH - 1; i++) {
        printw("%c", BORDER);
    }
    printw("\n");
    curs_set(0);
    return 0;
}

int scanPlayer(int playerY, char key) {
    switch (key) {
        case KEY_UP_2:
        case KEY_UP_1:
            if (playerY <= HIGHT - 3) {
                return playerY + 1;
            } else {
                return playerY;
            }
            break;
        case KEY_DOWN_2:
        case KEY_DOWN_1:
            if (playerY >= 3) {
                return playerY - 1;
            } else {
                return playerY;
            }
            break;
        case KEY_SKIP:
            return playerY;
            break;
        default:
            return playerY;
            break;
    }
    return playerY;
}

int flags(int x_ball, int y_ball, int y_rocket_1, int y_rocket_2, int player1_x, int player2_x) {
    if (((y_rocket_1 == y_ball) && (x_ball == player1_x + 1)) ||
        ((y_rocket_2 == y_ball) && (x_ball == player2_x - 1))) {  // hit center of racket
        return 4;
    } else if ((((y_rocket_1 - 1) == y_ball) && (x_ball == player1_x + 1)) ||
               (((y_rocket_2 - 1) == y_ball) && (x_ball == player2_x - 1))) {  // hit bottom of racket
        return 3;
    } else if ((((y_rocket_1 + 1) == y_ball) && (x_ball == player1_x + 1)) ||
               (((y_rocket_2 + 1) == y_ball) && (x_ball == player2_x - 1))) {  // hit top of racket
        return 5;
    } else if (x_ball == player1_x) {  // player 1 missed
        return 2;
    } else if (x_ball == player2_x) {  // player 2 missed
        return 1;
    } else if (y_ball == 1 || y_ball == HIGHT - 1) {  // hit border
        return 6;
    } else {
        return 0;  // no hit
    }
}

int ball_x_(int x_b, int v_x) {
    x_b += v_x;
    return x_b;
}

int ball_y_(int y_b, int v_y) {
    y_b += v_y;
    return y_b;
}

int ball_v_x(int v_x, int flag) {  // change movement x according to flag
    switch (flag) {
        case 1:
        case 2:
            v_x *= -1;
            return v_x;
            break;
        case 3:
        case 4:
        case 5:
            v_x *= -1;
            return v_x;
            break;
        default:
            return v_x;
    }
}

int ball_v_y(int v_y, int flag) {  // change movement y according to flag
    if ((flag == 6)) {
        v_y = v_y * (-1);
        return v_y;
    } else {
        return v_y;
    }
}
