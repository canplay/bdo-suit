#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
	class Version : public HttpController<Version>
	{
	public:
		METHOD_LIST_BEGIN
			METHOD_ADD(Version::count, "/count", Options, Get, "api::JwtFilter");
		METHOD_ADD(Version::add, "/add", Options, Post, "api::JwtFilter");
		METHOD_ADD(Version::update, "/update", Options, Post, "api::JwtFilter");
		METHOD_ADD(Version::info, "/info", Options, Post, "api::JwtFilter");
		METHOD_ADD(Version::latest, "/latest", Options, Get, "api::LogFilter");
		METHOD_ADD(Version::diffStatus, "/diff", Options, Get, "api::LogFilter");
		METHOD_LIST_END

	public:
		void count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void add(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

		void update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void latest(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void diffStatus(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

	private:
		void setDiffStatus(std::string val)
		{
			m_diffStatus = val;
		}

	private:
		std::string m_diffStatus;
	};
}
