#include "SunNode.h"
#include "SunBitmap.h"
#include "SunReader.h"
#include "SunTools.h"
#include "SunAudio.h"
#include <vector>
#include <sstream>

SunNode::SunNode(const std::shared_ptr<SunReader>& reader) : nodeType_(NodeType::NONE), parent_(nullptr)
{
	this->reader_ = reader;
	children_size_ = 0;
	offset_ = 0;
}

SunNode::SunNode(const std::shared_ptr<SunReader>& reader, SunNode* node) : parent_(node)
{
	this->reader_ = reader;
	children_size_ = 0;
	offset_ = 0;
}

auto SunNode::Load(const std::shared_ptr<SunReader>& reader) -> void
{
	parent_ = nullptr;
	nodeType_ = NodeType::NONE;
	this->reader_ = reader;
	children_size_ = 0;
	offset_ = 0;
	Root();
}

auto SunNode::begin() -> SunNodes::iterator
{
	if (children_.size() == 0 && children_size_ == 0 && offset_ != 0)
		Try_Expand();
	return children_.begin();
}

auto SunNode::end() -> SunNodes::iterator
{
	return children_.end();
}

auto SunNode::operator*() const -> SunNode
{
	return *this;
}

auto SunNode::operator==(SunNode const& a) const -> bool
{
	return this->identity_ == a.identity_;
}

auto SunNode::operator!=(SunNode const& a) const -> bool
{
	return this->identity_ != a.identity_;
}

auto SunNode::operator<(SunNode const& f) const -> bool
{
	// Not implemented
	return false;
}

SunNode::operator bool() const
{
	return data_.use ? true : false;
}

auto SunNode::operator[](unsigned int n) -> SunNode&
{
	return operator[](std::to_string(n));
}

auto SunNode::operator[](signed int n)  -> SunNode&
{
	return operator[](std::to_string(n));
}

auto SunNode::operator[](unsigned long n)  -> SunNode&
{
	return operator[](std::to_string(n));
}

auto SunNode::operator[](signed long n)  -> SunNode&
{
	return operator[](std::to_string(n));
}

auto SunNode::operator[](unsigned long long n)  -> SunNode&
{
	return operator[](std::to_string(n));
}

auto SunNode::operator[](signed long long n)  -> SunNode&
{
	return operator[](std::to_string(n));
}

auto SunNode::operator[](std::string const& name)  -> SunNode&
{
	return operator[](name.c_str());
}

auto SunNode::operator[](char const* name)  -> SunNode&
{
	SunNode& node = Get_Child(name);
	if (node.reader_.get() == nullptr)
	{
		node.reader_ = reader_;
	}

	return node;
}

auto SunNode::operator[](SunNode& node)  -> SunNode&
{
	return operator[](node.Get_String());
}

SunNode::operator unsigned char() const
{
	return static_cast<unsigned char>(Get_Integer());
}

SunNode::operator signed char() const
{
	return static_cast<signed char>(Get_Integer());
}

SunNode::operator unsigned short() const
{
	return static_cast<unsigned short>(Get_Integer());
}

SunNode::operator signed short() const
{
	return static_cast<signed short>(Get_Integer());
}

SunNode::operator unsigned int() const
{
	return static_cast<unsigned int>(Get_Integer());
}

SunNode::operator signed int() const
{
	return static_cast<signed int>(Get_Integer());
}

SunNode::operator unsigned long() const
{
	return static_cast<unsigned long>(Get_Integer());
}

SunNode::operator signed long() const
{
	return static_cast<signed long>(Get_Integer());
}

SunNode::operator unsigned long long() const
{
	return static_cast<unsigned long long>(Get_Integer());
}

SunNode::operator signed long long() const
{
	return static_cast<signed long long>(Get_Integer());
}

SunNode::operator float() const
{
	return static_cast<float>(Get_Real());
}

SunNode::operator double() const
{
	return static_cast<double>(Get_Real());
}

SunNode::operator long double() const
{
	return static_cast<long double>(Get_Real());
}

SunNode::operator std::string() const
{
	return Get_String();
}

SunNode::operator SunBitmap() const
{
	return SunBitmap(data_.bitmap.height, data_.bitmap.width, data_.bitmap.length, data_.bitmap.offset, reader_);
}

SunNode::operator SunAudio()
{
	return Get_Audio();
}

auto SunNode::Get_Integer(int64_t default_value) const -> int64_t
{
	if (!data_.use) return default_value;
	switch (nodeType_)
	{
	case SunNode::NodeType::NONE:
		break;
	case SunNode::NodeType::INTEGER:
		return data_.intreal;
	case SunNode::NodeType::REAL:
		return static_cast<int64_t>(data_.doublereal);
	case SunNode::NodeType::STRING:
		return std::stoll(data_.str);
	case SunNode::NodeType::VECTOR:
		break;
	case SunNode::NodeType::BITMAP:
		break;
	case SunNode::NodeType::AUDIO:
		break;
	default:
		break;
	}
	return default_value;
}

auto SunNode::Get_Real(double defaultValue) const -> double
{
	if (!data_.use) return defaultValue;
	switch (nodeType_)
	{
	case SunNode::NodeType::NONE:
		break;
	case SunNode::NodeType::INTEGER:
		return static_cast<double>(data_.intreal);
	case SunNode::NodeType::REAL:
		return data_.doublereal;
	case SunNode::NodeType::STRING:
		return std::stod(data_.str);	//Converts char sequence to double
	case SunNode::NodeType::VECTOR:
		break;
	case SunNode::NodeType::BITMAP:
		break;
	case SunNode::NodeType::AUDIO:
		break;
	default:
		break;
	}
	return 0;
}

auto SunNode::Get_String(std::string default_str) const -> std::string
{
	if (!data_.use) return default_str;
	switch (nodeType_)
	{
	case SunNode::NodeType::INTEGER:
		return std::to_string(data_.intreal);
	case SunNode::NodeType::REAL:
		return std::to_string(static_cast<int64_t>(data_.doublereal));
	case SunNode::NodeType::STRING:
		return data_.str;
	}
	return default_str;
}

auto SunNode::Get_Vector(vector2i default_v) -> vector2i
{
	return std::make_pair(data_.vector[0], data_.vector[1]);
}

auto SunNode::Get_Bitmap() const -> SunBitmap
{
	return SunBitmap(data_.bitmap.height, data_.bitmap.width, data_.bitmap.length, data_.bitmap.offset, reader_);
}

auto SunNode::Get_Audio() -> SunAudio
{
	return SunAudio(this->reader_, data_.audio.offset, data_.audio.length, data_.audio.header);
}

auto SunNode::Get_Boolean(bool def) const -> bool
{
	return (data_.use && nodeType_ == NodeType::INTEGER) ? (Get_Integer() == 1 ? true : false) : def;
}

auto SunNode::Get_Children_Size() ->int32_t
{
	//return children_size_;
	return Children_Size();
}

auto SunNode::X() const -> int32_t
{
	return data_.use && nodeType_ == NodeType::VECTOR ? data_.vector[0] : 0;
}

auto SunNode::Y() const -> int32_t
{
	return  data_.use && nodeType_ == NodeType::VECTOR ? data_.vector[1] : 0;
}

auto SunNode::Name() -> std::string&
{
	return identity_;
}

auto SunNode::Get_Size() const -> size_t
{
	return size_;
}

auto SunNode::Root()-> SunNode&
{
	if (reader_.get()->Valid())
	{
		reader_->Set_Position(reader_->header_.size);
		reader_->header_.end_of_data = Compute_End_Of_Data();
		reader_->Set_Position(reader_->header_.size);	//would be +2 for (short) version
		Expand_Root(identity_);
	}
	return *this;
}

auto SunNode::Resolve(const char* path) -> SunNode&
{
	int64_t current_offset = reader_->Get_Position();
	std::vector<std::string> arrays;
	sun_tools::Split(path, arrays, "/", true);
	std::vector<std::string> a2;
	for (auto& str : arrays)
	{
		if (str != "..")
		{
			a2.push_back(str);
		}
	}
	auto n = this;
	for (auto& part : a2) {
		n = &n->Get_Child(part.c_str());
	}
	reader_->Set_Position(current_offset);

	return *n;
}

auto SunNode::Resolve(bool expandZero) ->SunNode&
{
	std::vector<std::string> arrays;
	sun_tools::Split(data_.str, arrays, "/", true);
	std::vector<std::string> a2;
	SunNode* p = this->parent_;

	for (auto& str : arrays)
	{
		if (str != "..")
		{
			a2.push_back(str);
		}
		else {
			p = p->parent_;
		}
	}
	//int64_t current_offset = reader->getPosition();
	SunNode* root = p;
	for (auto& s : a2)
	{
		root = &root->Get_Child(s.c_str());
	}

	if (root->nodeType_ == NodeType::NONE && expandZero)
	{
		root = &root->Get_Child("0");
	}

	nodeType_ = root->nodeType_;
	data_ = root->data_;
	offset_ = root->offset_;
	parent_ = root->parent_;
	identity_ = root->identity_;
	children_size_ = root->children_size_;
	type_ = root->type_;
	//	size = root->size;
	children_ = root->children_;
	reader_->Set_Position(offset_);
	return *this;
}

auto SunNode::Get_Child(const char* name) -> SunNode&
{
	auto iter = children_.find(name);
	auto Resolve_Node = [&](SunNode& node) -> SunNode&
	{
		node.parent_ = this;
		return node;
	};
	if (iter == children_.end())
	{
		Try_Expand();
		return Resolve_Node(children_[name]);
	}
	return Resolve_Node(iter->second);
}

auto SunNode::Find(const std::string& name) -> SunNodes::iterator
{
	if (this->children_.size() == 0)
	{
		Try_Expand();
	}
	return children_.find(name);
}

auto SunNode::Compute_End_Of_Data() -> int32_t
{
	for (int count = reader_->Read_Compressed_Int(); count > 0; --count)
	{
		switch (reader_->Read_Byte())
		{
		case 1:	// File
			reader_->Read_String(reader_->header_.size + 1 + reader_->Read_Int());
			break;
		case 2:	// SunDirectory
			reader_->Read_String();
			break;
		case 3: // Image (not wzimage but like literal png)
			reader_->Read_String();
			break;
		case 4:	// SunProperty
			reader_->Read_String();
			break;
		default: return 0;
		}
		reader_->Read_Compressed_Int();	// Read compressed size
		reader_->Read_Int();			// Read compressed offset
	}

	//while (0 < children--)
	//	Compute_End_Of_Data();
	return (int)reader_->Get_Position();
}

auto SunNode::Query_Identity() -> std::string
{
	int32_t children = 0;
	for (int32_t count = reader_->Read_Compressed_Int(); count > 0; --count)
	{
		switch (reader_->Read_Byte())
		{
		case 1:
			reader_->Read_Decrypt_String(reader_->header_.size + 1 + reader_->Read_Int());
			++children;
			break;
		case 2:
			return reader_->Read_Decrypt_String(reader_->header_.size + 1 + reader_->Read_Int());
		case 3:
			reader_->Read_Decrypt_String();
			++children;
			break;
		case 4:
			return reader_->Read_Decrypt_String();
		default: return "";
		}
		reader_->Read_Compressed_Int();
		reader_->Read_Compressed_Int();
		reader_->Read_Int();
	}

	while (0 < children--)
	{
		std::string identity = Query_Identity();
		if (identity != "")
			return identity;
	}
	return "";
}

auto SunNode::Expand_Root(std::string name) -> bool
{
	std::list<SunNode> nodes;
	children_size_ = reader_->Read_Compressed_Int();
	for (int count = children_size_; count > 0; --count)
	{
		std::string identity = "";
		byte type = reader_->Read_Byte();
		switch (type)
		{
		case 1: case 2:	// File / Directory
			//identity = reader_->Read_Decrypt_String(reader_->header_.size + 1 + reader_->Read_Int());
			identity = reader_->Read_String();
			break;
		case 3: case 4:	// Image(png) / Property
			//identity = reader_->Read_Decrypt_String();
			identity = reader_->Read_String();
			break;
		default:
			return false;
		}

		int32_t size = reader_->Read_Compressed_Int();
		int32_t offset = reader_->Read_UInt();

		nodes.emplace_back(SunNode(reader_, this));
		SunNode& node = nodes.back();
		node.identity_ = identity;
		node.type_ = type;
		node.size_ = size;
		node.offset_ = offset;
	}
	for (auto& a : nodes) {
		std::string& name = a.identity_;

		if (a.type_ == 1)	//SunFile
		{
			children_.emplace(name, std::move(a)).first->second.Expand_Root(a.identity_);
		}
		else
		{
			children_.emplace(name, std::move(a));
		}
	}
	return true;
}

auto SunNode::Try_Expand() ->void
{
	if (reader_ == nullptr)
	{
		return;
	}
	reader_->Set_Position(offset_);
	Expand_Node(offset_, 0);
}

auto SunNode::Get_Children(bool expand) -> SunNodes&
{
	if (expand)
	{
		if (this->children_.size() == 0)
		{
			Try_Expand();
		}
	}
	return children_;
}

auto SunNode::Children_Size() -> int
{
	if (children_.size() == 0) {
		Try_Expand();
	}
	return this->children_.size();
}

auto SunNode::Clear() -> void
{
	this->children_.clear();
}

auto SunNode::Exist(const std::string& name) -> bool
{
	return Find(name) != end();
}

auto SunNode::Set_Node_Type(NodeType type) -> void
{
	this->nodeType_ = type;
}

auto SunNode::Get_Node_Type() -> NodeType
{
	return nodeType_;
}

auto SunNode::Get_Parent() -> SunNode*
{
	return parent_;
}

auto SunNode::Set_Parent(SunNode* parent) -> void
{
	this->parent_ = parent;
}

auto SunNode::Expand_Node(int64_t offset, int64_t eob) -> bool
{
	//std::string type = reader_->Transit_String(offset); // Reads the "Property" string preceding property info in .wz files
	//int index = sun_tools::GetIndexFromTypes(type);	// Type index (WzObject Type)
	//bool ret = false;
	//switch (index)
	//{
	//case 0:  ret = Expand_Shape_2dConvex2D(offset, eob); break;
	//case 1:  ret = Expand_Shape_2dVector2D(offset); break;
	//case 2:  ret = Expand_Sound_Dx8(offset, eob); break;
	//case 3:  ret = Expand_Property(offset); break;
	//case 4:  ret = Expand_Image(offset); break;
	//default: break;
	//}
	uint32_t count = reader_->Read_Compressed_Int();	// Read property count.
	bool ret = false;
	int type = reader_->Read_Byte();	// Read SunObjectType.
	switch (type)
	{
	case 1: // File
		ret = false;
		break;
	case 2:	// Directory
		ret = false;
		break;
	case 3:	// Image (png)
		ret = false;
		break;
	case 4:	// Property
		ret = Expand_Property(offset, count);
		break;
	}
	return ret;
}

auto SunNode::Expand_Shape_2dConvex2D(int64_t offset, int64_t eob) -> bool
{
	int count = reader_->Read_Compressed_Int();
	for (int index = 0; index < count; ++index)
		if (!Expand_Node(offset, eob))
			return false;
	return true;
}

auto SunNode::Expand_Shape_2dVector2D(int64_t offset) -> bool
{
	int x = reader_->Read_Compressed_Int();
	int y = reader_->Read_Compressed_Int();
	Set_Node_Type(NodeType::VECTOR);
	data_.use = true;
	data_.vector[0] = x;
	data_.vector[1] = y;
	return true;
}

auto SunNode::Expand_Sound_Dx8(int64_t offset, int64_t eob) -> bool
{
	int unknow = reader_->Read_Byte();// Always 0
	int size = reader_->Read_Compressed_Int(); //+ 82u;//?
	int unknow1 = reader_->Read_Compressed_Int();
	reader_->Set_Position(reader_->Get_Position() + 51);
	nodeType_ = NodeType::AUDIO;
	data_.use = true;
	data_.audio.offset = static_cast<uint32_t>(eob - size);
	data_.audio.length = size;
	data_.audio.header = reader_->Read_Byte();
	reader_->Set_Position(eob);
	return true;
}

auto SunNode::Expand_Property(int64_t offset, uint32_t count) -> bool
{
	//children_size_ = count;
	for (int index = 0; index < count; ++index)
	{
		//std::string identity = reader_->Transit_String(offset);
		std::string identity = reader_->Read_String();
		byte type = reader_->Read_Byte();
		int64_t position = reader_->Get_Position();	// Add image size to after SunPropertyType. in wz there are 2 and they're different?
		SunNode& n = children_.emplace(identity,
			SunNode(reader_, this)
		).first->second;
		n.data_.use = true;
		n.identity_ = identity;
		n.offset_ = position;

		switch (type)
		{
		case 0x00: case 0x01:	// NULL
			n.nodeType_ = NodeType::NONE;
			n.data_.use = false;
			break;
		case 0x02:	// SHORT
			n.nodeType_ = NodeType::INTEGER;
			n.data_.intreal = reader_->Read_Short();
			break;
		case 0x03:	// INT
			n.nodeType_ = NodeType::INTEGER;
			n.data_.intreal = reader_->Read_Compressed_Int();
			break;
		case 0x04:	// LONG
			n.nodeType_ = NodeType::INTEGER;
			n.data_.doublereal = reader_->Read_Compressed_Long();
			break;
		case 0x05:	// FLOAT
			n.nodeType_ = NodeType::REAL;
			n.data_.doublereal = reader_->Read_Compressed_Float();
			break;
		case 0x06:	// DOUBLE
			n.nodeType_ = NodeType::REAL;
			n.data_.doublereal = reader_->Read_Double();
			break;
		case 0x07:	// STRING
			n.nodeType_ = NodeType::STRING;
			n.data_.str = reader_->Read_String();
		case 0x08:	// IMAGE/BITMAP
			int length = reader_->Read_Int();
			//int64_t end_of_bytes = length + reader_->Get_Position();
			n.Expand_Image();
			//n.nodeType_ = NodeType::BITMAP;
			//n.data_.str = reader_->Read_String();
			break;
			//case 0x00:
			//	n.nodeType_ = NodeType::NONE;
			//	n.data_.use = false;
			//	break;
			//case 0x02: case 0x0b:
			//	n.nodeType_ = NodeType::INTEGER;
			//	n.data_.intreal = reader_->Read_Short();
			//	break;
			//case 0x03: case 0x13:
			//	n.nodeType_ = NodeType::INTEGER;
			//	n.data_.intreal = reader_->Read_Compressed_Int();
			//	break;
			//case 0x04:
			//	n.nodeType_ = NodeType::REAL;
			//	n.data_.doublereal = reader_->Read_Compressed_Float();
			//	break;
			//case 0x05:
			//	n.nodeType_ = NodeType::REAL;
			//	n.data_.doublereal = reader_->Read_Double();
			//	break;
			//case 0x08:
			//	n.nodeType_ = NodeType::STRING;
			//	n.data_.str = reader_->Transit_String(offset);
			//	break;
			//case 0x14:
			//	n.nodeType_ = NodeType::INTEGER;
			//	n.data_.intreal = reader_->Read_Compressed_Long();
			//	break;
			//case 0x09:
			//	n.data_.use = false;
			//	// Position of the next piece;
			//	int read = reader_->Read_Int();
			//	int64_t eob = read + reader_->Get_Position();
			//	n.Expand_Node(offset, eob);
			//	reader_->Set_Position(eob); // Restore location;
			//	break;
		}
	}
	return true;
}

auto SunNode::Expand_Image() -> bool
{
	if (reader_->Read_Byte() == 1)	// Image has properties.
		//if (!Expand_Property(offset))
		return false;

	int32_t width = reader_->Read_Compressed_Int();
	int32_t height = reader_->Read_Compressed_Int();
	int32_t size = reader_->Read_Int();
	int64_t offset = reader_->Get_Position();

	nodeType_ = NodeType::BITMAP;
	data_.use = true;
	data_.bitmap.length = size;
	data_.bitmap.offset = static_cast<uint32_t>(offset);
	data_.bitmap.width = width;
	data_.bitmap.height = height;
	reader_->Set_Position(reader_->Get_Position() + size);
	return true;
}

auto operator+(std::string s, SunNode& node) -> std::string
{
	return s + node.Get_String();
}

auto operator+(char const* s, SunNode& node) -> std::string
{
	return s + node.Get_String();
}

auto operator+(SunNode& node, std::string s)-> std::string
{
	return node.Get_String() + s;
}

auto operator+(SunNode& node, char const* s)-> std::string
{
	return node.Get_String() + s;
}