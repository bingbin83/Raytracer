#pragma once

#include "Vector4.h"

namespace rt {
namespace math {

const Vector4 Vector4::Zero()
{
    return { 0.0f, 0.0f, 0.0f, 0.0f };
}

#ifdef _DEBUG
Vector4::Vector4()
    : x(std::numeric_limits<float>::signaling_NaN())
    , y(std::numeric_limits<float>::signaling_NaN())
    , z(std::numeric_limits<float>::signaling_NaN())
    , w(std::numeric_limits<float>::signaling_NaN())
{}
#else
Vector4::Vector4() = default;
#endif // _DEBUG

Vector4::Vector4(const Vector4& other)
    : x(other.x), y(other.y), z(other.z), w(other.w)
{}

Vector4::Vector4(const float scalar)
    : x(scalar), y(scalar), z(scalar), w(scalar)
{}

Vector4::Vector4(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w)
{}

Vector4::Vector4(const int32 x, const int32 y, const int32 z, const int32 w)
    : i{ x, y, z, w }
{}

Vector4::Vector4(const uint32 x, const uint32 y, const uint32 z, const uint32 w)
    : u{ x, y, z, w }
{}

Vector4::Vector4(const int32 scalar)
    : i{ scalar, scalar, scalar, scalar }
{}

Vector4::Vector4(const uint32 scalar)
    : u{ scalar, scalar, scalar, scalar }
{}

Vector4::Vector4(const float* src)
    : x(src[0])
    , y(src[1])
    , z(src[2])
    , w(src[3])
{}

Vector4::Vector4(const Float2& src)
    : x(src.x)
    , y(src.y)
    , z(0.0f)
    , w(0.0f)
{}

Vector4::Vector4(const Float3& src)
    : x(src.x)
    , y(src.y)
    , z(src.z)
    , w(0.0f)
{}

Vector4& Vector4::operator = (const Vector4& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
    return *this;
}

const Vector4 Vector4::FromInteger(int32 x)
{
    return Vector4{ static_cast<float>(x) };
}

const Vector4 Vector4::FromIntegers(int32 x, int32 y, int32 z, int32 w)
{
    return Vector4{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w) };
}

uint32 Vector4::ToBGR() const
{
    const Vector4 scaled = (*this) * VECTOR_255;

    uint32 ret;
    ret  = math::Clamp(static_cast<int32>(scaled.z), 0, 255);
    ret |= math::Clamp(static_cast<int32>(scaled.y), 0, 255) << 8;
    ret |= math::Clamp(static_cast<int32>(scaled.x), 0, 255) << 16;
    return ret;
}

template<uint32 flipX, uint32 flipY, uint32 flipZ, uint32 flipW>
const Vector4 Vector4::ChangeSign() const
{
    if (!(flipX || flipY || flipZ || flipW))
    {
        // no operation
        return *this;
    }

    return Vector4
    {
        flipX > 0 ? -x : x,
        flipY > 0 ? -y : y,
        flipZ > 0 ? -z : z,
        flipW > 0 ? -w : w,
    };
}

const Vector4 Vector4::ChangeSign(const VectorBool4& flip) const
{
    return Vector4
    {
        flip.Get<0>() ? -x : x,
        flip.Get<1>() ? -y : y,
        flip.Get<2>() ? -z : z,
        flip.Get<3>() ? -w : w,
    };
}

template<uint32 maskX, uint32 maskY, uint32 maskZ, uint32 maskW>
RT_FORCE_INLINE const Vector4 Vector4::MakeMask()
{
    static_assert(!(maskX == 0 && maskY == 0 && maskZ == 0 && maskW == 0), "Useless mask");
    static_assert(!(maskX && maskY && maskZ && maskW), "Useless mask");

    // generate bit negation mask
    return Vector4{ maskX ? 0xFFFFFFFF : 0, maskY ? 0xFFFFFFFF : 0, maskZ ? 0xFFFFFFFF : 0, maskW ? 0xFFFFFFFF : 0 };
}

template<uint32 ix, uint32 iy, uint32 iz, uint32 iw>
const Vector4 Vector4::Swizzle() const
{
    static_assert(ix < 4, "Invalid X element index");
    static_assert(iy < 4, "Invalid Y element index");
    static_assert(iz < 4, "Invalid Z element index");
    static_assert(iw < 4, "Invalid W element index");

    return { f[ix], f[iy], f[iz], f[iw] };
}

const Vector4 Vector4::Swizzle(uint32 ix, uint32 iy, uint32 iz, uint32 iw) const
{
    return Vector4{ f[ix], f[iy], f[iz], f[iw] };
}

const Vector4 Vector4::Select(const Vector4& a, const Vector4& b, const VectorBool4& sel)
{
    return Vector4
    {
        sel.Get<0>() ? b.x : a.x,
        sel.Get<1>() ? b.y : a.y,
        sel.Get<2>() ? b.z : a.z,
        sel.Get<3>() ? b.w : a.w,
    };
}

template<uint32 selX, uint32 selY, uint32 selZ, uint32 selW>
const Vector4 Vector4::Select(const Vector4& a, const Vector4& b)
{
    static_assert(selX <= 1, "Invalid X index");
    static_assert(selY <= 1, "Invalid Y index");
    static_assert(selZ <= 1, "Invalid Z index");
    static_assert(selW <= 1, "Invalid W index");

    return Vector4
    {
        selX ? b.x : a.x,
        selY ? b.y : a.y,
        selZ ? b.z : a.z,
        selW ? b.w : a.w,
    };
}

const Vector4 Vector4::operator & (const Vector4& b) const
{
    return
    {
        i[0] & b.i[0],
        i[1] & b.i[1],
        i[2] & b.i[2],
        i[3] & b.i[3]
    };
}

const Vector4 Vector4::operator | (const Vector4& b) const
{
    return
    {
        i[0] | b.i[0],
        i[1] | b.i[1],
        i[2] | b.i[2],
        i[3] | b.i[3]
    };
}

const Vector4 Vector4::operator ^ (const Vector4& b) const
{
    return
    {
        i[0] ^ b.i[0],
        i[1] ^ b.i[1],
        i[2] ^ b.i[2],
        i[3] ^ b.i[3]
    };
}

Vector4& Vector4::operator &= (const Vector4& b)
{
    i[0] &= b.i[0];
    i[1] &= b.i[1];
    i[2] &= b.i[2];
    i[3] &= b.i[3];
    return *this;
}

Vector4& Vector4::operator |= (const Vector4& b)
{
    i[0] |= b.i[0];
    i[1] |= b.i[1];
    i[2] |= b.i[2];
    i[3] |= b.i[3];
    return *this;
}

Vector4& Vector4::operator ^= (const Vector4& b)
{
    i[0] ^= b.i[0];
    i[1] ^= b.i[1];
    i[2] ^= b.i[2];
    i[3] ^= b.i[3];
    return *this;
}

const Vector4 Vector4::operator - () const
{
    return Vector4{ -x, -y, -z, -w };
}

const Vector4 Vector4::operator + (const Vector4& b) const
{
    return Vector4{ x + b.x, y + b.y, z + b.z, w + b.w };
}

const Vector4 Vector4::operator - (const Vector4& b) const
{
    return Vector4{ x - b.x, y - b.y, z - b.z, w - b.w };
}

const Vector4 Vector4::operator * (const Vector4& b) const
{
    return Vector4{ x * b.x, y * b.y, z * b.z, w * b.w };
}

const Vector4 Vector4::operator / (const Vector4& b) const
{
    return Vector4{ x / b.x, y / b.y, z / b.z, w / b.w };
}

const Vector4 Vector4::operator * (float b) const
{
    return Vector4{ x * b, y * b, z * b, w * b };
}

const Vector4 Vector4::operator / (float b) const
{
    return Vector4{ x / b, y / b, z / b, w / b };
}

const Vector4 operator * (float a, const Vector4& b)
{
    return Vector4{ a * b.x, a * b.y, a * b.z, a * b.w };
}


Vector4& Vector4::operator += (const Vector4& b)
{
    x += b.x;
    y += b.y;
    z += b.z;
    w += b.w;
    return *this;
}

Vector4& Vector4::operator -= (const Vector4& b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;
    w -= b.w;
    return *this;
}

Vector4& Vector4::operator *= (const Vector4& b)
{
    x *= b.x;
    y *= b.y;
    z *= b.z;
    w *= b.w;
    return *this;
}

Vector4& Vector4::operator /= (const Vector4& b)
{
    x /= b.x;
    y /= b.y;
    z /= b.z;
    w /= b.w;
    return *this;
}

Vector4& Vector4::operator *= (float b)
{
    x *= b;
    y *= b;
    z *= b;
    w *= b;
    return *this;
}

Vector4& Vector4::operator /= (float b)
{
    x /= b;
    y /= b;
    z /= b;
    w /= b;
    return *this;
}

const Vector4 Vector4::Mod1(const Vector4& x)
{
    return x - Vector4::Floor(x);
}

const Vector4 Vector4::MulAndAdd(const Vector4& a, const Vector4& b, const Vector4& c)
{
    return a * b + c;
}

const Vector4 Vector4::MulAndSub(const Vector4& a, const Vector4& b, const Vector4& c)
{
    return a * b - c;
}

const Vector4 Vector4::NegMulAndAdd(const Vector4& a, const Vector4& b, const Vector4& c)
{
    return -(a * b) + c;
}

const Vector4 Vector4::NegMulAndSub(const Vector4& a, const Vector4& b, const Vector4& c)
{
    return -(a * b) - c;
}

const Vector4 Vector4::Floor(const Vector4& v)
{
    return Vector4{ floorf(v.x), floorf(v.y), floorf(v.z), floorf(v.w) };
}

const Vector4 Vector4::Sqrt(const Vector4& v)
{
    return Vector4{ sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w) };
}

const Vector4 Vector4::Reciprocal(const Vector4& v)
{
    return Vector4{ 1.0f / v.x, 1.0f / v.y, 1.0f / v.z, 1.0f / v.w };
}

const Vector4 Vector4::FastReciprocal(const Vector4& v)
{
    return Reciprocal(v); // TODO
}

const Vector4 Vector4::Min(const Vector4& a, const Vector4& b)
{
    return Vector4
    {
        math::Min(a.x, b.x),
        math::Min(a.y, b.y),
        math::Min(a.z, b.z),
        math::Min(a.w, b.w)
    };
}

const Vector4 Vector4::Max(const Vector4& a, const Vector4& b)
{
    return Vector4
    {
        math::Max(a.x, b.x),
        math::Max(a.y, b.y),
        math::Max(a.z, b.z),
        math::Max(a.w, b.w)
    };
}

const Vector4 Vector4::Abs(const Vector4& v)
{
    return Vector4
    {
        fabsf(v.x),
        fabsf(v.y),
        fabsf(v.z),
        fabsf(v.w)
    };
}

int Vector4::GetSignMask() const
{
    int ret = 0;
    ret |= x < 0.0f ? (1 << 0) : 0;
    ret |= y < 0.0f ? (1 << 1) : 0;
    ret |= z < 0.0f ? (1 << 2) : 0;
    ret |= w < 0.0f ? (1 << 3) : 0;
    return ret;
}

const Vector4 Vector4::HorizontalMax() const
{
    return Vector4{ math::Max(math::Max(x, y), math::Max(z, w)) };
}

const VectorBool4 Vector4::operator == (const Vector4& b) const
{
    return VectorBool4{ x == b.x, y == b.y, z == b.z, w == b.w };
}

const VectorBool4 Vector4::operator != (const Vector4& b) const
{
    return VectorBool4{ x != b.x, y != b.y, z != b.z, w != b.w };
}

const VectorBool4 Vector4::operator < (const Vector4& b) const
{
    return VectorBool4{ x < b.x, y < b.y, z < b.z, w < b.w };
}

const VectorBool4 Vector4::operator <= (const Vector4& b) const
{
    return VectorBool4{ x <= b.x, y <= b.y, z <= b.z, w <= b.w };
}

const VectorBool4 Vector4::operator > (const Vector4& b) const
{
    return VectorBool4{ x > b.x, y > b.y, z > b.z, w > b.w };
}

const VectorBool4 Vector4::operator >= (const Vector4& b) const
{
    return VectorBool4{ x >= b.x, y >= b.y, z >= b.z, w >= b.w };
}

float Vector4::Dot2(const Vector4& a, const Vector4& b)
{
    return a.x * b.x + a.y * b.y;
}

float Vector4::Dot3(const Vector4& a, const Vector4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vector4::Dot4(const Vector4& a, const Vector4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

const Vector4 Vector4::Dot2V(const Vector4& a, const Vector4& b)
{
    return Vector4{ Dot2(a, b) };
}

const Vector4 Vector4::Dot3V(const Vector4& a, const Vector4& b)
{
    return Vector4{ Dot3(a, b) };
}

const Vector4 Vector4::Dot4V(const Vector4& a, const Vector4& b)
{
    return Vector4{ Dot4(a, b) };
}

const Vector4 Vector4::Cross3(const Vector4& v1, const Vector4& v2)
{
    return Vector4
    {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}

float Vector4::Length2() const
{
    return sqrtf(SqrLength2());
}

const Vector4 Vector4::Length2V() const
{
    return Vector4{ Length2() };
}

float Vector4::Length3() const
{
    return sqrtf(Dot3(*this, *this));
}

float Vector4::SqrLength3() const
{
    return Dot3(*this, *this);
}

const Vector4 Vector4::Length3V() const
{
    return Vector4{ Length3() };
}

Vector4& Vector4::Normalize3()
{
    *this /= Length3V();
    return *this;
}

Vector4& Vector4::FastNormalize3()
{
    // TODO
    *this /= Length3V();
    return *this;
}

float Vector4::Length4() const
{
    return sqrtf(SqrLength4());
}

const Vector4 Vector4::Length4V() const
{
    return Vector4{ Length4() };
}

Vector4& Vector4::Normalize4()
{
    *this /= Length4V();
    return *this;
}

const Vector4 Vector4::Fmod1(const Vector4& v)
{
    return Vector4{ fmodf(v.x, 1.0f), fmodf(v.y, 1.0f), fmodf(v.z, 1.0f), fmodf(v.w, 1.0f) };
}

const VectorBool4 Vector4::IsZero() const
{
    return *this == Vector4::Zero();
}

const VectorBool4 Vector4::IsNaN() const
{
    return VectorBool4{ math::IsNaN(x), math::IsNaN(y), math::IsNaN(z), math::IsNaN(w) };
}

const VectorBool4 Vector4::IsInfinite() const
{
    return VectorBool4{ math::IsInfinity(x), math::IsInfinity(y), math::IsInfinity(z), math::IsInfinity(w) };
}

bool Vector4::IsValid() const
{
    return math::IsValid(x) && math::IsValid(y) && math::IsValid(z) && math::IsValid(w);
}

void Vector4::Transpose3(Vector4& a, Vector4& b, Vector4& c)
{
    std::swap(a.y, b.x);
    std::swap(a.z, c.x);
    std::swap(b.z, c.y);
}

const Vector4 Vector4::Orthogonalize(const Vector4& v, const Vector4& reference)
{
    // Gram�Schmidt process
    return Vector4::NegMulAndAdd(Vector4::Dot3V(v, reference), reference, v);
}

const Vector4 BipolarToUnipolar(const Vector4& x)
{
    return Vector4::MulAndAdd(x, VECTOR_HALVES, VECTOR_HALVES);
}

const Vector4 UnipolarToBipolar(const Vector4& x)
{
    return Vector4::MulAndSub(x, 2.0f, VECTOR_ONE);
}

} // namespace math
} // namespace rt
