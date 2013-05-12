#ifndef __RAYTRACE_H
#define __RAYTRACE_H

class bitmap_image;
class Ray;
class Scene;
class Point;

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

        bool hitSphere(const Ray& r, float& t);
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

// The following functions use pthreads to parrallelize the rendering.
void* partialDraw(void* ptr);
void parallelDraw(Scene* myScene, bitmap_image* pic, int numthreads);

struct arg{
    Scene* myScene;
    bitmap_image* pic; 
    int numthreads;
    int tId;
};

// The draw function is a serial version of the renderer.
void draw(const Scene& myScene, bitmap_image& pic);

// Shoot a ray into the scene and find the color that it returns.
void shootRay(Ray& viewRay, Scene& myScene, float& r, float& g, float& b, float coeff);

#endif







