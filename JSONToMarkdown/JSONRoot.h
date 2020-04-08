#pragma once
#ifndef GNOIX_INL_JSON_ROOT_H_
#define GNOIX_INL_JSON_ROOT_H_
#include "base.h"
#include "util.h"
#include "JSONObject.h"
class JSONRoot
{
public:
	JSONRoot(const std::string &input, bool isPath = true) { 
		if (isPath) {
			pcJsonStrContent = gnoixinl::readFile(input.c_str());
		} else {
			pcJsonStrContent = input;
		}

		if (pcJsonStrContent == "") return;
	};
	bool generateMDDoc();
	friend std::ostream& operator<<(std::ostream &os, JSONRoot& jsonRoot);

	virtual ~JSONRoot()
	{
		if (objJsonObject) delete objJsonObject;

		objJsonObject = nullptr;
	};
private:
	std::string pcJsonStrContent;
	JSONObject* objJsonObject = nullptr;
};

#endif // !JSON_ROOT_H_