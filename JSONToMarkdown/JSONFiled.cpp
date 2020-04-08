#include "JSONFiled.h"

std::ostream & operator<<(std::ostream & os, const JSONFiled & jsonFiled)
{
	os << "| " << jsonFiled.filedName << " | " << jsonFiled.filedType << " | " << jsonFiled.filedExplain << " |";
	return os;
}
