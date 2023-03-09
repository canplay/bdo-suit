#include "stdafx.h"
#include "slide.h"
#include "mssql.h"
#include "utility.h"

namespace api
{
	void Slide::count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		auto stmt = fmt::format("SELECT COUNT(id) FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide]");

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
			spdlog::warn("slide count error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Slide::add(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
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

		auto stmt = fmt::format("INSERT INTO [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide] ([id], [title], [desc], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [img]) VALUES ('{}', N'{}', N'{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}');", uuidSimple(), (*json)["title"].asString(), (*json)["desc"].asString(), (*json)["link"].asString(), timestamp, (*json)["create_user"].asString(), (*json)["create_id"].asString(), timestamp, (*json)["update_user"].asString(), (*json)["update_id"].asString(), (*json)["img"].asString());

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
				ret["msg"] = "slide add error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("slide add error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Slide::update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
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

		auto stmt = fmt::format("UPDATE [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide] SET [title] = N'{}', [desc] = N'{}', [link] = '{}', [update_date] = '{}', [update_user] = '{}', [update_id] = '{}', [img] = '{}' WHERE [id] = '{}';", utf8ToGBK((*json)["title"].asString()), utf8ToGBK((*json)["desc"].asString()), (*json)["link"].asString(), timestamp, (*json)["update_user"].asString(), (*json)["update_id"].asString(), (*json)["img"].asString(), (*json)["id"].asString());

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
				ret["msg"] = "slide update error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("slide update error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Slide::info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		std::string stmt = fmt::format("SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide] WHERE id NOT IN(SELECT TOP {} id FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide]", (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

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

		Json::Value ret;

		try
		{
			Json::Value infos;

			auto r = MsSql::exec(stmt);

			while (r.next())
			{
				Json::Value info;
				info["id"] = r.get<std::string>("id", "");
				info["title"] = gbkToUTF8(r.get<std::string>("title", ""));
				info["desc"] = gbkToUTF8(r.get<std::string>("desc", ""));
				info["link"] = r.get<std::string>("link", "");
				info["create_date"] = r.get<std::string>("create_date", "");
				info["create_user"] = r.get<std::string>("create_user", "");
				info["create_id"] = r.get<std::string>("create_id", "");
				info["update_date"] = r.get<std::string>("update_date", "");
				info["update_user"] = r.get<std::string>("update_user", "");
				info["update_id"] = r.get<std::string>("update_id", "");
				info["img"] = r.get<std::string>("img", "");
				infos.append(info);
			}

			ret["msg"] = infos;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("slide info error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}
}