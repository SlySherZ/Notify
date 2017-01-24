#include <Windows.h>
#include "File.h"
#include <string>
#define MAX_PLACE 10
#define TIMEOUT 30

class Placer{
private:
	bool has_place = false;
	int place_index;
	bool places[MAX_PLACE];
	std::wstring file;
	std::wstring app;
	HANDLE semaphore;

	bool expired(std::wstring filename);


public:
	Placer(std::wstring filename, std::wstring app_name);
	~Placer();
	int get_place();
	void release_place();
	bool lock();
	void unlock();
	void extract(int from, bool* to);
	int compact(bool* from);
};