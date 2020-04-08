#include "JSONObject.h"
#include "jsonUtil.h"

int JSONObject::parse()
{
	if (root.is_array())
		parseObject(*root.begin(), std::string("root"));
	else
		parseObject(root, std::string("root"));
	return 0;
}

int JSONObject::parseObject(const json & obj, const std::string &key)
{
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
	mapsJsonObjectStruct.insert(make_pair(key, JSONObjectStruct(vecJsonFiled)));
	return 0;
}

std::ostream & operator<<(std::ostream & os, const JSONObject & jsonObject)
{
	for (auto iter = jsonObject.mapsJsonObjectStruct.begin(); iter != jsonObject.mapsJsonObjectStruct.end(); iter++) {
		os << iter->second << std::endl;
	}
	return os;
}
