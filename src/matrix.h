#ifndef __matrix__h__
#define __matrix__h__

#include <string.h>
#include <math.h>


struct mat3d;
struct vec3d;

struct vec3d
{
    float x, y, z;

    vec3d() : x(0), y(0), z(0)
    {
    }

    vec3d(float x, float y, float z) : 
        x(x), y(y), z(z)
    {
    }

    ~vec3d()
    {
    }

    inline float square() const;
    inline float abs() const;
    inline float scal(vec3d const& a) const;
    inline mat3d cross() const;
    inline vec3d cross(vec3d const& a) const;
    inline vec3d normalized() const;

    inline vec3d operator / (float a) const;
    inline vec3d operator * (float a) const;
    inline float operator * (vec3d const& a) const;
    inline vec3d operator + (vec3d const& a) const;
    inline vec3d operator - (vec3d const& a) const;
    inline vec3d operator - () const;
};

struct mat3d
{
    float m11, m12, m13,
          m21, m22, m23,
          m31, m32, m33;

    mat3d() : 
      m11(0), m12(0), m13(0),
      m21(0), m22(0), m23(0),
      m31(0), m32(0), m33(0)
    {
    }

    mat3d(float m11, float m12, float m13, 
        float m21, float m22, float m23, 
        float m31, float m32, float m33) :

        m11(m11), m12(m12), m13(m13),
        m21(m21), m22(m22), m23(m23),
        m31(m31), m32(m32), m33(m33)
    {
    }

    mat3d(vec3d c1, vec3d c2, vec3d c3) : 

        m11(c1.x), m12(c2.x), m13(c3.x),
        m21(c1.y), m22(c2.y), m23(c3.y),
        m31(c1.z), m32(c2.z), m33(c3.z)
    {
    }

    ~mat3d()
    {
    }
    
    mat3d transpose() const
    {
        return mat3d(
            m11, m21, m31,
            m12, m22, m32,
            m13, m23, m33
         );
    }

    mat3d operator * (float a) const
    {
        return mat3d(
            a * m11, a * m12, a * m13, 
            a * m21, a * m22, a * m23, 
            a * m31, a * m32, a * m33
          );
    }

    vec3d operator * (vec3d const& a) const
    {
        return vec3d(
            m11 * a.x + m12 * a.y + m13 * a.z,
            m21 * a.x + m22 * a.y + m23 * a.z,
            m21 * a.x + m32 * a.y + m33 * a.z
          );
    }

    mat3d operator * (mat3d const& a) const
    {
        return mat3d(
            m11 * a.m11 + m12 * a.m21 + m13 * a.m31,
            m11 * a.m12 + m12 * a.m22 + m13 * a.m32,
            m11 * a.m13 + m12 * a.m23 + m13 * a.m33,

            m21 * a.m11 + m22 * a.m21 + m23 * a.m31,
            m21 * a.m12 + m22 * a.m22 + m23 * a.m32,
            m21 * a.m13 + m22 * a.m23 + m23 * a.m33,

            m31 * a.m11 + m32 * a.m21 + m33 * a.m31,
            m31 * a.m12 + m32 * a.m22 + m33 * a.m32,
            m31 * a.m13 + m32 * a.m23 + m33 * a.m33
          );
    }
    
    mat3d operator + (mat3d const& a) const
    {
        return mat3d(
            m11 + a.m11, m12 + a.m12, m13 + a.m13, 
            m21 + a.m21, m22 + a.m22, m23 + a.m23, 
            m31 + a.m31, m32 + a.m32, m33 + a.m33
          );
    }

    mat3d operator - (mat3d const& a) const
    {
        return mat3d(
            m11 - a.m11, m12 - a.m12, m13 - a.m13, 
            m21 - a.m21, m22 - a.m22, m23 - a.m23, 
            m31 - a.m31, m32 - a.m32, m33 - a.m33  
          );
    }

    mat3d operator - () const
    {
        return mat3d(
            -m11, -m12, -m13,
            -m21, -m22, -m23,
            -m31, -m32, -m33
          );
    }

    static mat3d eye()
    {
        return mat3d(
            1.0, 0.0, 0.0, 
            0.0, 1.0, 0.0, 
            0.0, 0.0, 1.0);
    }
};




inline mat3d vec3d::cross() const
{
    return mat3d(
          0.0,  -z,   y,
            z, 0.0,  -x,
           -y,   x, 0.0
        );
}

inline vec3d operator * (float a, vec3d const& v)
{
    return vec3d(v.x * a, v.y * a, v.z * a);
}

inline float vec3d::scal(vec3d const& a) const
{
    return x * a.x + y * a.y + z * a.z;
}

inline float vec3d::square() const
{
    return scal(*this);
}

inline float vec3d::abs() const 
{
    return sqrt(scal(*this));
}

inline vec3d vec3d::operator / (float a) const
{
    return vec3d(x / a, y / a, z / a);
}

inline vec3d vec3d::operator * (float a) const
{
    return vec3d(x * a, y * a, z * a);
}

inline float vec3d::operator * (vec3d const& a) const
{
    return this->scal(a);
}

inline vec3d vec3d::operator + (vec3d const& a) const
{
    return vec3d(x + a.x, y + a.y, z + a.z);
}

inline vec3d vec3d::operator - (vec3d const& a) const
{
    return vec3d(x - a.x, y - a.y, z - a.z);
}

inline vec3d vec3d::operator - () const
{
    return vec3d(-x, -y, -z);
}

inline vec3d vec3d::cross(vec3d const& a) const
{
    return vec3d(
            y * a.z - z * a.y,
           -x * a.z + z * a.x,
            x * a.y - y * a.x
          );
}

inline vec3d vec3d::normalized() const
{
    float k = abs();
    return vec3d(x / k, y / k, z / k);
}

#endif // __matrix__h__
