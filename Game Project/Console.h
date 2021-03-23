#pragma once

#include "Window.h"

#include "Singleton.h"

#include <unordered_set>
#include <string>
#include <iostream>
#define PRINT_WARNINGS
namespace game
{
#ifndef PRINT_WARNINGS
	class Console : public Singleton<Console>
	{
	public:
		void Print(const char*, const std::string&) {}
		void Print(const char*, const std::exception&) {}
		void Print(const std::string&) {}
	};
#else
	class Console : public Singleton<Console>
	{
	public:
		void Print(const char* func, const std::string& msg)
		{
			static const std::string delim = "() :::: ";
			Print(func + delim + msg);
		}

		void Print(const char* func, const std::exception& ex)
		{
			Print(func, ex.what());
		}

		void Print(const std::string& str)
		{
			if (!printed.count(str))
			{
				std::cout << str << std::endl;
				printed.insert(str);
			}
		}

	private:
		std::unordered_set<std::string> printed;
	};
#endif
}