#include<iostream>
#include <xmmintrin.h>

struct Vec3
{
	float x;
	float y;
	float z;
};

struct Mat33
{
	float m00, m01, m02;
	float m10, m11, m12;
	float m20, m21, m22;
};

struct alignas(16) Vec3sse
{
	__m128 v; // [x, y, z, padding]
};

struct alignas(16) Mat33sse
{
	__m128 col0; // [m00, m01, m02, 0]
	__m128 col1; // [m10, m11, m12, 0]
	__m128 col2; // [m20, m21, m22, 0]
};

void print(float a, float b, float c)
{
    std::cout << a << " " << b << " " << c << std::endl;
}
// Prevent the compiler from inlining this function.
// We want to see it clearly in disassembly.
__declspec(noinline)
Vec3 ScalarMatVec(const Mat33& m, const Vec3& v)
{
    Vec3 r;
    
    r.x = m.m00 * v.x;// +m.m01 * v.y + m.m02 * v.z;

    print(m.m00 * v.x, m.m01 * v.y, m.m02 * v.z);

    r.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
    r.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;

    return r;
}

__declspec(noinline)
Vec3sse SseMatVec(const Mat33sse& m, const Vec3sse& v)
{
    // v.v = [x, y, z, 0]

    __m128 vx = _mm_shuffle_ps(v.v, v.v, 0x00); // [x, x, x, x]
    __m128 vy = _mm_shuffle_ps(v.v, v.v, 0x55); // [y, y, y, y]
    __m128 vz = _mm_shuffle_ps(v.v, v.v, 0xAA); // [z, z, z, z]

    __m128 partX = _mm_mul_ps(vx, m.col0);
    __m128 partY = _mm_mul_ps(vy, m.col1);
    __m128 partZ = _mm_mul_ps(vz, m.col2);

    __m128 result = _mm_add_ps(_mm_add_ps(partX, partY), partZ);

    Vec3sse r;
    r.v = result;
    return r;
}

int main()
{
    Mat33 mScalar
    {
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f
    };

    Vec3 vScalar
    {
        10.0f,
        20.0f,
        30.0f
    };

    Vec3 rScalar = ScalarMatVec(mScalar, vScalar);

    std::cout << "Scalar result: "
        << rScalar.x << ", "
        << rScalar.y << ", "
        << rScalar.z << "\n";

    Mat33sse mSse;

    // Column-major storage:
    // col0 = [m00, m10, m20, 0]
    // col1 = [m01, m11, m21, 0]
    // col2 = [m02, m12, m22, 0]
    mSse.col0 = _mm_set_ps(0.0f, 7.0f, 4.0f, 1.0f);
    mSse.col1 = _mm_set_ps(0.0f, 8.0f, 5.0f, 2.0f);
    mSse.col2 = _mm_set_ps(0.0f, 9.0f, 6.0f, 3.0f);

    Vec3sse vSse;
    vSse.v = _mm_set_ps(0.0f, 30.0f, 20.0f, 10.0f);

    Vec3sse rSse = SseMatVec(mSse, vSse);

    alignas(16) float out[4];
    _mm_store_ps(out, rSse.v);

    std::cout << "SSE result:    "
        << out[0] << ", "
        << out[1] << ", "
        << out[2] << "\n";
} 