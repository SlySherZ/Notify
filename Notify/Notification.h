#include "Settings.h"
#include <Windows.h>
#include <time.h>

class Notification
{
private:
	// Drawing variables
	HDC			 hdc;
	HDC          hdcMem;
	HBITMAP      hbmMem;
	HANDLE       hOld;
	RECT rect;
	SIZE sz;
	HFONT hFont;
	HFONT hTmp;
	POINT mouse_position;
	HBRUSH background;
	BLENDFUNCTION m_bf;			// AlphaBlend function parameter
	int width = 0;
	int height = 0;

	// Settings variables
	int font_size;
	wstring	font_face;
	int font_color_red;
	int font_color_green;
	int font_color_blue;

	int bkg_color_red;
	int bkg_color_green;
	int bkg_color_blue;

	int location_x;
	int location_y;

	float border_percentage;
	int duration;			// In seconds
	int smoothness_speed;

	bool disappear_on_hover;

	void load_settings();
	void prepare_win_api(wchar_t* message, int ofset);
	void buffer_draw(wchar_t* message);
	void display();
	void clean_up();
	
	void initialize();


public:
	Notification();
	void load_notification(wchar_t* message, int ofset);
	void show();
	
};


int time_since(time_t begin_time);

// Fix values to a default value when they are falsy
template<typename T>
T DEFAULT(T value, T to);
