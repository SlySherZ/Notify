#include <fstream>

namespace File
{
	template<typename T>
	static void write(const wchar_t* file, T message)
	{
		std::wofstream out(file, std::ios_base::app);
		out << message;
		out.close();
	}

	template<typename T>
	static T read(const wchar_t* file)
	{
		std::wifstream in(file);
		T content;
		in >> content;
		in.close();
		return content;
	}

	template<typename T>
	static void over_write(const wchar_t* file, T message)
	{
		std::wofstream out(file);
		out << message;
		out.close();
	}

};
	
