#include "vertex.h"
#include <iostream>

Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t)
{
    mXYZ.x = x;
    mXYZ.y = y;
    mXYZ.z = z;

    mNormal.x = r;
    mNormal.y = g;
    mNormal.z = b;

    mST.x = s;
    mST.y = t;
}

Vertex::Vertex(gsl::vec3 a, gsl::vec3 b , gsl::vec2 c)
{
    mXYZ = a;
    mNormal = b;
    mST = c;
}

void Vertex::set_xyz(GLfloat *xyz)
{
    mXYZ.x = xyz[0];
    mXYZ.y = xyz[1];
    mXYZ.z = xyz[2];
}

void Vertex::set_xyz(GLfloat x, GLfloat y, GLfloat z)
{
    mXYZ.x = x;
    mXYZ.y = y;
    mXYZ.z = z;
}

void Vertex::set_xyz(gsl::vec3 xyz_in)
{
    mXYZ = xyz_in;
}

void Vertex::set_rgb(GLfloat *rgb)
{
    mNormal.x = rgb[0];
    mNormal.y = rgb[1];
    mNormal.z = rgb[2];
}

void Vertex::set_rgb(GLfloat r, GLfloat g, GLfloat b)
{
    mNormal.x = r;
    mNormal.y = g;
    mNormal.z = b;
}

void Vertex::set_normal(GLfloat *normal)
{
    mNormal.x = normal[0];
    mNormal.y = normal[1];
    mNormal.z = normal[2];
}

void Vertex::set_normal(GLfloat x, GLfloat y, GLfloat z)
{
    mNormal.x = x;
    mNormal.y = y;
    mNormal.z = z;
}

void Vertex::set_normal(gsl::vec3 normal_in)
{
    mNormal = normal_in;
}

void Vertex::set_st(GLfloat *st)
{
    mST.x = st[0];
    mST.y = st[1];
}

void Vertex::set_st(GLfloat s, GLfloat t)
{
    mST.x = s;
    mST.y = t;
}

void Vertex::set_uv(GLfloat u, GLfloat v)
{
    mST.x = u;
    mST.y = v;
}

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
    os << std::fixed;
    os << "(" << v.mXYZ.x << ", " << v.mXYZ.y << ", " << v.mXYZ.z << ") ";
    os << "(" << v.mNormal.x << ", " << v.mNormal.y << ", " << v.mNormal.z << ") ";
    os << "(" << v.mST.x << ", " << v.mST.y << ") ";
    return os;
}

std::istream& operator>> (std::istream& is, Vertex& v) {
    // needs 4 temps to get commas and parenthesis
    char temp, temp2, temp3, temp4;
    is >> temp >> v.mXYZ.x >> temp2 >> v.mXYZ.y >> temp3 >> v.mXYZ.z >> temp4;
    is >> temp >> v.mNormal.x >> temp2 >> v.mNormal.y >> temp3 >> v.mNormal.z >> temp4;
    is >> temp >> v.mST.x >> temp2 >> v.mST.y >> temp3;
    return is;
}
