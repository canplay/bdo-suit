#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
	class User : public HttpController<User>
	{
	public:
		METHOD_LIST_BEGIN
			METHOD_ADD(User::count, "/count", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::signup, "/signup", Options, Post, "api::LogFilter");
		METHOD_ADD(User::signin, "/signin", Options, Post, "api::LogFilter");
		METHOD_ADD(User::signout, "/signout", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::info, "/info", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::infoOne, "/info/{id}", Options, Get, "api::JwtFilter");
		METHOD_ADD(User::update, "/update/{type}", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::characterUpdate, "/character/update", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::mail, "/mail/{type}", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::adminUpdate, "/admin/update", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::adminCharacterUpdate, "/admin/character/update", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::adminMail, "/admin/mail", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::adminGmCount, "/admin/gm/count/{type}", Options, Get, "api::JwtFilter");
		METHOD_ADD(User::adminGmInfo, "/admin/gm/info/{type}", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::adminGmUpdate, "/admin/gm/update/{type}", Options, Put, Post, "api::JwtFilter");
		METHOD_ADD(User::blockChat, "/admin/blockchat", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::blockUser, "/admin/blockuser", Options, Post, "api::JwtFilter");
		METHOD_ADD(User::blockIp, "/admin/blockip", Options, Post, "api::JwtFilter");
		METHOD_LIST_END

	public:
		void count(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void signup(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void signin(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void signout(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void info(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void infoOne(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string& id) const;

		void mail(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string type) const;

		void update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string type) const;

		void characterUpdate(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void adminUpdate(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void adminCharacterUpdate(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void adminMail(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void adminGmCount(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string type) const;

		void adminGmInfo(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string type) const;

		void adminGmUpdate(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string type) const;

		void blockChat(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void blockUser(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

		void blockIp(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) const;

	private:
		Json::Value getPermission(const std::string userNo) const;
	};
}
