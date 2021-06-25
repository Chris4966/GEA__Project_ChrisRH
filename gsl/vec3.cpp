#include "vec3.h"
#include "math_constants.h"
#include "vec2.h"
#include <cmath>

namespace gsl
{

    vec3::vec3(GLfloat x_in, GLfloat y_in, GLfloat z_in) : x{x_in}, y{y_in}, z{z_in}
    {    }

    vec3::vec3(const int v) : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}
    {    }

    vec3::vec3(const double v) : x{static_cast<float>(v)}, y{static_cast<float>(v)}, z{static_cast<float>(v)}
    {    }

    const vec3& vec3::operator=(const vec3 &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;

        return *this;
    }

    vec3 vec3::operator+(const vec3 &rhs) const
    {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    vec3 vec3::operator-(const vec3 &rhs) const
    {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    vec3& vec3::operator+=(const vec3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }

    vec3& vec3::operator-=(const vec3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return *this;
    }

    GLfloat vec3::operator*(const vec3 &rhs) const
    {
        return x*rhs.x + y*rhs.y + z*rhs.z;
    }

    vec3 vec3::operator-() const
    {
        return {-x, -y, -z};
    }

    vec3 vec3::operator*(GLfloat rhs) const
    {
        return {x * rhs, y * rhs, z * rhs};
    }

    vec3 vec3::operator/(GLfloat rhs) const
    {
        return {x / rhs, y / rhs, z / rhs};
    }

    vec3 vec3::operator^(const vec3 &rhs) const
    {
        return {y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
    }

    bool vec3::operator>=(const vec3 &rhs) const
    {
        return x >= rhs.x && y >= rhs.y && z >= rhs.z;
    }

    bool vec3::operator<=(const vec3 &rhs) const
    {
        return x <= rhs.x && y <= rhs.y && z <= rhs.z;
    }

    bool vec3::operator==(const vec3 &rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    GLfloat vec3::length() const
    {
        return std::sqrt(std::pow(x, 2.f) + std::pow(y, 2.f) + std::pow(z, 2.f));
    }

    GLfloat vec3::lengthNoSqrt() const
    {
        return (std::pow(x, 2.f) + std::pow(y, 2.f) + std::pow(z, 2.f));
    }

    void vec3::normalize()
    {
        GLfloat l = length();

        if (l > 0.f)
        {
            x = x / l;
            y = y / l;
            z = z / l;
        }
    }

    vec3 vec3::normalized()
    {
        vec3 normalized;
        GLfloat l = length();

        if (l > 0.f)
        {
            normalized.x = x / l;
            normalized.y = y / l;
            normalized.z = z / l;
        }

        return normalized;
    }

    vec3 vec3::cross(const vec3 &v1, const vec3 &v2)
    {
        return {((v1.y * v2.z) - (v1.z * v2.y)), ((v1.z * v2.x) - (v1.x * v2.z)), ((v1.x * v2.y) - (v1.y * v2.x))};
    }

    GLfloat vec3::dot(const vec3 &v1, const vec3 &v2)
    {
        return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
    }

    void vec3::clamp(vec3 v, vec3 min, vec3 max)
    {
        x = std::max(min.x, std::min(max.x, v.x));
        y = std::max(min.y, std::min(max.y, v.y));
        z = std::max(min.z, std::min(max.z, v.z));
    }

    void vec3::rotateX(GLfloat angle)
    {
        vec3 dir;
        angle = gsl::deg2rad(angle);

        dir.y = std::cos(angle) * y - std::sin(angle) * z;
        dir.z = std::sin(angle) * y + std::cos(angle) * z;

        y = dir.y;
        z = dir.z;
    }

    void vec3::rotateY(GLfloat angle)
    {
        vec3 dir;
        angle = gsl::deg2rad(angle);

        dir.x = std::cos(angle) * x + std::sin(angle) * z;
        dir.z = std::cos(angle) * z - std::sin(angle) * x;

        x = dir.x;
        z = dir.z;
    }

    void vec3::rotateZ(GLfloat angle)
    {
        vec3 dir;
        angle = gsl::deg2rad(angle);

        dir.x = std::cos(angle) * x - std::sin(angle) * y;
        dir.y = std::sin(angle) * x + std::cos(angle) * y;

        x = dir.x;
        y = dir.y;
    }

    GLfloat *vec3::xP()
    {
        return &x;
    }

    GLfloat *vec3::yP()
    {
        return &y;
    }

    GLfloat *vec3::zP()
    {
        return &z;
    }

    // https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    // S.R.H. found this discussion : twice as fast as the regular version!
    float vec3::barycentricHeight(const vec3 &point, const vec3 &corner1, const vec3 &corner2, const vec3 &corner3)
    {
        // This calculation assumes that height is in the Y axis.
        // We also assume that the point is projected down the Y axis onto the triangle
        // Because of this we flatten the problem down to 2D
        vec2 p{point.x, point.z};
        vec2 a{corner1.x, corner1.z};
        vec2 b{corner2.x, corner2.z};
        vec2 c{corner3.x, corner3.z};

        vec2 v0 = b - a;
        vec2 v1 = c - a;
        vec2 v2 = p - a;
        float d00 = vec2::dot(v0, v0);
        float d01 = vec2::dot(v0, v1);
        float d11 = vec2::dot(v1, v1);
        float d20 = vec2::dot(v2, v0);
        float d21 = vec2::dot(v2, v1);
        float invDenom = 1.0f / (d00 * d11 - d01 * d01);

        // u, v, w are the barycentric coordinates
        float v = (d11 * d20 - d01 * d21) * invDenom;
        float w = (d00 * d21 - d01 * d20) * invDenom;
        float u = 1.0f - v - w;

//        qDebug() << "BaryCords:" << u << v << w;

        float heightOut;
        if(u >= 0.f && v >= 0.f && w >= 0.f )
            heightOut = corner1.y * u + corner2.y * v + corner3.y * w;
        else
            heightOut = -10000.f;

        return heightOut;
    }

} //namespace
