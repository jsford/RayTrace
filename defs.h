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

class Color{
    public:
        enum OFFSET
        {
            OFFSET_RED, 
            OFFSET_GREEN, 
            OFFSET_BLUE, 
            OFFSET_MAX
        };

        float red, green, blue;

        inline Color& operator+= (const Color& c2){
            red += c2.red;
            green += c2.green;
            blue += c2.blue;
            return *this;
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
inline Vector operator * (const Vector& v, float f){
    Vector retv = {v.x * f, v.y * f, v.z * f};
    return retv;
}

inline Vector operator * (float f, const Vector& v){
    Vector retv = {v.x * f, v.y * f, v.z * f};
    return retv;
}

// Vector = Vector * Vector
inline float operator * (const Vector& v1, const Vector& v2){
    float retv = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return retv;
}

// Color = Color * Color
inline Color operator * (const Color& c1, const Color &c2){
    Color retC = {c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
    return retC;
}

// Color = Constant * Color
inline Color operator * (float f, const Color &c){
    Color retC = {c.red * f, c.green * f, c.blue * f};
    return retC;
}

// Color = Color + Color
inline Color operator + (const Color& c1, const Color& c2){
    Color retC = {c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
    return retC;
} 

#endif
