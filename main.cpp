// JTracer
// Jordan Ford
// May 8, 2013

#include <iostream>         // Needed for cout and endl.
#include <time.h>           // Needed for tracking execution time.

#include "bitmap_image.h"   // Needed to manipulate the image.
#include "defs.h"           // Defines basic Point/Vector/Color classes.
#include "raytrace.h"       // Defines Light/Ray/Object classes.
#include "scene.h"          // Defines a Scene.

#define THREADS 4

#define CLS "\033[2J\033[1;1H"

using namespace std;

int main () {

    // Clear the terminal screen.
    cout << CLS;

    // Initialize the scene.
    // TODO: Don't hardcode it!
    Scene drawScene;
    
    // Initialize the image.
    bitmap_image pic(drawScene.sizex, drawScene.sizey);

    // Get the initial time.
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Draw the picture.
    parallelDraw(&drawScene, &pic, THREADS);

    // Get the final time.
    clock_gettime(CLOCK_MONOTONIC, &finish);
    
    // Calculate the elapsed time.
    double elapsed;
    elapsed = (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    elapsed += finish.tv_sec - start.tv_sec;

    // Display the elapsed time.
    cout << elapsed << endl;

    // Save the final rendering.
    pic.save_image("output.bmp");
 
    // Display the rendering.   
    system("eog output.bmp");

    return 0;
}



