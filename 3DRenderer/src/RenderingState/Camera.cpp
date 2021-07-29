#include "../pch.h"
#include "Camera.h"
#include "../App.h"

Camera::Camera(Vec3f position)
    :   mPosition(position),
        mX_yaw(0.0f),
        mY_yaw(0.0f),
        mVLookAt({0,0,1})
{
    mMatCameraRotYX.make_identity_matrix();
}

void Camera::handle_camera_event(const sf::Keyboard::Key& key, bool keyPressed)
{
    if (keyPressed)
    {
        Vec3f moveForwardVec = mVLookAt / 2.01f;
        Vec3f moveToSideVec = cross_product(mVLookAt, { 0,1,0 });

        switch (key)
        {
            case sf::Keyboard::W: mPosition += moveForwardVec; break;
            case sf::Keyboard::S: mPosition -= moveForwardVec; break;
            case sf::Keyboard::D: mPosition += moveToSideVec; break;
            case sf::Keyboard::A: mPosition -= moveToSideVec; break;
        }
    }
}

const Matrix4x4& Camera::get_camera_view_matrix(void)
{
    Matrix4x4 matCameraRot;
    matCameraRot = get_camera_rotation_matrix(App::DT.asSeconds());
    mVLookAt = Matrix4x4::multiply_matrix_by_vec(matCameraRot, { 0,0,1 });
    mVTarget = mPosition + mVLookAt;
    Matrix4x4 matCamera = point_at_matrix(mPosition, mVTarget, mVUp);
    return inverse_point_at_matrix(matCamera);
}

Matrix4x4& Camera::rotate_camera_to_mouse_pos(float dt)
{
    vec2f distanceFromCursorToCenter = {(float)(sf::Mouse::getPosition().x - App::WindowCenter.x),
                                        (float)(sf::Mouse::getPosition().y - App::WindowCenter.y)};
    
    static unsigned updateMouseIter = 0;
    static Matrix4x4& prevRotationMatrix = mMatCameraRotYX;
    float modX = 0.01f, modY = 0.01f;
    Matrix4x4 matCameraRotY, matCameraRotX;
    
    if (vec2_lenght(distanceFromCursorToCenter) < 2.0f)
    {
        sf::Mouse::setPosition(App::WindowCenter);
        return prevRotationMatrix;
    }   

    mY_yaw -= ((modY = -distanceFromCursorToCenter.x / 80.0f) * dt);
    mX_yaw -= ((modX = distanceFromCursorToCenter.y / 80.0f) * dt);

    if (mX_yaw > 3.6f) mX_yaw = 0.0f;
    if (mY_yaw > 3.6f) mY_yaw = 0.0f;

    matCameraRotY.make_Y_rotation_matrix(mY_yaw);
    matCameraRotX.make_X_rotation_matrix(mX_yaw);
    mMatCameraRotYX = Matrix4x4::multiply_matrix_by_matrix(matCameraRotX, matCameraRotY);

    if (updateMouseIter % 8 == 0)
    {
        sf::Mouse::setPosition(vec2i((int)(App::WindowCenter.x + distanceFromCursorToCenter.x / 2.2f), 
                                     (int)(App::WindowCenter.y + distanceFromCursorToCenter.y / 2.2f)));
    }
    updateMouseIter++;
    return (prevRotationMatrix = mMatCameraRotYX);
}

Matrix4x4& Camera::get_camera_rotation_matrix(float dt)
{

    mMatCameraRotYX = Matrix4x4::multiply_matrix_by_matrix( rotate_camera_to_mouse_pos(dt),
                                                            Matrix4x4::identity_matrix());
    return mMatCameraRotYX;
}


