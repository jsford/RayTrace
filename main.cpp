// JTracer
// Jordan Ford
// May 8, 2013


#include <iostream>
#include <sstream>
#include <string>
#include <time.h>

#include "bitmap_image.h"
#include "defs.h"
#include "raytrace.h"
#include "scene.h"

#define THREADS 1

using namespace std;

bitmap_image globalPic (640, 480);

int main () {

    cout << "\033[2J\033[1;1H";

    Scene drawScene;
    drawScene.sizex = 640;
    drawScene.sizey = 480;
    

    Material m0 = {.5, 1.0f, 1.0f, 0.0f};
    Material m1 = {.5, 0.0f, 1.0f, 1.0f};
    Material m2 = {.5, 1.0f, 0.0f, 1.0f};

    drawScene.materials.push_back(m0);
    drawScene.materials.push_back(m1);
    drawScene.materials.push_back(m2);

    Sphere s0 = {{233, 290, 0.0f}, 100.0f, 0};
    Sphere s1 = {{407, 290, 0.0f}, 100.0f, 1};
    Sphere s2 = {{320, 140, 0.0f}, 100.0f, 2};

    drawScene.spheres.push_back(s0);
    drawScene.spheres.push_back(s1);  
    drawScene.spheres.push_back(s2);

    Light l0 = {{0.0f,240,-100.0f},1,1,1};
    Light l1 = {{640, 240, -10000},.6,.7,1};

    drawScene.lights.push_back(l0);
    drawScene.lights.push_back(l1);     
        
    float begin = clock();
    struct timespec start, finish;
    double elapsed;
    
    clock_gettime(CLOCK_MONOTONIC, &start);

    parallelDraw(&drawScene, THREADS);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    
    elapsed = finish.tv_sec - start.tv_sec;
    elapsed += (finish.tv_nsec - start.tv_nsec)/1000000000.0f;    
    
    cout << elapsed << endl;

    globalPic.save_image("output.bmp");    
    system("eog output.bmp");


    return 0;
}



