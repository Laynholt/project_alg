#include "DrawingArea.h"

int16_t graphics::DrawingArea::_idCounter = 0;

graphics::DrawingArea::DrawingArea(int32_t x, int32_t y, int32_t width, int32_t heignt, uint32_t backgroud)
{
	_areaId = _idCounter++;
	_backgroundColor = backgroud;
	_areaRect = controls::WidgetRect(x, y, width, heignt);
	_canvas = new uint32_t[_areaRect.width * _areaRect.height];
}

graphics::DrawingArea::DrawingArea(controls::WidgetRect& areaSize, uint32_t backgroud)
{
	_areaId = _idCounter++;
	_areaRect = areaSize;
	_backgroundColor = backgroud;
	_canvas = new uint32_t[_areaRect.width * _areaRect.height];
}

graphics::DrawingArea::DrawingArea(const DrawingArea& area)
{
	_areaId = _idCounter++;
	_areaRect = area._areaRect;
	_backgroundColor = area._backgroundColor;

	if (_canvas != nullptr) delete[] _canvas;
	_canvas = new uint32_t[area._areaRect.width * area._areaRect.height];

	for (int16_t i = 0; i < area._areaRect.height; ++i)
	{
		for (int16_t j = 0; j < area._areaRect.width; ++j)
		{
			_canvas[i * _areaRect.width + j] = area._canvas[i * _areaRect.width + j];
		}
	}
}

graphics::DrawingArea::DrawingArea(DrawingArea&& area) noexcept
{
	_areaId = area._areaId;
	_areaRect = area._areaRect;
	_backgroundColor = area._backgroundColor;
	_canvas = area._canvas;
	area._canvas = nullptr;
}

graphics::DrawingArea::~DrawingArea()
{
	if (_canvas != nullptr)
		delete[] _canvas;
}

void graphics::DrawingArea::updateSize(controls::WidgetRect& newAreaRect)
{
	_areaRect = newAreaRect;

	uint32_t* tempCanvas = new uint32_t[_areaRect.width * _areaRect.height];
	delete[] _canvas;
	_canvas = tempCanvas;
}

void graphics::DrawingArea::addObject(int16_t objectType, std::wstring objectName, uint32_t color)
{
	int16_t size;
	objects.push_back(std::move(Object<float>(objectType, objectName, color)));
	size = objects.size();
	objects[size - 1].create();
}

graphics::Object<float>* graphics::DrawingArea::getObjectByIndex(int16_t index)
{
	if (index > objects.size() || index < 0) return nullptr;
	return &objects[index];
}

void graphics::DrawingArea::setObjectTransform(int16_t objectIndex, TransfromType type, float transformX, float transformY, float transformZ)
{
	if (objectIndex > objects.size() || objectIndex < 0) return;

	switch (type)
	{
	case graphics::TransfromType::Translation:
		objects[objectIndex].translate(transformX, transformY, transformZ);
		break;
	case graphics::TransfromType::Rotation:
		objects[objectIndex].rotate(transformX, transformY, transformZ);
		break;
	case graphics::TransfromType::Scale:
		objects[objectIndex].scale(transformX, transformY, transformZ);
		break;
	default:
		break;
	}
}

void graphics::DrawingArea::moveObject(graphics::Object<float>* objectPtr, int32_t moveX, int32_t moveY)
{
	objectPtr->setUserTranslateXY(moveX, moveY);
}

void graphics::DrawingArea::drawAreaEdge(uint32_t color)
{
	math::Vec2d<int32_t> points[] = { {_areaRect.left, _areaRect.top }, {_areaRect.right - 1, _areaRect.top}, 
		{_areaRect.right - 1, _areaRect.bottom - 1 }, {_areaRect.left, _areaRect.bottom - 1 } };

	drawPolygon(points, 4, config::widgets::color::areaBounds);
}

float graphics::DrawingArea::drawAreaObjects(int16_t algorithmType, int16_t matrixType, bool isFillingObject)
{
	clearCanvas(_areaRect.width * _areaRect.height);

	if (objects.size() == 0) return 0.0f;

	std::vector<Triangle<float>> transformedTriangles;
	calculateObjects(transformedTriangles, matrixType);
	return drawObjects(transformedTriangles, algorithmType, matrixType, isFillingObject);
}

void graphics::DrawingArea::calculateObjects(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType)
{
	// Выбор матрицы проекции
	math::Matrix<float>  matProjection;
	switch (matrixType)
	{
	case math::matrixType::PERSPECTIVE:
		{
			matProjection.projectionPerspective(90.0f, static_cast<float>(_areaRect.height) / _areaRect.width, 0.1f, 1000.0f);
			break;
		}
	case math::matrixType::ISOMETRIC:
		{
			matProjection.projectionIzometric();
			break;
		}
	case math::matrixType::CABINET:
		{
			matProjection.projectionCabinet();
			break;
		}
	default:
		break;
	}

	float scaleX = (matrixType == math::matrixType::PERSPECTIVE) ? _areaRect.width : _areaRect.width / 2.0f;
	float scaleY = (matrixType == math::matrixType::PERSPECTIVE) ? _areaRect.height : _areaRect.height / 2.0f;
	float scaleZ = (matrixType == math::matrixType::PERSPECTIVE) ? 1.0f : _areaRect.width / 2.0f;

	math::Matrix<float> centeringMatrix;
	centeringMatrix.init();
	centeringMatrix.translation(_areaRect.width / 2, _areaRect.height / 2, 10.0f);

	int16_t i = 0;
	for (auto& obj : objects)
	{
		// Устанавливаем начальные преобразования
		setObjectTransform(i, graphics::TransfromType::Translation, config::objectPreset::standartPositionX, config::objectPreset::standartPositionY, config::objectPreset::standartPositionZ);
		setObjectTransform(i, graphics::TransfromType::Scale, scaleX, scaleY, scaleZ);
		setObjectTransform(i, graphics::TransfromType::Rotation, config::objectPreset::standartRotationX, config::objectPreset::standartRotationY, config::objectPreset::standartRotationZ);


		for (auto& tri : obj.faces.triangles)
		{
			Triangle<float>  triTransformed;

			triTransformed = tri * obj.getTransform();
			triTransformed = triTransformed * matProjection;

			if (matrixType == math::matrixType::PERSPECTIVE)
				for (int16_t j = 0; j < 3; ++j)
					triTransformed.points[j] = triTransformed.points[j] / (triTransformed.points[j].w * 0.1f);
			triTransformed = triTransformed * centeringMatrix;

			triTransformed.setColor(obj.color);
			transformedTriangles.push_back(triTransformed);
			transformedTriangles.back().objectId = obj.objectId;
		}
		++i;
	}
}

void graphics::DrawingArea::clearCanvas(int32_t canvasSize)
{
	for (int32_t i = 0; i < canvasSize; ++i)
	{
		_canvas[i] = _backgroundColor;
	}
}


uint32_t& graphics::DrawingArea::at(int32_t x, int32_t y)
{
	if (!checkBounds(x, y))
		return _canvas[0];
	return _canvas[(y - _areaRect.top) * _areaRect.width + (x - _areaRect.left)];
}

uint32_t graphics::DrawingArea::at(int32_t x, int32_t y) const
{
	if (!checkBounds(x, y))
		return _canvas[0];
	return _canvas[(y - _areaRect.top) * _areaRect.width + (x - _areaRect.left)];
}


bool graphics::DrawingArea::checkBounds(int32_t x, int32_t y) const
{
	if ((x >= _areaRect.left) && (x <= _areaRect.right - 1) &&
		(y >= _areaRect.top) && (y <= _areaRect.bottom - 1))
		return true;
	return false;
}

bool graphics::DrawingArea::checkBounds(math::Vec2d<int32_t> point) const
{
	if ((point.x >= _areaRect.left) && (point.x <= _areaRect.right - 1) &&
		(point.y >= _areaRect.top) && (point.y <= _areaRect.bottom - 1))
		return true;
	return false;
}

bool graphics::DrawingArea::checkIntersection(math::Vec2d<int32_t>& point1, math::Vec2d<int32_t>& point2, math::Vec2d<int32_t>& boundPnt1, 
	math::Vec2d<int32_t>& boundPnt2, math::Vec2d<int32_t>& interPnt)
{
	// https://habr.com/ru/post/523440/

	float n;
	if (point2.y - point1.y != 0)
	{
		float q = (point2.x - point1.x) / (point1.y - point2.y);
		float sn = (boundPnt1.x - boundPnt2.x) + (boundPnt1.y - boundPnt2.y) * q;
		if (!sn)	return false;

		float fn = (boundPnt1.x - point1.x) + (boundPnt1.y - point1.y) * q;
		n = fn / sn;
	}
	else
	{
		if ((boundPnt1.y - boundPnt2.y) == 0)	return false;
		n = (boundPnt1.y - point1.y) / (boundPnt1.y - boundPnt2.y);
	}

	interPnt.x = boundPnt1.x + (boundPnt2.x - boundPnt1.x) * n;
	interPnt.y = boundPnt1.y + (boundPnt2.y - boundPnt1.y) * n;
	return true;
}

bool graphics::DrawingArea::clipLine(int32_t& x1, int32_t& y1, int32_t& x2, int32_t& y2)
{
	math::Vec2d<int32_t>  points[2] = { {x1, y1}, {x2, y2} };

	bool retVal = clipLine(points[0], points[1]);
	x1 = points[0].x, y1 = points[0].y;
	x2 = points[1].x, y2 = points[1].y;
	return retVal;
}

bool graphics::DrawingArea::clipLine(math::Vec2d<int32_t>& point1, math::Vec2d<int32_t>& point2)
{
	math::Vec2d<int32_t>  interPnt;

	// Границы области
	math::Vec2d<int32_t>  boundPnt[4] = { {_areaRect.left, _areaRect.top }, {_areaRect.right, _areaRect.top }, 
		{_areaRect.right, _areaRect.bottom }, {_areaRect.left, _areaRect.bottom } };

	for (int16_t i = 0, next = 0; i < 4; ++i)
	{
		next = (i + 1) % 4;

		// Если прямые совпадают
		if ((point1.x == boundPnt[i].x && point2.x == boundPnt[next].x) ||
			(point1.y == boundPnt[i].y && point2.y == boundPnt[next].y))
		{
			// !Может происходит реверс точек (была сверху, стала снизу)
			point1 = boundPnt[i];
			point2 = boundPnt[next];
			return true;;
		}

		if (checkIntersection(point1, point2, boundPnt[i], boundPnt[next], interPnt))
		{
			math::Vec2d<int32_t>  delt1, delt2;

			delt1 = interPnt - point1;
			delt2 = interPnt - point2;

			if (i == 0)
			{
				// Тк если вторая точка внутри, то разность отрицательная / внутри отрицательное
				if (delt1.y > delt2.y) { point1 = interPnt; }
				else { point2 = interPnt; }
			}
			else if (i == 1)
			{
				// Внутри положительное
				if (delt1.x < delt2.x) { point1 = interPnt; }
				else { point2 = interPnt; }
			}
			else if (i == 2)
			{
				// внутри положительное
				if (delt1.y < delt2.y) { point1 = interPnt; }
				else { point2 = interPnt; }
			}
			else if (i == 3)
			{
				// внутри отрицательное
				if (delt1.x > delt2.x) { point1 = interPnt; }
				else { point2 = interPnt; }
			}
		}
	}

	return false;
}