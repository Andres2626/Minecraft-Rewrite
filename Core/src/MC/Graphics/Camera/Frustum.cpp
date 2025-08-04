#include "Frustum.h"

namespace MC {
    namespace Graphics {

        void NormalizePlane(float frustum[6][4], int side) 
        {
            float magnitude = sqrt(frustum[side][0] * frustum[side][0] +
                frustum[side][1] * frustum[side][1] +
                frustum[side][2] * frustum[side][2]);

            frustum[side][0] /= magnitude;
            frustum[side][1] /= magnitude;
            frustum[side][2] /= magnitude;
            frustum[side][3] /= magnitude;
        }

        void Frustum::Calculate(Math::mat4 proj, Math::mat4 view) 
        {
            Math::mat4 _clip = proj * view;
            float* clip = (float*)&_clip; /* Convert mat4 to float array */

            ft[0][0] = clip[3] - clip[0];
            ft[0][1] = clip[7] - clip[4];
            ft[0][2] = clip[11] - clip[8];
            ft[0][3] = clip[15] - clip[12];
            NormalizePlane(ft, 0);

            ft[1][0] = clip[3] + clip[0];
            ft[1][1] = clip[7] + clip[4];
            ft[1][2] = clip[11] + clip[8];
            ft[1][3] = clip[15] + clip[12];
            NormalizePlane(ft, 1);

            ft[2][0] = clip[3] + clip[1];
            ft[2][1] = clip[7] + clip[5];
            ft[2][2] = clip[11] + clip[9];
            ft[2][3] = clip[15] + clip[13];
            NormalizePlane(ft, 2);

            ft[3][0] = clip[3] - clip[1];
            ft[3][1] = clip[7] - clip[5];
            ft[3][2] = clip[11] - clip[9];
            ft[3][3] = clip[15] - clip[13];
            NormalizePlane(ft, 3);

            ft[4][0] = clip[3] - clip[2];
            ft[4][1] = clip[7] - clip[6];
            ft[4][2] = clip[11] - clip[10];
            ft[4][3] = clip[15] - clip[14];
            NormalizePlane(ft, 4);

            ft[5][0] = clip[3] + clip[2];
            ft[5][1] = clip[7] + clip[6];
            ft[5][2] = clip[11] + clip[10];
            ft[5][3] = clip[15] + clip[14];
            NormalizePlane(ft, 5);
        }

        bool Frustum::PointInside(Math::vec3 pos) 
        {
            for (int i = 0; i < 6; i++) {
                if (ft[i][0] * pos.x + ft[i][1] * pos.y + ft[i][2] * pos.z + ft[i][3] <= 0)
                    return false;
            }
            return true;
        }

        bool Frustum::CubeInside(Physics::AABB& aabb) 
        {
            for (int i = 0; i < 6; i++) {
                if (ft[i][0] * aabb.p0.x + ft[i][1] * aabb.p0.y + ft[i][2] * aabb.p0.z + ft[i][3] > 0)
                    continue;
                if (ft[i][0] * aabb.p1.x + ft[i][1] * aabb.p0.y + ft[i][2] * aabb.p0.z + ft[i][3] > 0)
                    continue;
                if (ft[i][0] * aabb.p0.x + ft[i][1] * aabb.p1.y + ft[i][2] * aabb.p0.z + ft[i][3] > 0)
                    continue;
                if (ft[i][0] * aabb.p1.x + ft[i][1] * aabb.p1.y + ft[i][2] * aabb.p0.z + ft[i][3] > 0)
                    continue;
                if (ft[i][0] * aabb.p0.x + ft[i][1] * aabb.p0.y + ft[i][2] * aabb.p1.z + ft[i][3] > 0)
                    continue;
                if (ft[i][0] * aabb.p1.x + ft[i][1] * aabb.p0.y + ft[i][2] * aabb.p1.z + ft[i][3] > 0)
                    continue;
                if (ft[i][0] * aabb.p0.x + ft[i][1] * aabb.p1.y + ft[i][2] * aabb.p1.z + ft[i][3] > 0)
                    continue;
                if (ft[i][0] * aabb.p1.x + ft[i][1] * aabb.p1.y + ft[i][2] * aabb.p1.z + ft[i][3] > 0)
                    continue;
                return false;
            }
            return true;
        }

        bool Frustum::CubeFullyInside(Physics::AABB& aabb) 
        {
            for (int i = 0; i < 6; i++) {
                if (ft[i][0] * aabb.p0.x + ft[i][1] * aabb.p0.y + ft[i][2] * aabb.p0.z + ft[i][3] <= 0)
                    return false;
                if (ft[i][0] * aabb.p1.x + ft[i][1] * aabb.p0.y + ft[i][2] * aabb.p0.z + ft[i][3] <= 0)
                    return false;
                if (ft[i][0] * aabb.p0.x + ft[i][1] * aabb.p1.y + ft[i][2] * aabb.p0.z + ft[i][3] <= 0)
                    return false;
                if (ft[i][0] * aabb.p1.x + ft[i][1] * aabb.p1.y + ft[i][2] * aabb.p0.z + ft[i][3] <= 0)
                    return false;
                if (ft[i][0] * aabb.p0.x + ft[i][1] * aabb.p0.y + ft[i][2] * aabb.p1.z + ft[i][3] <= 0)
                    return false;
                if (ft[i][0] * aabb.p1.x + ft[i][1] * aabb.p0.y + ft[i][2] * aabb.p1.z + ft[i][3] <= 0)
                    return false;
                if (ft[i][0] * aabb.p0.x + ft[i][1] * aabb.p1.y + ft[i][2] * aabb.p1.z + ft[i][3] <= 0)
                    return false;
                if (ft[i][0] * aabb.p1.x + ft[i][1] * aabb.p1.y + ft[i][2] * aabb.p1.z + ft[i][3] <= 0)
                    return false;
            }
            return true;
        }

        bool Frustum::SphereInside(Math::vec3 pos, float r) 
        {
            for (int i = 0; i < 6; i++) {
                if (ft[i][0] * pos.x + ft[i][1] * pos.y + ft[i][2] * pos.z + ft[i][3] <= -r) {
                    return false;
                }
            }
            return true;
        }

    }
}