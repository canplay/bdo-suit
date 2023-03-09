#pragma once

#include <nanodbc/nanodbc.h>

namespace api
{
	class MsSql
	{
	public:
		static bool connect(std::string datasource, std::string username, std::string password);
		static void close();

		static nanodbc::result exec(std::string stmt);
	};
}
