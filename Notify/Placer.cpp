#include "stdafx.h"
#include "Placer.h"

DWORD WINAPI ThreadProc(LPVOID);


Placer::Placer(std::wstring filename, std::wstring app_name)
{
	file = filename;
	app = app_name;
	FILE *f;

	// Lock file to avoid concurrency problems
	lock();
	
	// Check if file exists.
	if (_wfopen_s(&f, file.c_str(), L"r") == 0){
		fclose(f);

		// Check if the file expired
		if (expired(file))
			// Clean it!
			File::over_write(file.c_str(), 0);
		
	}
	else
		// No! Create it
		File::over_write(file.c_str(), 0);

	unlock();
}

Placer::~Placer(){
	if (has_place)
		release_place();
	DeleteObject(semaphore);
}


int Placer::get_place()
{
	// Lock file to avoid concurrency problems
	lock();

	// Value used in case of error
	place_index = -1;

	// Check available positions
	extract(File::read<int>(file.c_str()), places);
	for (int i = 0; i < MAX_PLACE; i++){
		if (places[i] == false){
			places[i] = true;
			place_index = i;
			break;
		}
	}

	// Save so that other instances can see this position as full
	File::over_write(file.c_str(), compact(places));
	unlock();

	has_place = true;
	return place_index;
}


void Placer::release_place()
{
	lock();
	extract(File::read<int>(file.c_str()), places);

	// Free place
	places[place_index] = false;

	// Save
	File::over_write(file.c_str(), compact(places));
	unlock();
	has_place = false;
}


void Placer::extract(int from, bool* to)
{
	for (int i = 0; i < MAX_PLACE; ++i){
		to[i] = from % 2 != 0;
		from /= 2;
	}
}


int Placer::compact(bool* from)
{
	int to = 0;
	for (int i = MAX_PLACE - 1; i >= 0; --i){
		to *= 2;
		to += from[i];
	}

	return to;
}


bool Placer::lock()
{
	// Create a semaphore with initial and max counts of MAX_SEM_COUNT
	semaphore = CreateSemaphore(
		NULL,           // default security attributes
		1,				// initial count
		1,				// maximum count
		app.c_str());

	if (semaphore == NULL)
	{
		// This semaphore already exists. Open it
		semaphore = OpenSemaphore(
			SEMAPHORE_ALL_ACCESS,
			FALSE,
			app.c_str());

		if (semaphore == NULL)
		{
			// TODO: Error!
			return false;
		}
	}

	// Acquire semaphore
	DWORD acquireResult = 0;
	acquireResult = WaitForSingleObject(
		semaphore,
		TIMEOUT);  // timeout after 0 seconds trying to acquire

	if (acquireResult == WAIT_TIMEOUT)
	{
		// TODO: Error!
		return false;
	}
	return true;
}


void Placer::unlock()
{
	LONG prevCount = 0;
	BOOL releaseResult = ReleaseSemaphore(
		semaphore,
		1, // increment count by 1
		&prevCount);

	if (!releaseResult)
	{
		// TODO: Error!
	}
}


bool Placer::expired(std::wstring filename){
	// Open file
	HANDLE f = CreateFile(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (f == INVALID_HANDLE_VALUE){
		// TODO: error
		return false;
	}

	// Test if file longevity expired
	FILETIME creationTime,
		lpLastAccessTime,
		lastWriteTime;

	BOOL err = GetFileTime(f, &creationTime, &lpLastAccessTime, &lastWriteTime);

	// Close file
	CloseHandle(f);

	if (err){

		// Get current system time
		FILETIME now;
		SYSTEMTIME st;

		GetSystemTime(&st);
		SystemTimeToFileTime(&st, &now);

		// Calculate time difference
		LONGLONG diffInTicks =
			((LARGE_INTEGER*)&now)->QuadPart -
			((LARGE_INTEGER*)&lastWriteTime)->QuadPart;
		LONGLONG diff_in_seconds = diffInTicks / 10000 / 1000;

		// Check if file expired
		return diff_in_seconds > TIMEOUT;
	}
	else {
		// TODO: error!
	}
	return false;
}
