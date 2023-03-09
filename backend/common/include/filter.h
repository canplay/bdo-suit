#pragma once

#include <drogon\HttpFilter.h>

using namespace drogon;

namespace api
{
	class LogFilter :public drogon::HttpFilter<LogFilter>
	{
	public:
		virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb) override;
	};

	class JwtFilter :public drogon::HttpFilter<JwtFilter>
	{
	public:
		virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb) override;
	};

	class KeyFilter :public drogon::HttpFilter<KeyFilter>
	{
	public:
		virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb) override;
	};

	class CasdoorFilter :public drogon::HttpFilter<CasdoorFilter>
	{
	public:
		virtual void doFilter(const HttpRequestPtr& req, FilterCallback&& fcb, FilterChainCallback&& fccb) override;
	};
}
