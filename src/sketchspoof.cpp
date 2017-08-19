/**
 * Developed by devil_tux <https://duraki.github.io>
 *
 * A dead simple cpp lib to change local time
 * For use on SketchApp Trial license
 */

// Compile:
// $ g++ -shared sketchspoof.cpp -o SketchSpoof.dylib
// Run:
// $ DYLD_INSERT_LIBRARIES=SketchSpoof.dylib /Applications/Sketch.app/Contents/MacOS/Sketch

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

int main()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    printf("Current time: %d\n", &tv.tv_sec);

    tv.tv_sec = 1471620228; // sometimes in the past
    tv.tv_usec = 1471;

    if (settimeofday(&tv, NULL) == 0) {
        printf("settimeofday success\n");
        printf("Current time new: %d\n", &tv.tv_sec);
    } else {
        printf("settimeofday fail: %d\n", errno); 
    };

    return 1;
}
