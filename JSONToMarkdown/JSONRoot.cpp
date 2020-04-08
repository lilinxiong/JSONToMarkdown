#include "JSONRoot.h"

bool JSONRoot::generateMDDoc()
{
	if (pcJsonStrContent == "") {
		std::cout << "pcJsonStrContent is nullptr!" << std::endl;
		return false;
	}
	try {
		json root = json::parse(pcJsonStrContent.c_str());
		json_value_t type = root.type();
		if (type == json_value_t::object || type == json_value_t::array) {
			objJsonObject = new JSONObject(root);
			objJsonObject->parse();
		}	
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
}

std::ostream & operator<<(std::ostream & os, JSONRoot& jsonRoot)
{
	// TODO: 在此处插入 return 语句
	const JSONObject* obj = jsonRoot.objJsonObject;
	os << *obj << std::endl;
	return os;
}
