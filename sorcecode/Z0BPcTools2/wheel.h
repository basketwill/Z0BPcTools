
#ifndef WHEEL_H
#define WHEEL_H

#include <Windows.h>


void WheelInitialize(void);
void WheelUninitialize(void);

int WheelScrollLines(HWND hwnd, int iDelta, UINT nPage, BOOL isVertical);


#endif
