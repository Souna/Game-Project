#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include "SunTools.h"
class SunReader;

class SunAudio
{
public:
	SunAudio() = default;
	SunAudio(std::shared_ptr<SunReader> reader, uint32_t offset, uint32_t len, int32_t header);


	auto Get_Data() -> std::shared_ptr<std::vector<uint8_t>>&;
	auto Get_Audio_Data() -> const int8_t*;
	auto Get_Information() -> void;
	auto Get_Wave_Header() -> int8_t*;


	inline auto Get_Id() const { return offset_; };
	inline auto Get_Length() const { return length_; };
	inline auto Is_Pcm() const { return 0x12 == header_; };
	inline auto Get_Channel() const { return channel_; };
	inline auto Get_Rate() const { return rate_; };
	inline auto Get_Bits() const { return bits_; };


private:
	int32_t channel_;
	int32_t rate_;
	int32_t bits_;
	int32_t header_ = 0;
	uint32_t offset_ = 0;
	uint32_t length_ = 0;
	std::shared_ptr<std::vector<uint8_t>> data_;
	std::shared_ptr<SunReader> reader_;
	const int8_t* buffer_ = nullptr;

	static int32_t bit_rates_[2][3][16];
	static int32_t frequencies_[][4];
	static int32_t samples_per_frames_[][3];
	static int32_t coefficients_[][3];
	static int32_t slots_[3];
};