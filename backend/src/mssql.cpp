#include "stdafx.h"
#include "MsSql.h"

namespace api
{
	static nanodbc::connection odbc;

	bool MsSql::connect(std::string datasource, std::string username, std::string password)
	{
		try
		{
			odbc = nanodbc::connection(fmt::format("DSN={};UID={};PWD={};Mars_Connection=yes;", datasource, username, password), 15);
			return odbc.connected();
		}
		catch (const std::exception& e)
		{
			spdlog::error(e.what());
			return false;
		}
	}

	void MsSql::close()
	{
		try
		{
			odbc.disconnect();
		}
		catch (const std::exception& e)
		{
			spdlog::error(e.what());
		}
	}

	nanodbc::result MsSql::exec(std::string stmt)
	{
		return nanodbc::execute(odbc, stmt, 1, 15);
	}
}