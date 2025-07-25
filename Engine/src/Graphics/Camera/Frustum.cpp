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

        void Frustum::Calculate(glm::mat4 proj, glm::mat4 view) 
        {
            glm::mat4 _clip = proj * view;
            float* clip = (float*)&_clip; /* Convert mat4 to float array */

            this->ft[0][0] = clip[3] - clip[0];
            this->ft[0][1] = clip[7] - clip[4];
            this->ft[0][2] = clip[11] - clip[8];
            this->ft[0][3] = clip[15] - clip[12];
            NormalizePlane(this->ft, 0);

            this->ft[1][0] = clip[3] + clip[0];
            this->ft[1][1] = clip[7] + clip[4];
            this->ft[1][2] = clip[11] + clip[8];
            this->ft[1][3] = clip[15] + clip[12];
            NormalizePlane(this->ft, 1);

            this->ft[2][0] = clip[3] + clip[1];
            this->ft[2][1] = clip[7] + clip[5];
            this->ft[2][2] = clip[11] + clip[9];
            this->ft[2][3] = clip[15] + clip[13];
            NormalizePlane(this->ft, 2);

            this->ft[3][0] = clip[3] - clip[1];
            this->ft[3][1] = clip[7] - clip[5];
            this->ft[3][2] = clip[11] - clip[9];
            this->ft[3][3] = clip[15] - clip[13];
            NormalizePlane(this->ft, 3);

            this->ft[4][0] = clip[3] - clip[2];
            this->ft[4][1] = clip[7] - clip[6];
            this->ft[4][2] = clip[11] - clip[10];
            this->ft[4][3] = clip[15] - clip[14];
            NormalizePlane(this->ft, 4);

            this->ft[5][0] = clip[3] + clip[2];
            this->ft[5][1] = clip[7] + clip[6];
            this->ft[5][2] = clip[11] + clip[10];
            this->ft[5][3] = clip[15] + clip[14];
            NormalizePlane(this->ft, 5);
        }

        bool Frustum::PointInside(glm::vec3 pos) 
        {
            for (int i = 0; i < 6; i++) {
                if (this->ft[i][0] * pos.x + this->ft[i][1] * pos.y + this->ft[i][2] * pos.z + this->ft[i][3] <= 0)
                    return false;
            }
            return true;
        }

        bool Frustum::CubeInside(Physics::AABB& aabb) 
        {
            for (int i = 0; i < 6; i++) {
                if (this->ft[i][0] * aabb.p0.x + this->ft[i][1] * aabb.p0.y + this->ft[i][2] * aabb.p0.z + this->ft[i][3] > 0)
                    continue;
                if (this->ft[i][0] * aabb.p1.x + this->ft[i][1] * aabb.p0.y + this->ft[i][2] * aabb.p0.z + this->ft[i][3] > 0)
                    continue;
                if (this->ft[i][0] * aabb.p0.x + this->ft[i][1] * aabb.p1.y + this->ft[i][2] * aabb.p0.z + this->ft[i][3] > 0)
                    continue;
                if (this->ft[i][0] * aabb.p1.x + this->ft[i][1] * aabb.p1.y + this->ft[i][2] * aabb.p0.z + this->ft[i][3] > 0)
                    continue;
                if (this->ft[i][0] * aabb.p0.x + this->ft[i][1] * aabb.p0.y + this->ft[i][2] * aabb.p1.z + this->ft[i][3] > 0)
                    continue;
                if (this->ft[i][0] * aabb.p1.x + this->ft[i][1] * aabb.p0.y + this->ft[i][2] * aabb.p1.z + this->ft[i][3] > 0)
                    continue;
                if (this->ft[i][0] * aabb.p0.x + this->ft[i][1] * aabb.p1.y + this->ft[i][2] * aabb.p1.z + this->ft[i][3] > 0)
                    continue;
                if (this->ft[i][0] * aabb.p1.x + this->ft[i][1] * aabb.p1.y + this->ft[i][2] * aabb.p1.z + this->ft[i][3] > 0)
                    continue;
                return false;
            }
            return true;
        }

        bool Frustum::CubeFullyInside(Physics::AABB& aabb) 
        {
            for (int i = 0; i < 6; i++) {
                if (this->ft[i][0] * aabb.p0.x + this->ft[i][1] * aabb.p0.y + this->ft[i][2] * aabb.p0.z + this->ft[i][3] <= 0)
                    return false;
                if (this->ft[i][0] * aabb.p1.x + this->ft[i][1] * aabb.p0.y + this->ft[i][2] * aabb.p0.z + this->ft[i][3] <= 0)
                    return false;
                if (this->ft[i][0] * aabb.p0.x + this->ft[i][1] * aabb.p1.y + this->ft[i][2] * aabb.p0.z + this->ft[i][3] <= 0)
                    return false;
                if (this->ft[i][0] * aabb.p1.x + this->ft[i][1] * aabb.p1.y + this->ft[i][2] * aabb.p0.z + this->ft[i][3] <= 0)
                    return false;
                if (this->ft[i][0] * aabb.p0.x + this->ft[i][1] * aabb.p0.y + this->ft[i][2] * aabb.p1.z + this->ft[i][3] <= 0)
                    return false;
                if (this->ft[i][0] * aabb.p1.x + this->ft[i][1] * aabb.p0.y + this->ft[i][2] * aabb.p1.z + this->ft[i][3] <= 0)
                    return false;
                if (this->ft[i][0] * aabb.p0.x + this->ft[i][1] * aabb.p1.y + this->ft[i][2] * aabb.p1.z + this->ft[i][3] <= 0)
                    return false;
                if (this->ft[i][0] * aabb.p1.x + this->ft[i][1] * aabb.p1.y + this->ft[i][2] * aabb.p1.z + this->ft[i][3] <= 0)
                    return false;
            }
            return true;
        }

        bool Frustum::SphereInside(glm::vec3 pos, float r) 
        {
            for (int i = 0; i < 6; i++) {
                if (this->ft[i][0] * pos.x + this->ft[i][1] * pos.y + this->ft[i][2] * pos.z + this->ft[i][3] <= -r) {
                    return false;
                }
            }
            return true;
        }

    }
}