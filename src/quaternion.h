#include "matrix.h"


struct quat
{
    float s;
    vec3d v;
    
    quat() : s(0)
    {
    }

    quat(float s, float x, float y, float z) : s(s), v(x, y, z)
    {
    }

    quat(float s, vec3d v) : s(s), v(v)
    {
    }

    ~quat()
    {
    }

    inline quat operator * (quat const& a) const
    {
        return quat(s * a.s - v * a.v, s * a.v + v * a.s + v.cross(a.v));
    }

    inline quat operator * (float a) const
    {
        return quat(a * s, a * v);
    }

    inline quat operator * (vec3d const& v1) const
    {
        return quat(-(v * v1), s * v1 + v.cross(v1));
    }

    inline quat operator + (quat const& a) const
    {
        return quat(s + a.s, v + a.v);
    }

    inline quat operator - () const
    {
        return quat(-s, -v);
    }
    
    inline float abs() const
    {
        return sqrt(s * s + v * v);
    }

    static quat rotation(float a, vec3d const& l)
    {
        return quat(cos(a/2), l.normalized() * sin(a / 2));
    }

    inline float rotation_angle() const
    {
        return 2 * acos(s);
    }

    inline vec3d rotation_vect() const
    {
        return v.abs() == 0.0 ? vec3d(0, 0, 0) : v.normalized();
    }

    inline quat& normalize()
    {
        float n = abs();
        s = s / n;
        v = v / n;
        return *this;
    }
};

inline quat operator * (vec3d const& v, quat const& q)
{
    return quat(-(v * q.v), q.s * v + v.cross(q.v));
}

inline quat operator * (float k, quat const& q)
{
    return quat(k * q.s, k * q.v);
}




