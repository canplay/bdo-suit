#include "stdafx.h"
#include "user.h"
#include "httplib.h"
#include "jwt.h"
#include "utility.h"
#include "mssql.h"

namespace api
{
	void User::count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		auto stmt = fmt::format("SELECT COUNT(_userNo) FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation]");

		Json::Value ret;

		try
		{
			Json::Value info;

			auto r = MsSql::exec(stmt);
			r.next();
			ret["msg"] = r.get<INT64>(0, 0);
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("user count error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::signup(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);
		auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

		auto stmt = fmt::format("SELECT * FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] WHERE _userId LIKE '{},%'", (*json)["username"].asString());

		Json::Value ret;

		try
		{
			Json::Value info;

			auto r = MsSql::exec(stmt);
			r.next();

			if (r.rows() != 0)
			{
				ret["msg"] = "username already exist";
				ret["status"] = 0;
				return callback(HttpResponse::newHttpJsonResponse(ret));
			}
			else
			{
				std::string nickname = fmt::format("Adventurer{}", uuidSimple().substr(5, 5));

				auto stmt1 = fmt::format("INSERT INTO [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] ([_registerDate], [_isValid], [_userId], [_userNickname], [_password], [_paymentPassword], [_authenticKey], [_authenticExpiration], [_webAuthenticKey], [_lastLoginTime], [_lastLogoutTime], [_totalPlayTime], [_lastIp], [_lastServerNo], [_lastWorldNo], [_serviceType], [_failPasswordCount], [_publisherCryptToken], [_membershipType], [_isAdmissionToSpeedServer], [_isPcRoom], [_expirationDate], [_isGuestAccount], [_speedServerExpiration], [_accountExpirationDate], [_surveyHWAndSW], [_isAccessBlanceChannel], [_isPremiumChannelPermission], [_isIgnoreCheckCustomizeOnly], [_preAuthenticKey], [_isAdultWorldUser], [_shutDownTime], [_atField], [_isCompleteTesterSubmit], [_isOtp], [_lastMacAddress], [_allCharacterTotalLevel], [_isAppliedNickNameChange]) VALUES('{}', '1', N'{},{}', N'{}', 0xF6ADBC1E77E92C0F1725B83DCFCFA139AD87EABCB262E1E3652B485F427961BDC57382A8A729331B7FF8A7A90FFBFE80ED43EF1060FE5F7716266DC48227AED0, 0xF6ADBC1E77E92C0F1725B83DCFCFA139AD87EABCB262E1E3652B485F427961BDC57382A8A729331B7FF8A7A90FFBFE80ED43EF1060FE5F7716266DC48227AED0, 0, '{}', 0, '{}', '{}', 0, '127.0.0.1', -110, 12, 12, 0, NULL, 0, 2, '1', NULL, '0', '2033-12-30 00:00:00.000', NULL, '', '1', '0', '0', 0, '1', NULL, '', '0', '0', 0xF728BB816503A79FCCBF76A7AA2F6AF0294BBCCAB8E3140ECAEEAED08E2329A2228983F442A858128C9EA83ED089A12DA0D7076FF52E2F313466E7D42A0130CC, 0, '0')", timestamp, (*json)["username"].asString(), (*json)["password"].asString(), nickname, timestamp, timestamp, timestamp);

				auto r1 = MsSql::exec(stmt1);

				if (r1.affected_rows() == 1)
				{
					auto stmt2 = fmt::format("SELECT * FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] WHERE _userId = '{},{}'", (*json)["username"].asString(), (*json)["password"].asString());
					auto r2 = MsSql::exec(stmt2);
					r2.next();

					if (r2.rows() != 0)
					{
						Json::Value info;

						info["registerDate"] = r2.get<std::string>("_registerDate", "");
						info["valid"] = r2.get<std::string>("_isValid", "");
						info["userNo"] = r2.get<INT64>("_userNo", 0);
						info["userId"] = r2.get<std::string>("_userId", "");
						info["userNickname"] = r2.get<std::string>("_userNickname", "");
						info["lastLoginTime"] = r2.get<std::string>("_lastLoginTime", "");
						info["lastLogoutTime"] = r2.get<std::string>("_lastLogoutTime", "");
						info["totalPlayTime"] = r2.get<INT64>("_totalPlayTime", 0);
						info["membershipType"] = r2.get<INT64>("_membershipType", 0);
						info["pcroom"] = r2.get<INT64>("_isPcRoom", 0);

						jwt jwtGenerated = jwt::generateToken(
							{
								{"id", picojson::value(info["userNo"].asString())},
							},
							(*json).isMember("remember") && (*json)["remember"].asBool());
						std::int64_t jwtExpiration = jwtGenerated.getExpiration();
						info["token"] = jwtGenerated.getToken();
						info["expiresIn"] =
							jwtExpiration -
							std::chrono::duration_cast<std::chrono::seconds>(
								std::chrono::system_clock::now().time_since_epoch())
							.count();
						info["expiresAt"] = jwtExpiration;

						ret["msg"] = info;
						ret["status"] = 1;
					}

					ret["msg"] = "signup error";
					ret["status"] = 0;
				}
				else
				{
					ret["msg"] = "signup error";
					ret["status"] = 0;
				}
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("signup error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::signin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto stmt = fmt::format("SELECT * FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] WHERE _userId = '{},{}'", (*json)["username"].asString(), (*json)["password"].asString());

		Json::Value ret;

		try
		{
			Json::Value info;

			auto r = MsSql::exec(stmt);
			r.next();

			if (r.rows() != 0)
			{
				info["registerDate"] = r.get<std::string>("_registerDate", "");
				info["valid"] = r.get<std::string>("_isValid", "");
				info["userNo"] = r.get<INT64>("_userNo", 0);
				info["userId"] = r.get<std::string>("_userId", "");
				info["userNickname"] = r.get<std::string>("_userNickname", "");
				info["lastLoginTime"] = r.get<std::string>("_lastLoginTime", "");
				info["lastLogoutTime"] = r.get<std::string>("_lastLogoutTime", "");
				info["totalPlayTime"] = r.get<INT64>("_totalPlayTime", 0);
				info["membershipType"] = r.get<INT64>("_membershipType", 0);
				info["pcroom"] = r.get<INT64>("_isPcRoom", 0);
				info["permission"] = getPermission(std::to_string(r.get<INT64>("_userNo", 0)));

				jwt jwtGenerated = jwt::generateToken(
					{
						{"id", picojson::value(info["userNo"].asString())},
					},
					(*json).isMember("remember") && (*json)["remember"].asBool());
				std::int64_t jwtExpiration = jwtGenerated.getExpiration();
				info["token"] = jwtGenerated.getToken();
				info["expiresIn"] =
					jwtExpiration -
					std::chrono::duration_cast<std::chrono::seconds>(
						std::chrono::system_clock::now().time_since_epoch())
					.count();
				info["expiresAt"] = jwtExpiration;

				ret["msg"] = info;
				ret["status"] = 1;
			}
			else
			{
				ret["msg"] = "username or password error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("signup error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::signout(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{}

	void User::info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		std::string stmt = fmt::format("SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] WHERE _userNo NOT IN(SELECT TOP {} _userNo FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation]", (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

		if ((*json)["sortBy"].asString() != "")
		{
			stmt = fmt::format("{} ORDER BY [_{}] DESC) ORDER BY [_{}]", stmt, (*json)["sortBy"].asString(), (*json)["sortBy"].asString());

			if ((*json)["descending"].asBool())
				stmt = fmt::format("{} DESC", stmt);
			else
				stmt = fmt::format("{} ASC", stmt);
		}
		else
			stmt = fmt::format("{})", stmt);
		spdlog::info(stmt);
		Json::Value ret;

		try
		{
			Json::Value infos;

			auto r = MsSql::exec(stmt);

			while (r.next())
			{
				Json::Value info;
				info["registerDate"] = r.get<std::string>("_registerDate", "");
				info["valid"] = r.get<std::string>("_isValid", "");
				info["userNo"] = r.get<INT64>("_userNo", 0);
				info["userId"] = r.get<std::string>("_userId", "");
				info["userNickname"] = r.get<std::string>("_userNickname", "");
				info["lastLoginTime"] = r.get<std::string>("_lastLoginTime", "");
				info["lastLogoutTime"] = r.get<std::string>("_lastLogoutTime", "");
				info["totalPlayTime"] = r.get<INT64>("_totalPlayTime", 0);
				info["membershipType"] = r.get<INT64>("_membershipType", 0);
				info["pcroom"] = r.get<INT64>("_isPcRoom", 0);
				info["permission"] = getPermission(std::to_string(r.get<INT64>("_userNo", 0)));

				Json::Value characters;
				auto stmt1 = fmt::format("SELECT * FROM [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblCharacterInformation] WHERE [_userNo] = {}", r.get<INT64>("_userNo", 0));
				auto r1 = MsSql::exec(stmt1);
				spdlog::info(stmt1);
				while (r1.next())
				{
					Json::Value character;
					character["deletedDate"] = r1.get<std::string>("_deletedDate", "");
					character["characterNo"] = r1.get<INT64>("_characterNo", 0);
					character["characterName"] = r1.get<std::string>("_characterName", "");
					character["classType"] = r1.get<INT64>("_classType", 0);
					character["totalPlayTime"] = r1.get<INT64>("_totalPlayTime", 0);
					character["currentPositionX"] = r1.get<INT64>("_currentPositionX", 0);
					character["currentPositionY"] = r1.get<INT64>("_currentPositionY", 0);
					character["currentPositionZ"] = r1.get<INT64>("_currentPositionZ", 0);
					character["returnPositionX"] = r1.get<INT64>("_returnPositionX", 0);
					character["returnPositionY"] = r1.get<INT64>("_returnPositionY", 0);
					character["returnPositionZ"] = r1.get<INT64>("_returnPositionZ", 0);
					character["level"] = r1.get<INT64>("_level", 0);
					character["experience"] = r1.get<INT64>("_experience", 0);
					character["variedWeight"] = r1.get<INT64>("_variedWeight", 0);
					character["skillPointLevel"] = r1.get<INT64>("_skillPointLevel", 0);
					character["skillPointExperience"] = r1.get<INT64>("_skillPointExperience", 0);
					character["remainedSkillPoint"] = r1.get<INT64>("_remainedSkillPoint", 0);
					character["aquiredSkillPoint"] = r1.get<INT64>("_aquiredSkillPoint", 0);
					character["tendency"] = r1.get<INT64>("_tendency", 0);
					character["hp"] = r1.get<INT64>("_hp", 0);
					character["mp"] = r1.get<INT64>("_mp", 0);
					character["sp"] = r1.get<INT64>("_sp", 0);
					character["wp"] = r1.get<INT64>("_wp", 0);
					character["inventorySlotCount"] = r1.get<INT64>("_inventorySlotCount", 0);
					character["titleKey"] = r1.get<INT64>("_titleKey", 0);
					character["killRewardCount"] = r1.get<INT64>("_killRewardCount", 0);
					character["enchantFailCount"] = r1.get<INT64>("_enchantFailCount", 0);
					character["offenceValue"] = r1.get<INT64>("_offenceValue", 0);
					character["defenceValue"] = r1.get<INT64>("_defenceValue", 0);
					character["awakenValue"] = r1.get<INT64>("_awakenValue", 0);
					characters.append(character);
				}

				info["characters"] = characters;
				infos.append(info);
			}

			ret["msg"] = infos;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("user info error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::infoOne(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string& id) const
	{
		auto stmt = fmt::format("SELECT * FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] WHERE _userNo = {}", id);

		Json::Value ret;

		try
		{
			Json::Value info;

			auto r = MsSql::exec(stmt);
			r.next();
			info["registerDate"] = r.get<std::string>("_registerDate", "");
			info["valid"] = r.get<std::string>("_isValid", "");
			info["userNo"] = r.get<INT64>("_userNo", 0);
			info["userId"] = r.get<std::string>("_userId", "");
			info["userNickname"] = r.get<std::string>("_userNickname", "");
			info["lastLoginTime"] = r.get<std::string>("_lastLoginTime", "");
			info["lastLogoutTime"] = r.get<std::string>("_lastLogoutTime", "");
			info["totalPlayTime"] = r.get<INT64>("_totalPlayTime", 0);
			info["membershipType"] = r.get<INT64>("_membershipType", 0);
			info["pcroom"] = r.get<INT64>("_isPcRoom", 0);
			info["permission"] = getPermission(std::to_string(r.get<INT64>("_userNo", 0)));

			Json::Value characters;
			auto stmt1 = fmt::format("SELECT * FROM [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblCharacterInformation] WHERE [_userNo] = {}", r.get<INT64>("_userNo", 0));
			auto r1 = MsSql::exec(stmt1);
			while (r1.next())
			{
				Json::Value character;
				character["deletedDate"] = r1.get<std::string>("_deletedDate", "");
				character["characterNo"] = r1.get<INT64>("_characterNo", 0);
				character["characterName"] = r1.get<std::string>("_characterName", "");
				character["classType"] = r1.get<INT64>("_classType", 0);
				character["totalPlayTime"] = r1.get<INT64>("_totalPlayTime", 0);
				character["currentPositionX"] = r1.get<INT64>("_currentPositionX", 0);
				character["currentPositionY"] = r1.get<INT64>("_currentPositionY", 0);
				character["currentPositionZ"] = r1.get<INT64>("_currentPositionZ", 0);
				character["returnPositionX"] = r1.get<INT64>("_returnPositionX", 0);
				character["returnPositionY"] = r1.get<INT64>("_returnPositionY", 0);
				character["returnPositionZ"] = r1.get<INT64>("_returnPositionZ", 0);
				character["level"] = r1.get<INT64>("_level", 0);
				character["experience"] = r1.get<INT64>("_experience", 0);
				character["variedWeight"] = r1.get<INT64>("_variedWeight", 0);
				character["skillPointLevel"] = r1.get<INT64>("_skillPointLevel", 0);
				character["skillPointExperience"] = r1.get<INT64>("_skillPointExperience", 0);
				character["remainedSkillPoint"] = r1.get<INT64>("_remainedSkillPoint", 0);
				character["aquiredSkillPoint"] = r1.get<INT64>("_aquiredSkillPoint", 0);
				character["tendency"] = r1.get<INT64>("_tendency", 0);
				character["hp"] = r1.get<INT64>("_hp", 0);
				character["mp"] = r1.get<INT64>("_mp", 0);
				character["sp"] = r1.get<INT64>("_sp", 0);
				character["wp"] = r1.get<INT64>("_wp", 0);
				character["inventorySlotCount"] = r1.get<INT64>("_inventorySlotCount", 0);
				character["titleKey"] = r1.get<INT64>("_titleKey", 0);
				character["killRewardCount"] = r1.get<INT64>("_killRewardCount", 0);
				character["enchantFailCount"] = r1.get<INT64>("_enchantFailCount", 0);
				character["offenceValue"] = r1.get<INT64>("_offenceValue", 0);
				character["defenceValue"] = r1.get<INT64>("_defenceValue", 0);
				character["awakenValue"] = r1.get<INT64>("_awakenValue", 0);
				characters.append(character);
			}
			info["characters"] = characters;

			ret["msg"] = info;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("user info one error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);
		auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

		auto stmt1 = fmt::format("UPDATE [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] SET [_isValid] = '{}', [_userId] = N'{},{}', [_userNickname] = N'{}', [_isPcRoom] = {}, [_membershipType] = {} WHERE [_userNo] = {}", (*json)["isValid"].asInt64(), (*json)["username"].asString(), (*json)["password"].asString(), (*json)["userNickname"].asString(), (*json)["pcroom"].asInt64(), (*json)["membershipType"].asInt64(), (*json)["userNo"].asInt64());

		auto stmt2 = fmt::format("UPDATE [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblBriefUserInformation] SET [_userId] = N'{},{}', [_userNickname] = N'{}' WHERE [_userNo] = {}", (*json)["username"].asString(), (*json)["password"].asString(), (*json)["userNickname"].asString(), (*json)["userNo"].asInt64());

		Json::Value ret;

		try
		{
			auto r1 = MsSql::exec(stmt1);

			if (r1.affected_rows() == 1)
			{
				auto r2 = MsSql::exec(stmt2);

				if (r2.affected_rows() == 1)
				{
					ret["msg"] = "ok";
					ret["status"] = 1;
				}
				else
				{
					ret["msg"] = "user update error";
					ret["status"] = 0;
				}
			}
			else
			{
				ret["msg"] = "user update error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("user update error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::characterUpdate(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);
		auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

		auto stmt = fmt::format("UPDATE [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblCharacterInformation] SET [_characterName] = N'{}', [_currentPositionX] = {}, [_currentPositionY] = {}, [_currentPositionZ] = {}, [_returnPositionX] = {}, [_returnPositionY] = {}, [_returnPositionZ] = {}, [_level] = {}, [_experience] = {}, [_skillPointLevel] = {}, [_skillPointExperience] = {}, [_remainedSkillPoint] = {}, [_aquiredSkillPoint] = {}, [_tendency] = {}, [_hp] = {}, [_mp] = {}, [_sp] = {}, [_wp] = {}, [_inventorySlotCount] = {}, [_titleKey] = {}, [_killRewardCount] = {}, [_enchantFailCount] = {}, [_offenceValue] = {}, [_defenceValue] = {}, [_awakenValue] = {}, [_variedWeight] = {} WHERE [_characterNo] = {}", (*json)["characterName"].asString(), (*json)["currentPositionX"].asInt64(), (*json)["currentPositionY"].asInt64(), (*json)["currentPositionZ"].asInt64(), (*json)["currentPositionX"].asInt64(), (*json)["currentPositionY"].asInt64(), (*json)["currentPositionZ"].asInt64(), (*json)["level"].asInt64(), (*json)["experience"].asInt64(), (*json)["skillPointLevel"].asInt64(), (*json)["skillPointExperience"].asInt64(), (*json)["remainedSkillPoint"].asInt64(), (*json)["aquiredSkillPoint"].asInt64(), (*json)["tendency"].asInt64(), (*json)["hp"].asInt64(), (*json)["mp"].asInt64(), (*json)["sp"].asInt64(), (*json)["wp"].asInt64(), (*json)["inventorySlotCount"].asInt64(), (*json)["titleKey"].asInt64(), (*json)["killRewardCount"].asInt64(), (*json)["enchantFailCount"].asInt64(), (*json)["offenceValue"].asInt64(), (*json)["defenceValue"].asInt64(), (*json)["awakenValue"].asInt64(), (*json)["variedWeight"].asInt64(), (*json)["characterNo"].asInt64());

		Json::Value ret;

		try
		{
			auto r = MsSql::exec(stmt);

			if (r.affected_rows() == 1)
			{
				if (!(*json)["deletedDate"].asString().empty())
				{
					stmt = fmt::format("UPDATE [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblCharacterInformation] SET [_deletedDate] = '{}'", timestamp);
					r = MsSql::exec(stmt);
				}

				ret["msg"] = "ok";
				ret["status"] = 1;
			}
			else
			{
				ret["msg"] = "character update error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("character update error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::mail(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);
		auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

		std::string expirationDate_col = "[_expirationDate], ";
		std::string expirationDate_val = (*json)["expirationDate"].asString();
		if ((*json)["expirationDate"].asString() == "")
		{
			expirationDate_col = expirationDate_val = "";
		}

		auto stmt = fmt::format("INSERT INTO [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblMail] ({}[_registerDate], [_senderName], [_senderUserNo], [_receiverName], [_receiverUserNo], [_title], [_contents], [_mailType], [_variousNo], [_enchantLevel], [_itemCount], [_webItemType], [_chargeNo]) VALUES ('{}', N'{}', {}, N'{}', {}, N'{}', N'{}', {}, {}, {}, {}, {}, ''", expirationDate_col, timestamp, (*json)["senderName"].asString() == "" ? "GM" : (*json)["senderName"].asString(), (*json)["senderUserNo"].asInt64(), (*json)["receiverName"].asString(), (*json)["receiverUserNo"].asInt64(), (*json)["title"].asString(), (*json)["contents"].asString(), (*json)["mailType"].asInt64(), (*json)["variousNo"].asInt64(), (*json)["enchantLevel"].asInt64(), (*json)["itemCount"].asInt64(), (*json)["webItemType"].asInt64());

		if ((*json)["expirationDate"].asString() != "")
		{
			stmt = fmt::format("{}, '{}'", stmt, expirationDate_val);
		}

		stmt = fmt::format("{})", stmt);

		spdlog::info(stmt);

		Json::Value ret;

		try
		{
			auto r = MsSql::exec(stmt);

			if (r.affected_rows() == 1)
			{
				ret["msg"] = "ok";
				ret["status"] = 1;
			}
			else
			{
				ret["msg"] = "mail add error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("mail add error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::admin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string& type, const std::string& method) const
	{
		Json::Value ret;

		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);
		auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

		std::string stmt = "";

		if (method == "count")
		{
			if (type == "ingame")
			{
				stmt = fmt::format("SELECT COUNT(_UserNo) FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember]");
			}
			else
			{
				stmt = fmt::format("SELECT COUNT(id) FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[admin]");
			}

			try
			{
				auto r = MsSql::exec(stmt);
				r.next();

				if (r.rows() > 0)
				{
					ret["msg"] = r.get<INT64>(0, 0);
					ret["status"] = 1;
				}
				else
				{
					ret["msg"] = "admin get count error";
					ret["status"] = 0;
				}
			}
			catch (const std::exception& e)
			{
				spdlog::warn("admin get count error: {}", e.what());
				ret["msg"] = e.what();
				ret["status"] = 0;
			}

			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		switch (req->getMethod())
		{
			case Get:
				if (type == "ingame")
				{
					stmt = fmt::format("SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember] WHERE _UserNo NOT IN(SELECT TOP {} _UserNo FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember]", (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

					if ((*json)["sortBy"].asString() != "")
					{
						stmt = fmt::format("{} ORDER BY [{}] DESC) ORDER BY [{}]", stmt, (*json)["sortBy"].asString(), (*json)["sortBy"].asString());

						if ((*json)["descending"].asBool())
							stmt = fmt::format("{} DESC", stmt);
						else
							stmt = fmt::format("{} ASC", stmt);
					}
					else
						stmt = fmt::format("{})", stmt);

					try
					{
						auto r = MsSql::exec(stmt);
						r.next();

						if (r.rows() > 0)
						{
							Json::Value infos;

							while (r.next())
							{
								Json::Value info;
								info["UserNo"] = r.get<std::string>("_UserNo", "");
								info["roleGroupNo"] = r.get<std::string>("_roleGroupNo", "");
								info["macAddress"] = r.get<std::string>("_macAddress", "");
								info["IpAddress"] = r.get<std::string>("_IpAddress", "");
								info["password"] = r.get<std::string>("_password", "");
								infos.append(info);
							}

							ret["msg"] = infos;
							ret["status"] = 1;
						}
						else
						{
							ret["msg"] = "admin get error";
							ret["status"] = 0;
						}
					}
					catch (const std::exception& e)
					{
						spdlog::warn("admin get error: {}", e.what());
						ret["msg"] = e.what();
						ret["status"] = 0;
					}
				}
				else
				{
					stmt = fmt::format("SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[admin] WHERE id NOT IN(SELECT TOP {} id FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[admin]", (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

					if ((*json)["sortBy"].asString() != "")
					{
						stmt = fmt::format("{} ORDER BY [{}] DESC) ORDER BY [{}]", stmt, (*json)["sortBy"].asString(), (*json)["sortBy"].asString());

						if ((*json)["descending"].asBool())
							stmt = fmt::format("{} DESC", stmt);
						else
							stmt = fmt::format("{} ASC", stmt);
					}
					else
						stmt = fmt::format("{})", stmt);

					try
					{
						auto r = MsSql::exec(stmt);
						r.next();

						if (r.rows() > 0)
						{
							Json::Value infos;

							while (r.next())
							{
								Json::Value info;
								info["id"] = r.get<std::string>("id", "");
								info["userno"] = r.get<std::string>("userNo", "");
								info["familyname"] = r.get<std::string>("userNickname", "");
								info["create_date"] = r.get<std::string>("create_date", "");
								info["create_user"] = r.get<std::string>("create_user", "");
								info["update_date"] = r.get<std::string>("update_date", "");
								info["update_user"] = r.get<std::string>("update_user", "");
								infos.append(info);
							}

							ret["msg"] = infos;
							ret["status"] = 1;
						}
						else
						{
							ret["msg"] = "admin get error";
							ret["status"] = 0;
						}
					}
					catch (const std::exception& e)
					{
						spdlog::warn("admin get error: {}", e.what());
						ret["msg"] = e.what();
						ret["status"] = 0;
					}
				}
				break;
			case Put:
				if (type == "ingame")
				{
					stmt = fmt::format("INSERT INTO [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember] ([_UserNo], [_roleGroupNo], [_macAddress], [_IpAddress], [_password]) VALUES('{}', '1', '{}', '127.0.0.1', '{}')", (*json)["userNo"].asInt64(), (*json)["mac"].asString(), (*json)["password"].asString());
				}
				else
				{
					stmt = fmt::format("INSERT INTO [SA_BETA_WORLDDB_0002].[PaWebPublic].[account] ([id], [userNo], [create_date], [create_user], [update_date], [update_user] VALUES('{}', {}, '{}', '{}', '{}', '{}')", uuidSimple(), (*json)["userNo"].asInt64(), timestamp, (*json)["create_user"].asString(), timestamp, (*json)["update_user"].asString());
				}

				try
				{
					auto r = MsSql::exec(stmt);

					if (r.affected_rows() == 1)
					{
						ret["msg"] = "ok";
						ret["status"] = 1;
					}
					else
					{
						ret["msg"] = "admin put error";
						ret["status"] = 0;
					}
				}
				catch (const std::exception& e)
				{
					spdlog::warn("admin put error: {}", e.what());
					ret["msg"] = e.what();
					ret["status"] = 0;
				}
				break;
			case Post:
				break;
			case Delete:
				if (type == "ingame")
				{
					stmt = fmt::format("DELETE FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember] WHERE _UserNo = '{}'", (*json)["userNo"].asInt64());
				}
				else
				{
					stmt = fmt::format("DELETE FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[admin] WHERE id = '{}'", (*json)["id"].asString());
				}

				try
				{
					auto r = MsSql::exec(stmt);

					if (r.affected_rows() == 1)
					{
						ret["msg"] = "ok";
						ret["status"] = 1;
					}
					else
					{
						ret["msg"] = "admin put error";
						ret["status"] = 0;
					}
				}
				catch (const std::exception& e)
				{
					spdlog::warn("admin put error: {}", e.what());
					ret["msg"] = e.what();
					ret["status"] = 0;
				}
				break;
			default:
				ret["msg"] = "admin set no method";
				ret["status"] = 0;
				break;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::blockChat(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);
		auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

		auto stmt = fmt::format("INSERT INTO [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblBlockedChat] ([_registerDate], [_userNo], [_endDate], [_reasonMemo]) VALUES ('{}', {}, '{}', N'{}')", timestamp, (*json)["userNo"].asInt64(), (*json)["endDate"].asString(), (*json)["reasonMemo"].asString());

		Json::Value ret;

		try
		{
			auto r = MsSql::exec(stmt);

			if (r.affected_rows() == 1)
			{
				ret["msg"] = "ok";
				ret["status"] = 1;
			}
			else
			{
				ret["msg"] = "block chat add error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("block chat add error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::blockUser(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);
		auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

		auto stmt = fmt::format("INSERT INTO [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblBlockedUser] ([_registerDate], [_userNo], [_endDate], [_blockCode], [_operationMemo]) VALUES ('{}', {}, '{}', {}, N'{}')", timestamp, (*json)["userNo"].asInt64(), (*json)["endDate"].asString(), (*json)["blockCode"].asInt64(), (*json)["reasonMemo"].asString());

		Json::Value ret;

		try
		{
			auto r = MsSql::exec(stmt);

			if (r.affected_rows() == 1)
			{
				ret["msg"] = "ok";
				ret["status"] = 1;
			}
			else
			{
				ret["msg"] = "block user add error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("block user add error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void User::blockIp(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		auto now = std::chrono::system_clock::now();
		time_t time = std::chrono::system_clock::to_time_t(now);
		auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

		auto stmt = fmt::format("INSERT INTO [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblBlockedIP] ([_registerDate], [_startIP], [_bigintStartIP], [_endIP], [_bigintEndIP], [_blockCode], [_operationMemo]) VALUES ('{}', N'{}', {}, N'{}', {}, {}, N'{}')", timestamp, (*json)["startIP"].asString(), (*json)["intStartIP"].asInt64(), (*json)["endIP"].asString(), (*json)["intEndIP"].asInt64(), (*json)["blockCode"].asInt64(), (*json)["operationMemo"].asString());

		Json::Value ret;

		try
		{
			auto r = MsSql::exec(stmt);

			if (r.affected_rows() == 1)
			{
				ret["msg"] = "ok";
				ret["status"] = 1;
			}
			else
			{
				ret["msg"] = "block ip add error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("block ip add error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	Json::Value User::getPermission(const std::string userNo) const
	{
		auto stmt = fmt::format("SELECT * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[account] WHERE userNo = '{}'", userNo);

		Json::Value ret = {};

		try
		{
			auto r = MsSql::exec(stmt);
			r.next();

			if (r.rows() > 0)
			{
				stmt = fmt::format("SELECT * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[permissions] WHERE id = '{}'", r.get<std::string>("permission", ""));

				r = MsSql::exec(stmt);
				r.next();

				if (r.rows() > 0)
				{
					auto permission = r.get<std::string>("value", "{}");

					Json::Reader reader;
					reader.parse(permission, ret);
				}
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("get permission error: {}", e.what());
		}

		return ret;
	}
}