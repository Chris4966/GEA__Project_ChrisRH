#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include "vec3.h"
#include "gltypes.h"
#include <utility>
#include <iomanip>

namespace gsl
{

//class Matrix2x2;
class mat4;

class mat3
{
public:
    mat3(bool isIdentity = false);
    mat3(std::initializer_list<GLfloat> values);

    mat3 identity();
    void setToIdentity();

    GLfloat determinant();

    bool inverse();

    void transpose();

    mat4 toMatrix4();

    mat3 operator*(const mat3 &other);
    vec3 operator*(const vec3 &v);
    GLfloat& operator()(int y, int x);
    GLfloat operator()(int y, int x) const;

    friend std::ostream& operator<<(std::ostream &output, const mat3 &mIn)
    {
        output << std::setprecision(4) <<
                  "{" << mIn.matrix[0] << "\t, " << mIn.matrix[1] << "\t, " << mIn.matrix[2] << "}\n" <<
                  "{" << mIn.matrix[3] << "\t, " << mIn.matrix[4] << "\t, " << mIn.matrix[5] << "}\n" <<
                  "{" << mIn.matrix[6] << "\t, " << mIn.matrix[7] << "\t, " << mIn.matrix[8] << "}\n";
        return output;
    }

private:
    GLfloat matrix[9]{};
};

} //namespace

#endif // MATRIX3X3_H
