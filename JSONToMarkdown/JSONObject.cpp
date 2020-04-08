#include "JSONObject.h"
#include "jsonUtil.h"

const bool sortIdx(const MarkDownTable& first ,const MarkDownTable& second)
{
	return first.idx < second.idx;
}

int JSONObject::parse()
{
	if (root.is_array())
		parseObject(*root.begin(), std::string("root"));
	else
		parseObject(root, std::string("root"));
	std::sort(vecMarkdownTable.begin(), vecMarkdownTable.end(), sortIdx);
	return 0;
}

int JSONObject::parseObject(const json & obj, const std::string &key)
{
	int tempIdx = mIdx;
	mIdx++;
	std::vector<JSONFiled> vecJsonFiled;
	for (auto iter = obj.begin(); iter != obj.end(); iter++) {
		std::string key = iter.key();
		std::string filedType = jsonUtil::transferStr(iter->type());
		if (iter->type() != json_value_t::string) {
			vecJsonFiled.push_back(JSONFiled(key, filedType, key));
		} else {
			vecJsonFiled.push_back(JSONFiled(key, filedType, iter.value().dump()));
		}
		if (iter->type() == json_value_t::object) {
			parseObject(iter.value(), key);
		} else if (iter->type() == json_value_t::array) {
			parseObject(*iter.value().begin(), key);
		}
	}
	vecMarkdownTable.push_back(MarkDownTable(tempIdx, key, JSONObjectStruct(vecJsonFiled)));
	return 0;
}

std::ostream & operator<<(std::ostream & os, const JSONObject & jsonObject)
{
	for (auto iter = jsonObject.vecMarkdownTable.begin(); iter != jsonObject.vecMarkdownTable.end(); iter++) {
		os << *iter << std::endl;
	}
	return os;
}
