#pragma once

// Error codes to be checked after initialization.
class Error
{
public:
	enum Code
	{
		NONE,
		WINDOW,
		MISSING_FILE,
		MISSING_FILES,
		AUDIO,
		FILE_ERROR,
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
		return code == Code::MISSING_FILE == Code::MISSING_FILES;
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
		"Failed to create game window.",
		"Missing a game file.",
		"Missing game files.",
		"Failed to initialize audio files."
	};
};