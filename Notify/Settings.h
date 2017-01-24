
#include <fstream>
#include <string>

using namespace std;
class Settings
{
	static const wstring separator;
	const wchar_t* filename;
public:
	Settings(const wchar_t* file);
	wstring read(const wstring key);
	void set(const wstring key, const wstring value);
};
