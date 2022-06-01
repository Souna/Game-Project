#pragma once
#include <string>
#include "SunReader.h"
#include <memory>
#include <unordered_map>
#include <list>
#include <map>
#include "SunTools.h"

/**
* A single node within the SunFile.
*/
class SunNode
{
public:
	typedef std::pair<int32_t, int32_t> vector2i;

	// Map of SunNodes.
	typedef std::map<std::string, SunNode> SunNodes;

	// Enum of the different property types in the file.
	enum class SunPropertyType
	{
		NONE = 0,	// SunNullProperty
		INTEGER = 1,
		REAL = 2,
		STRING = 3,
		VECTOR = 4,
		BITMAP = 5,
		AUDIO = 6,
		SUBPROP = 7
	};

	enum class SunObjectType
	{
		NONE = 0,
		FILE = 1,
		IMAGE = 2,
		DIRECTORY = 3,
		PROPERTY = 4
	};

	SunNode() = default;
	SunNode(const std::shared_ptr<SunReader>& reader);
	SunNode(const std::shared_ptr<SunReader>& reader, SunNode* node);
	~SunNode() = default;

	auto Load(const std::shared_ptr<SunReader>& reader) -> void;
	auto begin()->SunNodes::iterator;
	auto end()->SunNodes::iterator;

	auto operator*() const->SunNode;
	auto operator==(SunNode const&) const -> bool;
	auto operator!=(SunNode const&) const -> bool;
	auto operator<(SunNode const&) const -> bool;
	// Checks whether or not the node points to an actual node.
	explicit operator bool() const;

	auto operator[](unsigned int)->SunNode&;
	auto operator[](signed int)->SunNode&;
	auto operator[](unsigned long)->SunNode&;
	auto operator[](signed long)->SunNode&;
	auto operator[](unsigned long long)->SunNode&;
	auto operator[](signed long long)->SunNode&;
	auto operator[](std::string const&)->SunNode&;
	auto operator[](char const*)->SunNode&;
	auto operator[](SunNode&)->SunNode&;

	operator unsigned char() const;
	operator signed char() const;
	operator unsigned short() const;
	operator signed short() const;
	operator unsigned int() const;
	operator signed int() const;
	operator unsigned long() const;
	operator signed long() const;
	operator unsigned long long() const;
	operator signed long long() const;
	operator float() const;
	operator double() const;
	operator long double() const;
	operator std::string() const;
	operator SunBitmap() const;
	operator SunAudio();
	operator vector2i();

	// Property-returning methods.
	auto Get_Integer(int64_t default_value = 0) const->int64_t;
	auto Get_Real(double default_value = 0) const -> double;
	auto Get_String(std::string default_str = "")const->std::string;
	auto Get_Vector(vector2i = { 0, 0 })->vector2i;
	auto Get_Bitmap() const->SunBitmap;
	auto Get_Audio()->SunAudio;
	auto Get_Boolean(bool def = false) const -> bool;
	auto Get_Children_Size()->int32_t;

	// Returns the x or y-coordinates of the Vector data value.
	auto X() const->int32_t;
	auto Y() const->int32_t;

	// Returns the name of the node as a reference.
	auto Name()->std::string&;

	auto Get_Size() const->size_t;

	// Returns the root node of the file this node was derived from.
	auto Root()->SunNode&;

	// Takes a '/' separated string and resolves the given path.
	auto Resolve(const char* path)->SunNode&;
	auto Resolve(bool expand_zero = true)->SunNode&;
	auto Get_Child(const char* name)->SunNode&;
	auto Find(const std::string& name)->SunNodes::iterator;
	// Computes file size.
	auto Compute_End_Of_Data()->int32_t;
	auto Query_Identity()->std::string;
	auto Expand_Root() -> bool;
	auto Try_Expand() -> void;
	auto Get_Children(bool expand = true)->SunNodes&;
	auto Children_Size() -> int;
	auto Clear() -> void;
	auto Exist(const std::string& name) -> bool;
	// Sets the type of data the node will contain.
	auto Set_Node_Type(SunPropertyType type) -> void;
	// Gets the type of data contained within the node.
	auto Get_Node_Type()->SunPropertyType;
	auto Get_Parent()->SunNode*;
	auto Set_Parent(SunNode* parent) -> void;
	std::string identity_;

private:
	auto Expand_Node(int64_t offset, int64_t eob) -> bool;
	auto Expand_Property() -> bool;
	auto Expand_Extended_Property() -> bool;
	auto Expand_Sub_Property() -> bool;
	auto Expand_Canvas_Property() -> bool;
	auto Expand_Vector_Property() -> bool;

	friend std::ostream& operator<< (std::ostream& os, const SunNode& n);

	// Trash
	auto Expand_Shape_2dConvex2D(int64_t offset, int64_t eob) ->bool;
	auto Expand_Sound_Property(int64_t offset, int64_t eob) -> bool;

	std::shared_ptr<SunReader> reader_;
	SunNodes children_;
	SunPropertyType sunPropertyType_ = SunPropertyType::NONE;

	SunObjectType sunObjectType_ = SunObjectType::NONE;
	int32_t size_ = 0;
	int32_t checksum_ = 0;
	int64_t offset_ = 0;
	Data data_;
	int32_t children_size_ = 0;
	SunNode* parent_ = nullptr;
};

//More convenience string concatenation operators
auto operator+(std::string, SunNode&)->std::string;
auto operator+(char const*, SunNode&)->std::string;
auto operator+(SunNode&, std::string)->std::string;
auto operator+(SunNode&, char const*)->std::string;
