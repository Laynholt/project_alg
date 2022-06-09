#include "Graphics.h"

void graphics::Graphics::drawPixel(int32_t x, int32_t y, uint32_t color)
{
	if (checkBounds(x, y)) at(x, y) = color;
}

void graphics::Graphics::drawPixel(int32_t x, int32_t y, uint32_t color, controls::WidgetRect* canvasRect)
{
	if ((x >= canvasRect->left) && (x <= canvasRect->right - 1) &&
		(y >= canvasRect->top) && (y <= canvasRect->bottom - 1)) at(x, y) = color;
}

void graphics::Graphics::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
{
	//int16_t pntIn = 0;
	//bool needClip = false;

	//if (checkBounds(x1, y1)) ++pntIn;
	//else needClip = true;

	//if (checkBounds(x2, y2)) ++pntIn;
	//else needClip = true;

	///*if (pntIn > 0 && needClip)
	//	clipLine(x1, y1, x2, y2);*/

	//if (pntIn > 0)
	{
		drawLineBresenham(x1, y1, x2, y2, color);
	}
}

void graphics::Graphics::drawLineBresenham(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color)
{
	/*	dx = (x2-x1)
		dy = (y2-y1)

		f(x,y) = dy*X - dx*Y - (x1*dy - y1*dx)

		f(M) = dy - dx/2
		f(T) = f(M) + dy - dx
		f(B) = f(M) + dy
	*/

	int16_t x, y;
	int16_t deltaX, deltaY;
	int16_t signX, signY;
	int16_t balance;

	signX = (x2 > x1) ? 1 : -1;
	signY = (y2 > y1) ? 1 : -1;

	deltaX = (signX > 0) ? (x2 - x1) : (x1 - x2);
	deltaY = (signY > 0) ? (y2 - y1) : (y1 - y2);

	x = x1; y = y1;

	if (deltaX >= deltaY)				// We should use larger axis [X>=Y]
	{
		deltaY <<= 1;
		balance = deltaY - deltaX;		// f(M)
		deltaX <<= 1;

		while (x != x2)
		{
			drawPixel(x, y, color);
			if (balance >= 0)
			{
				y += signY;
				balance -= deltaX;		// f(T) = f(M) - dx => half part f(T) [still need dy]
			}

			balance += deltaY;			// f(M) + dy => f(B) or part of f(T)
			x += signX;
		}

		drawPixel(x, y, color);
	}

	else								// Similarly for axis [Y>X]
	{
		deltaX <<= 1;
		balance = deltaX - deltaY;
		deltaY <<= 1;

		while (y != y2)
		{
			drawPixel(x, y, color);
			if (balance >= 0)
			{
				x += signX;
				balance -= deltaY;
			}

			balance += deltaX;
			y += signY;
		}

		drawPixel(x, y, color);
	}
}

math::Vec3d<float> graphics::Graphics::getIntersectPlane(math::Vec3d<float>& plane_p, math::Vec3d<float>& plane_n, math::Vec3d<float>& lineStart, math::Vec3d<float>& lineEnd)
{
	plane_n.normalize();
	float plane_d = -plane_n.dotProduct(plane_p);
	float ad = lineStart.dotProduct(plane_n);
	float bd = lineEnd.dotProduct(plane_n);
	float t = (-plane_d - ad) / (bd - ad);

	math::Vec3d<float> lineStartToEnd = lineEnd - lineStart;
	math::Vec3d<float> lineToIntersect = lineStartToEnd * t;

	return (lineStart + lineToIntersect);
}

int16_t graphics::Graphics::clipTriangle(math::Vec3d<float> plane_p, math::Vec3d<float> plane_n, Triangle<float>& in_tri, Triangle<float>& out_tri1, Triangle<float>& out_tri2)
{
	// Make sure plane normal is indeed normal
	plane_n.normalize();

	// Return signed shortest distance from point to plane, plane normal must be normalised
	auto dist = [&](math::Vec3d<float>& p)
	{
		math::Vec3d<float> n = p;
		n.normalize();
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - plane_n.dotProduct(plane_p));
	};

	// Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	math::Vec3d<float>* inside_points[3];  int nInsidePointCount = 0;
	math::Vec3d<float>* outside_points[3]; int nOutsidePointCount = 0;

	// Get signed distance of each point in triangle to plane
	float d0 = dist(in_tri.points[0]);
	float d1 = dist(in_tri.points[1]);
	float d2 = dist(in_tri.points[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[0]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.points[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[1]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.points[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.points[2]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.points[2]; }

	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		out_tri1 = in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// Copy appearance info to new triangle
		out_tri1.color = in_tri.color;

		// The inside point is valid, so keep that...
		out_tri1.points[0] = *inside_points[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		out_tri1.points[1] = getIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri1.points[2] = getIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles
		out_tri1.color = in_tri.color;
		out_tri2.color = in_tri.color;

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1.points[0] = *inside_points[0];
		out_tri1.points[1] = *inside_points[1];
		out_tri1.points[2] = getIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2.points[0] = *inside_points[1];
		out_tri2.points[1] = out_tri1.points[2];
		out_tri2.points[2] = getIntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

		return 2; // Return two newly formed triangles which form a quad
	}
}

math::Matrix<float> graphics::Graphics::pointAt(math::Vec3d<float>& pos, math::Vec3d<float>& target, math::Vec3d<float>& up)
{
	// Calculate new forward direction
	math::Vec3d<float> newForward = target -  pos;
	newForward.normalize();

	// Calculate new Up direction
	math::Vec3d<float> a = newForward * up.dotProduct(newForward);
	math::Vec3d<float> newUp = up - a;
	newUp.normalize();

	// New Right direction is easy, its just cross product
	math::Vec3d<float> newRight = newUp.crossProduct(newForward);

	// Construct Dimensioning and Translation Matrix	
	math::Matrix<float> matrix;
	matrix.mat[0][0] = newRight.x;	matrix.mat[0][1] = newRight.y;	matrix.mat[0][2] = newRight.z;	matrix.mat[0][3] = 0.0f;
	matrix.mat[1][0] = newUp.x;		matrix.mat[1][1] = newUp.y;		matrix.mat[1][2] = newUp.z;		matrix.mat[1][3] = 0.0f;
	matrix.mat[2][0] = newForward.x;	matrix.mat[2][1] = newForward.y;	matrix.mat[2][2] = newForward.z;	matrix.mat[2][3] = 0.0f;
	matrix.mat[3][0] = pos.x;			matrix.mat[3][1] = pos.y;			matrix.mat[3][2] = pos.z;			matrix.mat[3][3] = 1.0f;
	return matrix;
}

math::Matrix<float> graphics::Graphics::quickInverse(math::Matrix<float>& m)
{
	math::Matrix<float> matrix;
	matrix.mat[0][0] =   m.mat[0][0];  matrix.mat[0][1] = m.mat[1][0];  matrix.mat[0][2] = m.mat[2][0];  matrix.mat[0][3] = 0.0f;
	matrix.mat[1][0] =   m.mat[0][1];  matrix.mat[1][1] = m.mat[1][1];  matrix.mat[1][2] = m.mat[2][1];  matrix.mat[1][3] = 0.0f;
	matrix.mat[2][0] =	 m.mat[0][2];  matrix.mat[2][1] = m.mat[1][2];  matrix.mat[2][2] = m.mat[2][2];  matrix.mat[2][3] = 0.0f;
	matrix.mat[3][0] = -(m.mat[3][0] * matrix.mat[0][0] + m.mat[3][1] * matrix.mat[1][0] + m.mat[3][2] * matrix.mat[2][0]);
	matrix.mat[3][1] = -(m.mat[3][0] * matrix.mat[0][1] + m.mat[3][1] * matrix.mat[1][1] + m.mat[3][2] * matrix.mat[2][1]);
	matrix.mat[3][2] = -(m.mat[3][0] * matrix.mat[0][2] + m.mat[3][1] * matrix.mat[1][2] + m.mat[3][2] * matrix.mat[2][2]);
	matrix.mat[3][3] = 1.0f;
	return matrix;
}

bool graphics::Graphics::crossingNumber(math::Vec3d<float>* points[], int16_t numberPoint, math::Vec3d<float>* checkPoint)
{
	// This function determines number of intersections with polygon
	// https://stackoverflow.com/questions/27589796/check-point-within-polygon

	int16_t cn = 0;    // the  crossing number counter
	int16_t i = 0;

	do
	{
		int16_t next = (i + 1) % numberPoint;

		if (((points[i]->y <= checkPoint->y) && (points[next]->y >  checkPoint->y)) ||
			((points[i]->y >  checkPoint->y) && (points[next]->y <= checkPoint->y)))
		{
			// Высчитываем X точку пересечения, выпущенного луча с прямой (как в Scanline)
			float deltaXY = (points[next]->x - points[i]->x) / (points[next]->y - points[i]->y);
			if (checkPoint->x < points[i]->x + deltaXY * (checkPoint->y - points[i]->y))
				++cn;
		}
		i = next;
	} while (i != 0);

	return (cn & 1);    // 0 if even (out), and 1 if  odd (in)
}

bool graphics::Graphics::onSegment(const math::Vec3d<float>& p, const math::Vec3d<float>& q, const math::Vec3d<float>& r)
{
	// Find intersections between two segments
		// https://www.youtube.com/watch?v=bbTqI0oqL5U&t=596s&ab_channel=TECHDOSE

	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}

int16_t graphics::Graphics::orientation(const math::Vec3d<float>& p, const math::Vec3d<float>& q, const math::Vec3d<float>& r)
{
	// To find orientation of ordered triplet (p, q, r). 
	// The function returns following values 
	// 0 --> p, q and r are colinear 
	// 1 --> Clockwise 
	// 2 --> Counterclockwise 

	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
		// for details of below formula. 
	int32_t val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;			// colinear 

	return (val > 0) ? 1 : 2;		// clock or counterclock wise 
}

bool graphics::Graphics::doIntersect(const math::Vec3d<float>& p1, const math::Vec3d<float>& q1, const math::Vec3d<float>& p2, const math::Vec3d<float>& q2)
{
	// The main function that returns true if line segment 'p1q1' 
	// and 'p2q2' intersect. 

	// Find the four orientations needed for general and 
		// special cases 
	int16_t o1 = orientation(p1, q1, p2);
	int16_t o2 = orientation(p1, q1, q2);
	int16_t o3 = orientation(p2, q2, p1);
	int16_t o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases 
}

math::Vec3d<float> graphics::Graphics::segmentIntersection(const math::Vec3d<float>& A, const math::Vec3d<float>& B, math::Vec3d<float>& C, math::Vec3d<float>& D)
{
	// Line AB represented as a1x + b1y = c1
	float a1 = B.y - A.y;
	float b1 = A.x - B.x;
	float c1 = a1 * (A.x) + b1 * (A.y);

	// Line CD represented as a2x + b2y = c2
	float a2 = D.y - C.y;
	float b2 = C.x - D.x;
	float c2 = a2 * (C.x) + b2 * (C.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant <= 0.000001)
	{
		return math::Vec3d<float>(FLT_MAX, FLT_MAX, FLT_MAX);
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		float y = (a1 * c2 - a2 * c1) / determinant;
		float z = (C.z + D.z) / 2.0f;
		return math::Vec3d<float>(x, y, z);
	}
}

float graphics::Graphics::getZDepth(math::Vec3d<float>& A, math::Vec3d<float>& B, math::Vec3d<float>& C, math::Vec3d<float>& P)
{
	math::Vec3d<float> line1(B - A), line2(C - A);
	math::Vec3d<float> faceNormal = line1.crossProduct(line2);
	faceNormal.normalize();

	math::Vec3d<float> lineNormal(0.0f, 0.0f, -1.0f);
	lineNormal.normalize();

	float t;
	t = (faceNormal.dotProduct(A) - faceNormal.dotProduct(P)) / faceNormal.dotProduct(lineNormal);

	math::Vec3d<float> p = P + lineNormal * t;
	return p.z;
}

float graphics::Graphics::getZDepth2(math::Vec3d<float>& A, math::Vec3d<float>& B, math::Vec3d<float>& C, int32_t x, int32_t y)
{
	math::Vec3d<float> line1(B - A), line2(C - A);
	math::Vec3d<float> faceNormal = line1.crossProduct(line2);

	math::Vec3d<float> temp = B.crossProduct(C);
	float zDepth = A.dotProduct(temp) - x * faceNormal.x - y * faceNormal.y / faceNormal.z;
	return zDepth;
}

void graphics::Graphics::painterAlgorithm(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType, bool isFillingObject)
{
	// Sort triangles from back to front
	std::sort(transformedTriangles.begin(), transformedTriangles.end(), [this, matrixType](Triangle<float>& t1, Triangle<float>& t2)
		{
			float z1{}, z2{};
			if (matrixType != math::matrixType::CABINET)
			{
				z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
				z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;
			}
			else
			{
				z1 = getZDepth2(t1.points[0], t1.points[1], t1.points[2], (t1.points[0].x + t1.points[1].x + t1.points[2].x) / 3.0f,
					(t1.points[0].y + t1.points[1].y + t1.points[2].y) / 3.0f);
				z2 = getZDepth2(t2.points[0], t2.points[1], t2.points[2], (t2.points[0].x + t2.points[1].x + t2.points[2].x) / 3.0f,
					(t1.points[0].y + t2.points[1].y + t2.points[2].y) / 3.0f);
			}
			return z1 > z2;
	});

	for (auto& tri : transformedTriangles)
	{
		math::Vec3d<int32_t> points[3];
		for (int16_t i = 0; i < 3; i++)
		{
			points[i].x = roundf(tri.points[i].x);
			points[i].y = roundf(tri.points[i].y);
		}

		if (isFillingObject)
			fillTriangle(points, tri.color);
		//else
			drawTriangle(points, config::objectPreset::color::standartEdgesColor);
	}
}

void graphics::Graphics::robertsAlgorithm(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType, bool isFillingObject)
{
	math::Vec3d<float> normal, line1, line2;
	math::Vec3d<float> vCameraRay, vCamera;
	std::vector<std::pair<std::vector<Triangle<float>*>, int16_t>> visibleTriangles;

	controls::WidgetRect screen = getCanvasRect();
	if (matrixType == math::matrixType::ISOMETRIC)
		vCamera = { screen.right / 2.0f, screen.bottom / 2.0f, screen.right / 2.0f };
	else
		vCamera = { 0.0f, 0.0f, -1.0f };

	// Первая часть алгоритма (отбрасывание невидимых граней)
	for (auto& tri : transformedTriangles)
	{
		line1 = tri.points[1] - tri.points[0];
		line2 = tri.points[2] - tri.points[0];

		normal = line1.crossProduct(line2);
		normal.normalize();

		vCameraRay = tri.points[0] - vCamera;

		if (normal.dotProduct(vCameraRay) < 0.0f)
		{
			if (visibleTriangles.empty())
			{
				visibleTriangles.push_back(std::make_pair(std::vector<Triangle<float>*>(1), tri.objectId));
				visibleTriangles.back().first.back() = &tri;
			}
			else
			{
				bool found = false;
				for (auto& pairs : visibleTriangles)
				{
					if (pairs.second == tri.objectId)
					{
						pairs.first.push_back(&tri);
						found = true;
						break;
					}
				}

				if (!found)
				{
					visibleTriangles.push_back(std::make_pair(std::vector<Triangle<float>*>(1), tri.objectId));
					visibleTriangles.back().first.back() = &tri;
				}
			}
		}
	}

	if (visibleTriangles.size() == 1)
	{
		for (auto& tri : visibleTriangles.back().first)
		{
			math::Vec3d<int32_t> points[3];
			for (int16_t i = 0; i < 3; i++)
			{
				points[i].x = roundf(tri->points[i].x);
				points[i].y = roundf(tri->points[i].y);
			}

			fillTriangle(points, tri->color);
		}
		return;
	}
	else
	{
		std::vector<Triangle<float>> tmp;
		for (auto& pairs : visibleTriangles)
		{
			for (auto& tri : pairs.first)
				tmp.push_back(*tri);
		}

		zbufferAlgorithm(tmp, matrixType);
	}

	// Вторая часть алгоритма (анализ и разбиение ребер)

	// Сортируем треугольники у каждого объекта от ближнего до дальнего
	//for (auto& pairs : visibleTriangles)
	//{
	//	std::sort(pairs.first.begin(), pairs.first.end(), [this, matrixType](Triangle<float>& t1, Triangle<float>& t2)
	//		{
	//			float z1{}, z2{};
	//			if (matrixType != math::matrixType::CABINET)
	//			{
	//				z1 = (t1.points[0].z + t1.points[1].z + t1.points[2].z) / 3.0f;
	//				z2 = (t2.points[0].z + t2.points[1].z + t2.points[2].z) / 3.0f;
	//			}
	//			else
	//			{
	//				z1 = getZDepth2(t1.points[0], t1.points[1], t1.points[2], (t1.points[0].x + t1.points[1].x + t1.points[2].x) / 3.0f,
	//					(t1.points[0].y + t1.points[1].y + t1.points[2].y) / 3.0f);
	//				z2 = getZDepth2(t2.points[0], t2.points[1], t2.points[2], (t2.points[0].x + t2.points[1].x + t2.points[2].x) / 3.0f,
	//					(t1.points[0].y + t2.points[1].y + t2.points[2].y) / 3.0f);
	//			}
	//			return z1 < z2;
	//		}
	//	);
	//}

	//// Сортируем объекты по Z в соответствии с Z координатой их первой (максимально отдаленной грани)
	//std::sort(visibleTriangles.begin(), visibleTriangles.end(), [this, matrixType](std::pair<std::vector<Triangle<float>*>, int16_t> pair1, std::pair<std::vector<Triangle<float>*>, int16_t> pair2)
	//	{
	//		float z1{}, z2{};
	//		Triangle<float>* p1, *p2;

	//		p1 = pair1.first.front();
	//		p2 = pair2.first.front();

	//		if (matrixType != math::matrixType::CABINET)
	//		{
	//			z1 = (p1->points[0].z + p1->points[1].z + p1->points[2].z) / 3.0f;
	//			z2 = (p2->points[0].z + p2->points[1].z + p2->points[2].z) / 3.0f;
	//		}
	//		else
	//		{
	//			z1 = getZDepth2(p1->points[0], p1->points[1], p1->points[2], (p1->points[0].x + p1->points[1].x + p1->points[2].x) / 3.0f,
	//				(p1->points[0].y + p1->points[1].y + p1->points[2].y) / 3.0f);
	//			z2 = getZDepth2(p2->points[0], p2->points[1], p2->points[2], (p2->points[0].x + p2->points[1].x + p2->points[2].x) / 3.0f,
	//				(p1->points[0].y + p2->points[1].y + p2->points[2].y) / 3.0f);
	//		}
	//		return z1 < z2;
	//	}
	//);


}

//#define WARNOCK_OPTIMIZED
void graphics::Graphics::warnockAlgorithm(const std::vector<Triangle<float>*>& transformedTriangles, float left, float top, float right, float bottom, int16_t matrixType)
{
	math::Vec3d<float> windowRect[4] = {
		{left, top, 0.0f}, {right, top, 0.0f},
		{right, bottom, 0.0f}, {left, bottom, 0.0f}
	};

	math::Vec3d<float>* pointsCheck[4];
	int16_t crossingPointsCounter = 0;

	std::vector<std::pair<Triangle<float>*, int16_t>> tempTriangles;
	// Проходимся по всем треугольникам
	for (auto& tri : transformedTriangles)
	{
		crossingPointsCounter = 0;

		// Смотрим есть ли точки треугольника в окне
		pointsCheck[0] = &windowRect[0];
		pointsCheck[1] = &windowRect[1];
		pointsCheck[2] = &windowRect[2];
		pointsCheck[3] = &windowRect[3];

		for (int16_t i = 0; i < 3; ++i)
			if (crossingNumber(pointsCheck, 4, &tri->points[i])) ++crossingPointsCounter;
		
		// Лежит ли внутри хотя бы 1 точка?
		if (crossingPointsCounter)
		{
			tempTriangles.push_back(std::make_pair(tri, crossingPointsCounter));
			continue;
		}

		// Иначе треугольник не лежит в окне
		// Проверяем, лежит ли окно в треугольнике
		pointsCheck[0] = &tri->points[0];
		pointsCheck[1] = &tri->points[1];
		pointsCheck[2] = &tri->points[2];
		pointsCheck[3] = nullptr;

		crossingPointsCounter = 0;

		for (int16_t i = 0; i < 4; ++i)
			if (crossingNumber(pointsCheck, 3, &windowRect[i])) ++crossingPointsCounter;

		// Лежит ли внутри хотя бы 1 точка?
		if (crossingPointsCounter)
		{
			tempTriangles.push_back(std::make_pair(tri, -crossingPointsCounter));
		}

#ifdef WARNOCK_OPTIMIZED
		// Если ни треугольник, ни окно не содержат точки друг друга, то проверяем, пересекаются ли их отрезки

		float minX = min(min(tri->points[0].x, tri->points[1].x), tri->points[2].x);
		float maxX = max(max(tri->points[0].x, tri->points[1].x), tri->points[2].x);

		float minY = min(min(tri->points[0].y, tri->points[1].y), tri->points[2].y);
		float maxY = max(max(tri->points[0].y, tri->points[1].y), tri->points[2].y);

		if (minX > right || minY > bottom) continue;
		if (maxX < left  || maxY < top) continue;

		if ((right - left <= 1.0f) && (bottom - top <= 1.0f)) continue;

		int16_t i = 0;
		bool isIntersected = false;
		do
		{
			int16_t nextI = (i + 1) % 3;

			// Ищем пересечение каждого ребра треугольника с каждым ребром окна
			int16_t j = 0;
			do
			{
				int16_t nextJ = (j + 1) % 4;
				if (doIntersect(tri->points[i], tri->points[nextI], windowRect[j], windowRect[nextJ])) { isIntersected = true; break; }
				j = nextJ;
			} while (j != 0);
			if (isIntersected) break;

			i = nextI;
		} while (i != 0);

		if (isIntersected)
			tempTriangles.push_back(std::make_pair(tri, 0));
#endif // WARNOCK_OPTIMIZED
	}

	// Show net
	drawLine(left, top, right, top, config::objectPreset::color::standartEdgesColor);
	drawLine(left, top, left, bottom, config::objectPreset::color::standartEdgesColor);
	drawLine(left, bottom, right, bottom, config::objectPreset::color::standartEdgesColor);
	drawLine(right, top, right, bottom, config::objectPreset::color::standartEdgesColor);

	// Анализируем сохраненные нами треугольники
		// Если ничего нет, то просто выходим
	if (tempTriangles.empty())
	{
		//fillRect(left, top, right, bottom, 45000);
		return;
	}
	else if (tempTriangles.size() == 1)
	{
		// Если у нас всего 1 треугольник, смотрим какой он
		switch (tempTriangles.front().second)
		{
			// Если он закрывает собой весь экран, то красим окно в его цвет
		case -4:
			fillRect(left, top, right, bottom, tempTriangles.front().first->color);
			break;
			// Если он лежит внутри окна
		case 3:
		{
			math::Vec3d<int32_t> p[3];
			for (int16_t i = 0; i < 3; i++)
			{
				p[i].x = roundf(tempTriangles.front().first->points[i].x);
				p[i].y = roundf(tempTriangles.front().first->points[i].y);
			}
			fillTriangle(p, tempTriangles.front().first->color);
			break;
		}
			// В ином случае он частично лежит в окне
		default:
			// TODO:
			// FillPartOfTriangle(...)
			math::Vec3d<int32_t> p[3];
			for (int16_t i = 0; i < 3; i++)
			{
				p[i].x = roundf(tempTriangles.front().first->points[i].x);
				p[i].y = roundf(tempTriangles.front().first->points[i].y);
			}
			controls::WidgetRect canvasRect(left, top, right - left, bottom - top);
			//fillRect(left, top, right, bottom, 64000);
			fillTriangle(p, tempTriangles.front().first->color, &canvasRect);
			break;
		}
	}	// Если несколько треугольников в окне, то разбиваем его на подокна
	else
	{
		float deltaX, deltaY;
		deltaX = (right - left > 1.0f) ? ((right - left) / 2.0f) : 1.0f;
		deltaY = (bottom - top > 1.0f) ? ((bottom - top) / 2.0f) : 1.0f;
		
		if (deltaX) deltaX = roundf(deltaX);
		if (deltaY) deltaY = roundf(deltaY);
		
		// Если большие окна, то разбиваем их на 4 части
		if ((right - left > 1.0f) && (bottom - top > 1.0f))
		{
#ifdef WARNOCK_OPTIMIZED
			std::vector<Triangle<float>*> triangleOnWindow;
			for (auto& t : tempTriangles)
			{
				triangleOnWindow.push_back(t.first);
			}
			tempTriangles.clear();

			warnockAlgorithm(triangleOnWindow, left, top, left + deltaX, top + deltaY, matrixType);			// Левое верхнее
			warnockAlgorithm(triangleOnWindow, left + deltaX, top, right, top + deltaY, matrixType);		// Правое верхнее
			warnockAlgorithm(triangleOnWindow, left, top + deltaY, left + deltaX, bottom, matrixType);		// Левое нижнее
			warnockAlgorithm(triangleOnWindow, left + deltaX, top + deltaY, right, bottom, matrixType);		// Правое нижнее
#else			
			warnockAlgorithm(transformedTriangles, left, top, left + deltaX, top + deltaY, matrixType);			// Левое верхнее
			warnockAlgorithm(transformedTriangles, left + deltaX, top, right, top + deltaY, matrixType);		// Правое верхнее
			warnockAlgorithm(transformedTriangles, left, top + deltaY, left + deltaX, bottom, matrixType);		// Левое нижнее
			warnockAlgorithm(transformedTriangles, left + deltaX, top + deltaY, right, bottom, matrixType);		// Правое нижнее
#endif
		}
		// Если по Y 1 пиксель
		else if ((right - left > 1.0f) && (bottom - top <= 1.0f))
		{
#ifdef WARNOCK_OPTIMIZED
			std::vector<Triangle<float>*> triangleOnWindow;
			for (auto& t : tempTriangles)
			{
				triangleOnWindow.push_back(t.first);
			}
			tempTriangles.clear();

			// Разбиваем на два горизонтальных окна
			warnockAlgorithm(triangleOnWindow, left, top, left + deltaX, bottom, matrixType);		// Левое
			warnockAlgorithm(triangleOnWindow, left + deltaX, top, right, bottom, matrixType);		// Правое
#else
			warnockAlgorithm(transformedTriangles, left, top, left + deltaX, bottom, matrixType);		// Левое
			warnockAlgorithm(transformedTriangles, left + deltaX, top, right, bottom, matrixType);		// Правое
#endif
		}
		// Если по X 1 пиксель
		else if ((right - left <= 1.0f) && (bottom - top > 1.0f))
		{
#ifdef WARNOCK_OPTIMIZED
			std::vector<Triangle<float>*> triangleOnWindow;
			for (auto& t : tempTriangles)
			{
				triangleOnWindow.push_back(t.first);
			}
			tempTriangles.clear();

			// Разбиваем на два вертикальных окна
			warnockAlgorithm(triangleOnWindow, left, top, right, top + deltaY, matrixType);			// Верхнее
			warnockAlgorithm(triangleOnWindow, left, top + deltaY, right, bottom, matrixType);		// Нижнее
#else
			warnockAlgorithm(transformedTriangles, left, top, right, top + deltaY, matrixType);			// Верхнее
			warnockAlgorithm(transformedTriangles, left, top + deltaY, right, bottom, matrixType);		// Нижнее
#endif
		}
		else
		{
			// Если окно в 1 пиксель
			// Высчитываем минимальную глубину среди всех треугольников и закрашиваем нужным цветом

			math::Vec3d<float> p(static_cast<float>(left), static_cast<float>(top), 0.0f);
			uint32_t color{64000};
			float minZ = FLT_MAX; float tempZ{};

			for (auto& tri : tempTriangles)
			{
				// Работает для изометрической проекции
				if (matrixType != math::matrixType::CABINET)
					tempZ = getZDepth(tri.first->points[0], tri.first->points[1], tri.first->points[2], p);
				else
					tempZ = getZDepth2(tri.first->points[0], tri.first->points[1], tri.first->points[2], left, top);

				if (tempZ < minZ) 
				{ 
					minZ = tempZ; 
					color = tri.first->color;
				}
			}
			drawPixel(left, top, color);
		}
	}
}
#undef WARNOCK_OPTIMIZED

void graphics::Graphics::zbufferAlgorithm(std::vector<Triangle<float>>& transformedTriangles, int16_t matrixType)
{
	auto isPointInTriangle = [&, this](math::Vec3d<float> A, math::Vec3d<float> B, math::Vec3d<float> C, math::Vec3d<float> P)
	{
		// check whether point is in triangle in 2D
		//math::Vec3d<float> v0 = C - A;
		//math::Vec3d<float> v1 = B - A;
		//math::Vec3d<float> v2 = P - A;

		//float dot00 = v0.dotProduct(v0);
		//float dot01 = v0.dotProduct(v1);
		//float dot02 = v0.dotProduct(v2);
		//float dot11 = v1.dotProduct(v1);
		//float dot12 = v1.dotProduct(v2);

		//float inver = 1 / (dot00 * dot11 - dot01 * dot01);

		//float u = (dot11 * dot02 - dot01 * dot12) * inver;
		//if (u < 0 || u > 1)
		//	return false;

		//float v = (dot00 * dot12 - dot01 * dot02) * inver;
		//if (v < 0 || v > 1)
		//	return false;

		//return u + v <= 1;

		math::Vec3d<float>* pointsCheck[3];

		pointsCheck[0] = &A;
		pointsCheck[1] = &B;
		pointsCheck[2] = &C;
	
		return crossingNumber(pointsCheck, 3, &P);
	};

	int32_t bufferSizeWidth, bufferSizeHeight;
	controls::WidgetRect rect = getCanvasRect();
	bufferSizeWidth = rect.width;
	bufferSizeHeight = rect.height;

	std::vector<float> depthZ(bufferSizeWidth * bufferSizeHeight, 1000.0f);
	math::Vec3d<float> A, B, C;

	for (auto& tri : transformedTriangles)
	{
		A = tri.points[0];
		B = tri.points[1];
		C = tri.points[2];

		// find bounding box of triangle
		float minX = min(min(A.x, B.x), C.x);
		float maxX = max(max(A.x, B.x), C.x);

		float minY = min(min(A.y, B.y), C.y);
		float maxY = max(max(A.y, B.y), C.y);

		if (minX >= rect.right - 1 || minY >= rect.bottom - 1) continue;
		if (maxX <= rect.left || maxY <= rect.top ) continue;

		minX = (minX < rect.left) ? rect.left : minX;
		minY = (minY < rect.top) ? rect.top : minY;
		maxX = (maxX > rect.right - 1) ? (rect.right - 1) : maxX;
		maxY = (maxY > rect.bottom - 1) ? (rect.bottom - 1) : maxY;

		for (int32_t i = minY; i <= maxY; i++)
		{
			for (int32_t j = minX; j <= maxX; j++)
			{
				math::Vec3d<float> P(j, i, 0);
				if (isPointInTriangle(A, B, C, P))
				{
					float depth{};
					//if (matrixType != math::matrixType::CABINET)
						depth = getZDepth(A, B, C, P);
					//else
					//	depth = getZDepth2(A, B, C, j, i);

					if (depth < depthZ[(i - rect.top) * bufferSizeWidth + (j - rect.left)])
					{
						depthZ[(i - rect.top) * bufferSizeWidth + (j - rect.left)] = depth;
						drawPixel(j, i, tri.color);
					}
				}
			}
		}
	}
}

void graphics::Graphics::drawPolygon(math::Vec2d<int32_t>* points, int16_t cpt, uint32_t color)
{
	int16_t pntIn = 0;
	bool needClip = false;

	for (int16_t i = 0; i < cpt; ++i)
	{
		if (checkBounds(points[i].x, points[i].y)) ++pntIn;
		else needClip = true;
	}

	if (pntIn == 0) return;

	for (int16_t i = 0, next = 0; i < cpt; ++i)
	{
		next = (i + 1) % cpt;
		drawLine(points[i], points[next], color);
	}
}

void graphics::Graphics::drawTriangle(math::Vec3d<int32_t>* points, uint32_t color)
{
	drawLine(points[0], points[1], color);
	drawLine(points[1], points[2], color);
	drawLine(points[2], points[0], color);
}

// https://www.avrfreaks.net/sites/default/files/triangles.c
void graphics::Graphics::fillTriangle(math::Vec3d<int32_t>* points, uint32_t color, controls::WidgetRect* canvasRect)
{
	int16_t x1, x2, x3, y1, y2, y3;
	x1 = points[0].x; y1 = points[0].y;
	x2 = points[1].x; y2 = points[1].y;
	x3 = points[2].x; y3 = points[2].y;

	auto drawline = [&](int16_t sx, int16_t ex, int16_t ny) { for (int16_t i = sx; i <= ex; i++) if (canvasRect) drawPixel(i, ny, color, canvasRect); else drawPixel(i, ny, color); };

	int16_t t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int16_t signx1, signx2, dx1, dy1, dx2, dy2;
	int16_t e1, e2;
	// Sort vertices
	if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
	if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
	if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = static_cast<int16_t>(x2 - x1);
	if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = static_cast<int16_t>(y2 - y1);

	dx2 = static_cast<int16_t>(x3 - x1);
	if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = static_cast<int16_t>(y3 - y1);

	if (dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
	if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

	e2 = static_cast<int16_t>(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = static_cast<int16_t>(dx1 >> 1);

	for (int16_t i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x;
		if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x;
		if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
									// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;
	}
next:
	// Second half
	dx1 = static_cast<int16_t>(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = static_cast<int16_t>(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		std::swap(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = static_cast<int16_t>(dx1 >> 1);

	for (int16_t i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x;
		if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x;
		if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
	}
	

	// Получаем длины всех сторон
		// sqrt((y2 - y1)^2 + (x2 - x1)^2)
	//float deltaY, deltaX;

	//deltaX = (points[1].x - points[0].x);
	//deltaY = (points[1].y - points[0].y);
	//float ab = sqrtf((deltaY * deltaY) + (deltaX *deltaX));

	//deltaX = (points[2].x - points[1].x);
	//deltaY = (points[2].y - points[1].y);
	//float bc = sqrtf((deltaY * deltaY) + (deltaX * deltaX));

	//deltaX = (points[0].x - points[2].x);
	//deltaY = (points[0].y - points[2].y);
	//float ac = sqrtf((deltaY * deltaY) + (deltaX * deltaX));

	//if (((ab < bc) || (ab == bc)) && ((ab < bc) || (ab == bc)))
	//		tx = x1
	//		ty = y1
	//		vx = (x2 - x1) / d1
	//		vy = (y2 - y1) / d1
	//		counter = 0
	//		while (counter < d1)
	//			draw_a_line(x3, y3, tx, ty)
	//			//drawing a line from point(x3,y3) to point(tx,ty).
	//			tx = tx + vx
	//			ty = ty + vy
	//			counter = counter + 1
	//	else if ((d2 < d3) or (d2 = d3))
	//		tx = x2
	//		ty = y2
	//		vx = (x3 - x2) / d2
	//		vy = (y3 - y2) / d2
	//		counter = 0
	//		while (counter < d2)
	//			draw_a_line(x1, y1, tx, ty)
	//			tx = tx + vx
	//			ty = ty + vy
	//			counter = counter + 1
	//	else
	//		tx = x3
	//		ty = y3
	//		vx = (x1 - x3) / d3
	//		vy = (y1 - y3) / d3
	//		counter = 0
	//		while (counter < d3)
	//			draw_a_line(x2, y2, tx, ty)
	//			tx = tx + vx
	//			ty = ty + vy
	//			counter = counter + 1
}

void graphics::Graphics::fillRect(int32_t left, int32_t top, int32_t right, int32_t bottom, uint32_t color)
{
	for (int32_t x = left; x < right; x++)
		for (int32_t y = top; y < bottom; y++)
			drawPixel(x, y, color);
}

void graphics::Graphics::drawPixel(math::Vec2d<int32_t> coord, uint32_t color)
{
	drawPixel(coord.x, coord.y, color);
}

void graphics::Graphics::drawLine(math::Vec2d<int32_t> point1, math::Vec2d<int32_t> point2, uint32_t color)
{
	drawLine(point1.x, point1.y, point2.x, point2.y, color);
}

float graphics::Graphics::drawObjects(std::vector<Triangle<float>>& transformedTriangles, int16_t algorithmType, int16_t matrixType, bool isFillingObject)
{
	controls::WidgetRect canvasRect = getCanvasRect();

	// Делаем затемнение треугольников
	math::Vec3d<float> line1, line2, normal, center, lightDirection, lightPosition(canvasRect.top, -1500.0f, 100.0f);
	for (auto& tri : transformedTriangles)
	{
		center = (tri.points[0] + tri.points[1] + tri.points[2]) / 3.0f;
		line1 = tri.points[1] - tri.points[0];
		line2 = tri.points[2] - tri.points[0];

		normal = line1.crossProduct(line2);
		normal.normalize();

		normal = normal - center;
		lightDirection = lightPosition - center;

		normal.normalize();
		lightDirection.normalize();

		float illumination = lightDirection.dotProduct(normal) / (lightDirection.length() * normal.length());
		if (illumination < 0) illumination = 0;
		illumination += 0.3;

		uint32_t b = static_cast<uint32_t>(std::roundf(static_cast<uint8_t>(tri.color) * illumination));
		uint32_t g = static_cast<uint32_t>(std::roundf(static_cast<uint8_t>(tri.color >> 8) * illumination));
		uint32_t r = static_cast<uint32_t>(std::roundf(static_cast<uint8_t>(tri.color >> 16) * illumination));
		
		b = max(min(b, 255), 0);
		g = max(min(g, 255), 0);
		r = max(min(r, 255), 0);
		tri.color = b + (g << 8) + (r << 16);
	}

	// Начало работы алгоритма отбрасывания
	auto start = std::chrono::system_clock::now();

	switch (algorithmType)
	{
	case algorithmType::NONE:
	{
		for (auto& tri : transformedTriangles)
		{
			math::Vec3d<int32_t> points[3];
			for (int16_t i = 0; i < 3; i++)
			{
				points[i].x = roundf(tri.points[i].x);
				points[i].y = roundf(tri.points[i].y);
			}

			if (isFillingObject)
				fillTriangle(points, tri.color);
			else
				drawTriangle(points, config::objectPreset::color::standartEdgesColor);
		}
		break;
	}
	case algorithmType::WARNOCK:
	{
		std::vector<Triangle<float>*> ptrTransformedTriangles;
		for (auto& tri : transformedTriangles)
			ptrTransformedTriangles.push_back(&tri);

		warnockAlgorithm(ptrTransformedTriangles, canvasRect.left, canvasRect.top, canvasRect.right, canvasRect.bottom, matrixType);
		break;
	}
	case algorithmType::ROBERTS:
	{
		robertsAlgorithm(transformedTriangles, matrixType, isFillingObject);
		break;
	}
	case algorithmType::PAINTER:
	{
		painterAlgorithm(transformedTriangles, matrixType, isFillingObject);
		break;
	}
	case algorithmType::ZBUFFER:
		zbufferAlgorithm(transformedTriangles, matrixType);
		break;
	default:
		break;
	}

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsed = end - start;
	std::chrono::milliseconds elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
	return elapsedMs.count();
}
