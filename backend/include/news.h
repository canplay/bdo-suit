#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
	class News : public HttpController<News>
	{
	public:
		METHOD_LIST_BEGIN
			METHOD_ADD(News::count, "/count", Options, Get, "api::JwtFilter");
		METHOD_ADD(News::add, "/add", Options, Post, "api::JwtFilter");
		METHOD_ADD(News::update, "/update", Options, Post, "api::JwtFilter");
		METHOD_ADD(News::info, "/info", Options, Post, "api::JwtFilter");
		METHOD_ADD(News::latest, "/latest", Options, Get, "api::LogFilter");
		METHOD_LIST_END

	public:
		void count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void add(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void latest(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;
	};
}
