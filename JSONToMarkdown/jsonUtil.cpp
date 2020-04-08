#include "jsonUtil.h"

std::string jsonUtil::transferStr(const json_value_t & type)
{
	switch (type) {
	case json_value_t::boolean: {
		return std::string("boolean");
	}
	case json_value_t::number_float: {
		return std::string("float");
	}
	case json_value_t::number_integer: {
		return std::string("int");
	}
	case json_value_t::number_unsigned: {
		return std::string("long");
	}
	case json_value_t::string: {
		return std::string("String");
	}
	case json_value_t::array: {
		return std::string("array");
	}
	case json_value_t::object: {
		return std::string("object");
	}
	case json_value_t::discarded: {
		return std::string("discarded");
	}
	case json_value_t::null: {
		return std::string("null");
	}
	default:
		return std::string("");
	}
}
