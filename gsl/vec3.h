#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "gltypes.h"
#include <cmath>
#include <iostream>
#include <QDebug>
#include <QVector3D>

namespace gsl
{

class vec3
{
public:
    //Constructors
    vec3(GLfloat x_in = 0.f, GLfloat y_in = 0.f, GLfloat z_in = 0.f);
    vec3(const int v);
    vec3(const double v);

    //Operators
    const vec3& operator=(const vec3 &rhs);     // v = v
    vec3 operator+(const vec3 &rhs) const;      // v + v
    vec3 operator-(const vec3 &rhs) const;      // v - v
    vec3& operator+=(const vec3 &rhs);          // v += v
    vec3& operator-=(const vec3 &rhs);          // v -= v
    vec3 operator-() const;                     // -v
    vec3 operator*(GLfloat rhs) const;          // v * f                                       //GLfloat operatoren funker ikke med denne???
    vec3 operator/(GLfloat rhs) const;
    vec3 operator^(const vec3& rhs) const; // v x v  - cross product
    GLfloat operator*(const vec3& rhs) const;  // v * v
    bool operator >=(const vec3 &rhs) const; // v >= v
    bool operator <=(const vec3 &rhs) const; // v <= v
    bool operator ==(const vec3 &rhs) const; // v == v


    //Functions
    GLfloat length() const;
    GLfloat lengthNoSqrt() const;
    void normalize();
    vec3 normalized();
    static vec3 cross(const vec3 &v1, const vec3 &v2);
    static GLfloat dot(const vec3 &v1, const vec3 &v2);
    void clamp(vec3 v, vec3 min, vec3 max);

    ///Rotate around world X-axis
    void rotateX(GLfloat angle);

    ///Rotate around world Y-axis
    void rotateY(GLfloat angle);

    ///Rotate around world Z-axis
    void rotateZ(GLfloat angle);

    GLfloat *xP();
    GLfloat *yP();
    GLfloat *zP();

    static float barycentricHeight(const vec3 &point, const vec3 &corner1, const vec3 &corner2, const vec3 &corner3);

    //Friend functions
    friend std::ostream& operator<<(std::ostream &output, const vec3 &rhs )
    {
        output << "X = " << rhs.x << ", Y = " << rhs.y << ", Z = " << rhs.z;
        return output;
    }

    //OEF: Adding these again to be able to debug easier. cout does not print until program is finished!
    friend QDebug operator<<(QDebug out, const vec3 &rhs)
    {
        out << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
        return  out;
    }        //    QVector3D getQVector() const;   //for use with qDebug()

    QVector3D getQVector() const
    {
        return QVector3D(x, y, z);
    }   //for use with qDebug()

    //private:  //Making these public to avoid having to do things like setY(getY()+3) etc.
    GLfloat x;
    GLfloat y;
    GLfloat z;
};


} //namespace

#endif // VECTOR3D_H
