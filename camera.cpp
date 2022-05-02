
#include "camera.h"
#include <iostream>

Camera::Camera()
    : mEye{-2, 2, 2}
{
//    mPmatrix.setToIdentity ();
//    mVmatrix.setToIdentity ();

}

Camera::~Camera()
{

}

void Camera::init()
{
//    mPmatrix.setToIdentity ();
    mVmatrix.setToIdentity ();

//    mPmatrixUniform = pMatrixUniform;
//    mVmatrixUniform = vMatrixUniform;

}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPmatrix.perspective(degrees, aspect, nearplane, farplane);
}

void Camera::lookAt(const QVector3D &eye, const QVector3D &at, const QVector3D up)
{
    mVmatrix.setToIdentity();
    mVmatrix.lookAt(eye, at, up);
}

void Camera::lookAt(const QVector3D &at)
{
    mVmatrix.setToIdentity();
    mVmatrix.lookAt(mEye, at, QVector3D{0, 0, 1});
//        std::cout << "x: " <<at.x() << "| y: " <<at.y() << "| z: " <<at.z() << std::endl;

//    mVmatrix.lookAt(mEye, QVector3D{0, 0, 0}, QVector3D{0, 0, 1});
}

//void Camera::update()
//{
////    std::cout << "Camera::update()" << std::endl;
//    initializeOpenGLFunctions();
//    glUniformMatrix4fv(mPmatrixUniform, 1, GL_FALSE, mPmatrix.constData());
//    glUniformMatrix4fv(mVmatrixUniform, 1, GL_FALSE, mVmatrix.constData());
//}

void Camera::update(GLint pMat, GLint vMat)
{
    initializeOpenGLFunctions();
    glUniformMatrix4fv(pMat, 1, GL_FALSE, mPmatrix.constData());
    glUniformMatrix4fv(vMat, 1, GL_FALSE, mVmatrix.constData());
}

void Camera::translate(float dx, float dy, float dz)
{
//    std::cout << "x: " << dx << "| y: " << dy << "| z: " << dz << std::endl;
//    mVmatrix.translate(-dx, -dy, -dz);
//    update();
}

void Camera::Tick(float deltaTime)
{

    if(mTarget != nullptr)
    {
//        std::cout << "top if" << std::endl;
         mTargetPos = mTarget->getPosition();
    }
    else
    {
//        std::cout << "bot if" << std::endl;
        mTargetPos = QVector3D{0, 0, 0};
    }

    mTargetPos.setZ(mTargetPos.z()+2);
    static QVector3D oldTargetPos = mTargetPos;

    mCameraPos = mEye;
    mDirection = mCameraPos - mTargetPos;
    mDirection.normalize();
//    std::cout << "myPos: X: " << mCameraPos.x() << " Y: " << mCameraPos.y() << " Z: " <<  mCameraPos.z() << std::endl;
//    std::cout << "TargetPos: X: " << mTargetPos.x() << " Y: " << mTargetPos.y() << " Z: " <<  mTargetPos.z() << std::endl;


    //get new camera location from normalized direction and distance from target
    mCameraPos = mDirection * mDistanceToTarget + mTargetPos;

    static auto oldDistanceToTarget = mDistanceToTarget;
    mEye = mEye + (mTargetPos - oldTargetPos);
    mEye = mEye + GetForward() * (mDistanceToTarget - oldDistanceToTarget);
    oldDistanceToTarget = mDistanceToTarget;

    if(mTarget != nullptr)
    {
//        std::cout << "top if" << std::endl;
        auto targetPos2 = mTarget->getPosition();
//        std::cout << "x: " << targetPos2.x() << std::endl;
        lookAt(targetPos2);
    }
    else
    {
//        std::cout << "bot if" << std::endl;
        lookAt(QVector3D{0, 0, 0});
    }

//    update();
//    std::cout << "x: " <<mEye.x() << "| y: " <<mEye.y() << "| z: " <<mEye.z() << std::endl;
    oldTargetPos = mTargetPos;
}

void Camera::FollowMouseMovements(float x, float y)
{
    QMatrix4x4 rotation;
//    rotation.setToIdentity();
    QVector3D around;

    if (mTarget == nullptr) {
        around = {0, 0, 0};
    } else {
        around = mTarget->getPosition();
    }

    QVector3D oldPosition = mEye;
    QVector3D oldRight = GetRight();


    x *= mMovementSensitivity;
    y *= mMovementSensitivity;

    rotation.translate(around);
    rotation.rotate(y, GetRight());
    rotation.rotate(x * GetUp().z(), GetUp());
    rotation.translate(-around);

    mEye = rotation.map(mEye);

    if (QVector3D::dotProduct(GetRight(), oldRight) < 0)
        mEye = oldPosition;
    //std::cout << "X: " << Right().x() << "y: " << Right().y() << "Z: " << Right().z() << std::endl;

    mVmatrix.setToIdentity();

    mVmatrix.lookAt(mEye, around, QVector3D{0, 0, 1});
}

void Camera::SetPosition(const QVector3D &newPos)
{
        mEye = newPos;

}

QVector3D Camera::GetForward()
{
//    auto targetPos = mTarget != nullptr ? mTarget->getPosition() : QVector3D{0, 0, 0};
//    QVector3D out = targetPos - mEye;
//    out.normalize();

    //std::cout << "X: " << out.x() << "y: " << out.y() << "Z: " << out.z() << std::endl;
//    return out;

    QVector4D temp = mPmatrix.column(0);
    temp.normalize();
    return QVector3D{temp.x(), temp.y(), temp.z()};

//    return QVector3D{(mPmatrix.column(0)).x(), (mPmatrix.column(0)).y(), (mPmatrix.column(0).z)()};
}

QVector3D Camera::GetRight()
{
    QVector3D out = QVector3D::crossProduct(GetForward(), {0, 0, 1});

    out.normalize();

    return out;
}

QVector3D Camera::GetUp()
{
    QVector3D out = QVector3D::crossProduct(GetRight(), GetForward());

    out.normalize();
    //std::cout << "X: " << out.x() << "y: " << out.y() << "Z: " << out.z() << std::endl;
    return out;
}

void Camera::setTarget(InteractiveObject *target)
{
    mTarget = target;
}

void Camera::Zoom(float diff)
{
    diff = std::clamp(diff, -10.f, 10.f) * mZoomSensitivity;
    mDistanceToTarget = std::clamp(mDistanceToTarget + diff, 2.f, 40.f);
}
