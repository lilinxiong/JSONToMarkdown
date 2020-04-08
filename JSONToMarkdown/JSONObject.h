#pragma once
#ifndef GNOIX_INL_JSON_OBJECT_H_
#define GNOIX_INL_JSON_OBJECT_H_
#include "base.h"
#include "MDHeader.h"
#include "JSONFiled.h"
struct JSONObjectStruct
{
	MDHeader mdHeader;
	std::vector<JSONFiled> vecJsonFiled;
	JSONObjectStruct() { };
	JSONObjectStruct(std::vector<JSONFiled> mvecJsonFiled) :vecJsonFiled(mvecJsonFiled){ };
	friend std::ostream& operator<<(std::ostream &os, const JSONObjectStruct &jsonObjectStruct)
	{
		os << jsonObjectStruct.mdHeader << std::endl;
		const std::vector<JSONFiled>& mVec = jsonObjectStruct.vecJsonFiled;
		for (auto iter = mVec.begin(); iter != mVec.end(); iter++) {
			os << *iter << std::endl;
		}
		return os;
	}
};
struct MarkDownTable
{
	int idx = 0;
	std::string tableName;
	JSONObjectStruct jsonObject;
	MarkDownTable(int mIdx, const std::string &mTableName, const JSONObjectStruct& mJsonObjectStruct) :
		idx(mIdx), tableName(mTableName), jsonObject(mJsonObjectStruct)
	{ };

	friend std::ostream& operator<<(std::ostream & os, const MarkDownTable& markdownTable)
	{
		os << markdownTable.jsonObject << std::endl;
		return os;
	}
};
class JSONObject
{
public:
	JSONObject(const json &m_root):root(m_root){ };
	int parse();
	friend std::ostream& operator<<(std::ostream &os, const JSONObject &jsonObject);
private:
	int parseObject(const json &obj, const std::string &key);
private:
	int mIdx = 0;
	std::vector<MarkDownTable> vecMarkdownTable;
	json root;
};

#endif // !GNOIX_INL_JSON_OBJECT_H_