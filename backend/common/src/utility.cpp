#include "utility.h"
#include "stdafx.h"
#include "uuid.h"
#include "node_blf.h"
#include "openbsd.h"
//#include <openssl\md5.h>
//#include <openssl\sha.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp\md5.h>
#include <cryptopp\sha.h>
#include <cryptopp\hex.h>
#include <cryptopp\base64.h>

#include <crashpad\base\files\file_path.h>
#include <crashpad\client\crash_report_database.h>
#include <crashpad\client\settings.h>
#include <crashpad\client\crashpad_client.h>

namespace api
{
	std::string wideToMulit(const wchar_t* strWide)
	{
		int nLen =
			WideCharToMultiByte(CP_ACP, 0, strWide, -1, nullptr, 0, nullptr, nullptr);

		if (nLen <= 0)
			return std::string("");

		auto pszDst = new char[nLen];
		if (pszDst == nullptr)
			return std::string("");

		WideCharToMultiByte(CP_ACP, 0, strWide, -1, pszDst, nLen, nullptr, nullptr);
		pszDst[nLen - 1] = 0;

		std::string strTemp(pszDst);
		delete[] pszDst;
		pszDst = nullptr;
		return strTemp;
	}

	std::wstring mulitToWide(const char* strMulite)
	{
		int nSize =
			MultiByteToWideChar(CP_ACP, 0, strMulite, strlen(strMulite), 0, 0);
		if (nSize <= 0)
			return L"";

		auto pwszDst = new wchar_t[nSize + 1];
		if (pwszDst == nullptr)
			return L"";

		MultiByteToWideChar(CP_ACP, 0, strMulite, strlen(strMulite), pwszDst, nSize);
		pwszDst[nSize] = 0;

		if (pwszDst[0] == 0xFEFF)
			for (int i = 0; i < nSize; i++)
				pwszDst[i] = pwszDst[i + 1];

		std::wstring wcharString(pwszDst);
		delete[] pwszDst;
		pwszDst = nullptr;
		return wcharString;
	}

	std::string utf8ToGBK(const std::string& strUTF8)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, nullptr, 0);
		auto str1 = new wchar_t[len + 1];
		memset(str1, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, str1, len);

		len = WideCharToMultiByte(CP_ACP, 0, str1, -1, nullptr, 0, nullptr, nullptr);
		auto str2 = new char[len + 1];
		memset(str2, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, str1, -1, str2, len, nullptr, nullptr);

		std::string strTemp(str2);
		delete[] str2;
		str2 = nullptr;
		delete[] str1;
		str1 = nullptr;
		return strTemp;
	}

	std::string gbkToUTF8(const std::string& strGBK)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, nullptr, 0);
		auto str1 = new wchar_t[len + 1];
		memset(str1, 0, len * 2 + 2);
		MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, len);

		len = WideCharToMultiByte(CP_UTF8, 0, str1, -1, nullptr, 0, nullptr, nullptr);
		auto str2 = new char[len + 1];
		memset(str2, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, len, nullptr, nullptr);

		std::string strTemp(str2);
		delete[] str1;
		str1 = nullptr;
		delete[] str2;
		str2 = nullptr;
		return strTemp;
	}

	std::string stringToHexString(const std::string& strSrc)
	{
		std::string ret;
		const char* hex = "0123456789ABCDEF";
		for (int i = 0; i != strSrc.size(); ++i)
		{
			ret.push_back(hex[(strSrc[i] >> 4) & 0xf]);
			ret.push_back(hex[strSrc[i] & 0xf]);
		}
		return ret;
	}

	unsigned char charToHex(char ch)
	{
		static const char* hex = "0123456789ABCDEF";
		for (unsigned char i = 0; i != 16; ++i)
			if (ch == hex[i])
				return i;
		return 0;
	}

	std::string hexStringToString(const std::string& strSrc)
	{
		int i = 0;
		int cnt = 0;
		const char* tmp = strSrc.c_str();
		auto d = new unsigned char[strSrc.size()];
		memset(d, 0, strSrc.length());

		while (*tmp)
		{
			if (i & 1)
			{
				d[cnt++] |= charToHex(*tmp);
			}
			else
			{
				d[cnt] = charToHex(*tmp) << 4;
			}
			tmp++;
			i++;
		}

		std::string ret = (char*)d;
		delete[] d;
		d = nullptr;
		return ret;
	}

	std::string jsonToString(const Json::Value& json)
	{
		static Json::Value def = []()
		{
			Json::Value def;
			Json::StreamWriterBuilder::setDefaults(&def);
			def["emitUTF8"] = true;
			return def;
		}();

		std::ostringstream stream;
		Json::StreamWriterBuilder stream_builder;
		stream_builder.settings_ = def;
		std::unique_ptr<Json::StreamWriter> writer(stream_builder.newStreamWriter());
		writer->write(json, &stream);
		return stream.str();
	}

	std::string uuid()
	{
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size>{};
		std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 generator(seq);
		uuids::uuid_random_generator gen{ generator };
		return uuids::to_string(gen());
	}

	std::string uuidSimple()
	{
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size>{};
		std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 generator(seq);
		uuids::uuid_random_generator gen{ generator };
		auto str = uuids::to_string(gen());
		std::string::size_type pos = 0;
		while ((pos = str.find("-")) != std::string::npos)
		{
			str.replace(pos, 1, "");
		}
		return str;
	}

	std::string sha256(const std::string str)
	{
		CryptoPP::SHA256 hash;
		byte digest[CryptoPP::SHA256::DIGESTSIZE];
		hash.CalculateDigest(digest, (byte*)str.c_str(), str.length());
		CryptoPP::HexEncoder encoder;
		std::string sign;
		encoder.Attach(new CryptoPP::StringSink(sign));
		encoder.Put(digest, sizeof(digest));
		encoder.MessageEnd();
		//std::string sign;
		//unsigned char szTmp[SHA256_DIGEST_LENGTH];
		//SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.length(),
		//	   szTmp);
		//for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i)
		//{
		//	char szBuf[SHA256_DIGEST_LENGTH] = { 0 };
		//	sprintf(szBuf, "%02x", szTmp[i]);
		//	sign += szBuf;
		//}

		return sign;
	}

	std::string md5(const std::string str)
	{
		CryptoPP::Weak::MD5 hash;
		byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];
		hash.CalculateDigest(digest, (byte*)str.c_str(), str.length());
		CryptoPP::HexEncoder encoder;
		std::string sign;
		encoder.Attach(new CryptoPP::StringSink(sign));
		encoder.Put(digest, sizeof(digest));
		encoder.MessageEnd();
		//std::string sign;
		//unsigned char szTmp[MD5_DIGEST_LENGTH];
		//MD5(reinterpret_cast<const unsigned char*>(str.c_str()), str.length(),
		//	szTmp);
		//for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i)
		//{
		//	char szBuf[MD5_DIGEST_LENGTH] = { 0 };
		//	sprintf(szBuf, "%02x", szTmp[i]);
		//	sign += szBuf;
		//}

		return sign;
	}

	std::string base64Encode(const std::string str)
	{
		CryptoPP::Base64Decoder decoder;
		decoder.PutMessageEnd((const byte*)str.data(), str.size());
		if (!decoder.AnyRetrievable())
		{
			return "";
		}

		auto neededLength = decoder.MaxRetrievable();
		char* buffer = new char[neededLength];
		decoder.Get((byte*)buffer, neededLength);

		std::ostringstream oss;
		oss << buffer;

		delete[] buffer;
		return oss.str();
	}

	std::string base64Decode(const std::string str)
	{
		CryptoPP::Base64Decoder decoder;
		decoder.PutMessageEnd((const byte*)str.data(), str.size());
		if (!decoder.AnyRetrievable())
		{
			return "";
		}

		auto neededLength = decoder.MaxRetrievable();
		char* buffer = new char[neededLength];
		decoder.Get((byte*)buffer, neededLength);

		std::ostringstream oss;
		for (size_t i = 0; i < neededLength; i++)
		{
			oss << buffer[i];
		}

		delete[] buffer;
		return oss.str();
	}

	std::string bcryptGenerate(const std::string& password, unsigned int rounds)
	{
		char salt[_SALT_LEN];

		unsigned char seed[17]{};
		arc4random_buf(seed, 16);

		bcrypt_gensalt('b', rounds, seed, salt);

		std::string hash(61, '\0');
		node_bcrypt(password.c_str(), password.size(), salt, &hash[0]);
		hash.resize(60);
		return hash;
	}

	bool bcryptValidate(const std::string& password, const std::string& hash)
	{
		std::string got(61, '\0');
		node_bcrypt(password.c_str(), password.size(), hash.c_str(), &got[0]);
		got.resize(60);
		return hash == got;
	}

	bool initCrashPad()
	{
		if (!std::filesystem::exists("./crashpad_handler.exe"))
			return false;

		using namespace crashpad;

		std::map<std::string, std::string> annotations;
		annotations["format"] = "minidump";

		std::vector<std::string> arguments;
		arguments.push_back("--no-rate-limit");

		base::FilePath db(L"./crash");
		base::FilePath handler(L"./crashpad_handler.exe");
		base::FilePath attachment(L"./logs/log.log");

		std::unique_ptr<CrashReportDatabase> database =
			crashpad::CrashReportDatabase::Initialize(db);

		if (database == nullptr || database->GetSettings() == NULL)
		{
			spdlog::info("CrashReportDatabase Init Error");
			return false;
		}

		database->GetSettings()->SetUploadsEnabled(false);

		std::string url("http://127.0.0.1:8000");

		return CrashpadClient{}.StartHandler(handler, db, db, url, annotations, arguments, true, false, { attachment });
	}
}