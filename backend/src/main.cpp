#include "mssql.h"
#include "pch.h"
#include "utility.h"

using namespace drogon;

int main() {
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;
  GetConsoleMode(hStdin, &mode);
  mode &= ~ENABLE_QUICK_EDIT_MODE;
  mode &= ~ENABLE_INSERT_MODE;
  mode &= ~ENABLE_MOUSE_INPUT;
  SetConsoleMode(hStdin, mode);

  if (!std::filesystem::exists("./logs"))
    std::filesystem::create_directory("./logs");

  spdlog::init_thread_pool(8192, 1);
  spdlog::flush_on(spdlog::level::warn);
  spdlog::enable_backtrace(32);
  auto out_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      "./logs/log.log", 1048576 * 10, 5);
  out_sink->set_level(spdlog::level::trace);
  file_sink->set_level(spdlog::level::trace);
  std::vector<spdlog::sink_ptr> sinks{out_sink, file_sink};
  auto logger = std::make_shared<spdlog::async_logger>(
      "", sinks.begin(), sinks.end(), spdlog::thread_pool(),
      spdlog::async_overflow_policy::block);
  spdlog::set_default_logger(logger);
  spdlog::set_error_handler(
      [](const std::string &msg) { spdlog::error("{}", msg); });

  if (!api::initCrashPad())
    spdlog::info("========== init crashpad error ==========");

  spdlog::info("========== start server ==========");

  app().loadConfigFile("./config.json");

  auto mssqlConfig = app().getCustomConfig()["mssql"];
  if (!api::MsSql::connect(mssqlConfig["host"].asString(),
                           mssqlConfig["user"].asString(),
                           mssqlConfig["passwd"].asString())) {
    spdlog::error("========== connect mssql error ==========");
    return 0;
  }

  Json::Value ret;
  ret["msg"] = "404";
  ret["status"] = 0;
  app().setCustom404Page(HttpResponse::newHttpJsonResponse(ret));

  app().registerPreSendingAdvice(
      [](const HttpRequestPtr &req, const HttpResponsePtr &resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "OPTIONS, POST, GET");
        resp->addHeader("Access-Control-Allow-Credentials", "true");
        resp->addHeader("Access-Control-Allow-Headers",
                        "Origin, Content-Type, Authorization, "
                        "X-Requested-With, X-Cookie, Accept");
        resp->addHeader("Access-Control-Max-Age", "3600");
      });

  app().run();

  api::MsSql::close();

  spdlog::shutdown();
  spdlog::info("========== stop server ==========");

  return 0;
}