#include "SunFile.h"
//#include <array>
namespace sun_file
{
	SunNode Map, String;
	std::string devFilePath = "C:\\Users\\lapto\\Desktop\\stuff\\NewSunFiles";

	auto Load_All_Files(const std::string& file_path) -> void
	{
		Map.Load(std::make_shared<SunReader>(file_path + "\\" + "Map.sun"));
		//test.Load(std::make_shared<SunReader>(file_path + "test.sun"));
		//map.Load(std::make_shared<SunReader>(file_path + "simple.sun"));
	}

	auto Init() -> Error
	{
		int ct = 0;
		std::vector<std::string> missing_files;
		std::string s;

		for (auto filename : sun_file::filenames)
		{
			std::string fullPath = devFilePath + "\\" + filename;

			if (std::ifstream{ fullPath }.good() == false)
			{
				ct++;
				missing_files.emplace_back(filename);
				//return Error(Error::Code::MISSING_FILE, filename);
			}
		}

		//if (ct == 1) return Error(Error::Code::MISSING_FILE, reinterpret_cast<char*>(missing_files[0]));
		if (ct == 1) return Error(Error::Code::MISSING_FILE, (char*)missing_files[0].c_str());
		if (ct > 1)
		{
			for (std::string c : missing_files)
			{
				s += c.c_str();
				s += " ";
			}
			//const char *b = s.c_str();
			return Error(Error::Code::MISSING_FILES, s.c_str());
		}
		//(ct == 1) ? return Error(Error::Code::MISSING_FILE, missing_files[0]) : continue;

		// The files will be expected to be in the same directory as the .h/.cpp files but for now it points to the dev folder
		try
		{
			Load_All_Files(devFilePath);
		}
		catch (const std::exception& ex)
		{
			static const std::string message = ex.what();
			return Error(Error::Code::FILE_ERROR, message.c_str());
		}

		return Error::Code::NONE;
	}
}