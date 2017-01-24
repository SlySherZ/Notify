#include "stdafx.h"
#include "Notification.h"
#include "File.h"

#define INITIALIZATION_TIME 1		// In seconds
#define INITIALIZATION_LIMIT 350

void Notification::load_settings(){
	// Read values from settings file to initialize variables
	Settings setts(L"settings.txt");

	font_size =			DEFAULT(stoi(setts.read(L"font size")), 20);
	font_face =						 setts.read(L"font face");
	font_color_red =	DEFAULT(stoi(setts.read(L"font color red")), 125);
	font_color_green =  DEFAULT(stoi(setts.read(L"font color green")), 125);
	font_color_blue =	DEFAULT(stoi(setts.read(L"font color blue")), 255);

	bkg_color_red =		DEFAULT(stoi(setts.read(L"background color red")), 0);
	bkg_color_green =	DEFAULT(stoi(setts.read(L"background color green")), 0);
	bkg_color_blue =	DEFAULT(stoi(setts.read(L"background color blue")), 0);

	location_x =				stoi(setts.read(L"location x"));
	location_y =				stoi(setts.read(L"location y"));

	border_percentage =			stof(setts.read(L"border percentage"));
	duration =			DEFAULT(stoi(setts.read(L"time duration")), 7);			// In seconds
	smoothness_speed =	DEFAULT(stoi(setts.read(L"smoothness speed")), 40);

	disappear_on_hover =			 setts.read(L"disappear on hover") == L"true";
}


void Notification::initialize()
{
	load_settings();
}


void Notification::prepare_win_api(wchar_t* message, int ofset)
{
	// Acquire screen
	hdc = ::GetDC(0);


	//Create and select necessary font
	hFont = CreateFont(font_size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY | CLEARTYPE_QUALITY, 0, font_face.c_str());
	hTmp = (HFONT)SelectObject(hdc, hFont);


	// Create background color Brush
	background = CreateSolidBrush(RGB(bkg_color_red, bkg_color_green, bkg_color_blue));


	// Calculate size of the text
	GetTextExtentPoint32(hdc, message, wcslen(message), &sz);
	width = sz.cx + (int)(border_percentage * font_size);
	height = sz.cy + (int)(border_percentage * font_size);
	rect = { 0, 0, width, height };


	// If there are messages on screen already, show it somewhere else
	location_y += ofset * height;


	// Create an off-screen DC for double-buffering
	hdcMem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc, width, height);


	// Configure off-screen DC
	SetBkMode(hdcMem, TRANSPARENT);
	SetTextColor(hdcMem, RGB(font_color_red, font_color_green, font_color_blue));
	SelectObject(hdcMem, hFont);
	hOld = SelectObject(hdcMem, hbmMem);


	// Initialize AlphaBlender function parameter
	m_bf.BlendOp = AC_SRC_OVER;
	m_bf.BlendFlags = 0;
	m_bf.SourceConstantAlpha = smoothness_speed;
	m_bf.AlphaFormat = 0;

}


void Notification::buffer_draw(wchar_t* message)
{
	// Draw into hdcMem
	FillRect(hdcMem, &rect, background);
	DrawText(hdcMem, message, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP);
}


void Notification::display()
{

	// Prepare timer
	const clock_t begin_time = clock();
	int tick_counter = 0;


	// Draw loop
	while (true)
	{
		// Transfer backbuffer drawing to the screen
		if (smoothness_speed * tick_counter < INITIALIZATION_LIMIT){		// Start smoothly
			AlphaBlend(hdc, location_x, location_y, width, height, hdcMem, 0, 0, width, height, m_bf);
			++tick_counter;
		}
		else
			// and finish off sharp!
			BitBlt(hdc, location_x, location_y, width, height, hdcMem, 0, 0, SRCCOPY);


		if (time_since(begin_time) > INITIALIZATION_TIME){
			// Check if it's time to close
			if (time_since(begin_time) > duration)
				break;


			// Mouse is on top of the notification? Close!
			if (disappear_on_hover &&
				GetCursorPos(&mouse_position))
			{
				if (mouse_position.x > location_x &&
					mouse_position.x < location_x + width &&
					mouse_position.y > location_y &&
					mouse_position.y < location_y + height)
					break;
			}
		}

		// Don't eat all the cpu!
		Sleep(1);
	}

	// Delete notification right after time expires
	::InvalidateRect(0, &rect, false);
	::UpdateWindow(0);
	
}


void Notification::clean_up()
{
	// Free-up the off-screen DC
	DeleteObject(background);
	SelectObject(hdcMem, hOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);


	// Release created objects
	DeleteObject(SelectObject(hdc, hTmp));
	::ReleaseDC(0, hdc);
}


Notification::Notification()
{
	initialize();
}


void Notification::load_notification(wchar_t* message, int ofset)
{
	prepare_win_api(message, ofset);
	buffer_draw(message);
}


void Notification::show()
{
	display();
	clean_up();
}


int time_since(time_t begin_time)
{
	return (int)(clock() - begin_time) / CLOCKS_PER_SEC;
}


template<typename T>
T DEFAULT(T value, T to)
{
	if (value)
		return value;
	return to;
}

