#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
	class Status : public HttpController<Status>
	{
	public:
		METHOD_LIST_BEGIN
			METHOD_ADD(Status::system, "/system", Options, Get, "api::JwtFilter");
		METHOD_ADD(Status::overview, "/overview", Options, Get, "api::JwtFilter");
		METHOD_LIST_END

	public:
		void system(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void overview(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;
	};
}
