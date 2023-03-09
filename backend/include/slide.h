#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
	class Slide : public HttpController<Slide>
	{
	public:
		METHOD_LIST_BEGIN
			METHOD_ADD(Slide::count, "/count", Options, Get, "api::JwtFilter");
		METHOD_ADD(Slide::add, "/add", Options, Post, "api::JwtFilter");
		METHOD_ADD(Slide::update, "/update", Options, Post, "api::JwtFilter");
		METHOD_ADD(Slide::info, "/info", Options, Post, "api::LogFilter");
		METHOD_LIST_END

	public:
		void count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void add(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;
	};
}
