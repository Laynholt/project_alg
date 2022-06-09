#include "Presets.h"

namespace graphics
{
	namespace presets
	{
		// ���
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
			{0,1,2}, {0,2,3},	// �����
			{4,5,1}, {4,1,0},	// ����
			{3,2,6}, {3,6,7},	// �����
			{7,6,5}, {7,5,4},	// ���
			{4,0,3}, {4,3,7},	// ���
			{1,5,6}, {1,6,2}	// �����
		};


		// ��������
		std::vector<math::Vec3d<float>> verticesPresetPyramid = {
			{ 0.0f, -1.0f,  0.0f},
			{-1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f}
		};

		std::vector<ConnectedVertices> connectedVerticesPyramid = {
			{1,0,2},			// �����
			{4,0,1},			// ����
			{2,0,3},			// �����
			{3,0,4},			// ���
			{4,1,2}, {4,2,3}	// ���
		};

		// ������ (�����������)
		std::vector<math::Vec3d<float>> verticesPresetPrism = {
			{-1.0f, -1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{-1.0f, -1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f}
		};

		std::vector<ConnectedVertices> connectedVerticesPrism = {
			{0,1,2}, {0,2,3},	// �����
			{4,5,1}, {4,1,0},	// ����
			{3,2,5}, {3,5,4},	// �����
			{0,4,3},			// ���
			{1,5,2}				// �����
		};
	}
}