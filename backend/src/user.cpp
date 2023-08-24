#include "user.h"
#include "jwt.h"
#include "mssql.h"
#include "pch.h"
#include "utility.h"
#include <random>

namespace api {
void User::count(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const {
  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    Json::Value ret;
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  auto stmt = fmt::format(
      "SELECT COUNT(_userNo) FROM "
      "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation]");

  if ((*json)["username"].asString() != "") {
    stmt = fmt::format("{} WHERE _userId LIKE '%{}%'", stmt,
                       (*json)["username"].asString());
  }

  Json::Value ret;

  try {
    Json::Value info;

    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();
    ret["msg"] = r.get<int64_t>(0, 0);
    ret["status"] = 1;
    callback(HttpResponse::newHttpJsonResponse(ret));
  } catch (const std::exception &e) {
    spdlog::warn("user count error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::signup(
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

  auto stmt = fmt::format("SELECT * FROM "
                          "[SA_BETA_WORLDDB_0002].[PaGamePrivate].["
                          "TblUserInformation] WHERE _userId LIKE '{},%'",
                          (*json)["username"].asString());

  Json::Value ret;

  try {
    Json::Value info;

    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();

    if (r.rows() != 0) {
      ret["msg"] = "username already exist";
      ret["status"] = 0;
      return callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      std::random_device seed;
      std::ranlux48 engine(seed());
      std::uniform_int_distribution<> distrib(10000, 99999);
      int random = distrib(engine);

      std::string nickname = fmt::format("Adventurer{}", random);

      auto stmt1 = fmt::format(
          "INSERT INTO "
          "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] "
          "([_registerDate], [_isValid], [_userId], [_userNickname], "
          "[_password], [_authenticKey], [_authenticExpiration], "
          "[_webAuthenticKey], [_lastLoginTime], [_lastLogoutTime], "
          "[_totalPlayTime], [_lastIp], [_lastServerNo], [_failPasswordCount], "
          "[_membershipType], [_isAdmissionToSpeedServer], [_isPcRoom], "
          "[_isGuestAccount], [_speedServerExpiration], [_surveyHWAndSW], "
          "[_isAccessBlanceChannel], [_isPremiumChannelPermission], "
          "[_isIgnoreCheckCustomizeOnly], [_preAuthenticKey], "
          "[_isAdultWorldUser], [_shutDownTime], [_atField], "
          "[_isCompleteTesterSubmit], [_isOtp], [_lastMacAddress], "
          "[_allCharacterTotalLevel], [_isAppliedNickNameChange]) VALUES('{}', "
          "'1', N'{},{}', N'{}', "
          "0xF6ADBC1E77E92C0F1725B83DCFCFA139AD87EABCB262E1E3652B485F427961BDC5"
          "7382A8A729331B7FF8A7A90FFBFE80ED43EF1060FE5F7716266DC48227AED0, 0, "
          "'1900-01-01 00:00:00.000', 0, '{}', '{}', 0, '127.0.0.1', -1, 0, 0, "
          "2, '1', '0', '2033-12-30 00:00:00.000', '', '1', '0', '0', 0, '1', "
          "'1970-01-01 08:00:00.000', '', '0', '0', "
          "0xF728BB816503A79FCCBF76A7AA2F6AF0294BBCCAB8E3140ECAEEAED08E2329A222"
          "8983F442A858128C9EA83ED089A12DA0D7076FF52E2F313466E7D42A0130CC, 0, "
          "'1')",
          timestamp, (*json)["username"].asString(),
          (*json)["password"].asString(), nickname, timestamp, timestamp,
          timestamp);

      auto r1 = MsSql::exec(stmt1);

      if (r1.affected_rows() >= 1) {
        auto stmt2 = fmt::format("SELECT * FROM "
                                 "[SA_BETA_WORLDDB_0002].[PaGamePrivate].["
                                 "TblUserInformation] WHERE _userId = '{},{}'",
                                 (*json)["username"].asString(),
                                 (*json)["password"].asString());
        auto r2 = MsSql::exec(stmt2);
        r2.next();

        if (r2.rows() != 0) {
          Json::Value info;

          info["registerDate"] = r2.get<std::string>("_registerDate", "");
          info["valid"] = r2.get<std::string>("_isValid", "");
          info["userNo"] = r2.get<int64_t>("_userNo", 0);
          info["userId"] = r2.get<std::string>("_userId", "");
          info["userNickname"] = r2.get<std::string>("_userNickname", "");
          info["lastLoginTime"] = r2.get<std::string>("_lastLoginTime", "");
          info["lastLogoutTime"] = r2.get<std::string>("_lastLogoutTime", "");
          info["totalPlayTime"] = r2.get<int64_t>("_totalPlayTime", 0);
          info["membershipType"] = r2.get<int64_t>("_membershipType", 0);
          info["pcroom"] = r2.get<int64_t>("_isPcRoom", 0);

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
          return callback(HttpResponse::newHttpJsonResponse(ret));
        }

        ret["msg"] = "signup error";
        ret["status"] = 0;
        callback(HttpResponse::newHttpJsonResponse(ret));
      } else {
        ret["msg"] = "signup error";
        ret["status"] = 0;
        callback(HttpResponse::newHttpJsonResponse(ret));
      }
    }
  } catch (const std::exception &e) {
    spdlog::warn("signup error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::signin(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const {
  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    Json::Value ret;
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  auto stmt = fmt::format("SELECT * FROM "
                          "[SA_BETA_WORLDDB_0002].[PaGamePrivate].["
                          "TblUserInformation] WHERE _userId = '{},{}'",
                          (*json)["username"].asString(),
                          (*json)["password"].asString());

  Json::Value ret;

  try {
    Json::Value info;

    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();

    if (r.rows() != 0) {
      Json::Value info;

      auto r = MsSql::exec(utf8ToGBK(stmt));
      r.next();
      info["registerDate"] = r.get<std::string>("_registerDate", "");
      info["valid"] = r.get<std::string>("_isValid", "");
      info["userNo"] = r.get<int64_t>("_userNo", 0);
      info["userId"] = r.get<std::string>("_userId", "");
      info["userNickname"] = r.get<std::string>("_userNickname", "");
      info["lastLoginTime"] = r.get<std::string>("_lastLoginTime", "");
      info["lastLogoutTime"] = r.get<std::string>("_lastLogoutTime", "");
      info["totalPlayTime"] = r.get<int64_t>("_totalPlayTime", 0);
      info["membershipType"] = r.get<int64_t>("_membershipType", 0);
      info["pcroom"] = r.get<int64_t>("_isPcRoom", 0);
      info["permission"] =
          getPermission(std::to_string(r.get<int64_t>("_userNo", 0)));

      Json::Value characters;
      auto stmt1 = fmt::format("SELECT * FROM "
                               "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                               "TblCharacterInformation] WHERE [_userNo] = {}",
                               r.get<int64_t>("_userNo", 0));
      auto r1 = MsSql::exec(stmt1);
      while (r1.next()) {
        Json::Value character;
        character["deletedDate"] = r1.get<std::string>("_deletedDate", "");
        character["characterNo"] = r1.get<int64_t>("_characterNo", 0);
        character["characterName"] = r1.get<std::string>("_characterName", "");
        character["classType"] = r1.get<int64_t>("_classType", 0);
        character["totalPlayTime"] = r1.get<int64_t>("_totalPlayTime", 0);
        character["currentPositionX"] = r1.get<int64_t>("_currentPositionX", 0);
        character["currentPositionY"] = r1.get<int64_t>("_currentPositionY", 0);
        character["currentPositionZ"] = r1.get<int64_t>("_currentPositionZ", 0);
        character["returnPositionX"] = r1.get<int64_t>("_returnPositionX", 0);
        character["returnPositionY"] = r1.get<int64_t>("_returnPositionY", 0);
        character["returnPositionZ"] = r1.get<int64_t>("_returnPositionZ", 0);
        character["level"] = r1.get<int64_t>("_level", 0);
        character["experience"] = r1.get<int64_t>("_experience", 0);
        character["variedWeight"] = r1.get<int64_t>("_variedWeight", 0);
        character["skillPointLevel"] = r1.get<int64_t>("_skillPointLevel", 0);
        character["skillPointExperience"] =
            r1.get<int64_t>("_skillPointExperience", 0);
        character["remainedSkillPoint"] =
            r1.get<int64_t>("_remainedSkillPoint", 0);
        character["aquiredSkillPoint"] =
            r1.get<int64_t>("_aquiredSkillPoint", 0);
        character["tendency"] = r1.get<int64_t>("_tendency", 0);
        character["hp"] = r1.get<int64_t>("_hp", 0);
        character["mp"] = r1.get<int64_t>("_mp", 0);
        character["sp"] = r1.get<int64_t>("_sp", 0);
        character["wp"] = r1.get<int64_t>("_wp", 0);
        character["inventorySlotCount"] =
            r1.get<int64_t>("_inventorySlotCount", 0);
        character["titleKey"] = r1.get<int64_t>("_titleKey", 0);
        character["killRewardCount"] = r1.get<int64_t>("_killRewardCount", 0);
        character["enchantFailCount"] = r1.get<int64_t>("_enchantFailCount", 0);
        character["offenceValue"] = r1.get<int64_t>("_offenceValue", 0);
        character["defenceValue"] = r1.get<int64_t>("_defenceValue", 0);
        character["awakenValue"] = r1.get<int64_t>("_awakenValue", 0);
        characters.append(character);
      }
      info["characters"] = characters;

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
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "username or password error";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("signin error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::signout(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const {}

void User::info(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback) const {
  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    Json::Value ret;
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  std::string stmt =
      fmt::format("SELECT TOP {} * FROM "
                  "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] "
                  "WHERE _userNo NOT IN(SELECT TOP {} _userNo FROM "
                  "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation]",
                  (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

  if ((*json)["username"].asString() != "") {
    stmt = fmt::format(
        "SELECT TOP {} * FROM "
        "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] WHERE "
        "_userId LIKE '%{}%' AND _userNo NOT IN(SELECT TOP {} _userNo FROM "
        "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] WHERE "
        "_userId LIKE '%{}%'",
        (*json)["maxPage"].asInt64(), (*json)["username"].asString(),
        (*json)["curPage"].asInt64(), (*json)["username"].asString());
  }

  if ((*json)["sortBy"].asString() != "") {
    stmt =
        fmt::format("{} ORDER BY [_{}] DESC) ORDER BY [_{}]", stmt,
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
      info["registerDate"] = r.get<std::string>("_registerDate", "");
      info["valid"] = r.get<std::string>("_isValid", "");
      info["userNo"] = r.get<int64_t>("_userNo", 0);
      info["userId"] = r.get<std::string>("_userId", "");
      info["userNickname"] = r.get<std::string>("_userNickname", "");
      info["lastLoginTime"] = r.get<std::string>("_lastLoginTime", "");
      info["lastLogoutTime"] = r.get<std::string>("_lastLogoutTime", "");
      info["totalPlayTime"] = r.get<int64_t>("_totalPlayTime", 0);
      info["membershipType"] = r.get<int64_t>("_membershipType", 0);
      info["pcroom"] = r.get<int64_t>("_isPcRoom", 0);
      info["permission"] =
          getPermission(std::to_string(r.get<int64_t>("_userNo", 0)));

      Json::Value characters;
      auto stmt1 = fmt::format("SELECT * FROM "
                               "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                               "TblCharacterInformation] WHERE [_userNo] = {}",
                               r.get<int64_t>("_userNo", 0));
      auto r1 = MsSql::exec(stmt1);
      spdlog::info(stmt1);
      while (r1.next()) {
        Json::Value character;
        character["deletedDate"] = r1.get<std::string>("_deletedDate", "");
        character["characterNo"] = r1.get<int64_t>("_characterNo", 0);
        character["characterName"] = r1.get<std::string>("_characterName", "");
        character["classType"] = r1.get<int64_t>("_classType", 0);
        character["totalPlayTime"] = r1.get<int64_t>("_totalPlayTime", 0);
        character["currentPositionX"] = r1.get<int64_t>("_currentPositionX", 0);
        character["currentPositionY"] = r1.get<int64_t>("_currentPositionY", 0);
        character["currentPositionZ"] = r1.get<int64_t>("_currentPositionZ", 0);
        character["returnPositionX"] = r1.get<int64_t>("_returnPositionX", 0);
        character["returnPositionY"] = r1.get<int64_t>("_returnPositionY", 0);
        character["returnPositionZ"] = r1.get<int64_t>("_returnPositionZ", 0);
        character["level"] = r1.get<int64_t>("_level", 0);
        character["experience"] = r1.get<int64_t>("_experience", 0);
        character["variedWeight"] = r1.get<int64_t>("_variedWeight", 0);
        character["skillPointLevel"] = r1.get<int64_t>("_skillPointLevel", 0);
        character["skillPointExperience"] =
            r1.get<int64_t>("_skillPointExperience", 0);
        character["remainedSkillPoint"] =
            r1.get<int64_t>("_remainedSkillPoint", 0);
        character["aquiredSkillPoint"] =
            r1.get<int64_t>("_aquiredSkillPoint", 0);
        character["tendency"] = r1.get<int64_t>("_tendency", 0);
        character["hp"] = r1.get<int64_t>("_hp", 0);
        character["mp"] = r1.get<int64_t>("_mp", 0);
        character["sp"] = r1.get<int64_t>("_sp", 0);
        character["wp"] = r1.get<int64_t>("_wp", 0);
        character["inventorySlotCount"] =
            r1.get<int64_t>("_inventorySlotCount", 0);
        character["titleKey"] = r1.get<int64_t>("_titleKey", 0);
        character["killRewardCount"] = r1.get<int64_t>("_killRewardCount", 0);
        character["enchantFailCount"] = r1.get<int64_t>("_enchantFailCount", 0);
        character["offenceValue"] = r1.get<int64_t>("_offenceValue", 0);
        character["defenceValue"] = r1.get<int64_t>("_defenceValue", 0);
        character["awakenValue"] = r1.get<int64_t>("_awakenValue", 0);
        characters.append(character);
      }

      info["characters"] = characters;
      infos.append(info);
    }

    ret["msg"] = infos;
    ret["status"] = 1;
    callback(HttpResponse::newHttpJsonResponse(ret));
  } catch (const std::exception &e) {
    spdlog::warn("user info error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::infoOne(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback,
                   const std::string &id) const {
  auto stmt = fmt::format("SELECT * FROM "
                          "[SA_BETA_WORLDDB_0002].[PaGamePrivate].["
                          "TblUserInformation] WHERE _userNo = {}",
                          id);

  Json::Value ret;

  try {
    Json::Value info;

    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();
    info["registerDate"] = r.get<std::string>("_registerDate", "");
    info["valid"] = r.get<std::string>("_isValid", "");
    info["userNo"] = r.get<int64_t>("_userNo", 0);
    info["userId"] = r.get<std::string>("_userId", "");
    info["userNickname"] = r.get<std::string>("_userNickname", "");
    info["lastLoginTime"] = r.get<std::string>("_lastLoginTime", "");
    info["lastLogoutTime"] = r.get<std::string>("_lastLogoutTime", "");
    info["totalPlayTime"] = r.get<int64_t>("_totalPlayTime", 0);
    info["membershipType"] = r.get<int64_t>("_membershipType", 0);
    info["pcroom"] = r.get<int64_t>("_isPcRoom", 0);
    info["permission"] =
        getPermission(std::to_string(r.get<int64_t>("_userNo", 0)));

    Json::Value characters;
    auto stmt1 = fmt::format("SELECT * FROM "
                             "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                             "TblCharacterInformation] WHERE [_userNo] = {}",
                             r.get<int64_t>("_userNo", 0));
    auto r1 = MsSql::exec(stmt1);
    while (r1.next()) {
      Json::Value character;
      character["deletedDate"] = r1.get<std::string>("_deletedDate", "");
      character["characterNo"] = r1.get<int64_t>("_characterNo", 0);
      character["characterName"] = r1.get<std::string>("_characterName", "");
      character["classType"] = r1.get<int64_t>("_classType", 0);
      character["totalPlayTime"] = r1.get<int64_t>("_totalPlayTime", 0);
      character["currentPositionX"] = r1.get<int64_t>("_currentPositionX", 0);
      character["currentPositionY"] = r1.get<int64_t>("_currentPositionY", 0);
      character["currentPositionZ"] = r1.get<int64_t>("_currentPositionZ", 0);
      character["returnPositionX"] = r1.get<int64_t>("_returnPositionX", 0);
      character["returnPositionY"] = r1.get<int64_t>("_returnPositionY", 0);
      character["returnPositionZ"] = r1.get<int64_t>("_returnPositionZ", 0);
      character["level"] = r1.get<int64_t>("_level", 0);
      character["experience"] = r1.get<int64_t>("_experience", 0);
      character["variedWeight"] = r1.get<int64_t>("_variedWeight", 0);
      character["skillPointLevel"] = r1.get<int64_t>("_skillPointLevel", 0);
      character["skillPointExperience"] =
          r1.get<int64_t>("_skillPointExperience", 0);
      character["remainedSkillPoint"] =
          r1.get<int64_t>("_remainedSkillPoint", 0);
      character["aquiredSkillPoint"] = r1.get<int64_t>("_aquiredSkillPoint", 0);
      character["tendency"] = r1.get<int64_t>("_tendency", 0);
      character["hp"] = r1.get<int64_t>("_hp", 0);
      character["mp"] = r1.get<int64_t>("_mp", 0);
      character["sp"] = r1.get<int64_t>("_sp", 0);
      character["wp"] = r1.get<int64_t>("_wp", 0);
      character["inventorySlotCount"] =
          r1.get<int64_t>("_inventorySlotCount", 0);
      character["titleKey"] = r1.get<int64_t>("_titleKey", 0);
      character["killRewardCount"] = r1.get<int64_t>("_killRewardCount", 0);
      character["enchantFailCount"] = r1.get<int64_t>("_enchantFailCount", 0);
      character["offenceValue"] = r1.get<int64_t>("_offenceValue", 0);
      character["defenceValue"] = r1.get<int64_t>("_defenceValue", 0);
      character["awakenValue"] = r1.get<int64_t>("_awakenValue", 0);
      characters.append(character);
    }
    info["characters"] = characters;

    ret["msg"] = info;
    ret["status"] = 1;
    callback(HttpResponse::newHttpJsonResponse(ret));
  } catch (const std::exception &e) {
    spdlog::warn("user info one error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::mail(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback,
                const std::string type) const {
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

  Json::Value ret;
  std::string stmt = "";

  if (type == "send") {
    stmt = fmt::format(
        "INSERT INTO [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblMail] "
        "([_registerDate], [_senderName], [_senderUserNo], [_receiverName], "
        "[_receiverUserNo], [_title], [_contents], [_mailType], [_variousNo], "
        "[_enchantLevel], [_itemCount], [_webItemType], [_chargeNo]) VALUES "
        "('{}', N'系统管理员', 0, N'{}', {}, N系统管理员', N'系统管理员', 0, "
        "{}, "
        "{}, "
        "{}, 0, '')",
        timestamp, (*json)["receiverName"].asString(),
        (*json)["receiverUserNo"].asInt64(), (*json)["variousNo"].asInt64(),
        (*json)["enchantLevel"].asInt64(), (*json)["itemCount"].asInt64());
  } else if (type == "clear") {
    stmt = fmt::format(
        "DELETE FROM [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblMail] WHERE "
        "[_senderName] = N'系统管理员' AND [_receiverUserNo] = {}",
        (*json)["receiverUserNo"].asInt64());
  } else {
    ret["msg"] = "no method";
    ret["status"] = 0;

    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      ret["msg"] = "ok";
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "no mail";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("mail error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::update(const HttpRequestPtr &req,
                  std::function<void(const HttpResponsePtr &)> &&callback,
                  const std::string type) const {
  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    Json::Value ret;
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  Json::Value ret;

  if (type != "map" && type != "knowledge" &&
      ((*json)["username"].asString() == "" ||
       (*json)["password"].asString() == "" ||
       (*json)["familyname"].asString() == "")) {
    ret["msg"] = "用户名, 密码或家族名不能为空";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  auto now = std::chrono::system_clock::now();
  time_t time = std::chrono::system_clock::to_time_t(now);
  auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

  if (type == "user") {
    auto stmt = fmt::format("SELECT * FROM "
                            "[SA_BETA_WORLDDB_0002].[PaGamePrivate].["
                            "TblUserInformation] WHERE _userId LIKE '{},%'",
                            (*json)["username"].asString());

    Json::Value ret;

    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();

    if (r.rows() > 1) {
      ret["msg"] = "用户名已存在";
      ret["status"] = 0;
      return callback(HttpResponse::newHttpJsonResponse(ret));
    }

    auto stmt1 = fmt::format(
        "UPDATE [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] "
        "SET [_userId] = N'{},{}', [_userNickname] = N'{}' WHERE [_userNo] = "
        "{}",
        (*json)["username"].asString(), (*json)["password"].asString(),
        (*json)["userNickname"].asString(), (*json)["userNo"].asInt64());

    auto stmt2 = fmt::format(
        "UPDATE "
        "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblBriefUserInformation] SET "
        "[_userId] = N'{},{}', [_userNickname] = N'{}' WHERE [_userNo] = {}",
        (*json)["username"].asString(), (*json)["password"].asString(),
        (*json)["userNickname"].asString(), (*json)["userNo"].asInt64());

    auto stmt3 = fmt::format("SELECT COUNT(_userNo) FROM "
                             "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                             "TblBriefUserInformation] WHERE [_userNo] = {}",
                             (*json)["userNo"].asInt64());

    try {
      auto r3 = MsSql::exec(stmt3);
      r3.next();

      if (r3.get<int64_t>(0, 0) > 0) {
        auto r1 = MsSql::exec(stmt1);

        if (r1.affected_rows() >= 1) {
          auto r2 = MsSql::exec(stmt2);

          if (r2.affected_rows() >= 1) {
            ret["msg"] = "ok";
            ret["status"] = 1;
            callback(HttpResponse::newHttpJsonResponse(ret));
          } else {
            ret["msg"] = "user update error";
            ret["status"] = 0;
            callback(HttpResponse::newHttpJsonResponse(ret));
          }
        } else {
          ret["msg"] = "user update error";
          ret["status"] = 0;
          callback(HttpResponse::newHttpJsonResponse(ret));
        }
      } else {
        auto r1 = MsSql::exec(stmt1);

        if (r1.affected_rows() >= 1) {
          ret["msg"] = "ok";
          ret["status"] = 1;
          callback(HttpResponse::newHttpJsonResponse(ret));
        } else {
          ret["msg"] = "user update error";
          ret["status"] = 0;
          callback(HttpResponse::newHttpJsonResponse(ret));
        }
      }
    } catch (const std::exception &e) {
      spdlog::warn("user update error: {}", e.what());
      ret["msg"] = e.what();
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } else if (type == "map") {
    async_run([=]() -> Task<> {
      for (size_t i = 1; i < 2001; ++i) {
        Json::Value info;

        auto stmt = fmt::format(
            "IF NOT EXISTS (SELECT * FROM "
            "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblPlantRepository] WHERE "
            "[_userNo] = {} AND [_waypointKey] = {}) INSERT INTO "
            "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblPlantRepository] "
            "([_registerDate], [_userNo], [_waypointKey], [_type], [_subType], "
            "[_value], [_experience], [_dateTime]) VALUES ('{}', {}, {}, 0, 0, "
            "1, 0, '{}') ELSE UPDATE "
            "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblPlantRepository] SET "
            "[_value] = 1 WHERE [_userNo] = {} AND [_waypointKey] = {}",
            (*json)["userNo"].asInt64(), i, timestamp,
            (*json)["userNo"].asInt64(), i, timestamp,
            (*json)["userNo"].asInt64(), i);

        MsSql::exec(utf8ToGBK(stmt));
      }

      co_return;
    });

    ret["msg"] = "ok";
    ret["status"] = 1;
    callback(HttpResponse::newHttpJsonResponse(ret));
  } else if (type == "knowledge") {
    async_run([=]() -> Task<> {
      for (size_t i = 1; i < 10853; ++i) {
        Json::Value info;

        auto stmt = fmt::format(
            "IF NOT EXISTS (SELECT * FROM "
            "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblMentalCardList] WHERE "
            "[_userNo] = {} AND [_cardKey] = {}) INSERT INTO "
            "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblMentalCardList] "
            "([_registerDate], [_userNo], [_cardKey], [_level]) VALUES ('{}', "
            "{}, {}, 1) ELSE UPDATE "
            "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblMentalCardList] SET "
            "[_level] = 1 WHERE [_userNo] = {} AND [_cardKey] = {}",
            (*json)["userNo"].asInt64(), i, timestamp,
            (*json)["userNo"].asInt64(), i, (*json)["userNo"].asInt64(), i);

        MsSql::exec(utf8ToGBK(stmt));
      }

      co_return;
    });

    ret["msg"] = "ok";
    ret["status"] = 1;
    callback(HttpResponse::newHttpJsonResponse(ret));
  } else {
    ret["msg"] = "no method";
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::characterUpdate(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const {
  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    Json::Value ret;
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  Json::Value ret;

  auto stmt =
      fmt::format("SELECT COUNT(_characterNo) FROM "
                  "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                  "TblCharacterInformation] WHERE [_characterName] = N'{}'",
                  (*json)["characterNameOld"].asString());

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();

    auto now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);
    auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

    if (r.get<int64_t>(0, 0) != 0) {
      stmt = fmt::format(
          "UPDATE "
          "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblCharacterInformation] SET "
          "[_characterName] = N'{}', [_currentPositionX] = {}, "
          "[_currentPositionY] = {}, [_currentPositionZ] = {}, "
          "[_returnPositionX] = {}, [_returnPositionY] = {}, "
          "[_returnPositionZ] = {} WHERE [_characterNo] = {}",
          (*json)["characterName"].asString(),
          (*json)["currentPositionX"].asInt64(),
          (*json)["currentPositionY"].asInt64(),
          (*json)["currentPositionZ"].asInt64(),
          (*json)["currentPositionX"].asInt64(),
          (*json)["currentPositionY"].asInt64(),
          (*json)["currentPositionZ"].asInt64(),
          (*json)["characterNo"].asInt64());
    } else {
      stmt =
          fmt::format("UPDATE "
                      "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                      "TblCharacterInformation] SET [_currentPositionX] = {}, "
                      "[_currentPositionY] = {}, [_currentPositionZ] = {}, "
                      "[_returnPositionX] = {}, [_returnPositionY] = {}, "
                      "[_returnPositionZ] = {} WHERE [_characterNo] = {}",
                      (*json)["currentPositionX"].asInt64(),
                      (*json)["currentPositionY"].asInt64(),
                      (*json)["currentPositionZ"].asInt64(),
                      (*json)["currentPositionX"].asInt64(),
                      (*json)["currentPositionY"].asInt64(),
                      (*json)["currentPositionZ"].asInt64(),
                      (*json)["characterNo"].asInt64());
    }

    r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      if ((*json)["deletedDate"].asString() != "") {
        stmt = fmt::format("UPDATE "
                           "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                           "TblCharacterInformation] SET [_deletedDate] = '{}'",
                           timestamp);
        r = MsSql::exec(utf8ToGBK(stmt));
      }

      ret["msg"] = "ok";
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "character update error";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("character update error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::adminUpdate(
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

  Json::Value ret;

  try {
    auto stmt1 = fmt::format(
        "UPDATE [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblUserInformation] "
        "SET [_isValid] = '{}', [_userId] = N'{},{}', [_userNickname] = N'{}', "
        "[_isPcRoom] = {}, [_membershipType] = {} WHERE [_userNo] = {}",
        (*json)["isValid"].asString(), (*json)["username"].asString(),
        (*json)["password"].asString(), (*json)["userNickname"].asString(),
        (*json)["pcroom"].asInt64(), (*json)["membershipType"].asInt64(),
        (*json)["userNo"].asInt64());

    auto stmt2 = fmt::format(
        "UPDATE "
        "[SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblBriefUserInformation] SET "
        "[_userId] = N'{},{}', [_userNickname] = N'{}' WHERE [_userNo] = {}",
        (*json)["username"].asString(), (*json)["password"].asString(),
        (*json)["userNickname"].asString(), (*json)["userNo"].asInt64());

    auto stmt3 = fmt::format("SELECT COUNT(_userNo) FROM "
                             "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                             "TblBriefUserInformation] WHERE [_userNo] = {}",
                             (*json)["userNo"].asInt64());

    auto r3 = MsSql::exec(stmt3);
    r3.next();

    if (r3.get<int64_t>(0, 0) > 0) {
      auto r1 = MsSql::exec(stmt1);

      if (r1.affected_rows() >= 1) {
        auto r2 = MsSql::exec(stmt2);

        if (r2.affected_rows() >= 1) {
          ret["msg"] = "ok";
          ret["status"] = 1;
          callback(HttpResponse::newHttpJsonResponse(ret));
        } else {
          ret["msg"] = "admin update error";
          ret["status"] = 0;
          callback(HttpResponse::newHttpJsonResponse(ret));
        }
      } else {
        ret["msg"] = "admin update error";
        ret["status"] = 0;
        callback(HttpResponse::newHttpJsonResponse(ret));
      }
    } else {
      auto r = MsSql::exec(stmt1);

      if (r.affected_rows() >= 1) {
        ret["msg"] = "ok";
        ret["status"] = 1;
        callback(HttpResponse::newHttpJsonResponse(ret));
      } else {
        ret["msg"] = "admin update error";
        ret["status"] = 0;
        callback(HttpResponse::newHttpJsonResponse(ret));
      }
    }
  } catch (const std::exception &e) {
    spdlog::warn("admin update error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::adminCharacterUpdate(
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
      "UPDATE [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblCharacterInformation] "
      "SET [_characterName] = N'{}', [_currentPositionX] = {}, "
      "[_currentPositionY] = {}, [_currentPositionZ] = {}, [_returnPositionX] "
      "= {}, [_returnPositionY] = {}, [_returnPositionZ] = {}, [_level] = {}, "
      "[_experience] = {}, [_skillPointLevel] = {}, [_skillPointExperience] = "
      "{}, [_remainedSkillPoint] = {}, [_aquiredSkillPoint] = {}, [_tendency] "
      "= {}, [_hp] = {}, [_mp] = {}, [_sp] = {}, [_wp] = {}, "
      "[_inventorySlotCount] = {}, [_titleKey] = {}, [_killRewardCount] = {}, "
      "[_enchantFailCount] = {}, [_offenceValue] = {}, [_defenceValue] = {}, "
      "[_awakenValue] = {}, [_variedWeight] = {} WHERE [_characterNo] = {}",
      (*json)["characterName"].asString(),
      (*json)["currentPositionX"].asInt64(),
      (*json)["currentPositionY"].asInt64(),
      (*json)["currentPositionZ"].asInt64(),
      (*json)["currentPositionX"].asInt64(),
      (*json)["currentPositionY"].asInt64(),
      (*json)["currentPositionZ"].asInt64(), (*json)["level"].asInt64(),
      (*json)["experience"].asInt64(), (*json)["skillPointLevel"].asInt64(),
      (*json)["skillPointExperience"].asInt64(),
      (*json)["remainedSkillPoint"].asInt64(),
      (*json)["aquiredSkillPoint"].asInt64(), (*json)["tendency"].asInt64(),
      (*json)["hp"].asInt64(), (*json)["mp"].asInt64(), (*json)["sp"].asInt64(),
      (*json)["wp"].asInt64(), (*json)["inventorySlotCount"].asInt64(),
      (*json)["titleKey"].asInt64(), (*json)["killRewardCount"].asInt64(),
      (*json)["enchantFailCount"].asInt64(), (*json)["offenceValue"].asInt64(),
      (*json)["defenceValue"].asInt64(), (*json)["awakenValue"].asInt64(),
      (*json)["variedWeight"].asInt64(), (*json)["characterNo"].asInt64());

  Json::Value ret;

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      if (!(*json)["deletedDate"].asString().empty()) {
        stmt = fmt::format("UPDATE "
                           "[SA_BETA_GAMEDB_0002].[PaGamePrivate].["
                           "TblCharacterInformation] SET [_deletedDate] = '{}'",
                           timestamp);
        r = MsSql::exec(utf8ToGBK(stmt));
      }

      ret["msg"] = "ok";
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "character update error";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("character update error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::adminMail(
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

  std::string expirationDate_col = "[_expirationDate], ";
  std::string expirationDate_val = (*json)["expirationDate"].asString();
  if ((*json)["expirationDate"].asString() == "") {
    expirationDate_col = expirationDate_val = "";
  }

  auto stmt = fmt::format(
      "INSERT INTO [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblMail] "
      "({}[_registerDate], [_senderName], [_senderUserNo], [_receiverName], "
      "[_receiverUserNo], [_title], [_contents], [_mailType], [_variousNo], "
      "[_enchantLevel], [_itemCount], [_webItemType], [_chargeNo]) VALUES "
      "('{}', N'{}', {}, N'{}', {}, N'{}', N'{}', {}, {}, {}, {}, {}, ''",
      expirationDate_col, timestamp,
      (*json)["senderName"].asString() == "" ? "GM"
                                             : (*json)["senderName"].asString(),
      (*json)["senderUserNo"].asInt64(), (*json)["receiverName"].asString(),
      (*json)["receiverUserNo"].asInt64(), (*json)["title"].asString(),
      (*json)["contents"].asString(), (*json)["mailType"].asInt64(),
      (*json)["variousNo"].asInt64(), (*json)["enchantLevel"].asInt64(),
      (*json)["itemCount"].asInt64(), (*json)["webItemType"].asInt64());

  if ((*json)["expirationDate"].asString() != "") {
    stmt = fmt::format("{}, '{}'", stmt, expirationDate_val);
  }

  stmt = fmt::format("{})", stmt);

  Json::Value ret;

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      ret["msg"] = "ok";
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "admin mail add error";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("admin mail add error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::adminGmCount(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback,
                        const std::string type) const {
  Json::Value ret;

  auto stmt = fmt::format(
      "SELECT COUNT(_UserNo) FROM "
      "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember]");

  if (type != "ingame") {
    stmt = fmt::format(
        "SELECT COUNT(id) FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[account]");
  }

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();

    if (r.rows() > 0) {
      ret["msg"] = r.get<int64_t>(0, 0);
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "admin count error";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("admin count error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::adminGmInfo(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback,
                       const std::string type) const {
  Json::Value ret;

  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  if (type == "ingame") {
    auto stmt = fmt::format(
        "SELECT TOP {} * FROM "
        "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember] WHERE "
        "_UserNo NOT IN(SELECT TOP {} _UserNo FROM "
        "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember]",
        (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

    if ((*json)["sortBy"].asString() != "") {
      stmt = fmt::format("{} ORDER BY [{}] DESC) ORDER BY [{}]", stmt,
                         (*json)["sortBy"].asString(),
                         (*json)["sortBy"].asString());

      if ((*json)["descending"].asBool())
        stmt = fmt::format("{} DESC", stmt);
      else
        stmt = fmt::format("{} ASC", stmt);
    } else
      stmt = fmt::format("{})", stmt);

    try {
      auto r = MsSql::exec(utf8ToGBK(stmt));

      Json::Value infos;

      while (r.next()) {
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
      callback(HttpResponse::newHttpJsonResponse(ret));
    } catch (const std::exception &e) {
      spdlog::warn("admin info error: {}", e.what());
      ret["msg"] = e.what();
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } else {
    auto stmt = fmt::format(
        "SELECT TOP {} * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[account] "
        "WHERE id NOT IN(SELECT TOP {} id FROM "
        "[SA_BETA_WORLDDB_0002].[PaWebPublic].[account]",
        (*json)["maxPage"].asInt64(), (*json)["curPage"].asInt64());

    if ((*json)["sortBy"].asString() != "") {
      stmt = fmt::format("{} ORDER BY [{}] DESC) ORDER BY [{}]", stmt,
                         (*json)["sortBy"].asString(),
                         (*json)["sortBy"].asString());

      if ((*json)["descending"].asBool())
        stmt = fmt::format("{} DESC", stmt);
      else
        stmt = fmt::format("{} ASC", stmt);
    } else
      stmt = fmt::format("{})", stmt);

    try {
      auto r = MsSql::exec(utf8ToGBK(stmt));

      Json::Value infos;

      while (r.next()) {
        Json::Value info;
        info["id"] = r.get<std::string>("id", "");
        info["userno"] = r.get<std::string>("userNo", "");
        info["permission"] = r.get<std::string>("permission", "");
        info["create_date"] = r.get<std::string>("create_date", "");
        info["create_user"] = r.get<std::string>("create_user", "");
        info["update_date"] = r.get<std::string>("update_date", "");
        info["update_user"] = r.get<std::string>("update_user", "");
        info["delete"] = r.get<int>("delete", -1);
        infos.append(info);
      }

      ret["msg"] = infos;
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } catch (const std::exception &e) {
      spdlog::warn("admin info error: {}", e.what());
      ret["msg"] = e.what();
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  }
}

void User::adminGmUpdate(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string type) const {
  Json::Value ret;

  std::shared_ptr<Json::Value> json = req->getJsonObject();
  if (!json) {
    ret["msg"] = "error";
    ret["status"] = 0;
    return callback(HttpResponse::newHttpJsonResponse(ret));
  }

  auto now = std::chrono::system_clock::now();
  time_t time = std::chrono::system_clock::to_time_t(now);
  auto timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time));

  std::string stmt = "";

  switch (req->getMethod()) {
  case Put:
    stmt = fmt::format(
        "INSERT INTO "
        "[SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblRoleGroupMember] "
        "([_UserNo], [_roleGroupNo], [_macAddress], [_IpAddress], [_password]) "
        "VALUES('{}', '1', '{}', '127.0.0.1', '{}')",
        (*json)["userNo"].asInt64(), (*json)["mac"].asString(),
        (*json)["password"].asString());

    if (type != "ingame") {
      stmt = fmt::format(
          "INSERT INTO [SA_BETA_WORLDDB_0002].[PaWebPublic].[account] ([id], "
          "[userNo], [permission], [create_date], [create_user], "
          "[update_date], [update_user], [delete]) VALUES('{}', {}, '{}', "
          "'{}', '{}', '{}', '{}', 0)",
          uuidSimple(), (*json)["userNo"].asInt64(),
          (*json)["permission"].asString(), timestamp,
          (*json)["create_user"].asString(), timestamp,
          (*json)["update_user"].asString());
    }

    try {
      auto r = MsSql::exec(utf8ToGBK(stmt));

      if (r.affected_rows() >= 1) {
        ret["msg"] = "ok";
        ret["status"] = 1;
        callback(HttpResponse::newHttpJsonResponse(ret));
      } else {
        ret["msg"] = "admin gm update error";
        ret["status"] = 0;
        callback(HttpResponse::newHttpJsonResponse(ret));
      }
    } catch (const std::exception &e) {
      spdlog::warn("admin gm update error: {}", e.what());
      ret["msg"] = e.what();
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
    break;
  case Post:
    stmt = fmt::format("DELETE FROM "
                       "[SA_BETA_WORLDDB_0002].[PaGamePrivate].["
                       "TblRoleGroupMember] WHERE _UserNo = '{}'",
                       (*json)["userNo"].asInt64());

    if (type != "ingame") {
      stmt =
          fmt::format("UPDATE [SA_BETA_WORLDDB_0002].[PaWebPublic].[account] "
                      "SET [delete] = 1 WHERE id = '{}'",
                      (*json)["id"].asString());
    }

    try {
      auto r = MsSql::exec(utf8ToGBK(stmt));

      if (r.affected_rows() >= 1) {
        ret["msg"] = "ok";
        ret["status"] = 1;
        callback(HttpResponse::newHttpJsonResponse(ret));
      } else {
        ret["msg"] = "admin gm delete error";
        ret["status"] = 0;
        callback(HttpResponse::newHttpJsonResponse(ret));
      }
    } catch (const std::exception &e) {
      spdlog::warn("admin gm delete error: {}", e.what());
      ret["msg"] = e.what();
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
    break;
  default:
    ret["msg"] = "admin gm update no method";
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
    break;
  }
}

void User::blockChat(
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
      "INSERT INTO [SA_BETA_GAMEDB_0002].[PaGamePrivate].[TblBlockedChat] "
      "([_registerDate], [_userNo], [_endDate], [_reasonMemo]) VALUES ('{}', "
      "{}, '{}', N'{}')",
      timestamp, (*json)["userNo"].asInt64(), (*json)["endDate"].asString(),
      (*json)["reasonMemo"].asString());

  Json::Value ret;

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      ret["msg"] = "ok";
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "block chat add error";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("block chat add error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::blockUser(
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
      "INSERT INTO [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblBlockedUser] "
      "([_registerDate], [_userNo], [_endDate], [_blockCode], "
      "[_operationMemo]) VALUES ('{}', {}, '{}', {}, N'{}')",
      timestamp, (*json)["userNo"].asInt64(), (*json)["endDate"].asString(),
      (*json)["blockCode"].asInt64(), (*json)["reasonMemo"].asString());

  Json::Value ret;

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      ret["msg"] = "ok";
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "block user add error";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("block user add error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

void User::blockIp(
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
      "INSERT INTO [SA_BETA_WORLDDB_0002].[PaGamePrivate].[TblBlockedIP] "
      "([_registerDate], [_startIP], [_bigintStartIP], [_endIP], "
      "[_bigintEndIP], [_blockCode], [_operationMemo]) VALUES ('{}', N'{}', "
      "{}, N'{}', {}, {}, N'{}')",
      timestamp, (*json)["startIP"].asString(), (*json)["intStartIP"].asInt64(),
      (*json)["endIP"].asString(), (*json)["intEndIP"].asInt64(),
      (*json)["blockCode"].asInt64(), (*json)["operationMemo"].asString());

  Json::Value ret;

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));

    if (r.affected_rows() >= 1) {
      ret["msg"] = "ok";
      ret["status"] = 1;
      callback(HttpResponse::newHttpJsonResponse(ret));
    } else {
      ret["msg"] = "block ip add error";
      ret["status"] = 0;
      callback(HttpResponse::newHttpJsonResponse(ret));
    }
  } catch (const std::exception &e) {
    spdlog::warn("block ip add error: {}", e.what());
    ret["msg"] = e.what();
    ret["status"] = 0;
    callback(HttpResponse::newHttpJsonResponse(ret));
  }
}

Json::Value User::getPermission(const std::string userNo) const {
  auto stmt = fmt::format(
      "SELECT * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[account] WHERE "
      "[delete] = 0 AND userNo = '{}'",
      userNo);

  Json::Value ret = {};

  try {
    auto r = MsSql::exec(utf8ToGBK(stmt));
    r.next();

    if (r.rows() > 0) {
      stmt = fmt::format(
          "SELECT * FROM [SA_BETA_WORLDDB_0002].[PaWebPublic].[permissions] "
          "WHERE id = '{}'",
          r.get<std::string>("permission", ""));

      r = MsSql::exec(utf8ToGBK(stmt));
      r.next();

      if (r.rows() > 0) {
        auto json = r.get<std::string>("value", "");

        Json::Reader reader;
        reader.parse(json, ret);
      }
    }
  } catch (const std::exception &e) {
    spdlog::warn("get permission error: {}", e.what());
  }

  return ret;
}
} // namespace api