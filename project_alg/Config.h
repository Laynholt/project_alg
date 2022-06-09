#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <cstdint>

namespace config
{
	constexpr int16_t mainWindowWidth = 1100;
	constexpr int16_t mainWindowHeight = 650;
			  
	constexpr int16_t axisPanelWidth = 250;
	constexpr int16_t axisPanelHeight = 200;
			  
	constexpr int16_t controlPanelWidth = 250;
	constexpr int16_t controlPanelHeight = 450;

#ifndef APP_DARK_THEME
	constexpr uint32_t background = 0xC4B8B8;
#else
	constexpr uint32_t background = 0x1C1C1C;
#endif

	namespace widgets
	{
		constexpr int16_t standartPaddingWidth = 5;
		constexpr int16_t standartPaddingHeight = 5;

		constexpr int16_t standartControlHeight = 25;

		namespace staticControl
		{
			constexpr int16_t standartStaticControlHeight = 30;
		}

		namespace editControl
		{
			constexpr int16_t standartEditControlHeight = 40;

			constexpr int16_t positionLimitMax = 900;
			constexpr int16_t positionLimitMin = -900;

			constexpr int16_t rotateLimitMax = 360;
			constexpr int16_t rotateLimitMin = -360;

			constexpr int16_t scaleLimitMax = 30;
			constexpr int16_t scaleLimitMin = -30;

			constexpr int16_t digitLimitSize = 3;
		}

		namespace fontSize
		{
			constexpr int16_t tiny = 12;
			constexpr int16_t normal = 14;
			constexpr int16_t large = 16;
			constexpr int16_t gigant = 18;
		}

		namespace color
		{
#ifndef APP_DARK_THEME
			constexpr uint32_t areaBackground = 0x2F4F4F; //0xFFFFFF; //0xAD8EBF;
			constexpr uint32_t areaBounds = 0xE82929;
					  
			constexpr uint32_t checkboxBackground = 0xD4D4D4;
			constexpr uint32_t checkboxTextColor = 0x282828;
					  
			constexpr uint32_t comboboxBackground = 0xD4D4D4;
			constexpr uint32_t comboboxTextColor = 0x282828;
#else				  
			constexpr uint32_t areaBackground = 0x232323;
			constexpr uint32_t areaBounds = 0xE82929;
					  
			constexpr uint32_t checkboxBackground = 0x282828;
			constexpr uint32_t checkboxTextColor = 0xD4D4D4;
					  
			constexpr uint32_t comboboxBackground = 0x282828;
			constexpr uint32_t comboboxTextColor = 0xD4D4D4;
#endif
		}
	}

	namespace objectPreset
	{
		constexpr float standartPositionX = 0.0f;
		constexpr float standartPositionY = 0.0f;
		constexpr float standartPositionZ = 15.0f;

		constexpr float standartRotationX = 10.0f;
		constexpr float standartRotationY = 10.0f;
		constexpr float standartRotationZ = 10.0f;

		constexpr float standartScalingX = 0.3f;
		constexpr float standartScalingY = 0.3f;		// Коэффициент, на который нужно умножить getWidth & getHeight
		constexpr float standartScalingZ = 0.3f;

		namespace color
		{
			constexpr uint32_t standartColorBlue = 0x191970;
			constexpr uint32_t standartColorGreen = 0x228B22;
			constexpr uint32_t standartColorRed = 0x8B0000;

			constexpr uint32_t standartEdgesColor = 0xF0FFF0; //0x323232; //0x2F4F4F;
		}
	}
}

#endif // !_CONFIG_H_
