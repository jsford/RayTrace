#include <cmath>            // Needed for sqrtf() 
#include <cfloat>           // Needed for FLT_MAX
#include <pthread.h>        // Needed to parallelize the rendering.

#include "bitmap_image.h"   // Needed to manipulate the image.
#include "defs.h"           // Defines basic Point/Vector/Color classes.
#include "raytrace.h"       // Defines Light/Ray/Object classes.
#include "scene.h"          // Defines a Scene.

#define SUPERSAMPLE 256

using namespace std;

bool Sphere::hitSphere(const Ray& r, float& t){
    
    // Ray R
    // Sphere S
    //
    // o = R.start - S.pos
    //
    // A = R.dir .* R.dir
    // B = 2(o .* R.dir)
    // C = (o .* o) - S.rad^2

    Vector o = r.start - this->pos;      
    
    float A = r.dir * r.dir;
    float B = 2 * (r.dir * o);
    float C = o*o - this->rad*this->rad;

    float disc = B*B - 4*A*C;
    if(disc < 0.0f){
        return false;
    }

    float q;
    float distSqrt = sqrtf(disc);
    
    if(B < 0.0f){
        q = ( -B + distSqrt ) / 2.0;
    }else{
        q = ( -B - distSqrt ) / 2.0;
    }

    float t0 = q/A;
    float t1 = C/q;

    if(t0 > t1){
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
    
    if(t1 < 0.01){
        return false;
    }

    if(t0 < 0.01){
        t = t1;
    }else{
        t = t0;
    }

    return true;
}

void parallelDraw(Scene* myScene, bitmap_image* pic, int numthreads){
    
    pthread_t threads[numthreads];

    for(int tId = 0; tId <= numthreads-1; tId++){

        arg* inputs = new arg();
        inputs->myScene = myScene;
        inputs->pic = pic;
        inputs->tId = tId;
        inputs->numthreads = numthreads;

        pthread_create(&threads[tId], NULL, &partialDraw, (void*)inputs);
    }

    for(int tId = 0; tId < numthreads; tId++){
        pthread_join(threads[tId], NULL);
    }
}

void* partialDraw(void* ptr){
    arg* myArgs = (arg*)ptr;

    float red, green, blue;
    float fragRed, fragGreen, fragBlue;
    
    for(int y = myArgs->tId; y < myArgs->pic->height(); y+=myArgs->numthreads)
    for(int x = 0; x < myArgs->pic->width(); x++){
        
        red = 0; green = 0; blue = 0;
        for(float fragX = x; fragX < x+1; fragX += sqrtf(SUPERSAMPLE)/SUPERSAMPLE)
        for(float fragY = y; fragY < y+1; fragY += sqrtf(SUPERSAMPLE)/SUPERSAMPLE){

            Ray viewRay = {{fragX,fragY, -1000}, {0,0,1}};
            shootRay(viewRay, *(myArgs->myScene), fragRed, fragGreen, fragBlue, 1.0/SUPERSAMPLE);

            red += fragRed;
            green += fragGreen;
            blue += fragBlue;
        }
        myArgs->pic->set_pixel(x,y, min(red*255.0f, 255.0f), min(green*255.0f, 255.0f), min(blue*255.0f, 255.0f));
    }
    pthread_exit(NULL);
}
   

void draw(Scene& myScene, bitmap_image& pic){
    
    float red, green, blue;
    float fragRed, fragGreen, fragBlue;

    for(int y = 0; y < pic.height(); y++)
    for(int x = 0; x < pic.width(); x++){
        
        red = 0; green = 0; blue = 0;
        for(float fragX = x; fragX < x+1; fragX += sqrtf(SUPERSAMPLE)/SUPERSAMPLE)
        for(float fragY = y; fragY < y+1; fragY += sqrtf(SUPERSAMPLE)/SUPERSAMPLE){

            Ray viewRay = {{fragX,fragY, -1000}, {0,0,1}};
            shootRay(viewRay, myScene, fragRed, fragGreen, fragBlue, 1.0/SUPERSAMPLE);

            red += fragRed;
            green += fragGreen;
            blue += fragBlue;
        }
        pic.set_pixel(x,y, min(red*255.0f, 255.0f), min(green*255.0f, 255.0f), min(blue*255.0f, 255.0f));
    }
}

void shootRay(Ray& viewRay, Scene& myScene, float& r, float& g, float& b, float coef){
    
    r = 0; g = 0; b = 0;
    int depth = 0;

    do{
        float t;
        int currentObj = -1;
        float minT = FLT_MAX;

        for(unsigned int i = 0; i < myScene.spheres.size(); ++i){
            if(myScene.spheres[i].hitSphere(viewRay, t) && t <= minT){
                currentObj = i;
                minT = t;
            }
        }
        t = minT;

        // Break if no intersections found.
        if(currentObj == -1){ break; }

        Point intersection = viewRay.start + t*viewRay.dir;

        Vector norm = intersection - myScene.spheres[currentObj].pos;
        float temp = norm * norm;
        if (temp == 0.0f){ break; }

        temp = 1.0f / sqrtf(temp);
        norm = temp * norm;

        Material currentMat = myScene.materials[myScene.spheres[currentObj].materialId];
        
        for (unsigned int j = 0; j < myScene.lights.size(); ++j) {
            Light current = myScene.lights[j];
            Vector dist = current.pos - intersection;

            if (norm * dist <= 0.0f){ continue; }
            
            float t = sqrtf(dist * dist);
            
            if ( t <= 0.0f ){ continue; }
            Ray lightRay;
            lightRay.start = intersection;
            lightRay.dir = (1/t) * dist;
            // computation of the shadows
            bool inShadow = false; 
            for (unsigned int i = 0; i < myScene.spheres.size(); ++i) {
                if (myScene.spheres[i].hitSphere(lightRay, t)) {
                    inShadow = true;
                    break;
                }
            }
            if (!inShadow) {

                 float lambert = (lightRay.dir * norm) * coef;
                 r += lambert * current.red * currentMat.red;
                 g += lambert * current.green * currentMat.green;
                 b += lambert * current.blue * currentMat.blue;
            }

        }

        // We iterate on the next reflection
        coef *= currentMat.reflection;
        float reflect = 2.0f * (viewRay.dir * norm);
        viewRay.start = intersection;
        viewRay.dir = viewRay.dir - reflect * norm;

        depth++;
    }while(coef > 0.0f && depth < 10);
}

