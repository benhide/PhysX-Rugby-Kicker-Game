// Define the font renderer
#ifndef __GL_FONT_RENDERER__
#define __GL_FONT_RENDERER__

// Font renderer class
class GLFontRenderer
{
private:
	// Initialisation flag
	static bool m_isInit;

	// Number of texture objects
	static unsigned int m_textureObject;

	// Screen width
	static int m_screenWidth;

	// Screen height
	static int m_screenHeight;

	// Colours array
	static float m_color[4];

public:
	// Font renderer initialisation
	static bool Init();

	// Print text
	static void Print(float x, float y, float fontSize, const char* pString, bool forceMonoSpace = false, int monoSpaceWidth = 11, bool doOrthoProj = true);

	// Set the screen resolution
	static void SetScreenResolution(int screenWidth, int screenHeight);

	// Set the colour
	static void SetColor(float r, float g, float b, float a);
};
#endif // __GL_FONT_RENDERER__