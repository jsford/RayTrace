#ifndef __SCENE_H
#define __SCENE_H

#include <vector>       
#include "raytrace.h"       

class Scene{
    public:
        std::vector<Material> materials;
        std::vector<Sphere> spheres;
        std::vector<Light> lights;
        int sizex, sizey;
        
        Scene(){
            sizex = 640;
            sizey = 480;

            Material m0 = {.5, 1.0f, 1.0f, 0.0f};
            Material m1 = {.5, 0.0f, 1.0f, 1.0f};
            Material m2 = {.5, 1.0f, 0.0f, 1.0f};

            materials.push_back(m0);
            materials.push_back(m1);
            materials.push_back(m2);

            Sphere s0 = {{233, 290, 0.0f}, 100.0f, 0};
            Sphere s1 = {{407, 290, 0.0f}, 100.0f, 1};
            Sphere s2 = {{320, 140, 0.0f}, 100.0f, 2};

            spheres.push_back(s0);
            spheres.push_back(s1);  
            spheres.push_back(s2);

            Light l0 = {{0.0f,240,-100.0f},1,1,1};
            Light l1 = {{640, 240, -10000},.6,.7,1};

            lights.push_back(l0);
            lights.push_back(l1);     
        }
};




#endif
