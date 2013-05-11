#ifndef __RAYTRACE_H
#define __RAYTRACE_H

#include <string>

#include "defs.h"
#include "bitmap_image.h"

class Scene;

class Material{
    public:
        float reflection;
        float red, green, blue;
};

class Sphere{
    public:
        Point pos;
        float rad;
        int materialId;
};

class Light{
    public:
        Point pos;
        float red, green, blue;
};

class Ray{
    public:
        Point start;
        Vector dir;
};

bool hitSphere(const Ray& r, const Sphere& s, float& t);

void* partialDraw(void* ptr);
void parallelDraw(Scene* myScene, int numthreads);

void draw(Scene& myScene, bitmap_image& pic);
void shootRay(Ray& viewRay, Scene& myScene, float& r, float& g, float& b, float coeff);
#endif
