#ifndef _DRAWINGAREA_H_
#define _DRAWINGAREA_H_

#include "Graphics.h"

#include "Vec.h"
#include "Matrix.h"
#include "WidgetRect.h"

namespace graphics
{
	enum class TransfromType : int16_t
	{
		Translation,
		Rotation,
		Scale
	};

	class DrawingArea : public Graphics
	{
	public:
		DrawingArea(int32_t x, int32_t y, int32_t width, int32_t heignt, uint32_t backgroud);
		DrawingArea(controls::WidgetRect& areaSize, uint32_t backgroud);
		DrawingArea(const DrawingArea& area);
		DrawingArea(DrawingArea&& area) noexcept;
		~DrawingArea();

		int16_t getAreaId() const { return _areaId; }
		int32_t getX() const { return _areaRect.left; }
		int32_t getY() const { return _areaRect.top; }
		int32_t getWidth() const { return _areaRect.width; }
		int32_t getHeight() const { return _areaRect.height; }
		controls::WidgetRect getRect() const { return _areaRect; }
		uint32_t* getCanvas() const { return _canvas; }
		uint32_t getBackgroundColor() const { return _backgroundColor; }
		
		void setBackgroundColor(uint32_t background) { _backgroundColor = background; }

		void updateSize(controls::WidgetRect& newAreaRect);

		void addObject(int16_t objectType, std::wstring objectName, uint32_t color);
		Object<float>* getObjectByIndex(int16_t index);
		void setObjectTransform(int16_t objectIndex, TransfromType type, float transformX, float transformY, float transformZ);
		void moveObject(graphics::Object<float>* objectPtr, int32_t moveX, int32_t moveY);

		void drawAreaEdge(uint32_t color);
		float drawAreaObjects(int16_t algorithmType, int16_t matrixType, bool isFillingObject);

	protected:
		virtual controls::WidgetRect getCanvasRect() override { return _areaRect; }
		virtual void clearCanvas(int32_t canvasSize) override;

		virtual uint32_t& at(int32_t x, int32_t y) override;
		virtual uint32_t at(int32_t x, int32_t y) const override;

		virtual void calculateObjects(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType) override;

		virtual bool checkBounds(int32_t x, int32_t y) const override;
		virtual bool checkBounds(math::Vec2d<int32_t> point) const override;

		bool checkIntersection(math::Vec2d<int32_t>& point1, math::Vec2d<int32_t>& point2, math::Vec2d<int32_t>& boundPnt1, 
			math::Vec2d<int32_t>& boundPnt2, math::Vec2d<int32_t>& interPnt);

		virtual bool clipLine(int32_t& x1, int32_t& y1, int32_t& x2, int32_t& y2) override;
		virtual bool clipLine(math::Vec2d<int32_t>& point1, math::Vec2d<int32_t>& point2) override;

	private:
		int16_t _areaId;
		static int16_t _idCounter;
		controls::WidgetRect _areaRect;
		uint32_t _backgroundColor;

		std::vector<Object<float>> objects;

	public:
		math::Vec3d<float> vCamera{};	// Location of camera in world space
		math::Vec3d<float> vLookDir{};	// Direction vector along the direction camera points
		float fYaw{};		// FPS Camera rotation in XZ plane
	};
}
#endif