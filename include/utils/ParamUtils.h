#ifndef __PARMUTILS_H_
#define __PARMUTILS_H_
#include <map>
#include <list>
#include <vector>
#include <assert.h>
#include "NacosString.h"
#include "NacosExceptions.h"
#include "Constants.h"
#include "Debug.h"

class ParamUtils
{
public:
	static NacosString getNthElem(std::list<NacosString> &parm, size_t i)
	{
		assert(parm.size() > i);
		std::list<NacosString>::iterator it = parm.begin();
		for (size_t skipper = 0; skipper < i; skipper++)
		{
			it++;
		}

		return *it;
	}
	static NacosString trim(const NacosString &content)
	{
		int start = 0;
		int end = content.size() - 1;

		while (start < end && isBlank(content[start]))
		{
			start++;
		}

		while (start < end && isBlank(content[end]))
		{
			end--;
		}

		return NacosString(content.substr(start, end - start + 1));
	}

	static NacosString null2defaultGroup(const NacosString &group)
	{
		return (isNull(group)) ? Constants::DEFAULT_GROUP : ParamUtils::trim(group);
	}

	static void parseString2KeyGroupTenant(const NacosString &stringToParse, NacosString &dataId, NacosString &group, NacosString &tenant)
	{
		std::vector<NacosString> KGT;//KeyGroupTenant
		Explode(KGT, stringToParse, Constants::WORD_SEPARATOR);
		dataId = KGT[0];
		group = KGT[1];
		if (KGT.size() == 3)//with tenant
		{
			tenant = KGT[2];
		}
		else
		{
			tenant = NULLSTR;
		}
	}

	static bool isBlank(char character)
	{
		switch (character)
		{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				return true;
			default:
				return false;
		}
	}
	static bool isBlank(const NacosString &content)
	{
		//TODO:Apply ParamUtils.Java's logic to here, support whitespaces in other countries/zones
		if (content.size() == 0)
		{
			return true;
		}

		for (size_t i = 0; i < content.size(); i++)
		{
			if (!isBlank(content[i]))
				return false;
		}

		return true;
	};

	static bool isValid(const NacosString &content)
	{
		return false;
	};

	static void checkParam(const NacosString &dataId, const NacosString &group, const NacosString &content) throw (NacosException)
	{
		if (isBlank(content))
		{
            throw NacosException(NacosException::CLIENT_INVALID_PARAM, "content invalid");
		}
	};

	//A little trick here for NacosString constants
	static void Explode(std::vector<NacosString> &explodedList, const NacosString &stringToExplode, const NacosString &separator)
	{
		Explode(explodedList, stringToExplode, separator[0]);
	}

	static void Explode(std::vector<NacosString> &explodedList, const NacosString &stringToExplode, char separator)
	{
		size_t start_pos = 0;
		size_t cur_pos = 0;
		cur_pos = stringToExplode.find(separator, start_pos);

		//break the string with separator
		while (cur_pos != std::string::npos)
		{
			NacosString cur_addr = stringToExplode.substr(start_pos, cur_pos - start_pos);
			explodedList.push_back(cur_addr);
			start_pos = cur_pos + 1;
			cur_pos = stringToExplode.find(separator, start_pos);
		}

		//deal with the last string
		NacosString last_addr = stringToExplode.substr(start_pos);
		explodedList.push_back(last_addr);
	}

	static void Explode(std::list<NacosString> &explodedList, const NacosString &stringToExplode, char separator)
	{
		size_t start_pos = 0;
		size_t cur_pos = 0;
		cur_pos = stringToExplode.find(separator, start_pos);

		//break the string with separator
		while (cur_pos != std::string::npos)
		{
			NacosString cur_addr = stringToExplode.substr(start_pos, cur_pos - start_pos);
			explodedList.push_back(cur_addr);
			start_pos = cur_pos + 1;
			cur_pos = stringToExplode.find(separator, start_pos);
		}

		//deal with the last string
		NacosString last_addr = stringToExplode.substr(start_pos);
		explodedList.push_back(last_addr);
	}

	//use ',' as separator by default
	static NacosString Implode(std::list<NacosString> &toImplode)
	{
		return Implode(toImplode, ',');
	}

	static NacosString Implode(std::list<NacosString> &toImplode, char separator)
	{
		NacosString implodedString;
		for (std::list<NacosString>::iterator it = toImplode.begin(); it != toImplode.end(); /*it++ is within the for ... loop*/)
		{
			implodedString += *it;
			it++;
			if (it != toImplode.end())
			{
				implodedString += ",";
			}
		}

		return implodedString;
	}

	//use ',' as default separator to serialize a map
	static NacosString Implode(std::map<NacosString, NacosString> &toImplode)
	{
		return Implode(toImplode, ',');
	}

	static NacosString Implode(std::map<NacosString, NacosString> &toImplode, char separator)
	{
		NacosString implodedString;
		for (std::map<NacosString, NacosString>::iterator it = toImplode.begin(); it != toImplode.end(); it++)
		{
			implodedString += it->first + "=" + it->second;
			if (it != toImplode.end())
			{
				implodedString += ",";
			}
		}

		return implodedString;
	}

	static bool contains(const NacosString &haystack, char needle)
	{
		if (haystack.find(needle) != std::string::npos)
		{
			return true;
		}

		return false;
	}

	static bool contains(const NacosString &haystack, const NacosString &needle)
	{
		if (haystack.find(needle) != std::string::npos)
		{
			return true;
		}

		return false;
	}
};

#endif
