#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Vec.h"
#include <cstdint>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef toRadian
#define toRadian(value)            (((value) * PI) / 180.0f)
#endif

#ifndef toDegree
#define toDegree(value)            (((value) * 180.0f) / PI)
#endif


namespace math
{
	namespace matrixType
	{
		enum MatrixType
		{
			PERSPECTIVE = 0,
			ISOMETRIC = 1,
			CABINET = 2,
			TEMP_MATRIX = 3,
		};
	}

	template <class T>
	class Matrix
	{
	public:
		T mat[4][4]{};

	public:
		Matrix();

		Matrix& operator=(const Matrix& matrix);
		Matrix operator*(const Matrix& matrix) const;
		void operator*=(const Matrix& matrix);
		Vec3d<T> operator*(const Vec3d<T>& vec) const;

		void init();
		void projectionPerspective(float fovDegrees, float aspectRatio, float nearSurface, float farSurface);
		void projectionIzometric();
		void projectionCabinet();
		void rotateX(float angleRad);
		void rotateY(float angleRad);
		void rotateZ(float angleRad);
		void scale(float xScale, float yScale, float zScale);
		void translation(float xShift, float yShift, float zShift);
	};


	template<class T>
	inline Matrix<T>::Matrix()
	{
		init();
	}

	template<class T>
	inline Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix)
	{
		for (int16_t i = 0; i < 4; ++i)
		{
			for (int16_t j = 0; j < 4; ++j)
			{
				mat[i][j] = matrix.mat[i][j];
			}
		}
		return *this;
	}

	template<class T>
	inline Matrix<T> Matrix<T>::operator*(const Matrix<T>& matrix) const
	{
		Matrix<T> result;

		for (int16_t i = 0; i < 4; ++i)
		{
			for (int16_t j = 0; j < 4; ++j)
			{
				result.mat[i][j] = 0;
				for (int16_t k = 0; k < 4; ++k)
				{
					result.mat[i][j] += mat[i][k] * matrix.mat[k][j];
				}
			}
		}
		return result;
	}

	template<class T>
	inline void Matrix<T>::operator*=(const Matrix<T>& matrix)
	{
		Matrix<T> result;

		for (int16_t i = 0; i < 4; ++i)
		{
			for (int16_t j = 0; j < 4; ++j)
			{
				result.mat[i][j] = 0;
				for (int16_t k = 0; k < 4; ++k)
				{
					result.mat[i][j] += matrix.mat[i][k] * mat[k][j];
				}
			}
		}
		operator=(result);
	}

	template<class T>
	inline Vec3d<T> Matrix<T>::operator*(const Vec3d<T>& vec) const
	{
		Vec3d<T> result;

		result.x = vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + vec.w * mat[3][0];
		result.y = vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + vec.w * mat[3][1];
		result.z = vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + vec.w * mat[3][2];
		result.w = vec.x * mat[0][3] + vec.y * mat[1][3] + vec.z * mat[2][3] + vec.w * mat[3][3];

		return result;
	}

	template<class T>
	inline void Matrix<T>::init()
	{
		mat[0][0] = 1;
		mat[1][1] = 1;
		mat[2][2] = 1;
		mat[3][3] = 1;
	}

	template<class T>
	inline void Matrix<T>::projectionPerspective(float fovDegrees, float aspectRatio, float nearSurface, float farSurface)
	{
		float fovRad = 1.0f / tanf(fovDegrees * 0.5f / 180.0f * PI);

		mat[0][0] = aspectRatio * fovRad;
		mat[1][1] = fovRad;
		mat[2][2] = (farSurface + nearSurface) / (farSurface - nearSurface);
		mat[3][2] = (-2.0f * farSurface * nearSurface) / (farSurface - nearSurface);
		mat[2][3] = 1.0f;
		mat[3][3] = 0.0f;
	}

	template<class T>
	inline void Matrix<T>::projectionIzometric()
	{
		mat[0][0] = 0.7071f;	// sqrt(1.0f / 2.0f);
		mat[0][1] = -0.4082f;	// -sqrt(1.0f / 6.0f);
		mat[0][2] = 0.5774f;	// sqrt(1.0f / 3.0f);
		mat[1][1] = 0.8165f;	// sqrt(2.0f / 3.0f);
		mat[1][2] = 0.5774f;	// sqrt(1.0f / 3.0f);
		mat[2][0] = -0.7071f;	// -sqrt(1.0f / 2.0f);
		mat[2][1] = -0.4082f;	// -sqrt(1.0f / 6.0f);
		mat[2][2] = 0.5774f;	// sqrt(1.0f / 3.0f);
		mat[3][3] = 1.0f;
	}

	template<class T>
	inline void Matrix<T>::projectionCabinet()
	{
		mat[0][0] = 1.0f;
		mat[1][1] = 1.0f;
		mat[2][0] = 0.5f;
		mat[2][1] = -0.5f;
		mat[2][2] = 0.0f;
		mat[3][3] = 1.0f;
	}

	template<class T>
	inline void Matrix<T>::rotateX(float angle)
	{
		float angleRad = toRadian(angle);
		mat[0][0] = 1.0f;
		mat[1][1] = cosf(angleRad);
		mat[1][2] = sinf(angleRad);
		mat[2][1] = -sinf(angleRad);
		mat[2][2] = cosf(angleRad);
		mat[3][3] = 1.0f;
	}

	template<class T>
	inline void Matrix<T>::rotateY(float angle)
	{
		float angleRad = toRadian(angle);
		mat[0][0] = cosf(angleRad);
		mat[0][2] = sinf(angleRad);
		mat[2][0] = -sinf(angleRad);
		mat[1][1] = 1.0f;
		mat[2][2] = cosf(angleRad);
		mat[3][3] = 1.0f;
	}

	template<class T>
	inline void Matrix<T>::rotateZ(float angle)
	{
		float angleRad = toRadian(angle);
		mat[0][0] = cosf(angleRad);
		mat[0][1] = sinf(angleRad);
		mat[1][0] = -sinf(angleRad);
		mat[1][1] = cosf(angleRad);
		mat[2][2] = 1.0f;
		mat[3][3] = 1.0f;
	}

	template<class T>
	inline void Matrix<T>::scale(float xScale, float yScale, float zScale)
	{
		mat[0][0] = xScale;
		mat[1][1] = yScale;
		mat[2][2] = zScale;
		mat[3][3] = 1.0f;
	}

	template<class T>
	inline void Matrix<T>::translation(float xShift, float yShift, float zShift)
	{
		mat[0][0] = 1.0f;
		mat[1][1] = 1.0f;
		mat[2][2] = 1.0f;
		mat[3][3] = 1.0f;
		mat[3][0] = xShift;
		mat[3][1] = yShift;
		mat[3][2] = zShift;
	}
}
#endif // !_MATRIX_H_
