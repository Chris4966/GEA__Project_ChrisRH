#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "vec3.h"
#include "vec4.h"
#include "gltypes.h"
#include <iostream>
#include <iomanip>

namespace gsl
{
//class Matrix2x2;
class mat3;

class mat4
{
public:
    mat4(bool isIdentity = false);
    mat4(std::initializer_list<GLfloat> values);

    mat4 identity();
    void setToIdentity();

    bool inverse();

    void translateX(GLfloat x = 0.f);
    void translateY(GLfloat y = 0.f);
    void translateZ(GLfloat z = 0.f);

    void setPosition(GLfloat x = 0.f, GLfloat y = 0.f, GLfloat z = 0.f);
    void setPosition(const vec3 &positionIn);
    gsl::vec3 getPosition();

    //Rotate using EulerMatrix
    void rotateX(GLfloat degrees = 0.f);
    void rotateY(GLfloat degrees = 0.f);
    void rotateZ(GLfloat degrees = 0.f);
//    void rotate(GLfloat angle, Vector3D vector);
//    void rotate(GLfloat angle, GLfloat xIn, GLfloat yIn, GLfloat zIn);

    void scale(vec3 s);
    void scale(GLfloat uniformScale);
    void scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);

    GLfloat* constData();

    void transpose();

    void ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat nearPlane, GLfloat farPlane);
    void frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    void perspective(GLfloat fieldOfView, GLfloat aspectRatio, GLfloat nearPlane, GLfloat farPlane);

    void lookAt(const vec3 &eye, const vec3 &center, const vec3 &up_axis);

    void setRotationToVector(const vec3 &direction, vec3 up = vec3(0.f,1.f,0.f));

    void translate(GLfloat x = 0.f, GLfloat y = 0.f, GLfloat z = 0.f);
    void translate(vec3 positionIn);

    mat3 toMatrix3() const;

    GLfloat& operator()(const int &y, const int &x);
    GLfloat operator()(const int &y, const int &x) const;

    mat4 operator*(const mat4 &other);
    vec4 operator*(const vec4 &v);

    friend std::ostream& operator<<(std::ostream &output, const mat4 &mIn)
    {
        output << std::setprecision(4) <<
                  "{" << mIn.matrix[0] << "\t, " << mIn.matrix[4] << "\t, " << mIn.matrix[8] << "\t, " << mIn.matrix[12] << "}\n" <<
                  "{" << mIn.matrix[1] << "\t, " << mIn.matrix[5] << "\t, " << mIn.matrix[9] << "\t, " << mIn.matrix[13] << "}\n" <<
                  "{" << mIn.matrix[2] << "\t, " << mIn.matrix[6] << "\t, " << mIn.matrix[10] << "\t, " << mIn.matrix[14] << "}\n" <<
                  "{" << mIn.matrix[3] << "\t, " << mIn.matrix[7] << "\t, " << mIn.matrix[11] << "\t, " << mIn.matrix[15] << "}\n";
        return output;
    }
    GLfloat getFloat(int space);
    vec4 getRowVec(int r);
    vec4 getColVec(int c);

private:
    GLfloat matrix[16]{};
};

} //namespace

#endif // MATRIX4X4_H
