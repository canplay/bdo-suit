#include "stdafx.h"
#include "items.h"
#include "mssql.h"
#include "utility.h"

namespace api
{
	void Items::count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		std::string stmt = "SELECT COUNT(id) FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[items]";

		if ((*json)["filter"].asString() != "")
		{
			stmt = fmt::format("{} WHERE [name] LIKE N'%{}%'", stmt, utf8ToGBK((*json)["filter"].asString()));
		}

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
			spdlog::warn("items count error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Items::info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		std::string stmt = fmt::format("SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[items] WHERE id NOT IN(SELECT TOP {} id FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[items]", (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

		if ((*json)["filter"].asString() != "")
		{
			stmt = fmt::format("{} WHERE [name] LIKE N'%{}%'", stmt, utf8ToGBK((*json)["filter"].asString()));
		}

		if ((*json)["sortBy"].asString() != "")
		{
			stmt = fmt::format("{} ORDER BY [{}] DESC) AND [name] LIKE N'%{}%' ORDER BY [{}]", stmt, (*json)["sortBy"].asString(), utf8ToGBK((*json)["filter"].asString()), (*json)["sortBy"].asString());

			if ((*json)["descending"].asBool())
				stmt = fmt::format("{} DESC", stmt);
			else
				stmt = fmt::format("{} ASC", stmt);
		}
		else
			stmt = fmt::format("{})", stmt);

		Json::Value ret;

		try
		{
			Json::Value infos;

			auto r = MsSql::exec(stmt);

			while (r.next())
			{
				Json::Value info;
				info["id"] = r.get<std::string>("id", "");
				info["gameid"] = r.get<std::string>("gameid", "");
				info["name"] = gbkToUTF8(r.get<std::string>("name", ""));
				infos.append(info);
			}

			ret["msg"] = infos;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("items info error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}
}