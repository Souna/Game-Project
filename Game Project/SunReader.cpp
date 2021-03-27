#include "SunReader.h"
#include "StringUtils.h"
#include "Console.h"
#include <clocale>
#include <algorithm>
#include <array>
#include <iostream>
#include <cmath>
#include <limits.h>
#include <windows.h>

// i am so stupid and idk what this USE_MMAP stuff is for
SunReader::SunReader(const std::string& p) : length_(0), position_(0), file_(nullptr), path_(p)
{
#ifdef USE_MMAP
	file_handle_ = CreateFileA(path_.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, nullptr);

	if (file_handle_ == INVALID_HANDLE_VALUE) { printf("Failed to open this file: %s", p.c_str());  exit(0); }

	map_ = CreateFileMappingA(file_handle_, 0, PAGE_READONLY, 0, 0, nullptr);

	if (!map_) { printf("Failed to open this file: %s", p.c_str()); exit(0); }

	length_ = GetFileSize(file_handle_, 0);
	// Should be magic #
	base_ = static_cast<int8_t*>(MapViewOfFile(map_, FILE_MAP_READ, 0, 0, 0));
	//offset_ = nullptr;
	offset_ = base_;
#endif
}

SunReader::~SunReader()
{
#ifdef USE_MMAP
	::UnmapViewOfFile(base_);
	::CloseHandle(map_);
	::CloseHandle(file_handle_);
#endif
}

auto SunReader::Valid() -> bool
{
	// Why should the file be >= 64?
	if (Get_Length() >= 64)
	{
		// Read file identifier. int32_t is good enough for PKG1 (wzfile identifier)
		// but my SunFile identifier - SUNFILE :) - is 10 bytes.
		//int signature = Read<int32_t>();
		uint64_t signature = Read<uint64_t>();
		// Skip ASCII (112 bytes)
		Set_Position(Get_Position() + 112);
		// Read file size from header.
		uint64_t file_size = Read<uint64_t>();
		// Read file start (header size).
		uint32_t header_size = Read<uint32_t>();
		// Read copyright string
		std::string copyright = Read_Null_Terminated_String();

		//
		if (0x616e756f536e7553 == signature && Get_Length() == file_size + header_size)
		{
			header_.size = header_size;
		}
		return true;
	}
	return false;
}

auto SunReader::Compute_Offset() -> int
{
	unsigned int value = Read<uint32_t>();
	//?
	unsigned int offset = (unsigned int)(Get_Position() - header_.size - 4) ^ 0xffffffff;
	if (header_.conclusion > -1)
	{
		offset = Compute_Offset(offset, header_.factors[header_.conclusion], value);
	}
	else
	{					// ?????
		for (int index = 0; index < 10; ++index)
		{
			unsigned int position = Compute_Offset(offset, header_.factors[index], value);
			if (header_.end_of_data == position)
			{
				offset = position;
				header_.conclusion = index;
				break;
			}
		}
	}
	return (int)offset;
}

auto SunReader::Compute_Offset(unsigned int offset, unsigned int factor, unsigned int value) -> int
{
	// wtf are these values
	offset = offset * factor - 0x581c3f6d;
	factor = offset & 0x1f;
	return (((offset << (int)factor) | (offset >> (0x20 - (int)factor))) ^ value) + 0x78;
}

auto SunReader::Read_Null_Terminated_String() -> std::string
{
	char c;
	std::string string;
	while ((c = Read<char>()) != 0)
	{
		string += c;
	}
	return string;
}

auto SunReader::Transit_String(int64_t offset) -> std::string
{
	uint8_t a = Read<uint8_t>();
	switch (a)
	{
	case 0x00: case 0x73:
		return Read_Decrypt_String();
	case 0x01: case 0x1b:
		return Read_Decrypt_String(offset + Read<int32_t>());
	}
	return "";
}

auto SunReader::Read_String() -> std::string
{
	std::string string;
	auto size = Read<int8_t>();
	while (string.length() < size)
	{
		string += Read<char>();
	}
	return string;
}

auto SunReader::Read_String(int32_t length) -> std::string
{
	// Have yet to verify this works well
	std::string string;
	while (string.length() < length)
	{
		string += Read<char>();
	}
	return string;
}

auto SunReader::Read_Decrypt_String() -> std::string
{
	auto size = Read<int8_t>();
	if (size > 0)
	{
		//Unicode
		auto strlen = size == 127 ? Read<uint32_t>() : size;
		if (strlen >= USHRT_MAX) return "";
		auto bytes = Read_Bytes(strlen * 2);
		std::string str = Decrypt_Unicode_String(bytes.get(), strlen * 2);
		return str;
	}

	else if (size < 0)
	{
		//ASCII
		auto strlen = size == -128 ? Read<uint32_t>() : -size;
		if (strlen >= USHRT_MAX) return "";
		auto bytes = Read_Bytes(strlen);
		std::string s = Decrypt_Ascii_String(bytes.get(), strlen);
		return s;
	}
	return "";	// else just..
}

auto SunReader::Read_Decrypt_String(int64_t offset) -> std::string
{
	uint64_t current_pos = Get_Position();
	Set_Position(offset);
	std::string result = Read_Decrypt_String();
	Set_Position(current_pos);
	return result;
}

auto SunReader::Read_Binary(int32_t size) -> void*
{
	void* a = offset_;
	offset_ += size;
	position_ += size;
	return a;
}

auto SunReader::Read_Byte() -> int8_t
{
	return Read<int8_t>();
}

auto SunReader::Read_Bytes(int32_t length) -> std::unique_ptr<int8_t[]>
{
	std::unique_ptr<int8_t[]> array(new int8_t[length]);
	memset(array.get(), 0, length);
	memcpy(array.get(), offset_, length);
	offset_ += sizeof(int8_t) * length;
	position_ += sizeof(int8_t) * length;
	return array;
}

auto SunReader::Read_Short() -> int16_t
{
	return Read<int16_t>();
}

auto SunReader::Read_UShort() -> uint16_t
{
	return Read<uint16_t>();
}

auto SunReader::Read_Int() -> int32_t
{
	return Read<int32_t>();
}

auto SunReader::Read_UInt() -> uint32_t
{
	return Read<uint32_t>();
}

auto SunReader::Read_Double() -> double
{
	return Read<double>();
}

auto SunReader::Get_Offset(int32_t offset) -> const int8_t*
{
	Set_Position(offset);
	return offset_;
}

auto SunReader::Read_Compressed_Int() -> int32_t
{
	int8_t value = Read<int8_t>();
	if (value == SCHAR_MIN)
		return Read<int32_t>();
	return value;
}

auto SunReader::Read_Compressed_Float() -> float
{
	int8_t value = Read<int8_t>();
	if (value == SCHAR_MIN)
		return Read<float>();
	return value;
}

auto SunReader::Read_Compressed_Long() -> int64_t
{
	int8_t value = Read<int8_t>();
	if (value == SCHAR_MIN)
		return Read<int64_t>();
	return value;
}

auto SunReader::Set_Position(int64_t position) -> bool
{
	if (position > length_)
	{
		game::Console::Get().Print(__func__, "OutOfFileSizeException");
		return false;
	}
	else if (position == length_)
	{
		game::Console::Get().Print(__func__, "Set position to very end of file. No more to go over.");
		return false;
	}
	offset_ = base_ + position;
	this->position_ = position;
	return true;
}

auto SunReader::Decrypt_Ascii_String(const int8_t* bytes, int32_t len) -> std::string
{
	return std::string();
}

auto SunReader::Decrypt_Unicode_String(const int8_t* bytes, int32_t len) -> std::string
{
	return std::string();
}

auto SunReader::Convert_String(std::u16string const& p_str)->std::string
{
	std::string out;
	StringUtils::UTF16ToUTF8(p_str, out);
	return out;
}