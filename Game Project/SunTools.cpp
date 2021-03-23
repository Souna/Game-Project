#include "SunTools.h"

auto sun_tools::Split(const std::string& str, std::vector<std::string>& ret, std::string sep, bool allowNull) -> int32_t
{
	if (str.empty())
	{
		return 0;
	}
	std::string tmp;
	std::string::size_type pos_begin = str.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;
	while (pos_begin != std::string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != std::string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			if (tmp == "" && !allowNull) 
			{
				ret.push_back(tmp);
			}
			else {
				ret.push_back(tmp);
			}
			tmp.clear();
		}
	}
	return 0;
}
auto sun_tools::EndsWith(const std::string& str, const std::string& endStr) -> bool
{
	if (str.empty() || endStr.empty())
	{
		return false;
	}
	if (str.size() < endStr.size()) 
	{
		return false;
	}
	return str.compare(str.size() - endStr.size(), endStr.size(), endStr) == 0 ? true : false;
}

auto sun_tools::GetIndexFromTypes(std::string& str) -> int32_t
{

	static std::string types[6] =
	{
		"Shape2D#Convex2D",
		"Shape2D#Vector2D",
		"Sound_DX8",
		"Property",
		"Canvas"
	};

	int i = 0;
	for (auto& a : types)
	{
		if (a == str) 
		{
			return i;
		}
		i++;
	}
	return -1;
}