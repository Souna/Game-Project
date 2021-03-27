#include "Animation.h"
#include "Constants.h"
#include "Misc.h"
#include <set>

namespace game
{
	Frame::Frame(SunNode src)
	{
		texture_ = src;
		bounds_ = src;
		head_ = src["head"];
		delay_ = src["delay"];	if (delay_ == 0) delay_ = 100;
		// Learn about all this
		bool has_a_0 = src["a0"].Get_Node_Type() == SunNode::NodeType::INTEGER;
		bool has_a_1 = src["a1"].Get_Node_Type() == SunNode::NodeType::INTEGER;

		if (has_a_0 && has_a_1)
		{
			opacities_ = { src["a0"], src["a1"] };
		}
		else if (has_a_0)
		{
			uint8_t a0 = src["a0"];
			opacities_ = { a0, 255 - a0 };
		}
		else if (has_a_1)
		{
			uint8_t a1 = src["a1"];
			opacities_ = { 255 - a1, a1 };
		}
		else
		{
			opacities_ = { 255, 255 };
		}

		bool has_z_0 = src["z0"].Get_Node_Type() == SunNode::NodeType::INTEGER;
		bool has_z_1 = src["z1"].Get_Node_Type() == SunNode::NodeType::INTEGER;

		if (has_z_0 && has_z_1)
			scales_ = { src["z0"], src["z1"] };
		else if (has_z_0)
			scales_ = { src["z0"], 0 };
		else if (has_z_1)
			scales_ = { 100, src["z1"] };
		else
			scales_ = { 100, 100 };
	}
	Frame::Frame()
	{
		delay_ = 0;
		opacities_ = { 0,0 };
		scales_ = { 0,0 };
	}

	auto Frame::Draw() const -> void
	{
		texture_.Draw();
	}

	auto Frame::Start_Opacity() const -> uint8_t
	{
		return opacities_.first;
	}

	auto Frame::Start_Scale() const -> uint16_t
	{
		return scales_.first;
	}

	auto Frame::Get_Delay() const -> uint16_t
	{
		return delay_;
	}

	auto Frame::Get_Origin() const -> Point<int16_t>
	{
		return texture_.Get_Origin();
	}

	auto Frame::Get_Dimensions() const -> Point<int16_t>
	{
		return texture_.Get_Dimensions();
	}

	auto Frame::Get_Head() const -> Point<int16_t>
	{
		return head_;
	}

	auto Frame::Get_Bounds() const -> Rectangle<int16_t>
	{
		return bounds_;
	}

	auto Frame::Opacity_Step(uint16_t timestep) const -> float
	{
		return timestep * static_cast<float>(opacities_.second - opacities_.first) / delay_;
	}

	auto Frame::Scale_Step(uint16_t timestep) const -> float
	{
		return timestep * static_cast<float>(scales_.second - scales_.first) / delay_;
	}

	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================
	//===================================================================================================

	Animation::Animation(SunNode src)
	{
		bool is_texture = src.Get_Node_Type() == SunNode::NodeType::BITMAP;
		std::map<std::string, SunNode>::iterator iter;

		if (is_texture)
		{
			frames_.push_back(src);
		}
		else
		{
			std::set<int16_t> frame_ids;

			// WTF IS THIS FUCKNIG SHIT
			for (iter = src.begin(); iter != src.end(); iter++)
			{
				if (src.Get_Child(iter->first.c_str()).Get_Node_Type() == SunNode::NodeType::BITMAP)
				{
					int16_t frame_id = string_conversion::Or_Default<int16_t>(iter->first.c_str(), -1);
					if (frame_id >= 0)
						frame_ids.insert(frame_id);
				}
			}

			for (auto& id : frame_ids)
			{
				auto sub = src[std::to_string(id)];
				frames_.push_back(sub);
			}

			if (frames_.empty())
				frames_.push_back(Frame());
		}
		animated_ = frames_.size() > 1;
		zigzag_ = src["zigzag"].Get_Boolean();
		Reset();
	}

	Animation::Animation()
	{
		animated_ = false;
		zigzag_ = false;
		frames_.push_back(Frame());
		Reset();
	}

	auto Animation::Update() -> bool
	{
		return Update(constants::TIMESTEP);
	}

	auto Animation::Update(uint16_t timestep) -> bool
	{
		const Frame& frame_data = Get_Frame();

		opacity_ += frame_data.Opacity_Step(timestep);

		if (opacity_.Last() < 0.0f)
			opacity_.Set(0.0f);
		else if (opacity_.Last() > 255.0f)
			opacity_.Set(255.0f);

		xy_scale_ += frame_data.Scale_Step(timestep);

		if (xy_scale_.Last() < 0.0f)
			opacity_.Set(0.0f);

		if (timestep >= delay_)
		{
			int16_t last_frame = static_cast<int16_t>(frames_.size() - 1);
			int16_t next_frame;
			bool ended;

			if (zigzag_ && last_frame > 0)
			{
				if (framestep_ == 1 && frame_ == last_frame)
				{
					framestep_ = -framestep_;
					ended = false;
				}
				else if (framestep_ == -1 && frame_ == 0)
				{
					framestep_ = -framestep_;
					ended = true;
				}
				else
				{
					ended = false;
				}

				next_frame = frame_ + framestep_;
			}
			else
			{
				if (frame_ == last_frame)
				{
					next_frame = 0;
					ended = true;
				}
				else
				{
					next_frame = frame_ + 1;
					ended = false;
				}
			}

			uint16_t delta = timestep - delay_;
			float threshold = static_cast<float>(delta) / timestep;
			frame_.Next(next_frame, threshold);

			delay_ = frames_[next_frame].Get_Delay();

			if (delay_ >= delta)
				delay_ -= delta;

			opacity_.Set(frames_[next_frame].Start_Opacity());
			xy_scale_.Set(frames_[next_frame].Start_Scale());

			return ended;
		}
		else
		{
			frame_.Normalize();

			delay_ -= timestep;

			return false;
		}
	}

	auto Animation::Reset() -> void
	{
		frame_.Set(0);
		opacity_.Set(frames_[0].Start_Opacity());
		xy_scale_.Set(frames_[0].Start_Scale());
		delay_ = frames_[0].Get_Delay();
		framestep_ = 1;
	}

	auto Animation::Draw() const -> void
	{
		frames_[0].Draw();
	}

	auto Animation::Get_Delay(int16_t frame_id) const -> uint16_t
	{
		return frame_id < frames_.size() ? frames_[frame_id].Get_Delay() : 0;
	}

	auto Animation::Get_Delay_Until(int16_t frame_id) const -> uint16_t
	{
		uint16_t total = 0;

		for (int16_t i = 0; i < frame_id; i++)
		{
			if (i >= frames_.size())
				break;

			total += frames_[frame_id].Get_Delay();
		}

		return total;
	}

	auto Animation::Get_Origin() const -> Point<int16_t>
	{
		return Get_Frame().Get_Origin();
	}

	auto Animation::Get_Dimensions() const -> Point<int16_t>
	{
		return Get_Frame().Get_Dimensions();
	}

	auto Animation::Get_Head() const -> Point<int16_t>
	{
		return Get_Frame().Get_Head();
	}

	auto Animation::Get_Bounds() const -> Rectangle<int16_t>
	{
		return Get_Frame().Get_Bounds();
	}

	auto Animation::Get_Frame() const -> const Frame&
	{
		return frames_[frame_.Get()];
	}
}