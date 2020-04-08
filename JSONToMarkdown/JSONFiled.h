#pragma once
#ifndef GNOIX_INL_JSON_FILED_H_
#define GNOIX_INL_JSON_FILED_H_
#include "base.h"

class JSONFiled
{
public:
	JSONFiled() { };
	JSONFiled(const std::string &m_filedName, const std::string &m_filedType, const std::string &m_filedExpain) :
		filedName(m_filedName), filedType(m_filedType), filedExplain(m_filedExpain)
	{ };

	std::string getFiledName() const { return this->filedName; }
	std::string getFiledType() const { return this->filedType; }
	std::string getFiledExplain() const { return this->filedExplain; }

	void setFiledName(const std::string &m_filedName) { this->filedName = m_filedName; }
	void setFiledType(const std::string & m_filedType) { this->filedType = m_filedType; }
	void setFiledExplain(const std::string &m_filedExplain) { this->filedExplain = m_filedExplain; }

	friend std::ostream& operator<<(std::ostream &os, const JSONFiled &jsonFiled);
	virtual ~JSONFiled() { };


private:
	std::string filedName;
	std::string filedType;
	std::string filedExplain;
};

#endif // !GNOIX_INL_JSON_FILED_H_