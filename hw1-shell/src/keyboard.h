#ifndef __KEYBOARD_H__

#define __KEYBOARD_H__

#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <stdio.h>

#define ENTER 10

int getch();
int kbhit();

#endif // !__KEYBOARD_H__