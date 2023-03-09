#include "stdafx.h"
#include "filter.h"
#include "httplib.h"
#include "jwt.h"
#include "utility.h"

namespace api
{
	void LogFilter::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb)
	{
		std::string params = "";
		std::shared_ptr<Json::Value> json = req->getJsonObject();

		if (json)
		{
			params = (*json).toStyledString();
		}

		std::string path_fix = "";

		for (auto i : req->getParameters())
		{
			if (i == *req->getParameters().begin())
			{
				path_fix += fmt::format("?{}={}", i.first, i.second);
			}
			else
			{
				path_fix += fmt::format("&{}={}", i.first, i.second);
			}
		}

		spdlog::info("{} {} [{}] {}{} {}", req->getPeerAddr().toIpPort(), req->getHeader("User-Agent"), req->getMethodString(), utf8ToGBK(req->getPath()), utf8ToGBK(path_fix), utf8ToGBK(params));

		fccb();
	}

	void JwtFilter::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb)
	{
		std::string params = "";
		std::shared_ptr<Json::Value> json = req->getJsonObject();

		if (json)
		{
			params = (*json).toStyledString();
		}

		std::string path_fix = "";

		for (auto i : req->getParameters())
		{
			if (i == *req->getParameters().begin())
			{
				path_fix += fmt::format("?{}={}", i.first, i.second);
			}
			else
			{
				path_fix += fmt::format("&{}={}", i.first, i.second);
			}
		}

		spdlog::info("{} {} [{}] {}{} {}", req->getPeerAddr().toIpPort(), req->getHeader("User-Agent"), req->getMethodString(), utf8ToGBK(req->getPath()), utf8ToGBK(path_fix), utf8ToGBK(params));

		if (req->getMethod() == HttpMethod::Options) fccb();

		std::string token = req->getHeader("Authorization");

		Json::Value ret;

		if (token.empty())
		{
			ret["msg"] = "no jwt token";
			ret["status"] = 0;

			return fcb(HttpResponse::newHttpJsonResponse(ret));
		}

		token = token.substr(7);

		std::map<std::string, any> jwtAttributes = jwt::decodeToken(token);
		if (jwtAttributes.empty())
		{
			ret["msg"] = "jwt token is invalid";
			ret["status"] = 0;

			return fcb(HttpResponse::newHttpJsonResponse(ret));
		}

		for (auto& attribute : jwtAttributes)
			req->getAttributes()->insert("jwt_" + attribute.first, attribute.second);

		fccb();
	}

	void KeyFilter::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb)
	{
		std::string params = "";
		std::shared_ptr<Json::Value> json = req->getJsonObject();

		if (json)
		{
			params = (*json).toStyledString();
		}

		std::string path_fix = "";

		for (auto i : req->getParameters())
		{
			if (i == *req->getParameters().begin())
			{
				path_fix += fmt::format("?{}={}", i.first, i.second);
			}
			else
			{
				path_fix += fmt::format("&{}={}", i.first, i.second);
			}
		}

		spdlog::info("{} {} [{}] {}{} {}", req->getPeerAddr().toIpPort(), req->getHeader("User-Agent"), req->getMethodString(), utf8ToGBK(req->getPath()), utf8ToGBK(path_fix), utf8ToGBK(params));

		if (req->getMethod() == HttpMethod::Options) fccb();

		std::string token = req->getHeader("Authorization");

		Json::Value ret;

		if (token.empty())
		{
			ret["msg"] = "no key token";
			ret["status"] = 0;

			return fcb(HttpResponse::newHttpJsonResponse(ret));
		}
		else if (token != app().getCustomConfig()["access_token"].asString())
		{
			ret["msg"] = "key token is invalid";
			ret["status"] = 0;

			return fcb(HttpResponse::newHttpJsonResponse(ret));
		}

		fccb();
	}

	void CasdoorFilter::doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb)
	{
		std::string params = "";
		std::shared_ptr<Json::Value> json = req->getJsonObject();

		if (json)
		{
			params = (*json).toStyledString();
		}

		std::string path_fix = "";

		for (auto i : req->getParameters())
		{
			if (i == *req->getParameters().begin())
			{
				path_fix += fmt::format("?{}={}", i.first, i.second);
			}
			else
			{
				path_fix += fmt::format("&{}={}", i.first, i.second);
			}
		}

		spdlog::info("{} {} [{}] {}{} {}", req->getPeerAddr().toIpPort(), req->getHeader("User-Agent"), req->getMethodString(), utf8ToGBK(req->getPath()), utf8ToGBK(path_fix), utf8ToGBK(params));

		if (req->getMethod() == HttpMethod::Options) fccb();

		std::string token = req->getHeader("Authorization");

		if (token.empty())
		{
			Json::Value ret;
			ret["msg"] = "no proxy auth token";
			ret["status"] = 0;

			return fcb(HttpResponse::newHttpJsonResponse(ret));
		}

		httplib::Client client("http://106.58.165.187:51530");
		httplib::Headers headers = {
			{"Accept-Encoding", "gzip, deflate"},
			{"User-Agent", "CaNplay/1.0"},
			{"Authorization", token},
		};
		client.set_connection_timeout(180);
		client.set_read_timeout(180);
		client.set_write_timeout(180);

		if (auto res = client.Post("/api/auth/validate", headers, "", "application/json"))
		{
			if (res->status == 200)
			{
				Json::Reader reader;
				Json::Value body;
				if (reader.parse(res->body, body))
				{
					if (body["msg"].asString() == "ok")
					{
						fccb();
					}
					else
					{
						Json::Value ret;
						ret["msg"] = "proxy auth token is invalid";
						ret["status"] = 0;
						fcb(HttpResponse::newHttpJsonResponse(ret));
					}
				}
			}
		}
		else
		{
			Json::Value ret;
			ret["msg"] = "validate proxy auth token error";
			ret["status"] = 0;
			fcb(HttpResponse::newHttpJsonResponse(ret));
		}
	}
}