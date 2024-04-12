//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once


#include <array>
#include <cstdint>
//#include <gl/GL.h>

namespace game
{
	// Simple color class which stores r-g-b-a components.
	class Color
	{
	public:
		static constexpr size_t LENGTH = 4;
		using underlying_t = std::array<float, LENGTH>;

		// Codes of predefined colors.
		enum Code : uint32_t
		{
			CNONE = 0x00000000,
			CWHITE = 0xFFFFFFFF,
			CBLACK = 0x000000FF,
			CRED = 0xFF0000FF,
			CGREEN = 0x00FF00FF,
			CBLUE = 0x0000FFFF,
			CYELLOW = 0xFFFF00FF,
			CTURQUOISE = 0x00FFFFFF,
			CPURPLE = 0xFF00FFFF
		};

		// Name of predefined colors.
		enum Name : uint32_t
		{
			BLACK,
			WHITE,
			YELLOW,
			BLUE,
			RED,
			DARKRED,
			BROWN,
			JAMBALAYA,
			LIGHTGREY,
			DARKGREY,
			ORANGE,
			MEDIUMBLUE,
			VIOLET,
			TOBACCOBROWN,
			EAGLE,
			LEMONGRASS,
			TUNA,
			GALLERY,
			DUSTYGRAY,
			EMPEROR,
			MINESHAFT,
			HALFANDHALF,
			ENDEAVOUR,
			BROWNDERBY,
			PORCELAIN,
			IRISHCOFFEE,
			BOULDER,
			GREEN,
			LIGHTGREEN,
			JAPANESELAUREL,
			GRAYOLIVE,
			ELECTRICLIME,
			SUPERNOVA,
			CHARTREUSE,
			MALIBU,
			NUM_COLORS
		};

		// Predefined colors by name
		static constexpr std::array<std::array<uint8_t, 3>, Name::NUM_COLORS> colors = {
			{{0, 0, 0},         // Black
			 {255, 255, 255},   // White
			 {255, 255, 0},     // Yellow
			 {0, 0, 255},       // Blue
			 {255, 0, 0},       // Red
			 {204, 0, 0},       // DarkRed
			 {128, 64, 0},      // Brown
			 {87, 51, 18},      // Jambalaya
			 {128, 128, 128},   // Lightgrey
			 {64, 64, 64},      // Darkgrey
			 {255, 128, 0},     // Orange
			 {0, 191, 255},     // Mediumblue
			 {128, 0, 128},     // Violet
			 {120, 102, 69},    // Tobacco Brown
			 {189, 189, 171},   // Eagle
			 {153, 153, 138},   // Lemon Grass
			 {51, 51, 69},      // Tuna
			 {240, 240, 240},   // Gallery
			 {153, 153, 153},   // Dusty Gray
			 {87, 87, 87},      // Emperor
			 {51, 51, 51},      // Mine Shaft
			 {255, 255, 222},   // Half and Half
			 {0, 102, 171},     // Endeavour
			 {77, 51, 26},      // Brown Derby
			 {240, 242, 242},   // Porcelain
			 {87, 69, 36},      // Irish Coffee
			 {120, 120, 120},   // Boulder
			 {0, 191, 0},       // Green (Mob HP Bar)
			 {0, 255, 0},       // Light Green (Mob HP Bar)
			 {0, 128, 0},       // Japanese Laurel (Mob HP Bar)
			 {171, 171, 153},   // Gray Olive
			 {204, 255, 0},     // Electric Lime
			 {255, 204, 0},     // Supernova
			 {120, 255, 0},     // Chartreuse
			 {120, 204, 255}}   // Malibu
		};

		// Create a color by an array of real numbers [0.0f, 1.0f]
		constexpr Color(underlying_t comps) : rgba(comps) {}
		// Create a color by real numbers [0.0f, 1.0f]
		constexpr Color(float red, float green, float blue, float alpha) : Color(underlying_t{ red, green, blue, alpha }) {}
		// Create a color by an array of natural numbers [0, 255]
		constexpr Color(const std::array<uint8_t, Color::LENGTH> comps) : Color(comps[0], comps[1], comps[2], comps[3]) {}

		// Create a color by natural numbers [0, 255]
		constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : Color(
			static_cast<float>(red) / 255,
			static_cast<float>(green) / 255,
			static_cast<float>(blue) / 255,
			static_cast<float>(alpha) / 255
		) {}

		// Create a color by code.
		constexpr Color(uint32_t code) : Color(
			static_cast<uint8_t>(code >> 24),
			static_cast<uint8_t>(code >> 16),
			static_cast<uint8_t>(code >> 8),
			static_cast<uint8_t>(code)
		) {}

		// Create a color by named code.
		constexpr Color(Code code) : Color((uint32_t)code) {}
		constexpr Color() : Color(Code::CNONE) {}

		// Check whether the color is completely invisble.
		constexpr bool invisible() const
		{
			return rgba[3] <= 0.0f;
		}

		// Return the red component.
		constexpr float r() const
		{
			return rgba[0];
		}

		// Return the green component.
		constexpr float g() const
		{
			return rgba[1];
		}

		// Return the blue component.
		constexpr float b() const
		{
			return rgba[2];
		}

		// Return the alpha (opacity) component.
		constexpr float a() const
		{
			return rgba[3];
		}

		// Return all components.
		const float* data() const;

		// Return a begin iterator.
		underlying_t::const_iterator begin() const;

		// Return an end iterator.
		underlying_t::const_iterator end() const;

		// Blend the second color into the first.
		Color blend(const Color& other, float alpha) const;

		// Combine two colors.
		constexpr Color operator*(const Color& o) const
		{
			return Color(
				r() * o.r(),
				g() * o.g(),
				b() * o.b(),
				a() * o.a()
			);
		}

		// Combine two colors.
		constexpr Color operator/(const Color& o) const
		{
			return Color(
				r() / o.r(),
				g() / o.g(),
				b() / o.b(),
				a() / o.a()
			);
		}

	private:
		underlying_t rgba;
	};
}