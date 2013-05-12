#ifndef __DEFS_H
#define __DEFS_H

#include <sstream>      // toString() uses stringstream to build strings.
#include <string>       // toString() returns std::string representation of class.

class Point{
    public:
    float x,y,z;

    std::string toString(){
        std::stringstream ss;
        ss << x << "," << y << "," << z;
        return ss.str();
    }
};

class Vector{
    public:
    float x, y, z;

    Vector& operator+= (const Vector &that){
        this->x += that.x;
        this->y += that.y;
        this->z += that.z;
        return *this;
    } 

    std::string toString(){
        std::stringstream ss;
        ss << x << "," << y << "," << z;
        return ss.str();
    }
};

// Point = Point + Vector
inline Point operator + (const Point& p, const Vector& v){
    Point retp = {p.x + v.x, p.y + v.y, p.z + v.z};
    return retp;
}

inline Point operator + (const Vector& v, const Point& p){
    Point retp = {p.x + v.x, p.y + v.y, p.z + v.z};
    return retp;
}

// Vector = Vector + Vector
inline Vector operator + (const Vector& v1, const Vector& v2){
    Vector retv = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    return retv;
}

// Vector = Point - Point
inline Vector operator - (const Point& p1, const Point& p2){
    Vector retv = {p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
    return retv;
}

// Point = Point - Vector
inline Point operator - (const Point& p, const Vector& v){
    Point retp = {p.x - v.x, p.y - v.y, p.z - v.z};
    return retp;
}

// Vector = Vector - Vector
inline Vector operator - (const Vector& v1, const Vector& v2){
    Vector retv = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    return retv;
}

// Vector = Vector * Constant
inline Vector operator * (const Vector& v, float c){
    Vector retv = {v.x * c, v.y * c, v.z * c};
    return retv;
}

inline Vector operator * (float c, const Vector& v){
    Vector retv = {v.x * c, v.y * c, v.z * c};
    return retv;
}

//Vector = Vector * Vector
inline float operator * (const Vector& v1, const Vector& v2){
    float retv = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return retv;
}

#endif
