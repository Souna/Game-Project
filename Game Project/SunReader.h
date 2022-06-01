#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <string.h>

#define USE_MMAP	// Find out wtf this means

/**
* Class with methods to read from SunFiles.
*/
class SunReader
{
	// Header structure
	struct SunHeader
	{
		int32_t end_of_data;
		int32_t size = 0;
		uint32_t factors[50] = { 0 }; //?
		int32_t conclusion = -1;	//FFFF FFFF FFFF FFFF
	};

public:
	SunReader() = default;
	SunReader(const std::string& path);
	SunReader(const SunReader& reader) = delete;
	SunReader& operator=(const SunReader& reader) = delete;
	~SunReader();

	// Header methods.
	// Verifies that the header in the SunFiles is valid.
	auto Valid() -> bool;
	auto Compute_Offset() -> int;	// Used for reading encrypted offsets.
	SunHeader header_;

	auto Read_Null_Terminated_String()->std::string;
	auto Transit_String(int64_t offset)->std::string; // Used for reading encrypted strings in .wz files.
	auto Read_String()->std::string;
	auto Read_String(int32_t length)->std::string;
	auto Read_Decrypt_String()->std::string;
	auto Read_Decrypt_String(int64_t offset)->std::string;
	auto Read_Binary(int32_t size) -> void*;

	// The main reading method.
	template <typename T>
	inline auto Read() -> T
	{
		auto& v = *reinterpret_cast<T const*>(offset_);
		offset_ += sizeof(T);
		position_ += sizeof(T);
		return v;
	}

	auto Read_Byte()->int8_t;
	auto Read_Bytes(int32_t length)->std::unique_ptr<int8_t[]>;
	auto Read_Short()->int16_t;
	auto Read_UShort()->uint16_t;
	auto Read_Int()->int32_t;
	auto Read_UInt()->uint32_t;
	auto Read_Double() -> double;
	auto Get_Offset(int32_t length) -> const int8_t*;
	auto Read_Compressed_Int()->int32_t;
	auto Read_Compressed_Float() -> float;
	auto Read_Compressed_Long()->int64_t;
	auto Set_Position(int64_t position) -> bool;

	auto Decrypt_Ascii_String(const int8_t* bytes, int32_t len)->std::string;
	auto Decrypt_Unicode_String(const int8_t* bytes, int32_t len)->std::string;
	auto Convert_String(std::u16string const& p_str)->std::string;

	inline auto Get_File() -> FILE* { return file_; };
	inline auto& Get_File_Path() const { return path_; };
	inline auto Get_Length()  const { return length_; };
	inline auto Get_Position() const { return position_; };

private:
	auto Compute_Offset(unsigned int offset, unsigned int factor, unsigned int value) -> int;

	int64_t length_ = 0;
	uint64_t position_ = 0;
	FILE* file_ = nullptr;
	std::string path_;
	int8_t* base_;
	int8_t* offset_;
	void* file_handle_ = nullptr;
	void* map_ = nullptr;
};