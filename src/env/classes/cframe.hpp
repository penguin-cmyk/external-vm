//
// Created by sigma on 05/08/2025.
//

#ifndef EXTERNAL_LUA_CFRAME_HPP
#define EXTERNAL_LUA_CFRAME_HPP

#include <cmath>
#include "vector3.hpp"

class CFrame {
public:
    float m[16];

    CFrame() {
        for (int i = 0; i < 16; i++) m[i] = 0;
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    CFrame(float x, float y, float z) {
        for (int i = 0; i < 16; i++) m[i] = 0;
        m[0] = m[5] = m[10] = m[15] = 1.0f;
        m[12] = x; m[13] = y; m[14] = z;
    }

    CFrame(float x, float y, float z,
           float r00, float r01, float r02,
           float r10, float r11, float r12,
           float r20, float r21, float r22) {
        m[0] = r00; m[1] = r10; m[2] = r20; m[3] = 0;
        m[4] = r01; m[5] = r11; m[6] = r21; m[7] = 0;
        m[8] = r02; m[9] = r12; m[10] = r22; m[11] = 0;
        m[12] = x; m[13] = y; m[14] = z; m[15] = 1;
    }

    CFrame operator*(const CFrame& other) const {
        CFrame result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[j * 4 + i] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[j * 4 + i] += m[k * 4 + i] * other.m[j * 4 + k];
                }
            }
        }
        return result;
    }

    Vector3 getPosition() const {
        return Vector3(m[12], m[13], m[14]);
    }

    Vector3 getLookVector() const {
        return Vector3(-m[8], -m[9], -m[10]);
    }

    Vector3 getRightVector() const {
        return Vector3(m[0], m[1], m[2]);
    }

    Vector3 getUpVector() const {
        return Vector3(m[4], m[5], m[6]);
    }

    CFrame inverse() const {
        CFrame result;
        result.m[0] = m[0]; result.m[1] = m[4]; result.m[2] = m[8];
        result.m[4] = m[1]; result.m[5] = m[5]; result.m[6] = m[9];
        result.m[8] = m[2]; result.m[9] = m[6]; result.m[10] = m[10];

        result.m[12] = -(m[0] * m[12] + m[1] * m[13] + m[2] * m[14]);
        result.m[13] = -(m[4] * m[12] + m[5] * m[13] + m[6] * m[14]);
        result.m[14] = -(m[8] * m[12] + m[9] * m[13] + m[10] * m[14]);

        result.m[3] = result.m[7] = result.m[11] = 0;
        result.m[15] = 1;

        return result;
    }

    static CFrame lookAt(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3(0, 1, 0)) {
        Vector3 forward = (target - eye).normalize();
        Vector3 right = forward.cross(up).normalize();
        Vector3 newUp = right.cross(forward);

        return CFrame(eye.x, eye.y, eye.z,
                      right.x, newUp.x, -forward.x,
                      right.y, newUp.y, -forward.y,
                      right.z, newUp.z, -forward.z);
    }

    static CFrame fromEulerAnglesXYZ(float x, float y, float z) {
        float cx = cos(x), sx = sin(x);
        float cy = cos(y), sy = sin(y);
        float cz = cos(z), sz = sin(z);

        return CFrame(0, 0, 0,
                      cy * cz, -cy * sz, sy,
                      cx * sz + sx * sy * cz, cx * cz - sx * sy * sz, -sx * cy,
                      sx * sz - cx * sy * cz, sx * cz + cx * sy * sz, cx * cy);
    }
};

#endif //EXTERNAL_LUA_CFRAME_HPP