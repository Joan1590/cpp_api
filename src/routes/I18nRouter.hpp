#pragma once
#include "Router.hpp"
#include "../controllers/I18nController.hpp"

namespace Routes
{
  class I18nRouter : public Router
  {
  public:
    void register_routes(crow::App<Middlewares::JWTMiddleware> &app) override
    {
      // Initialize translations on router registration
      Controllers::I18nController::initializeTranslations();

      // Translate a key
      CROW_ROUTE(app, "/api/i18n/translate")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::I18nController::translate(req); });

      // Get available locales
      CROW_ROUTE(app, "/api/i18n/locales")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::I18nController::getLocales(req); });

      // Demo translations in all languages
      CROW_ROUTE(app, "/api/i18n/demo")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::I18nController::demo(req); });

      // Set locale
      CROW_ROUTE(app, "/api/i18n/locale")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::I18nController::setLocale(req); });
    }
  };
}
