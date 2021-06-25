#include "mat3x3.h"
#include "mat4x4.h"

namespace gsl
{
mat3::mat3(bool isIdentity)
{
    if(isIdentity)
    {
        identity();
    }
    else
    {
        for(int i = 0; i < 4; i++)
            matrix[i] = 0.f;
    }
}

mat3::mat3(std::initializer_list<GLfloat> values)
{
    int i = 0;
    for(auto value : values)
        matrix[i++] = value;
}

mat3 mat3::identity()
{
    setToIdentity();

    return *this;
}

void mat3::setToIdentity()
{
    *this =
    {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
}

GLfloat mat3::determinant()
{
    //det = a(ei − fh) − b(di − fg) + c(dh − eg)
    //where
    //    a, b, c,
    //    d, e, f
    //    g, h, i

    return matrix[0]*(matrix[4]*matrix[8]-matrix[5]*matrix[7])
            - matrix[1]*(matrix[3]*matrix[8]-matrix[5]*matrix[6])
            + matrix[2]*(matrix[3]*matrix[7]-matrix[4]*matrix[6]);
}

bool mat3::inverse()
{
    GLfloat a = (*this)(0, 0), b = (*this)(0, 1), c = (*this)(0, 2),
            d = (*this)(1, 0), e = (*this)(1, 1), f = (*this)(1, 2),
            g = (*this)(2, 0), h = (*this)(2, 1), i = (*this)(2, 2);

    GLfloat A =  (e*i-f*h), B = -(d*i-f*g), C =  (d*h-e*g),
            D = -(b*i-c*h), E =  (a*i-c*g), F = -(a*h-b*g),
            G =  (b*f-c*e), H = -(a*f-c*d), I =  (a*e-b*d);

    GLfloat det = determinant();

    if(det == 0.f)
        return false;

    det = 1.f/det;

    *this =
    {
        det*A, det*D, det*G,
                det*B, det*E, det*H,
                det*C, det*F, det*I
    };

    return true;
}

void mat3::transpose()
{
    std::swap(matrix[1], matrix[4]);
    std::swap(matrix[2], matrix[6]);
    std::swap(matrix[5], matrix[7]);
}

mat4 mat3::toMatrix4()
{
    return mat4
    {
        matrix[0], matrix[1], matrix[2], 0,
                matrix[3], matrix[4], matrix[5], 0,
                matrix[6], matrix[7], matrix[8], 0,
                0,        0,         0,      0
    };
}

mat3 mat3::operator*(const mat3 &other)
{
    return
    {
        matrix[0] * other.matrix[0] + matrix[1] * other.matrix[3] + matrix[2] * other.matrix[6],
                matrix[0] * other.matrix[1] + matrix[1] * other.matrix[4] + matrix[2] * other.matrix[7],
                matrix[0] * other.matrix[2] + matrix[1] * other.matrix[5] + matrix[2] * other.matrix[8],

                matrix[3] * other.matrix[0] + matrix[4] * other.matrix[3] + matrix[5] * other.matrix[6],
                matrix[3] * other.matrix[1] + matrix[4] * other.matrix[4] + matrix[5] * other.matrix[7],
                matrix[3] * other.matrix[2] + matrix[4] * other.matrix[5] + matrix[5] * other.matrix[8],

                matrix[6] * other.matrix[0] + matrix[7] * other.matrix[3] + matrix[8] * other.matrix[6],
                matrix[6] * other.matrix[1] + matrix[7] * other.matrix[4] + matrix[8] * other.matrix[7],
                matrix[6] * other.matrix[2] + matrix[7] * other.matrix[5] + matrix[8] * other.matrix[8]
    };
}

vec3 mat3::operator*(const vec3 &v)
{
    return vec3(matrix[0] * v.x + matrix[1] * v.y + matrix[2] * v.z,
            matrix[3] * v.x + matrix[4] * v.y + matrix[5] * v.z,
            matrix[6] * v.x + matrix[7] * v.y + matrix[8] * v.z);
}

GLfloat& mat3::operator()(int y, int x)
{
    return matrix[y * 3 + x];
}

GLfloat mat3::operator()(int y, int x) const
{
    return matrix[y * 3 + x];
}

} //namespace
