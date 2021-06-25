#include "vec4.h"
#include "math_constants.h"
#include "vec3.h"
#include "vec2.h"
#include <cmath>

namespace gsl
{

vec4::vec4(GLfloat x_in, GLfloat y_in, GLfloat z_in, GLfloat w_in) : x{x_in}, y{y_in}, z{z_in}, w{w_in}
{

}

vec4::vec4(const int v) : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}, w{static_cast<float>(v)}
{

}

vec4::vec4(const double v) : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}, w{static_cast<float>(v)}
{

}

const vec4 &vec4::operator=(const vec4 &rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
    return *this;
}

vec4 vec4::operator+(const vec4 &rhs) const
{
    return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
}

vec4 vec4::operator-(const vec4 &rhs) const
{
    return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
}

vec4 &vec4::operator+=(const vec4 &rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}

vec4 &vec4::operator-=(const vec4 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}

GLfloat vec4::operator*(const vec4 &rhs) const
{
    return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

vec4 vec4::operator-() const
{
    return {-x, -y, -z, -w};
}

vec4 vec4::operator*(GLfloat rhs) const
{
    return {x * rhs, y * rhs, z * rhs, w * rhs};
}

bool vec4::operator>=(const vec4 &rhs) const
{
    return x >= rhs.x && y >= rhs.y && z >= rhs.z && w >= rhs.w;
}

bool vec4::operator<=(const vec4 &rhs) const
{
    return x <= rhs.x && y <= rhs.y && z <= rhs.z && w <= rhs.w;
}

bool vec4::operator==(const vec4 &rhs) const
{
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

GLfloat vec4::length() const
{
    return std::sqrt(std::pow(x, 2) + std::pow (y, 2) + std::pow(z, 2) + std::pow(w, 2));
}

GLfloat vec4::lengthNoSqrt() const
{
    return std::pow(x, 2) + std::pow (y, 2) + std::pow(z, 2) + std::pow(w, 2);
}

void vec4::normalize()
{
    GLfloat l = length();
    if (l > 0.f)
    {
        x = x/l;
        y = y/l;
        z = z/l;
        w = w/l;
    }
}

vec4 vec4::normalized()
{
    vec4 n;
    GLfloat l = length();
    if (l > 0.f)
    {
        n.x = x/l;
        n.y = y/l;
        n.z = z/l;
        n.w = w/l;
    }
    return n;
}

GLfloat vec4::dot(const vec4 &v1, const vec4 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

void vec4::clamp(vec4 v, vec4 min, vec4 max)
{
    x = std::max(min.x, std::min(max.x, v.x));
    y = std::max(min.y, std::min(max.y, v.y));
    z = std::max(min.z, std::min(max.z, v.z));
    w = std::max(min.w, std::min(max.w, v.w));
}

GLfloat *vec4::xP()
{
    return &x;
}

GLfloat *vec4::yP()
{
    return &y;
}

GLfloat *vec4::zP()
{
    return &z;
}

GLfloat *vec4::wP()
{
    return &w;
}
}
