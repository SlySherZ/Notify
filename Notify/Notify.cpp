#include "stdafx.h"
#include "Notification.h"
#include "Placer.h"
#include "iostream"
#include <Windows.h>

#define ERROR_MESSAGE L"Ops. Something went wrong!"

wchar_t ofset_file[] = L"notify_slots.log";

INT __stdcall WinMain(	HINSTANCE hInstance, 
						HINSTANCE hPrevInstance,
						PSTR lpCmdLine, 
						INT nCmdShow )
{
	int ofset;
	wchar_t message[100];
	wchar_t app_name[] = L"Notify.exe";
	Placer placer(ofset_file, app_name);



	// Set default arrow cursor
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	// Save command-line arguments to message; They are showed by the notification
	MultiByteToWideChar(0, 0,
		lpCmdLine,
		strlen(lpCmdLine),
		message,
		100
		);
	message[strlen(lpCmdLine)] = L'\0';

	// Check status of on screen notifications
	ofset = placer.get_place();

	// Bring up notification
	Notification notify;
	notify.load_notification(message, ofset);
	notify.show();
	

	// Set status for next notification
	placer.release_place();


	return 0;
}
