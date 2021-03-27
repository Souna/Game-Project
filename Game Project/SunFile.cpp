#include "SunFile.h"
//#include <array>
namespace sun_file
{
	SunNode map;
	//constexpr uint8_t NUM_FILES = 1;
	//constexpr std::array<const char*, NUM_FILES> filenames =
	//{
	//	"base.sun"
	//};

	auto Load_All_Files(const std::string& file_path) -> void
	{
		map.Load(std::make_shared<SunReader>(file_path + "map.sun"));
	}

	auto Init() -> Error
	{
		int ct = 0;
		for (auto filename : sun_file::filenames)
			if (std::ifstream{ filename }.good() == false)
			{
				ct++;
				return Error(Error::Code::MISSING_FILE, filename);
			}

		// Try loading the files now_.
		try
		{
			Load_All_Files("./");
		}
		catch (const std::exception& ex)
		{
			static const std::string message = ex.what();
			return Error(Error::Code::FILE_ERROR, message.c_str());
		}

		return Error::Code::NONE;
	}
}