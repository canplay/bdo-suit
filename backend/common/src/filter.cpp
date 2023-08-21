#include "filter.h"
#include "jwt.h"
#include "pch.h"
#include "utility.h"

namespace api {
void LogFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb,
                         FilterChainCallback &&fccb) {
  std::string params = "";
  std::shared_ptr<Json::Value> json = req->getJsonObject();

  if (json) {
    params = (*json).toStyledString();
  }

  std::string path_fix = "";

  for (auto i : req->getParameters()) {
    if (i == *req->getParameters().begin()) {
      path_fix += fmt::format("?{}={}", i.first, i.second);
    } else {
      path_fix += fmt::format("&{}={}", i.first, i.second);
    }
  }

  spdlog::info("{} {} [{}] {}{} {}", req->getPeerAddr().toIpPort(),
               req->getHeader("User-Agent"), req->getMethodString(),
               utf8ToGBK(req->getPath()), utf8ToGBK(path_fix),
               utf8ToGBK(params));

  fccb();
}

void JwtFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb,
                         FilterChainCallback &&fccb) {
  std::string params = "";
  std::shared_ptr<Json::Value> json = req->getJsonObject();

  if (json) {
    params = (*json).toStyledString();
  }

  std::string path_fix = "";

  for (auto i : req->getParameters()) {
    if (i == *req->getParameters().begin()) {
      path_fix += fmt::format("?{}={}", i.first, i.second);
    } else {
      path_fix += fmt::format("&{}={}", i.first, i.second);
    }
  }

  spdlog::info("{} {} [{}] {}{} {}", req->getPeerAddr().toIpPort(),
               req->getHeader("User-Agent"), req->getMethodString(),
               utf8ToGBK(req->getPath()), utf8ToGBK(path_fix),
               utf8ToGBK(params));

  if (req->getMethod() == HttpMethod::Options)
    fccb();

  std::string token = req->getHeader("Authorization");

  Json::Value ret;

  if (token.empty()) {
    ret["msg"] = "no jwt token";
    ret["status"] = 0;

    return fcb(HttpResponse::newHttpJsonResponse(ret));
  }

  token = token.substr(7);

  std::map<std::string, any> jwtAttributes = jwt::decodeToken(token);
  if (jwtAttributes.empty()) {
    ret["msg"] = "jwt token is invalid";
    ret["status"] = 0;

    return fcb(HttpResponse::newHttpJsonResponse(ret));
  }

  for (auto &attribute : jwtAttributes)
    req->getAttributes()->insert("jwt_" + attribute.first, attribute.second);

  fccb();
}

void KeyFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb,
                         FilterChainCallback &&fccb) {
  std::string params = "";
  std::shared_ptr<Json::Value> json = req->getJsonObject();

  if (json) {
    params = (*json).toStyledString();
  }

  std::string path_fix = "";

  for (auto i : req->getParameters()) {
    if (i == *req->getParameters().begin()) {
      path_fix += fmt::format("?{}={}", i.first, i.second);
    } else {
      path_fix += fmt::format("&{}={}", i.first, i.second);
    }
  }

  spdlog::info("{} {} [{}] {}{} {}", req->getPeerAddr().toIpPort(),
               req->getHeader("User-Agent"), req->getMethodString(),
               utf8ToGBK(req->getPath()), utf8ToGBK(path_fix),
               utf8ToGBK(params));

  if (req->getMethod() == HttpMethod::Options)
    fccb();

  std::string token = req->getHeader("Authorization");

  Json::Value ret;

  if (token.empty()) {
    ret["msg"] = "no key token";
    ret["status"] = 0;

    return fcb(HttpResponse::newHttpJsonResponse(ret));
  } else if (token != app().getCustomConfig()["access_token"].asString()) {
    ret["msg"] = "key token is invalid";
    ret["status"] = 0;

    return fcb(HttpResponse::newHttpJsonResponse(ret));
  }

  fccb();
}
} // namespace api