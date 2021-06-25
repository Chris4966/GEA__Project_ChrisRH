#ifndef VEC4_H
#define VEC4_H

#include "gltypes.h"
#include <cmath>
#include <iostream>
#include <QDebug>
#include <QVector4D>

namespace gsl
{

class vec4
{
public:
    //Constructors
    vec4(GLfloat x_in = 0.f, GLfloat y_in = 0.f, GLfloat z_in = 0.f, GLfloat w_in = 0.f);
    vec4(const int v);
    vec4(const double v);

    //Operators
    const vec4& operator=(const vec4 &rhs);     // v = v
    vec4 operator+(const vec4 &rhs) const;      // v + v
    vec4 operator-(const vec4 &rhs) const;      // v - v
    vec4& operator+=(const vec4 &rhs);          // v += v
    vec4& operator-=(const vec4 &rhs);          // v -= v
    vec4 operator-() const;                     // -v
    vec4 operator*(GLfloat rhs) const;          // v * f                                    //GLfloat operatoren funker ikke med denne???
    vec4 operator/(GLfloat rhs) const;
    GLfloat operator*(const vec4& rhs) const;  // v * v
    bool operator >=(const vec4 &rhs) const; // v >= v
    bool operator <=(const vec4 &rhs) const; // v <= v
    bool operator ==(const vec4 &rhs) const; // v == v

    //Functions
    GLfloat length() const;
    GLfloat lengthNoSqrt() const;
    void normalize();
    vec4 normalized();
    static GLfloat dot(const vec4 &v1, const vec4 &v2);
    void clamp(vec4 v, vec4 min, vec4 max);

    GLfloat *xP();
    GLfloat *yP();
    GLfloat *zP();
    GLfloat *wP();

    //Friend functions
    friend std::ostream& operator<<(std::ostream &output, const vec4 &rhs )
    {
        output << "X = " << rhs.x << ", Y = " << rhs.y << ", Z = " << rhs.z << ", W = " << rhs.w;
        return output;
    }

    //OEF: Adding these again to be able to debug easier. cout does not print until program is finished!
    friend QDebug operator<<(QDebug out, const vec4 &rhs)
    {
        out << "(" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << ")";
        return  out;
    }        //    QVector3D getQVector() const;   //for use with qDebug()

    QVector4D getQVector() const
    {
        return QVector4D(x, y, z, w);
    }   //for use with qDebug()


    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
};

}

#endif // VEC4_H
