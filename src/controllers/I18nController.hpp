#pragma once
#include "crow.h"
#include "BaseController.hpp"
#include "../i18n/I18n.hpp"

namespace Controllers
{
  class I18nController : public BaseController
  {
  public:
    // Initialize default translations
    static void initializeTranslations()
    {
      // English translations
      I18n::Translator::load("en", {{"messages", {{"welcome", "Welcome to our API"}, {"goodbye", "Goodbye!"}, {"user_created", "User :name has been created successfully"}, {"user_not_found", "User not found"}, {"validation_error", "Validation error occurred"}, {"server_error", "Internal server error"}, {"unauthorized", "You are not authorized to perform this action"}}},
                                    {"errors", {{"required", "The :field field is required"}, {"email", "The :field must be a valid email address"}, {"min_length", "The :field must be at least :min characters"}, {"max_length", "The :field must not exceed :max characters"}}}});

      // Spanish translations
      I18n::Translator::load("es", {{"messages", {{"welcome", "Bienvenido a nuestra API"}, {"goodbye", "¡Adiós!"}, {"user_created", "El usuario :name ha sido creado exitosamente"}, {"user_not_found", "Usuario no encontrado"}, {"validation_error", "Ocurrió un error de validación"}, {"server_error", "Error interno del servidor"}, {"unauthorized", "No está autorizado para realizar esta acción"}}},
                                    {"errors", {{"required", "El campo :field es requerido"}, {"email", "El campo :field debe ser un correo electrónico válido"}, {"min_length", "El campo :field debe tener al menos :min caracteres"}, {"max_length", "El campo :field no debe exceder :max caracteres"}}}});

      // French translations
      I18n::Translator::load("fr", {{"messages", {{"welcome", "Bienvenue sur notre API"}, {"goodbye", "Au revoir!"}, {"user_created", "L'utilisateur :name a été créé avec succès"}, {"user_not_found", "Utilisateur non trouvé"}, {"validation_error", "Une erreur de validation s'est produite"}, {"server_error", "Erreur interne du serveur"}, {"unauthorized", "Vous n'êtes pas autorisé à effectuer cette action"}}},
                                    {"errors", {{"required", "Le champ :field est requis"}, {"email", "Le champ :field doit être une adresse email valide"}, {"min_length", "Le champ :field doit contenir au moins :min caractères"}, {"max_length", "Le champ :field ne doit pas dépasser :max caractères"}}}});

      // Portuguese translations
      I18n::Translator::load("pt", {{"messages", {{"welcome", "Bem-vindo à nossa API"}, {"goodbye", "Adeus!"}, {"user_created", "O usuário :name foi criado com sucesso"}, {"user_not_found", "Usuário não encontrado"}, {"validation_error", "Ocorreu um erro de validação"}, {"server_error", "Erro interno do servidor"}, {"unauthorized", "Você não está autorizado a realizar esta ação"}}},
                                    {"errors", {{"required", "O campo :field é obrigatório"}, {"email", "O campo :field deve ser um endereço de email válido"}, {"min_length", "O campo :field deve ter pelo menos :min caracteres"}, {"max_length", "O campo :field não deve exceder :max caracteres"}}}});
    }

    // Get translation
    static crow::response translate(const crow::request &req)
    {
      try
      {
        // Parse Accept-Language header or use query param
        std::string acceptLang = req.get_header_value("Accept-Language");
        std::string locale = I18n::Translator::parseAcceptLanguage(acceptLang);

        // Allow locale override via query param
        auto localeParam = req.url_params.get("locale");
        if (localeParam)
        {
          locale = localeParam;
        }

        I18n::Translator::setLocale(locale);

        auto body = parse_body(req);
        std::string key = body.contains("key") ? body["key"].get<std::string>() : "";

        if (key.empty())
        {
          return bad_request("Translation key is required");
        }

        std::map<std::string, std::string> replacements;
        if (body.contains("replacements") && body["replacements"].is_object())
        {
          for (auto it = body["replacements"].begin(); it != body["replacements"].end(); ++it)
          {
            if (it.value().is_string())
            {
              replacements[it.key()] = it.value().get<std::string>();
            }
          }
        }

        std::string translated = I18n::Translator::t(key, replacements);

        json response = {
            {"locale", locale},
            {"key", key},
            {"translation", translated}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Get all available locales
    static crow::response getLocales(const crow::request &req)
    {
      try
      {
        json response = {
            {"current_locale", I18n::Translator::getLocale()},
            {"available_locales", I18n::Translator::getAvailableLocales()}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Demo endpoint showing translations in all languages
    static crow::response demo(const crow::request &req)
    {
      try
      {
        json response = json::object();

        auto locales = I18n::Translator::getAvailableLocales();
        for (const auto &locale : locales)
        {
          response[locale] = {
              {"welcome", I18n::Translator::getForLocale(locale, "messages.welcome")},
              {"goodbye", I18n::Translator::getForLocale(locale, "messages.goodbye")},
              {"user_created", I18n::Translator::getForLocale(locale, "messages.user_created", {{"name", "John"}})}};
        }

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Set locale
    static crow::response setLocale(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        if (!body.contains("locale") || !body["locale"].is_string())
        {
          return bad_request("Locale is required");
        }

        std::string locale = body["locale"].get<std::string>();
        I18n::Translator::setLocale(locale);

        json response = {
            {"message", "Locale updated successfully"},
            {"locale", locale}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }
  };
}
