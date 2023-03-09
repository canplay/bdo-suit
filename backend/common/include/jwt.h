#pragma once

#include <jwt-cpp/jwt.h>

namespace api
{
	class jwt
	{
	public:
		jwt(const std::string& token, const std::int64_t& expiration);

		std::string getToken() const;
		std::int64_t getExpiration() const;

		static jwt generateToken(const std::map<std::string, ::jwt::traits::kazuho_picojson::value_type>& claims = {}, const bool& extension = false);
		static std::map<std::string, std::any> decodeToken(const std::string& encodedToken);

	private:
		std::string token;
		std::int64_t expiration;

		static bool verifyToken(const ::jwt::decoded_jwt<::jwt::traits::kazuho_picojson>& jwt);
		static void addClaimToAttributes(std::map<std::string, std::any>& attributes, const std::pair<std::string, ::jwt::basic_claim<::jwt::traits::kazuho_picojson>>& claim);
	};
}
