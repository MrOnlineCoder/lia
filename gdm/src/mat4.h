#pragma once

#include "mathbase.h"
#include "vec4.h"
#include "vec3.h"
#include <cmath>

namespace gdm
{
    /**
     * Vectors are treated as row, resulting in a matrix that is represented as follows,
     * where tx, ty and tz are the translation components of the matrix:
     *
     * 1   0   0   0
     * 0   1   0   0
     * 0   0   1   0
     * tx  ty  tz  1
     */
    struct mat4
    {
    protected:
        float m[4][4];

    public:
        /**
        * Constructs an identity matrix.
        */
        mat4();

        /**
        * Constructs a matrix initialized to the specified value.
        *
        * @param m00 The first element of the first row.
        * @param m01 The second element of the first row.
        * @param m02 The third element of the first row.
        * @param m03 The fourth element of the first row.
        * @param m10 The first element of the second row.
        * @param m11 The second element of the second row.
        * @param m12 The third element of the second row.
        * @param m13 The fourth element of the second row.
        * @param m20 The first element of the third row.
        * @param m21 The second element of the third row.
        * @param m22 The third element of the third row.
        * @param m23 The fourth element of the third row.
        * @param m30 The first element of the fourth row.
        * @param m31 The second element of the fourth row.
        * @param m32 The third element of the fourth row.
        * @param m33 The fourth element of the fourth row.
        */
        mat4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);

        /**
        * Constructs a matrix initialized by the vectors.
        *
        * @param row1 The x unit basis vector
        * @param row2 The y unit basis vector
        * @param row3 The z unit basis vector
        * @param row4 The translation vector
        */
        mat4(vec4 row1, vec4 row2, vec4 row3, vec4 row4);

        /**
         * Constructs an matrix with scalar by diagonal.
         *
         * @param scalar Diagonal scalar value
         */
        mat4(float scalar);

        float& operator()(int i, int j)
        {
            return m[i][j];
        }

        const float& operator()(int i, int j) const
        {
            return m[i][j];
        }

        float determinant() const
        {
            float a0 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
            float a1 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
            float a2 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
            float a3 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
            float a4 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
            float a5 = m[0][2] * m[1][3] - m[0][3] * m[1][2];
            float b0 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
            float b1 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
            float b2 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
            float b3 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
            float b4 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
            float b5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];

            return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
        }

        vec4 GetRow(int index) const
        {
            return vec4(m[index][0], m[index][1], m[index][2], m[index][3]);
        }
    };

    inline mat4 operator*(const mat4& mat1, const mat4& mat2)
    {
        float p[16];

        for (int16_t col = 0; col < 4; col++)
        {
            for (int16_t row = 0; row < 4; row++)
            {
                float sum = 0.0f;
                for (int16_t i = 0; i < 4; i++)
                {
                    sum += mat1(row, i) * mat2(i, col);
                }
                p[col + row * 4] = sum;
            }
        }

        return mat4(p[0], p[1], p[2], p[3],
            p[4], p[5], p[6], p[7],
            p[8], p[9], p[10], p[11],
            p[12], p[13], p[14], p[15]);
    }

    inline vec4 operator*(const mat4& mat, const vec4& vec)
    {
        return vec4(mat(0, 0) * vec.x + mat(0, 1) * vec.y + mat(0, 2) * vec.z + mat(0, 3) * vec.w,
            mat(1, 0) * vec.x + mat(1, 1) * vec.y + mat(1, 2) * vec.z + mat(1, 3) * vec.w,
            mat(2, 0) * vec.x + mat(2, 1) * vec.y + mat(2, 2) * vec.z + mat(2, 3) * vec.w,
            mat(3, 0) * vec.x + mat(3, 1) * vec.y + mat(3, 2) * vec.z + mat(3, 3) * vec.w);
    }

    inline std::ostream& operator<<(std::ostream& stream, const mat4& mat)
    {
        stream << "mat4 {\n";
        stream << mat(0, 0) << " " << mat(0, 1) << " " << mat(0, 2) << " " << mat(0, 3) << "\n";
        stream << mat(1, 0) << " " << mat(1, 1) << " " << mat(1, 2) << " " << mat(1, 3) << "\n";
        stream << mat(2, 0) << " " << mat(2, 1) << " " << mat(2, 2) << " " << mat(2, 3) << "\n";
        stream << mat(3, 0) << " " << mat(3, 1) << " " << mat(3, 2) << " " << mat(3, 3) << "\n";
        stream << "}\n";

        return stream;
    }

    inline bool canBeInverse(const mat4& mat)
    {
        // if determinant close to zero, can't convert
        return !(std::abs(mat.determinant()) <= TOLERANCE);
    }

    inline mat4 inverse(const mat4& mat)
    {
        // if determinant close to zero, can't convert
        if (std::abs(mat.determinant()) <= TOLERANCE)
            return mat4();

        const vec3& a = vec3(mat(0, 0), mat(1, 0), mat(2, 0));
        const vec3& b = vec3(mat(0, 1), mat(1, 1), mat(2, 1));
        const vec3& c = vec3(mat(0, 2), mat(1, 2), mat(2, 2));
        const vec3& d = vec3(mat(0, 3), mat(1, 3), mat(2, 3));

        const float& x = mat(3, 0);
        const float& y = mat(3, 1);
        const float& z = mat(3, 2);
        const float& w = mat(3, 3);

        vec3 s = cross(a, b);
        vec3 t = cross(c, d);
        vec3 u = a * y - b * x;
        vec3 v = c * w - d * z;

        float invDet = 1.0F / (dot(s, v) + dot(t, u));
        s *= invDet;
        t *= invDet;
        u *= invDet;
        v *= invDet;

        vec3 r0 = cross(b, v) + t * y;
        vec3 r1 = cross(v, a) - t * x;
        vec3 r2 = cross(d, u) + s * w;
        vec3 r3 = cross(u, c) - s * z;

        return (mat4(r0.x, r0.y, r0.z, -dot(b, t),
            r1.x, r1.y, r1.z, dot(a, t),
            r2.x, r2.y, r2.z, -dot(d, s),
            r3.x, r3.y, r3.z, dot(c, s)));
    }

    inline mat4 transpose(const mat4& mat)
    {
        return mat4(vec4(mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0)),
            vec4(mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1)),
            vec4(mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2)),
            vec4(mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3)));
    }

    inline mat4 translate(const mat4& mat, const vec3& translation)
    {
        vec4 translationRow = mat.GetRow(3);
        translationRow.x += translation.x;
        translationRow.y += translation.y;
        translationRow.z += translation.z;

        return mat4(mat.GetRow(0), mat.GetRow(1), mat.GetRow(2), translationRow);
    }

    /**
    * @param angle The angle in radians
    */
    inline mat4 rotate(const mat4& mat, const float& angle, const vec3& axis)
    {
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        float d = 1.0f - cos;

        float x = axis.x * d;
        float y = axis.y * d;
        float z = axis.z * d;
        float axay = x * axis.y;
        float axaz = x * axis.y;
        float ayaz = y * axis.z;

        return mat4(cos + x * axis.x, axay - sin * axis.z, axaz + sin * axis.y, mat(0, 3),
                    axay + sin * axis.z, cos + y * axis.y, ayaz - sin * axis.x, mat(1, 3),
                    axaz - sin * axis.y, ayaz + sin * axis.x, cos + z * axis.z, mat(2, 3),
                    mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3));
    }

    inline mat4 scale(const mat4& mat, const vec3& scale)
    {
        return mat4(mat(0, 0) * scale.x, mat(0, 1), mat(0, 2), mat(0, 3),
                    mat(1, 0), mat(1, 1) * scale.y, mat(1, 2), mat(1, 3),
                    mat(2, 0), mat(2, 1), mat(2, 2) * scale.z, mat(2, 3),
                    mat(3, 0), mat(3, 1), mat(3, 2), mat(3, 3));
    }
}
