#ifndef USEHANDLER_H
#define USEHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;
//using database::Order;

#include "../database/auth.h"
#include "../helper.h"

//сравнивает строки
static bool hasSubstr(const std::string &str, const std::string &substr)
{
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}

//принимает ссылку на запрос и ссылку на ответ. 
class AuthHandler : public HTTPRequestHandler
{
private:
    bool check_name(const std::string &name, std::string &reason)
    {
        if (name.length() < 3)
        {
            reason = "Name must be at leas 3 signs";
            return false;
        }

        if (name.find(' ') != std::string::npos)
        {
            reason = "Name can't contain spaces";
            return false;
        }

        if (name.find('\t') != std::string::npos)
        {
            reason = "Name can't contain spaces";
            return false;
        }

        return true;
    };

    bool check_email(const std::string &email, std::string &reason)
    {
        if (email.find('@') == std::string::npos)
        {
            reason = "Email must contain @";
            return false;
        }

        if (email.find(' ') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        if (email.find('\t') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        return true;
    };

public:
    AuthHandler(const std::string &format) : _format(format)
    {
    }

    Poco::JSON::Object::Ptr remove_password(Poco::JSON::Object::Ptr src)
    {
        if (src->has("password"))
            src->set("password", "*******");
        return src;
    }

    long get_user_id()
	{
		return user_id;
	}

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            //если запрос get
            if (form.has("id") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long id = atol(form.get("id").c_str());

                std::optional<database::Auth> result = database::Auth::read_by_id(id);
                if (result)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(remove_password(result->toJSON()), ostr);
                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_found");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "user ot found");
                    root->set("instance", "/user");
                    std::ostream &ostr = response.send(); //получает поток, в который можно записать данные в ответ на запрос.
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            else if (hasSubstr(request.getURI(), "/auth"))
            {
                std::cout << "here3.1"<< std::endl;
                std::string scheme;
                std::string info;
                request.getCredentials(scheme, info); //получить учетные данные
                std::cout << "scheme: " << scheme << " identity: " << info << std::endl;

                std::string login, password;
                if (scheme == "Basic")
                {
                    std::cout << "here3.2"<< std::endl;
                    get_identity(info, login, password);
                    if (auto id = database::Auth::auth(login, password))
                    {
                        

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << "{ \"id\" : \"" << *id << "\"}" << std::endl;
                        return;
                    }
                }

                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("type", "/errors/unauthorized");
                root->set("title", "Internal exception");
                root->set("status", "401");
                root->set("detail", "not authorized");
                root->set("instance", "/auth");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
            else if (hasSubstr(request.getURI(), "/search"))
            {
                std::string fn = form.get("first_name");
                std::string ln = form.get("last_name");
                auto results = database::Auth::search(fn, ln);
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(remove_password(s.toJSON()));//добавляет значение в массив
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr); //Записывает строковое представление значения в выходной поток.

                return;
            }
            else if (hasSubstr(request.getURI(), "/loginsearch"))
            {
                std::string lg = form.get("login");
                auto results = database::Auth::login_search(lg);
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(s.toJSON());//добавляет значение в массив
                
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr); //Записывает строковое представление значения в выходной поток.

                return;
            }
            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)//если запрос post
            {
                if (form.has("first_name") && form.has("last_name") && form.has("email") && form.has("title") && form.has("login") && form.has("password"))
                {
                    database::Auth auth;
                    auth.first_name() = form.get("first_name");
                    auth.last_name() = form.get("last_name");
                    auth.email() = form.get("email");
                    auth.title() = form.get("title");
                    auth.login() = form.get("login");
                    auth.password() = form.get("password");

                    bool check_result = true;
                    std::string message;
                    std::string reason;

                    if (!check_name(auth.get_first_name(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (!check_name(auth.get_last_name(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (!check_email(auth.get_email(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (check_result)
                    {
                        auth.save_to_mysql();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << auth.get_id();
                        return;
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
                        return;
                    }
                }
            }
        }
        catch (...)
        {
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request ot found");
        root->set("instance", "/user");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
    long user_id;
};
#endif