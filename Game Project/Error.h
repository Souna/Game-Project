#pragma once

// Error codes to be checked after initialization.
class Error
{
public:
	enum Code
	{
		NONE,
		FILE_ERROR,
		MISSING_FILE,
		WINDOW,
		AUDIO,
		LENGTH
	};

	constexpr Error(Code c) : Error(c, "") {}
	constexpr Error(Code c, const char* args) : code(c), args(args) {}

	constexpr operator bool() const
	{
		return code != Code::NONE;
	}

	constexpr bool can_retry() const
	{
		return code == Code::MISSING_FILE;
	}

	constexpr const char* get_message() const
	{
		return messages[code];
	}

	constexpr const char* get_args() const
	{
		return args;
	}

private:
	Code code;
	const char* args;

	static constexpr const char* messages[Code::LENGTH] =
	{
		"",
		"Missing a game file: ",
		"Failed to create game window.",
		"Failed to initialize audio files."
	};
};