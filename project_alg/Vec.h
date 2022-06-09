#ifndef _VEC_H_
#define _VEC_H_

#include <cmath>

constexpr float PI = 3.14159f;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace math
{
	template <class T> class Vec2d;
	template <class T> class Vec3d;

	template <class T>
	class Vec2d
	{
	public:
		T x, y;

	public:
		Vec2d() : x(0), y(0) {}
		Vec2d(T X, T Y) : x(X), y(Y) {}
		Vec2d(const Vec2d& obj) { x = obj.x; y = obj.y; }
		Vec2d(const Vec3d<T>& obj) { x = obj.x; y = obj.y; }

		Vec2d& operator=(const Vec2d& obj) { x = obj.x; y = obj.y; return *this; }
		Vec2d& operator=(const Vec3d<T>& obj) { x = obj.x; y = obj.y; return *this; }

		void operator+=(const Vec2d& obj) { x += obj.x; y += obj.y; }
		void operator-=(const Vec2d& obj) { x -= obj.x; y -= obj.y; }
		void operator+=(float value) { x += value; y += value; }
		void operator-=(float value) { x -= value; y -= value; }
		void operator*=(float value) { x *= value; y *= value; }
		void operator/=(float value) { x /= value; y /= value; }

		Vec2d operator+(const Vec2d& obj) const { return Vec2d(x + obj.x, y + obj.y); }
		Vec2d operator-(const Vec2d& obj) const { return Vec2d(x - obj.x, y - obj.y); }
		Vec2d operator+(float value) const { return Vec2d(x + value, y + value); }
		Vec2d operator-(float value) const { return Vec2d(x - value, y - value); }
		Vec2d operator*(float value) const { return Vec2d(x * value, y * value); }
		Vec2d operator/(float value) const { return Vec2d(x / value, y / value); }

		float dotProduct(const Vec2d& vec) { return (x * vec.x + y * vec.y); }
		float length() { return sqrt(dotProduct(*this)); }
		void normalize() { float l = length(); if (l > 0.0000001f) *this /= l; }
	};


	template <class T>
	class Vec3d
	{
	public:
		T x, y, z, w;

	public:
		Vec3d() : x(0), y(0), z(0), w(1) {}
		Vec3d(T X, T Y, T Z) : x(X), y(Y), z(Z), w(1) {}
		Vec3d(const Vec2d<T>& obj) { x = obj.x; y = obj.y; z = 0; w = 1; }
		Vec3d(const Vec3d& obj) { x = obj.x; y = obj.y; z = obj.z; w = obj.w; }

		Vec3d& operator=(const Vec2d<T>& obj) { x = obj.x; y = obj.y; z = 0; w = 1; return *this; }
		Vec3d& operator=(const Vec3d& obj) { x = obj.x; y = obj.y; z = obj.z; w = obj.w; return *this; }

		void operator+=(const Vec3d& obj) { x += obj.x; y += obj.y; z += obj.z; }
		void operator-=(const Vec3d& obj) { x -= obj.x; y -= obj.y; z -= obj.z; }
		void operator+=(float value) { x += value; y += value; z += value; }
		void operator-=(float value) { x -= value; y -= value; z -= value; }
		void operator*=(float value) { x *= value; y *= value; z *= value; }
		void operator/=(float value) { x /= value; y /= value; z /= value; }

		Vec3d operator+(const Vec3d& obj) const { return Vec3d(x + obj.x, y + obj.y, z + obj.z); }
		Vec3d operator-(const Vec3d& obj) const { return Vec3d(x - obj.x, y - obj.y, z - obj.z); }
		Vec3d operator+(float value) const { return Vec3d(x + value, y + value, z + value); }
		Vec3d operator-(float value) const { return Vec3d(x - value, y - value, z - value); }
		Vec3d operator*(float value) const { return Vec3d(x * value, y * value, z * value); }
		Vec3d operator/(float value) const { return Vec3d(x / value, y / value, z / value); }

		float dotProduct(const Vec3d& vec) { return (x * vec.x + y * vec.y + z * vec.z); }
		float length() { return sqrt(dotProduct(*this)); }
		void normalize() { float l = length(); if (l > 0.0000001f) *this /= l; }
		Vec3d crossProduct(const Vec3d& vec) 
		{
			Vec3d v;
			v.x = y * vec.z - vec.y * z;
			v.y = -(x * vec.z - vec.x * z);
			v.z = x * vec.y - vec.x * y;
			return v;
		}
	};
}

#endif