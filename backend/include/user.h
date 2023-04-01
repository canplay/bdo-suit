#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
	class User : public HttpController<User>
	{
	public:
		METHOD_LIST_BEGIN
			METHOD_ADD(User::count, "/count", Options, Get, "api::JwtFilter");
		METHOD_ADD(User::signup, "/signup", Options, Post, "api::LogFilter");
		METHOD_ADD(User::signin, "/signin", Options, Post, "api::LogFilter");
		METHOD_ADD(User::signout, "/signout", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::info, "/info", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::infoOne, "/info/{id}", Options, Get, "api::JwtFilter");
		METHOD_ADD(User::update, "/update", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::characterUpdate, "/character/update", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::mail, "/mail", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::adminCount, "/admin/count/{type}", Options, Get, "api::JwtFilter");
		METHOD_ADD(User::adminInfo, "/admin/info/{type}", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::adminUpdate, "/admin/update/{type}", Options, Put, Post, "api::JwtFilter");
		METHOD_ADD(User::blockChat, "/blockchat", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::blockUser, "/blockuser", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::blockIp, "/blockip", Options, Post, "api::JwtFilter");
		METHOD_LIST_END

	public:
		void count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void signup(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void signin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void signout(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void infoOne(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string& id) const;

		void update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void characterUpdate(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void mail(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void adminCount(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string type) const;

		void adminInfo(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string type) const;

		void adminUpdate(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string type) const;

		void blockChat(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void blockUser(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void blockIp(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

	private:
		Json::Value getPermission(const std::string userNo) const;
	};
}
