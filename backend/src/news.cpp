#include "stdafx.h"
#include "news.h"
#include "mssql.h"
#include "utility.h"

namespace api
{
	void News::count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		auto stmt = fmt::format("SELECT COUNT(id) FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[news]");

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
			spdlog::warn("news count error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void News::add(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
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

		auto stmt = fmt::format("INSERT INTO [SA_BETA_WORLDDB_0002].[PaWebPublic].[news] ([id], [title], [date], [author], [content], [create_date], [create_user], [update_date], [update_user], [delete]) VALUES ('{}', N'{}', '{}', N'{}', N'{}', '{}', '{}', '{}', '{}', 0);", uuidSimple(), utf8ToGBK((*json)["title"].asString()), (*json)["date"].asString(), utf8ToGBK((*json)["author"].asString()), (*json)["content"].asString(), timestamp, (*json)["create_user"].asString(), timestamp, (*json)["update_user"].asString());

		Json::Value ret;

		try
		{
			auto r = MsSql::exec(stmt);

			if (r.affected_rows() >= 1)
			{
				ret["msg"] = "ok";
				ret["status"] = 1;
			}
			else
			{
				ret["msg"] = "news add error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("news add error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void News::update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
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

		auto stmt = fmt::format("UPDATE [SA_BETA_WORLDDB_0002].[PaWebPublic].[news] SET [title] = N'{}', [date] = '{}', [author] = N'{}', [content] = N'{}', [update_date] = '{}', [update_user] = '{}', [delete] = {} WHERE [id] = '{}'", utf8ToGBK((*json)["title"].asString()), (*json)["date"].asString(), utf8ToGBK((*json)["author"].asString()), (*json)["content"].asString(), timestamp, (*json)["update_user"].asString(), (*json)["delete"].asInt64(), (*json)["id"].asString());

		spdlog::info(stmt);

		Json::Value ret;

		try
		{
			auto r = MsSql::exec(stmt);

			if (r.affected_rows() >= 1)
			{
				ret["msg"] = "ok";
				ret["status"] = 1;
			}
			else
			{
				ret["msg"] = "news update error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("news update error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void News::info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		std::string stmt = fmt::format("SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[news] WHERE id NOT IN(SELECT TOP {} id FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[news]", (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

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
				info["date"] = r.get<std::string>("date", "");
				info["author"] = gbkToUTF8(r.get<std::string>("author", ""));
				info["content"] = gbkToUTF8(r.get<nanodbc::string>("content", ""));
				info["create_date"] = r.get<std::string>("create_date", "");
				info["create_user"] = r.get<std::string>("create_user", "");
				info["update_date"] = r.get<std::string>("update_date", "");
				info["update_user"] = r.get<std::string>("update_user", "");
				info["delete"] = r.get<INT64>("delete", 0);
				infos.append(info);
			}

			ret["msg"] = infos;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("news info error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void News::latest(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::string stmt = fmt::format("SELECT TOP 10 * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[news] WHERE id NOT IN(SELECT TOP 0 id FROM[SA_BETA_WORLDDB_0002].[PaWebPublic].[news] ORDER BY[create_date] DESC) AND [delete] = 0 ORDER BY [create_date] DESC");

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
				info["date"] = r.get<std::string>("date", "");
				info["author"] = gbkToUTF8(r.get<std::string>("author", ""));
				info["content"] = gbkToUTF8(r.get<nanodbc::string>("content", ""));
				info["create_date"] = r.get<std::string>("create_date", "");
				info["create_user"] = r.get<std::string>("create_user", "");
				info["update_date"] = r.get<std::string>("update_date", "");
				info["update_user"] = r.get<std::string>("update_user", "");
				info["delete"] = r.get<INT64>("delete", 0);
				infos.append(info);
			}

			ret["msg"] = infos;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("news info error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}
}