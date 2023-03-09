#include "stdafx.h"
#include "status.h"
#include "mssql.h"

namespace api
{
	void Status::system(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		Json::Value ret;

		std::ifstream ifs("config.json", std::ios::binary);
		if (ifs.is_open())
		{
			Json::Reader reader;
			Json::Value info;

			if (reader.parse(ifs, info))
			{
				ifs.close();
				ret["msg"] = info;
				ret["status"] = 1;
				return callback(HttpResponse::newHttpJsonResponse(ret));
			}
		}

		ifs.close();
		ret["msg"] = "status system error";
		ret["status"] = 0;
		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Status::overview(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		auto stmt1 = fmt::format("SELECT COUNT (_userNo) FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation]");
		auto stmt2 = fmt::format("SELECT COUNT (_characterNo) FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblBriefCharacterInformation] WHERE [_deletedDate] IS NULL");
		auto stmt3 = fmt::format("SELECT COUNT (_guildNo) FROM [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblGuild]");
		auto stmt4 = fmt::format("SELECT TOP 10 * FROM [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblCurrentInformation] ORDER BY [_updatedDate] DESC");
		auto stmt5 = fmt::format("SELECT COUNT (_mainKey) FROM [SA_BETA_TRADEDB_0002].[PaGamePrivate].[TblWorldMarket]");

		Json::Value ret;

		try
		{
			Json::Value info;

			auto r1 = MsSql::exec(stmt1);
			r1.next();
			info["account"] = r1.get<INT64>(0, 0);

			auto r2 = MsSql::exec(stmt2);
			r2.next();
			info["character"] = r2.get<INT64>(0, 0);

			auto r3 = MsSql::exec(stmt3);
			r3.next();
			info["guild"] = r3.get<INT64>(0, 0);

			auto r4 = MsSql::exec(stmt4);
			Json::Value online;
			while (r4.next())
			{
				Json::Value server;
				server["date"] = r4.get<std::string>("_updatedDate", "");
				server["world"] = r4.get<INT64>("_worldNo", 0);
				server["server"] = r4.get<INT64>("_serverNo", 0);
				server["loginning"] = r4.get<INT64>("_loginningUserCount", 0);
				server["playing"] = r4.get<INT64>("_playingUserCount", 0);
				online.append(server);
			}
			info["online"] = online;

			auto r5 = MsSql::exec(stmt5);
			r5.next();
			info["market"] = r5.get<INT64>(0, 0);

			ret["msg"] = info;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("status overview error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}
}