#include "MDHeader.h"

std::ostream & operator<<(std::ostream & os, const MDHeader & mdHeader)
{
	os << "| ";
	for (auto iter = mdHeader.headers.begin(); iter != mdHeader.headers.end(); iter++)
		os << *iter << " |";
	os << std::endl;
	os << "|";
	for (int i = 0; i < mdHeader.headers.size(); i++) {
		os << " ---- |";
	}
	return os;
}
