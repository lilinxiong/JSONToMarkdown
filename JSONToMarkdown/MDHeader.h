#pragma once
#ifndef GNOIX_INL_MD_HEADER_H_
#define GNOIX_INL_MD_HEADER_H_

#include "base.h"
class MDHeader
{
public:
	MDHeader() {
		headers.push_back("filedName");
		headers.push_back("filedType");
		headers.push_back("filedExplain");
	};
	virtual ~MDHeader() { headers.clear(); };
	friend std::ostream& operator<<(std::ostream &os, const MDHeader &mdHeader);
private:
	std::vector<std::string> headers;
};

#endif // !GNOIX_INL_MD_HEADER_H_