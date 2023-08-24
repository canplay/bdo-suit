#include "slide.h"
#include "mssql.h"
#include "pch.h"
#include "utility.h"

namespace api {
void Slide::count(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const {
  auto stmt = fmt::format(
      "SELECT COUNT(id) FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide]");

  Json::Value ret;

  try {
    Json::Value info;

    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();
    ret["msg"] = r.get<int64_t>(0, 0);
    ret["status"] = 1;
  } catch (const std::exception &e) {
    spdlog::warn("slide count error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
  }

  callback(HttpResponse::newHttpJsonResponse(ret));
}

void Slide::add(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback) const {
  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    Json::Value ret;
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  auto now = std::chrono::system_clock::now();
  time_t time = std::chrono::system_clock::to_time_t(now);
  auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

  auto stmt = fmt::format(
      "INSERT INTO [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide] ([id], "
      "[title], [desc], [link], [create_date], [create_user], [update_date], "
      "[update_user], [img]) VALUES ('{}', N'{}', N'{}', '{}', '{}', '{}', "
      "'{}', '{}', '{}');",
      uuidSimple(), (*json)["title"].asString(), (*json)["desc"].asString(),
      (*json)["link"].asString(), timestamp, (*json)["create_user"].asString(),
      timestamp, (*json)["update_user"].asString(), (*json)["img"].asString());

  Json::Value ret;

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      ret["msg"] = "ok";
      ret["status"] = 1;
    } else {
      ret["msg"] = "slide add error";
      ret["status"] = 0;
    }
  } catch (const std::exception &e) {
    spdlog::warn("slide add error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
  }

  callback(HttpResponse::newHttpJsonResponse(ret));
}

void Slide::update(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const {
  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    Json::Value ret;
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  auto now = std::chrono::system_clock::now();
  time_t time = std::chrono::system_clock::to_time_t(now);
  auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

  auto stmt = fmt::format(
      "UPDATE [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide] SET [title] = "
      "N'{}', [desc] = N'{}', [link] = '{}', [update_date] = '{}', "
      "[update_user] = '{}', [img] = '{}' WHERE [id] = '{}';",
      (*json)["title"].asString(), (*json)["desc"].asString(),
      (*json)["link"].asString(), timestamp, (*json)["update_user"].asString(),
      (*json)["img"].asString(), (*json)["id"].asString());

  Json::Value ret;

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      ret["msg"] = "ok";
      ret["status"] = 1;
    } else {
      ret["msg"] = "slide update error";
      ret["status"] = 0;
    }
  } catch (const std::exception &e) {
    spdlog::warn("slide update error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
  }

  callback(HttpResponse::newHttpJsonResponse(ret));
}

void Slide::info(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const {
  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    Json::Value ret;
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  std::string stmt = fmt::format(
      "SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[slide] WHERE "
      "id NOT IN(SELECT TOP {} id FROM "
      "[SA_BETA_WORLDDB_0002].[PaWebPublic].[slide]",
      (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

  if ((*json)["sortBy"].asString() != "") {
    stmt =
        fmt::format("{} ORDER BY [{}] DESC) ORDER BY [{}]", stmt,
                    (*json)["sortBy"].asString(), (*json)["sortBy"].asString());

    if ((*json)["descending"].asBool())
      stmt = fmt::format("{} DESC", stmt);
    else
      stmt = fmt::format("{} ASC", stmt);
  } else
    stmt = fmt::format("{})", stmt);

  Json::Value ret;

  try {
    Json::Value infos;

    auto r = MsSql::exec(utf8ToGBK(stmt));

    while (r.next()) {
      Json::Value info;
      info["id"] = r.get<std::string>("id", "");
      info["title"] = r.get<std::string>("title", "");
      info["desc"] = r.get<std::string>("desc", "");
      info["link"] = r.get<std::string>("link", "");
      info["create_date"] = r.get<std::string>("create_date", "");
      info["create_user"] = r.get<std::string>("create_user", "");
      info["update_date"] = r.get<std::string>("update_date", "");
      info["update_user"] = r.get<std::string>("update_user", "");
      info["img"] = r.get<std::string>("img", "");
      infos.append(info);
    }

    ret["msg"] = infos;
    ret["status"] = 1;
  } catch (const std::exception &e) {
    spdlog::warn("slide info error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
  }

  callback(HttpResponse::newHttpJsonResponse(ret));
}
} // namespace api