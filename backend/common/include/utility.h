#pragma once

#include <string>
#include <json\value.h>

namespace api
{
	std::string wideToMulit(const wchar_t* strWide);
	std::wstring mulitToWide(const char* strMulite);

	std::string utf8ToGBK(const std::string& strUTF8);
	std::string gbkToUTF8(const std::string& strGBK);

	std::string stringToHexString(const std::string& strSrc);
	std::string hexStringToString(const std::string& strSrc);

	std::string jsonToString(const Json::Value& json);

	std::string uuid();
	std::string uuidSimple();
	std::string sha256(const std::string str);
	std::string md5(const std::string str);

	std::string base64Encode(const std::string str);
	std::string base64Decode(const std::string str);

	std::string bcryptGenerate(const std::string& password, unsigned int rounds = 10);
	bool bcryptValidate(const std::string& password, const std::string& hash);

	bool initCrashPad();
};
