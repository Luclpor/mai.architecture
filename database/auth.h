#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Auth{
        private:
            long _id;
            std::string _first_name;
            std::string _last_name;
            std::string _email;
            std::string _title;
            std::string _login;
            std::string _password;

        public:

            static Auth fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_first_name() const;
            const std::string &get_last_name() const;
            const std::string &get_email() const;
            const std::string &get_title() const;
            const std::string &get_login() const;
            const std::string &get_password() const;

            long&        id();
            std::string &first_name();
            std::string &last_name();
            std::string &email();
            std::string &title();
            std::string &login();
            std::string &password();

            static void init();
            static std::optional<Auth> read_by_id(long id);
            static std::optional<long> auth(std::string &login, std::string &password);
            static std::vector<Auth> read_all();
            static std::vector<Auth> search(std::string first_name,std::string last_name);
            static std::vector<Auth> login_search(std::string login);
            void save_to_mysql();

            void save_to_cache();
            static std::optional<Auth> read_from_cache_by_id(long id);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif