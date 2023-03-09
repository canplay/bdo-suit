#include "stdafx.h"
#include "version.h"
#include "mssql.h"
#include "utility.h"

namespace api
{
	void Version::count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		auto stmt = fmt::format("SELECT COUNT(id) FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[version]");

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
			spdlog::warn("version count error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Version::add(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		async_run([=]()->Task<>
		{
			Json::Value ret;

			try
			{
				std::string hdiff_r;

				if ((*json)["name"].asString() != "app")
				{
					char buf[10240];
					auto hdiff = std::make_shared<FILE*>(_popen(fmt::format("diff -s-512m -p-2 -c-lzma -f \"{}\" \"{}\" \"{}/{}_{}.patch\"", (*json)["old"].asString(), (*json)["new"].asString(), (*json)["save"].asString(), (*json)["name"].asString(), (*json)["version"].asString()).c_str(), "r"));

					if (hdiff == nullptr)
					{
						spdlog::warn("diff error");
						ret["msg"] = "diff error";
						ret["status"] = 0;
						callback(HttpResponse::newHttpJsonResponse(ret));
						co_return;
					}

					while (fgets(buf, sizeof(buf), *hdiff.get()))
					{
						hdiff_r += buf;
					}

					_pclose(*hdiff.get());
				}

				auto now = std::chrono::system_clock::now();
				time_t time = std::chrono::system_clock::to_time_t(now);
				auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

				auto stmt = fmt::format("INSERT INTO [SA_BETA_WORLDDB_0002].[PaWebPublic].[version] ([id], [name], [version], [force], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [delete], [oldPath], [newPath], [savePath]) VALUES ('{}', '{}', '{}', {}, '{}', '{}', '{}', '{}', '{}', '{}', '{}', 0, '{}', '{}', '{}');", uuidSimple(), (*json)["name"].asString(), (*json)["version"].asString(), (*json)["force"].asInt(), (*json)["link"].asString(), timestamp, (*json)["create_user"].asString(), (*json)["create_id"].asString(), timestamp, (*json)["update_user"].asString(), (*json)["update_id"].asString(), (*json)["oldPath"].asString(), (*json)["newPath"].asString(), (*json)["savePath"].asString());

				auto r = MsSql::exec(stmt);

				if (r.affected_rows() == 1)
				{
					if (hdiff_r.find("patch check diff data ok") != std::string::npos)
					{
						ret["msg"] = "ok";
						ret["status"] = 1;
					}
					else if ((*json)["name"].asString() == "app")
					{
						ret["msg"] = "ok";
						ret["status"] = 1;
					}
					else
					{
						ret["msg"] = "version generate error";
						ret["status"] = 0;
					}
				}
				else
				{
					ret["msg"] = "version add error";
					ret["status"] = 0;
				}
			}
			catch (const std::exception& e)
			{
				spdlog::warn("version add error: {}", e.what());
				ret["msg"] = e.what();
				ret["status"] = 0;
			}

			callback(HttpResponse::newHttpJsonResponse(ret));
			co_return;
		});
	}

	void Version::update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
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

		auto stmt = fmt::format("UPDATE [SA_BETA_WORLDDB_0002].[PaWebPublic].[version] SET [name] = '{}', [version] = '{}', [force] = {}, [link] = '{}', [update_date] = '{}', [update_user] = '{}', [update_id] = '{}', [delete] = {}, [oldPath] = '{}', [newPath] = '{}', [savePath] = '{}' WHERE [id] = '{}';", (*json)["name"].asString(), (*json)["version"].asString(), (*json)["force"].asInt(), (*json)["link"].asString(), timestamp, (*json)["update_user"].asString(), (*json)["update_id"].asString(), (*json)["delete"].asInt(), (*json)["oldPath"].asString(), (*json)["newPath"].asString(), (*json)["savePath"].asString(), (*json)["id"].asString());

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
				ret["msg"] = "version update error";
				ret["status"] = 0;
			}
		}
		catch (const std::exception& e)
		{
			spdlog::warn("version update error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Version::info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		std::shared_ptr<Json::Value> json = req->getJsonObject();
		if (!json)
		{
			Json::Value ret;
			ret["msg"] = "error";
			ret["status"] = 0;
			return callback(HttpResponse::newHttpJsonResponse(ret));
		}

		std::string stmt = fmt::format("SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[version] WHERE id NOT IN(SELECT TOP {} id FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[version]", (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

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
				info["name"] = r.get<std::string>("name", "");
				info["version"] = r.get<std::string>("version", "");
				info["force"] = r.get<INT64>("force", 0);
				info["link"] = r.get<std::string>("link", "");
				info["create_date"] = r.get<std::string>("create_date", "");
				info["create_user"] = r.get<std::string>("create_user", "");
				info["create_id"] = r.get<std::string>("create_id", "");
				info["update_date"] = r.get<std::string>("update_date", "");
				info["update_user"] = r.get<std::string>("update_user", "");
				info["update_id"] = r.get<std::string>("update_id", "");
				info["delete"] = r.get<INT64>("delete", 0);
				info["oldPath"] = r.get<std::string>("oldPath", "");
				info["newPath"] = r.get<std::string>("newPath", "");
				info["savePath"] = r.get<std::string>("savePath", "");
				infos.append(info);
			}

			ret["msg"] = infos;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("version info error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Version::latest(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const
	{
		Json::Value ret;

		try
		{
			Json::Value infos;

			std::string stmt = fmt::format("SELECT TOP 1 * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[version] WHERE id NOT IN(SELECT TOP 0 id FROM[SA_BETA_WORLDDB_0002].[PaWebPublic].[version] ORDER BY[version] DESC) AND [name] = 'app' AND [delete] = 0 ORDER BY [version] DESC");
			auto r = MsSql::exec(stmt);
			r.next();
			Json::Value app;
			app["id"] = r.get<std::string>("id", "");
			app["name"] = r.get<std::string>("name", "");
			app["version"] = r.get<std::string>("version", "");
			app["force"] = r.get<INT64>("force", 0);
			app["link"] = r.get<std::string>("link", "");
			infos.append(app);

			stmt = fmt::format("SELECT TOP 1 * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[version] WHERE id NOT IN(SELECT TOP 0 id FROM[SA_BETA_WORLDDB_0002].[PaWebPublic].[version] ORDER BY[version] DESC) AND [name] = 'resource' AND [delete] = 0 ORDER BY [version] DESC");
			r = MsSql::exec(stmt);
			r.next();
			Json::Value res;
			res["id"] = r.get<std::string>("id", "");
			res["name"] = r.get<std::string>("name", "");
			res["version"] = r.get<std::string>("version", "");
			res["force"] = r.get<INT64>("force", 0);
			res["link"] = r.get<std::string>("link", "");
			infos.append(res);

			ret["msg"] = infos;
			ret["status"] = 1;
		}
		catch (const std::exception& e)
		{
			spdlog::warn("version latest error: {}", e.what());
			ret["msg"] = e.what();
			ret["status"] = 0;
		}

		callback(HttpResponse::newHttpJsonResponse(ret));
	}

	void Version::diffStatus(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
	{
		Json::Value ret;
		ret["msg"] = m_diffStatus;
		ret["status"] = 1;
		callback(HttpResponse::newHttpJsonResponse(ret));
	}
}