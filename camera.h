#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector3D>
#include <QMatrix4x4>

#include "interactiveobject.h"


class Camera : public QOpenGLFunctions_4_1_Core
{
public:
    Camera();
    ~Camera();
//~
    void init();
    void perspective(int degrees, double aspect, double nearplane, double farplane);
    void lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D up);
    void lookAt(const QVector3D &at);
//    void update();
    void update(GLint pMat, GLint vMat);
    void translate(float dx, float dy, float dz);
    void Tick(float deltaTime);
    void FollowMouseMovements(float x, float y);



//    void rotate(){mVmatrix.rotate(1, {0, 0, 1});}

    QVector3D GetPosition() {return mEye;}
    void SetPosition(const QVector3D &newPos);

    QVector3D GetForward();
    QVector3D GetRight();
    QVector3D GetUp();
    void setTarget(InteractiveObject *target);

    void setIsEditorCamera(bool boolToSet) {bIsEditorCamera = boolToSet;}

//~
private:
    GLint  mPmatrixUniform;
    GLint  mVmatrixUniform;
    QVector3D mEye;
    QMatrix4x4 mPmatrix;
    QMatrix4x4 mVmatrix;

    InteractiveObject* mTarget  {nullptr};
    QVector3D mCameraPos;
    QVector3D mTargetPos;
    QVector3D mDirection;

    float mDistanceToTarget{50.f};        // Wanted distance to target
    float mMovementSensitivity = 0.5f;      // Mouse sensitivity

    float mZoomSensitivity {0.7f};

    bool bIsEditorCamera {true};
public:
    void Zoom(float diff);
//    void MoveForward(float value);
//    void MoveRight(float value);
//    void MoveUp(float value);


//~//

};

#endif // CAMERA_H
