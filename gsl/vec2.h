#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "gltypes.h"
#include <cmath>
#include <iostream>

namespace gsl
{

class vec2
{
public:
    //Constructors
    vec2(GLfloat x_in = 0.f, GLfloat y_in = 0.f);
    vec2(const int v);
    vec2(const double v);

    //Operators
    vec2 operator+(const vec2 &rhs) const;      // v + v
    vec2 operator-(const vec2 &rhs) const;      // v - v
    vec2& operator+=(const vec2 &rhs);          // v += v
    vec2& operator-=(const vec2 &rhs);          // v -= v
    vec2 operator-() const;                     // -v
    vec2 operator*(GLfloat lhs) const;          // v * f
    vec2 operator*(vec2 lhs) const;
    const vec2& operator =(const vec2 &rhs);    // v = v

    //Functions
    GLfloat length() const;
    void normalize();
    vec2 normalized();
    static GLfloat cross(const vec2 &v1, const vec2 &v2);
    static GLfloat dot(const vec2 &v1, const vec2 &v2);

    //Getters and setters
    GLfloat getX() const;
    void setX(const GLfloat &value);

    GLfloat getY() const;
    void setY(const GLfloat &value);

    //Friend functions
    friend std::ostream& operator<<(std::ostream &output, const vec2 &rhs)
    {
        output << "(" << rhs.x << "," << rhs.y << ")";
        return output;
    }

    GLfloat x;
    GLfloat y;
};

} //namespace

#endif // VECTOR2D_H
