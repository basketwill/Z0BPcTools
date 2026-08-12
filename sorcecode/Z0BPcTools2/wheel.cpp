#include "stdafx.h"
#include "wheel.h"


static CRITICAL_SECTION csWheelLock;


void 
WheelInitialize(void)
{
    InitializeCriticalSection(&csWheelLock);
}

void 
WheelUninitialize(void)
{
    DeleteCriticalSection(&csWheelLock);
}



// Helper function for calculation of scrolling lines for provided mouse wheel
// delta value. This function is quite generic and can be used/shared with
// many controls.
int
WheelScrollLines(HWND hwnd, int iDelta, UINT nPage, BOOL isVertical)
{
    // We accumulate the wheel_delta until there is enough to scroll for
    // at least a single line. This improves the feel for strange values
    // of SPI_GETWHEELSCROLLLINES and for some mouses.
    static HWND hwndCurrent = NULL;         // HWND we accumulate the delta for.
    static int iAccumulator[2] = { 0, 0 };  // The accumulated value (vert. and horiz.).
    static DWORD dwLastActivity[2] = { 0, 0 };

    UINT uSysParam;
    UINT uLinesPerWHEELDELTA;   // Scrolling speed (how much to scroll per WHEEL_DELTA).
    int iLines;                 // How much to scroll for currently accumulated value.
    int iDirIndex = (isVertical ? 0 : 1);  // The index into iAccumulator[].
    DWORD dwNow;

    dwNow = GetTickCount();

    // Even when page is below one line, we still want to scroll at least a little.
    if (nPage < 1)
        nPage = 1;

    // Ask the system for scrolling speed. 
    uSysParam = (isVertical ? SPI_GETWHEELSCROLLLINES : SPI_GETWHEELSCROLLCHARS);
    if (!SystemParametersInfo(uSysParam, 0, &uLinesPerWHEELDELTA, 0))
        uLinesPerWHEELDELTA = 3;  // default when SystemParametersInfo() fails.
    if (uSysParam == WHEEL_PAGESCROLL) {
        // System tells to scroll over whole pages.
        uLinesPerWHEELDELTA = nPage;
    }
    if (uLinesPerWHEELDELTA > nPage) {
        // Slow down if page is too small. We don't want to scroll over multiple
        // pages at once.
        uLinesPerWHEELDELTA = nPage;
    }

    EnterCriticalSection(&csWheelLock);

    // In some cases, we do want to reset the accumulated value(s).
    if (hwnd != hwndCurrent) {
        // Do not carry accumulated values between different HWNDs.
        hwndCurrent = hwnd;
        iAccumulator[0] = 0;
        iAccumulator[1] = 0;
    } else if (dwNow - dwLastActivity[iDirIndex] > GetDoubleClickTime() * 2) {
        // Reset the accumulator if there was a long time of wheel inactivity.
        iAccumulator[iDirIndex] = 0;
    } else if ((iAccumulator[iDirIndex] > 0) == (iDelta < 0)) {
        // Reset the accumulator if scrolling direction has been reversed.
        iAccumulator[iDirIndex] = 0;
    }

    if (uLinesPerWHEELDELTA > 0) {
        // Accumulate the delta.
        iAccumulator[iDirIndex] += iDelta;

        // Compute the lines to scroll.
        iLines = (iAccumulator[iDirIndex] * (int)uLinesPerWHEELDELTA) / WHEEL_DELTA;

        // Decrease the accumulator for the consumed amount.
        // (Corresponds to the remainder of the integer divide above.)
        iAccumulator[iDirIndex] -= (iLines * WHEEL_DELTA) / (int)uLinesPerWHEELDELTA;
    } else {
        // uLinesPerWHEELDELTA == 0, i.e. likely configured to no scrolling
        // with mouse wheel.
        iLines = 0;
        iAccumulator[iDirIndex] = 0;
    }

    dwLastActivity[iDirIndex] = dwNow;
    LeaveCriticalSection(&csWheelLock);

    // Note that for vertical wheel, Windows provides the delta with opposite
    // sign. Hence the minus.
    return (isVertical ? -iLines : iLines);
}
