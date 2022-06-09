#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <cstdint>
#include <chrono>

#include "Config.h"
#include "Vec.h"
#include "Matrix.h"
#include "Object.h"

#include "WidgetRect.h"

namespace graphics
{
	namespace algorithmType
	{
		enum AlgorithmType
		{
			NONE = -1,
			WARNOCK = 0,
			ROBERTS = 1,
			PAINTER = 2,
			ZBUFFER = 3,
		};
	}

	class Graphics
	{
	protected:
		uint32_t* _canvas = nullptr;

	public:
		Graphics() {};

	public:
		void drawPixel(int32_t x, int32_t y, uint32_t color);
		void drawPixel(int32_t x, int32_t y, uint32_t color, controls::WidgetRect* canvasRect);
		void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
		void drawPolygon(math::Vec2d<int32_t>* points, int16_t cpt, uint32_t color);
		void drawTriangle(math::Vec3d<int32_t>* points, uint32_t color);
		void fillTriangle(math::Vec3d<int32_t>* points, uint32_t color, controls::WidgetRect* canvasRect = nullptr);
		void fillRect(int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t color);

		void drawPixel(math::Vec2d<int32_t> coord, uint32_t color);
		void drawLine(math::Vec2d<int32_t> point1, math::Vec2d<int32_t> point2, uint32_t color);

		float drawObjects(std::vector<Triangle<float>>& transformedTriangles, int16_t algorithmType, int16_t matrixType, bool isFillingObject);

	protected:
		void drawLineBresenham(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
		math::Vec3d<float> getIntersectPlane(math::Vec3d<float>& plane_p, math::Vec3d<float>& plane_n, math::Vec3d<float>& lineStart, math::Vec3d<float>& lineEnd);
		int16_t clipTriangle(math::Vec3d<float> plane_p, math::Vec3d<float> plane_n, Triangle<float>& in_tri, Triangle<float>& out_tri1, Triangle<float>& out_tri2);

		math::Matrix<float> pointAt(math::Vec3d<float>& pos, math::Vec3d<float>& target, math::Vec3d<float>& up);
		math::Matrix<float> quickInverse(math::Matrix<float>& m);

		// Алгоритмы пересечения отрезков и многоугольников
		bool crossingNumber(math::Vec3d<float>* points[], int16_t numberPoint, math::Vec3d<float>* checkPoint);
		bool onSegment(const math::Vec3d<float>& p, const math::Vec3d<float>& q, const math::Vec3d<float>& r);
		int16_t orientation(const math::Vec3d<float>& p, const math::Vec3d<float>& q, const math::Vec3d<float>& r);
		bool doIntersect(const math::Vec3d<float>& p1, const math::Vec3d<float>& q1, const math::Vec3d<float>& p2, const math::Vec3d<float>& q2);
		math::Vec3d<float> segmentIntersection(const math::Vec3d<float>& A, const math::Vec3d<float>& B, math::Vec3d<float>& C, math::Vec3d<float>& D);
		float getZDepth(math::Vec3d<float>& A, math::Vec3d<float>& B, math::Vec3d<float>& C, math::Vec3d<float>& P);
		float getZDepth2(math::Vec3d<float>& A, math::Vec3d<float>& B, math::Vec3d<float>& C, int32_t x, int32_t y);

		// algorithm deleting invisible segmentrs
		void painterAlgorithm(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType, bool isFillingObject);
		void robertsAlgorithm(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType, bool isFillingObject);
		void warnockAlgorithm(const std::vector<Triangle<float>*>& transformedTriangles, float left, float top, float right, float bottom, int16_t matrixType);
		void zbufferAlgorithm(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType);

		virtual controls::WidgetRect getCanvasRect() = 0;

		virtual void clearCanvas(int32_t canvasSize) = 0;

		virtual uint32_t& at(int32_t x, int32_t y) = 0;
		virtual uint32_t at(int32_t x, int32_t y) const = 0;

		virtual void calculateObjects(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType) = 0;

		virtual bool checkBounds(int32_t x, int32_t y) const = 0;
		virtual bool checkBounds(math::Vec2d<int32_t> point) const = 0;

		virtual bool clipLine(int32_t& x1, int32_t& y1, int32_t& x2, int32_t& y2) = 0;
		virtual bool clipLine(math::Vec2d<int32_t>& point1, math::Vec2d<int32_t>& point2) = 0;
	};
}
#endif