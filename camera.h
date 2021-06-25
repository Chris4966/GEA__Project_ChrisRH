#ifndef CAMERA_H
#define CAMERA_H

#include "gslpch.h"
#include "ecs/components.h"

/**
    @brief Camera class.
 */

class Camera
{
public:
    Camera();

    void pitch(float degrees);
    void yaw(float degrees);
    void updateRightVector();
    void updateForwardVector();
    void update();

    gsl::mat4 mViewMatrix;
    gsl::mat4 mProjectionMatrix;

    void setPosition(const gsl::vec3 &position);
    void setRotation(const gsl::vec3 &rotation);
    void setSpeed(float speed);
    void updateHeigth(float deltaHeigth);
    void moveRight(float delta);

    gsl::vec3 position() const;
    gsl::vec3 up() const;

    /** Creates frustum planes using the Camera's view and projection matrices.
       @param pMatrix
       @param vMatrix
     */
    void createFrustum(gsl::mat4 pMatrix, gsl::mat4 vMatrix);

    /** Checks if a position is within the frustum
       @param pt - position
       @return - returns boolean
     */
    bool frustumCull(gsl::vec3 pt);

    /** Checks if a bounding box is within the frustum
       @param OBB - bounding box
       @param scale - scale of entity
       @return - returns boolean
     */
    bool frustumCull(ecs::Collider OBB, gsl::vec3 scale);

    gsl::vec3 mForward{0.f, 0.f, -1.f};
    gsl::vec3 mRight{1.f, 0.f, 0.f};
    gsl::vec3 mUp{0.f, 1.f, 0.f};

    gsl::vec3 mPosition{0.f, 0.f, 0.f};
    float mPitch{0.f};
    float mYaw{0.f};

    gsl::mat4 mYawMatrix;
    gsl::mat4 mPitchMatrix;

    float mSpeed{0.f}; //camera will move by this speed along the mForward vector

    gsl::vec4 mFrustum[6];
};

#endif // CAMERA_H
