#include "camera.h"

#include "math_constants.h"
#include <QDebug>

Camera::Camera()
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();
}

void Camera::pitch(float degrees)
{
    //  rotate around mRight
    mPitch -= degrees;
    updateForwardVector();
}

void Camera::yaw(float degrees)
{
    // rotate around mUp ?
    // rotate around world up?
    mYaw -= degrees;
    updateRightVector();
    updateForwardVector();
}

void Camera::updateRightVector()
{
    //Since camera is not allowed to roll, right will always be in XZ plane
    mRight = gsl::vec3{1.f, 0.f, 0.f};      //gsl::RIGHT
    mRight.rotateY(mYaw);
    mRight.normalize();
}

void Camera::updateForwardVector()
{
    mForward = gsl::vec3{0.f, 0.f, -1.f};       //gsl::FORWARD;
    mForward.rotateX(mPitch);
    mForward.rotateY(mYaw);     //Rotate around world up
    mForward.normalize();

    mUp = mRight^mForward;
}

void Camera::update()
{
    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    mPitchMatrix.rotateX(mPitch);
    mYawMatrix.rotateY(mYaw);

    mPosition -= mForward * mSpeed;

    mViewMatrix = mPitchMatrix* mYawMatrix;
    mViewMatrix.translate(-mPosition);

    createFrustum(mProjectionMatrix, mViewMatrix);
}

void Camera::setPosition(const gsl::vec3 &position)
{
    mPosition = position;
}

void Camera::setRotation(const gsl::vec3 &rotation)
{
    mYaw = rotation.y/2;
    mPitch = -rotation.z;
    updateRightVector();
    updateForwardVector();
}

void Camera::setSpeed(float speed)
{
    mSpeed = speed;
}

void Camera::updateHeigth(float deltaHeigth)
{
    mPosition.y += deltaHeigth;
}

void Camera::moveRight(float delta)
{
    mPosition += mRight * delta;
}

gsl::vec3 Camera::position() const
{
    return mPosition;
}

gsl::vec3 Camera::up() const
{
    return mUp;
}

void Camera::createFrustum(gsl::mat4 pMatrix, gsl::mat4 vMatrix)
{
    gsl::mat4 clPlanes = pMatrix * vMatrix;

    //R frustum plane
    mFrustum[0] =
    {
        clPlanes.getColVec(1).w - clPlanes.getColVec(1).x,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).x,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).x,
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).x
    };
    mFrustum[0].normalize();

    //L frustum plane
    mFrustum[1] =
    {
        clPlanes.getColVec(1).w + clPlanes.getColVec(1).x,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).x,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).x,
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).x
    };
    mFrustum[1].normalize();

    //Bt Frustum plane
    mFrustum[2] =
    {
        clPlanes.getColVec(1).w + clPlanes.getColVec(1).y,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).y,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).y,
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).y
    };
    mFrustum[2].normalize();

    //Tp Frustum plane
    mFrustum[3] =
    {
        clPlanes.getColVec(1).w - clPlanes.getColVec(1).y,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).y,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).y,
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).y
    };
    mFrustum[3].normalize();

    //B Frustum plane
    mFrustum[4] =
    {
        clPlanes.getColVec(1).w - clPlanes.getColVec(1).z,
        clPlanes.getColVec(2).w - clPlanes.getColVec(2).z,
        clPlanes.getColVec(3).w - clPlanes.getColVec(3).z,
        clPlanes.getColVec(4).w - clPlanes.getColVec(4).z
    };
    mFrustum[4].normalize();

    //F Frustum plane
    mFrustum[5] =
    {
        clPlanes.getColVec(1).w + clPlanes.getColVec(1).z,
        clPlanes.getColVec(2).w + clPlanes.getColVec(2).z,
        clPlanes.getColVec(3).w + clPlanes.getColVec(3).z,
        clPlanes.getColVec(4).w + clPlanes.getColVec(4).z
    };
    mFrustum[5].normalize();
}


bool Camera::frustumCull(gsl::vec3 pt)
{
    for (int i{0}; i < 6; ++i)
    {
        if (mFrustum[i].x * pt.x + mFrustum[i].y * pt.y + mFrustum[i].z + pt.z + mFrustum[i].w <= 0)
            return false;
    }

    return true;
}

bool Camera::frustumCull(ecs::Collider OBB, gsl::vec3 scale)
{
    bool bInFrustum{false};
    gsl::vec3 v = scale * OBB.mMinCenter;
    bInFrustum = frustumCull(v);

    return bInFrustum;
}
