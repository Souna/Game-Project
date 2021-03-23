#include "SunAudio.h"
#include "SunReader.h"
#include "SunNode.h"
#include <memory.h>


SunAudio::SunAudio(std::shared_ptr<SunReader> reader, uint32_t offset, uint32_t length, int32_t header)
	: reader_(reader), offset_(offset), length_(length), header_(header), bits_(NULL), channel_(NULL), rate_(NULL)
{
}

auto SunAudio::Get_Data() -> std::shared_ptr<std::vector<uint8_t>>&
{
	if (data_ != nullptr && data_->size() > 0)
	{
		return data_;
	}
	reader_->Set_Position(offset_);
	auto dataPtr = reader_->Read_Bytes(this->length_);
	data_ = std::make_shared<std::vector<uint8_t>>();
	data_->insert(data_->begin(), dataPtr.get(), dataPtr.get() + length_);
	//delete[] dataPtr;
	return data_;
}

auto SunAudio::Get_Audio_Data() -> const int8_t*
{
	if (reader_ == nullptr)
	{
		return nullptr;
	}
	if (buffer_ == nullptr) 
	{
		buffer_ = reader_->Get_Offset(offset_);
	}
	/*reader_->Set_Position(offset_);
	auto ptr = reader_->Read_Bytes(this->length_);
	return ptr.release();*/
	return buffer_;
}

auto SunAudio::Get_Information() -> void
{
	if (!Is_Pcm()) //MP3
	{
		int8_t* raw = nullptr;
		std::unique_ptr<int8_t[]> rawPtr = nullptr;
		int32_t tag = 0;
		int32_t id3 = 0;
		if (length_ > 128)
		{
			reader_->Set_Position(offset_ + length_ - 128);

			rawPtr = reader_->Read_Bytes(3);
			raw = rawPtr.get();
			if (raw[0] == 0x54 && raw[1] == 0x41 && raw[2] == 0x47)
				tag = 128;
		}
		reader_->Set_Position(offset_);

		rawPtr = reader_->Read_Bytes(4);
		raw = rawPtr.get();

		if (raw[0] == 0x49 && raw[1] == 0x44 && raw[2] == 0x33)
		{
			reader_->Set_Position(reader_->Get_Position() + 2);

			rawPtr = reader_->Read_Bytes(4);
			raw = rawPtr.get();
			id3 = 10 + (((raw[0] & 0x7f) << 21) + ((raw[1] & 0x7f) << 14) + ((raw[2] & 0x7f) << 7) + (raw[3] & 0x7f));

			reader_->Set_Position(offset_ + id3);

			rawPtr = reader_->Read_Bytes(4);
			raw = rawPtr.get();
		}

		while (0xff == raw[0] && 0xe0 == (raw[1] & 0xe0) && 0xf0 != (raw[2] & 0xf0))
		{
			int32_t version;
			int32_t layer;
			int32_t crc;
			int32_t bitrate;
			int32_t frequency;
			int32_t padding;
			int32_t framesize; /*
							int framescount;*/
			int32_t song_length;

			// ---vv--- (0 = mpeg2.5, 1 = reserved, 2 = mpeg2, 3 = mpeg1)
			version = (raw[1] >> 3) & 3;

			if (version == 1)
				break;

			// -----ll- (0 = reserved, 1 = layer3, 2 = layer2, 3 = layer1)
			// 3 - layer
			layer = 3 - ((raw[1] >> 1) & 3);

			if (layer == 3)
				break;

			// -------c
			crc = raw[1] & 1;

			// bbbb----
			bitrate = (raw[2] >> 4) & 15;

			if (bitrate == 0)
				break;

			bits_ = bitrate = bit_rates_[3 == version ? 0 : 1][layer][bitrate];

			// ----ff--
			frequency = (raw[2] >> 2) & 3;

			if (frequency == 3)
				break;

			frequency = frequencies_[version][frequency];

			// ------p-
			padding = (raw[2] >> 1) & 1;

			framesize = (coefficients_[3 == version ? 0 : 1][layer] * bitrate * 1000 / frequency + padding) * slots_[layer];

			// song length...
			song_length = (int32_t)(((length_ - tag - id3) / bitrate) * 8.0);

			rate_ = frequency;

			break;
			//	return string.Format("{0:d2}:{1:d2}.{2,-4:d} {3,3:d}kbps {4:g}khz", song_length / 60000, (song_length / 1000) % 60, song_length % 1000, bitrate_, frequency / 1000.0);
		}
	}
	else
	{
		int8_t* raw = Get_Wave_Header();
		int16_t channel;
		memcpy(&channel, raw + 2, sizeof(int16_t));
		this->channel_ = channel;

		int32_t f;
		memcpy(&f, raw + 4, sizeof(int32_t));
		double frequency = f / 1000.0;
		int16_t bitrate;
		memcpy(&bitrate, raw + 0x0e, sizeof(int16_t)); //14
		int32_t ms = (int)((length_ / bitrate) * 8.0 / frequency);

		//return string.Format("{0:d2}:{1:d2}.{2,-4:d} {3,3:d}kbps {4:g}khz", ms / 60000, (ms / 1000) % 60, ms % 1000, (int)(frequency * bitrate), frequency) + " : PCM";
		if (raw)
		{
			delete[] raw;
		}
	}
}

auto SunAudio::Get_Wave_Header() -> int8_t*
{
	reader_->Set_Position(offset_ - header_);
	if (reader_->Read_Short() == 1)
	{
		reader_->Set_Position(offset_ - header_);
		return reader_->Read_Bytes(0x10).release();
	}
	reader_->Set_Position(offset_ - header_);
	//return reader_->DecryptBytes(reader_->Read_Bytes(0x10).release(), 0x10);
	return reader_->Read_Bytes(0x10).release();
}

int32_t SunAudio::bit_rates_[2][3][16] = 
{
	//mpeg1
	{
		{
			0,
			32,
			64,
			96,
			128,
			160,
			192,
			224,
			256,
			288,
			320,
			352,
			384,
			416,
			448,
			0,
		}, // layer1
		{
			0,
			32,
			48,
			56,
			64,
			80,
			96,
			112,
			128,
			160,
			192,
			224,
			256,
			320,
			384,
			0,
		}, // layer2
		{
			0,
			32,
			40,
			48,
			56,
			64,
			80,
			96,
			112,
			128,
			160,
			192,
			224,
			256,
			320,
			0,
		}, // layer3
	},
	//mpeg2 & 2.5
	{
		{
			0,
			32,
			48,
			56,
			64,
			80,
			96,
			112,
			128,
			144,
			160,
			176,
			192,
			224,
			256,
			0,
		}, // layer1
		{
			0,
			8,
			16,
			24,
			32,
			40,
			48,
			56,
			64,
			80,
			96,
			112,
			128,
			144,
			160,
			0,
		}, // layer2
		{
			0,
			8,
			16,
			24,
			32,
			40,
			48,
			56,
			64,
			80,
			96,
			112,
			128,
			144,
			160,
			0,
		}, // layer3
	} 
};

int32_t SunAudio::frequencies_[][4] =
{
	{
		11025,
		12000,
		8000,
		0,
	}, // mpeg2.5
	{
		0,
		0,
		0,
		0,
	}, // reserved
	{
		22050,
		24000,
		16000,
		0,
	}, // mpeg2
	{
		44100,
		48000,
		32000,
		0,
	}, // mpeg1
};

int32_t SunAudio::samples_per_frames_[][3] =
{
	//mpeg1
	{
		384,  // layer1
		1152, // layer2
		1152, // layer3
	},
	//mpeg2 & 2.5
	{
		384,  // layer1
		1152, // layer2
		576,  // layer3
	},
};

int32_t SunAudio::coefficients_[][3] = 
{
	//mpeg1
	{
		12,  // layer1
		144, // layer2
		144, // layer3
	},
	//mpeg2 & 2.5
	{
		12,  // layer1
		144, // layer2
		72,  // layer3
	},
};

int32_t SunAudio::slots_[3] =
{
	4, // layer1
	1, // layer2
	1, // layer3
};