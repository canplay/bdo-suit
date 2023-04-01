#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
	class Items : public HttpController<Items>
	{
	public:
		METHOD_LIST_BEGIN
			METHOD_ADD(Items::count, "/count", Options, Post, "api::JwtFilter");
		METHOD_ADD(Items::info, "/info", Options, Post, "api::LogFilter");
		METHOD_LIST_END

	public:
		void count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;
	};
}
