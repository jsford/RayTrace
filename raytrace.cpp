#include <cmath>            // Needed for sqrtf() 
#include <cfloat>           // Needed for FLT_MAX
#include <pthread.h>        // Needed to parallelize the rendering.

#include "bitmap_image.h"   // Needed to manipulate the image.
#include "defs.h"           // Defines basic Point/Vector/Color classes.
#include "raytrace.h"       // Defines Light/Ray/Object classes.
#include "scene.h"          // Defines a Scene.

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

    for(float y = 0; y < myArgs->pic->height()/ZOOM; y+=1.0/ZOOM)
    for(float x = myArgs->tId/ZOOM; x < myArgs->pic->width()/ZOOM; x+=myArgs->numthreads/ZOOM){

        Color thisColor = {0.0, 0.0, 0.0};
        Ray viewRay = {{x,y, -1000}, {0,0,1}};
        thisColor = shootRay(viewRay, *(myArgs->myScene));

        myArgs->pic->set_pixel(x*ZOOM,y*ZOOM, round(min(thisColor.red*255.0f, 255.0f)), 
                                              round(min(thisColor.green*255.0f, 255.0f)), 
                                              round(min(thisColor.blue*255.0f, 255.0f)));
    }
    pthread_exit(NULL);
}

Color shootRay(Ray& viewRay, Scene& myScene){
    
    Color retColor = {0.0f, 0.0f, 0.0f};
    float coef = 1.0f;
    int depth = 0;

    do{
        float temp_t;
        int currentObj = -1;
        float t = FLT_MAX;

        for(unsigned int i = 0; i < myScene.spheres.size(); ++i){
            if(myScene.spheres[i].hitSphere(viewRay, temp_t) && temp_t <= t){
                currentObj = i;
                t = temp_t;
            }
        }

        // Break if no hitPts found.
        if(currentObj == -1){ break; }

        Point hitPt = viewRay.start + t*viewRay.dir;

        Vector norm = hitPt - myScene.spheres[currentObj].pos;
        Vector perturb = {10*(float)rand()/RAND_MAX, 10*(float)rand()/RAND_MAX, 10*(float)rand()/RAND_MAX};
        norm += perturb;
        float temp = norm * norm;
        if (temp == 0.0f){ break; }

        temp = 1.0f / sqrtf(temp);
        norm = temp * norm;

        Material currentMat = myScene.materials[myScene.spheres[currentObj].materialId];
        
        Ray lightRay;
        lightRay.start = hitPt;

        for (unsigned int j = 0; j < myScene.lights.size(); ++j) {
            Light current = myScene.lights[j];
            lightRay.dir = current.pos - hitPt;

            float fLightProjection = lightRay.dir * norm;

            if (fLightProjection <= 0.0f){ continue; }

            float lightDist = lightRay.dir * lightRay.dir;
            {
                float temp;
			    if ( lightDist == 0.0f )
				    continue;
                temp = 1.0/sqrtf(lightDist);
			    lightRay.dir = temp * lightRay.dir;
                fLightProjection = temp * fLightProjection;
            }
                        
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
                 retColor.red += lambert * current.intensity.red * currentMat.diffuse.red;
                 retColor.green += lambert * current.intensity.green * currentMat.diffuse.green;
                 retColor.blue += lambert * current.intensity.blue * currentMat.diffuse.blue;

                 float exposure = -1.2f;
                 retColor.red = 1.0 - expf(retColor.red * exposure);                
                 retColor.blue = 1.0 - expf(retColor.blue * exposure);
                 retColor.green = 1.0 - expf(retColor.green * exposure);

                 float fViewProjection = viewRay.dir * norm;
				 Vector blinnDir = lightRay.dir - viewRay.dir;
				 float temp = blinnDir * blinnDir;
				 if (temp != 0.0f )
				 {
					 float blinn = 1.0/sqrtf(temp) * max(fLightProjection - fViewProjection , 0.0f);
                     blinn = coef * powf(blinn, currentMat.power);
					 retColor += blinn * currentMat.specular * current.intensity;
				 }
            }

        }

        // We iterate on the next reflection
        coef *= currentMat.reflection;
        float reflect = 2.0f * (viewRay.dir * norm);
        viewRay.start = hitPt;
        viewRay.dir = viewRay.dir - reflect * norm;

        depth++;
    }while(coef > 0.0f && depth < 10);
    
    return retColor;
}

