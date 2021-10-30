#pragma once

typedef unsigned char byte;

class Color
{
public:
	byte rgba[4];

	inline byte& operator[](const int i)
	{
		return rgba[i];
	}

	Color(byte r, byte g, byte b, byte a = 255)
	{
		rgba[0] = r;
		rgba[1] = g;
		rgba[2] = b;
		rgba[3] = a;
	}
	Color(byte rgb, byte alpha = 255)
	{
		rgba[0] = rgb;
		rgba[1] = rgb;
		rgba[2] = rgb;

		rgba[3] = alpha;
	}
	Color() { }

	static inline void ClampFl(float &fl)
	{
		if (fl > 1.0f)
			fl = 1.0f;
		else if (fl < 0.0f)
			fl = 0.0f;
	}

	static Color FromHSB(float hue, float saturation, float brightness, int alpha = 255)
	{
		ClampFl(hue);
		ClampFl(saturation);
		ClampFl(brightness);

		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				(byte)(brightness * 255),
				(byte)(t * 255),
				(byte)(p * 255),
				alpha
			);
		}
		else if (h < 2)
		{
			return Color(
				(byte)(q * 255),
				(byte)(brightness * 255),
				(byte)(p * 255),
				alpha
			);
		}
		else if (h < 3)
		{
			return Color(
				(byte)(p * 255),
				(byte)(brightness * 255),
				(byte)(t * 255),
				alpha
			);
		}
		else if (h < 4)
		{
			return Color(
				(byte)(p * 255),
				(byte)(q * 255),
				(byte)(brightness * 255),
				alpha
			);
		}
		else if (h < 5)
		{
			return Color(
				(byte)(t * 255),
				(byte)(p * 255),
				(byte)(brightness * 255),
				alpha
			);
		}
		else
		{
			return Color(
				(byte)(brightness * 255),
				(byte)(p * 255),
				(byte)(q * 255),
				alpha
			);
		}
	}
};