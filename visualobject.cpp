#include "visualobject.h"
#include "shader.h"

VisualObject::~VisualObject()
{
    initializeOpenGLFunctions();
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void VisualObject::init()
{
}

void VisualObject::drawOutline()
{
    glBindVertexArray( mVAO );
    if(mIndiceCount > 0)
        glDrawElements(GL_TRIANGLES, mIndiceCount, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    glBindVertexArray(0);
}

void VisualObject::drawNormals(GLint matrixUniform)
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, mMatrix.constData());   //move to shader???
    if(mIndiceCount > 0)
        glDrawElements(GL_TRIANGLES, mIndiceCount, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    glBindVertexArray(0);
}

void VisualObject::drawMousePicker(GLint matrixUniform, GLint colorUniform, gsl::vec3 color)
{
    glBindVertexArray( mVAO );

    glUniformMatrix4fv(matrixUniform, 1, GL_TRUE, mMatrix.constData());   //move to shader???
    glUniform3f(colorUniform, color.x, color.y, color.z);
    if (mIndiceCount > 0)
        glDrawElements(GL_TRIANGLES, mIndiceCount, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}

void VisualObject::updateMatrix()
{
    mMatrix.setToIdentity();
    mMatrix.translate(mTransform.position);

    mMatrix.rotateX(mTransform.rotation.x);
    mMatrix.rotateY(mTransform.rotation.y);
    mMatrix.rotateZ(mTransform.rotation.z);

    mMatrix.scale(mTransform.scale);
}
