#include "Presets.h"

namespace graphics
{
	namespace presets
	{
		// Куб
		std::vector<math::Vec3d<float>> verticesPresetCube = {
			{-1.0f, -1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f}
		};

		std::vector<ConnectedVertices> connectedVerticesCube = {
			{0,1,2}, {0,2,3},	// перед
			{4,5,1}, {4,1,0},	// лево
			{3,2,6}, {3,6,7},	// право
			{7,6,5}, {7,5,4},	// зад
			{4,0,3}, {4,3,7},	// низ
			{1,5,6}, {1,6,2}	// вверх
		};


		// Пирамида
		std::vector<math::Vec3d<float>> verticesPresetPyramid = {
			{ 0.0f, -1.0f,  0.0f},
			{-1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f}
		};

		std::vector<ConnectedVertices> connectedVerticesPyramid = {
			{1,0,2},			// перед
			{4,0,1},			// лево
			{2,0,3},			// право
			{3,0,4},			// зад
			{4,1,2}, {4,2,3}	// низ
		};

		// Призма (Треугольная)
		std::vector<math::Vec3d<float>> verticesPresetPrism = {
			{-1.0f, -1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f}
		};

		std::vector<ConnectedVertices> connectedVerticesPrism = {
			{0,1,2}, {0,2,3},	// перед
			{4,5,1}, {4,1,0},	// лево
			{3,2,5}, {3,5,4},	// право
			{0,4,3},			// низ
			{1,5,2}				// вверх
		};
	}
}