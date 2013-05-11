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
};

#endif
