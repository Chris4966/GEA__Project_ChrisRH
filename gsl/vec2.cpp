#include "vec2.h"
#include "vec3.h"
#include <cmath>

namespace gsl
{
    vec2::vec2(GLfloat x_in, GLfloat y_in) : x{x_in}, y{y_in}
    {    }

    vec2::vec2(const int v) : x{static_cast<GLfloat>(v)}, y{static_cast<GLfloat>(v)}
    {    }

    vec2::vec2(const double v) : x{static_cast<GLfloat>(v)}, y{static_cast<GLfloat>(v)}
    {    }

    const vec2& vec2::operator=(const vec2 &rhs)
    {
        x = rhs.x;
        y = rhs.y;

        return *this;
    }

    vec2 vec2::operator+(const vec2 &rhs) const
    {
        return {x + rhs.x, y + rhs.y};
    }

    vec2 vec2::operator-(const vec2 &rhs) const
    {
        return {x - rhs.x, y - rhs.y};
    }

    vec2& vec2::operator+=(const vec2 &rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    vec2& vec2::operator-=(const vec2 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    vec2 vec2::operator-() const
    {
        return {-x, -y};
    }

    vec2 vec2::operator*(float lhs) const
    {
        return {x * lhs, y * lhs};
    }

    vec2 vec2::operator*(vec2 lhs) const
    {
        return {x * lhs.x, y * lhs.y};
    }

    GLfloat vec2::length() const
    {
        return std::sqrt(std::pow(x, 2.f) + std::pow(y, 2.f));
    }

    void vec2::normalize()
    {
        GLfloat l = length();

        if (l > 0.f)
        {
            x = (x / l);
            y = (y / l);
        }
    }

    vec2 vec2::normalized()
    {
        vec2 normalized;
        GLfloat l = length();

        if (l > 0.f)
        {
            normalized.setX(x / l);
            normalized.setY(y / l);
        }

        return normalized;
    }

    GLfloat vec2::cross(const vec2 &v1, const vec2 &v2)
    {
        return std::abs((v1.x * v2.y) - (v1.y * v2.x));
    }

    GLfloat vec2::dot(const vec2  &v1, const vec2  &v2)
    {
        return ((v1.x * v2.x) + (v1.y * v2.y));
    }

    GLfloat vec2::getX() const
    {
        return x;
    }

    void vec2::setX(const GLfloat &value)
    {
        x = value;
    }

    GLfloat vec2::getY() const
    {
        return y;
    }

    void vec2::setY(const GLfloat &value)
    {
        y = value;
    }
} //namespace
