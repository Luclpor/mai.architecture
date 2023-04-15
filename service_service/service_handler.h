#ifndef USEHANDLER_H
#define USEHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
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
#include "Poco/JSON/Object.h"
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
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/SharedPtr.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <Poco/URIStreamFactory.h>
#include <Poco/URIStreamOpener.h>
#include <Poco/Net/HTTPSStreamFactory.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include <Poco/Base64Encoder.h>
#include <Poco/Base64Decoder.h>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::NameValueCollection;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../database/service.h"
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
class ServiceHandler : public HTTPRequestHandler
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

    bool check_category(const std::string &category, std::string &reason)
    {
        if (category.find(' ') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        if (category.find('\t') != std::string::npos)
        {
            reason = "EMail can't contain spaces";
            return false;
        }

        return true;
    };

public:
    ServiceHandler(const std::string &format) : _format(format)
    {
    }

    std::optional<std::string> do_get(const std::string &url, const std::string &login, const std::string &password)
    {
        std::string string_result;
        try
        {
            std::cout << "here1"<< std::endl;
            std::string token = login + ":" + password;
            std::ostringstream os;
            Poco::Base64Encoder b64in(os);
            b64in << token;
            b64in.close();
            std::string identity = "Basic " + os.str();
            std::cout << "here2"<< std::endl;

            Poco::URI uri(url);
            Poco::Net::HTTPClientSession s(uri.getHost(), uri.getPort());
            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.toString());
            request.setVersion(Poco::Net::HTTPMessage::HTTP_1_1);
            request.setContentType("application/json");
            request.set("Authorization", identity);
            request.set("Accept", "application/json");
            request.setKeepAlive(true);

            std::cout << "here3"<< std::endl;
            s.sendRequest(request);
            std::cout << "here4"<< std::endl;

            Poco::Net::HTTPResponse response;
            std::istream &rs = s.receiveResponse(response);

            while (rs)
            {
                char c{};
                rs.read(&c, 1);
                if (rs)
                    string_result += c;
            }

            if (response.getStatus() != 200)
                return {};
        }
        catch (Poco::Exception &ex)
        {
            std::cout << "exception:" << ex.what() << std::endl<< ex.displayText() << std::endl;;
            return std::optional<std::string>();
        }

        
        
        return string_result;
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            HTMLForm form(request, request.stream());
            std::string scheme;
            std::string info;
            std::string login, password;

            request.getCredentials(scheme, info);
            if (scheme == "Basic")
            {
                get_identity(info, login, password);
                std::cout << "login:" << login << std::endl;
                std::cout << "password:" << password << std::endl;
                std::string host = "localhost";
                std::string url;

                if(std::getenv("SERVICE_HOST")!=nullptr) host = std::getenv("SERVICE_HOST");
                url = "http://" + host+":8082/auth";
                std::cout << url<< std::endl;
                if (auto  str = do_get(url, login, password)) // do authentificate
                {

                    Poco::JSON::Parser parser;
                    Poco::Dynamic::Var result =parser.parse(str.value());
                    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                    userId = object->getValue<long>("id");
                    std::cout << "user"<< userId << std::endl;

                    /*response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("result", "some result");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);*/
                }
            }
            //если запрос get
            if (form.has("id") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long id = atol(form.get("id").c_str());

                std::optional<database::Service> result = database::Service::read_by_id(id);
                if (result)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify((result->toJSON()), ostr);
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
            else if (hasSubstr(request.getURI(), "/search"))
            {
                auto results = database::Service::read_all();
                Poco::JSON::Array arr;
                std::cout<<"heandlesearch"<< std::endl;
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
                if (form.has("service_name") && form.has("category") && form.has("description_service") && form.has("portfolio") && form.has("price"))
                {
                    database::Service service;
                    service.service_name() = form.get("service_name");
                    service.category() = form.get("category");
                    service.description_service() = form.get("description_service");
                    service.executor_id() = userId;
                    service.portfolio() = form.get("portfolio");
                    service.price() = atol(form.get("price").c_str());

                    bool check_result = true; 
                    std::string message;
                    std::string reason;

                    if (!check_name(service.get_service_name(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (!check_name(service.get_category(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (check_result)
                    {
                        std::cout<<"here"<< std::endl;
                        service.save_to_mysql();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << service.get_id();
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
    long userId;
};
#endif