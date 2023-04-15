#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Service{
        private:
            long _id;
            std::string _service_name;
            std::string _category;
            std::string _description_service;
            long _executor_id;
            std::string _portfolio;
            std::string _password;
            long _price;

        public:

            static Service fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_service_name() const;
            const std::string &get_category() const;
            const std::string &get_description_service() const;
            long               get_executor_id() const;
            const std::string &get_portfolio() const;
            const std::string &get_password() const;
            long               get_price() const;

            long&        id();
            std::string &service_name();
            std::string &category();
            std::string &description_service();
            long&        executor_id();
            std::string &portfolio();
            long&        price();

            static void init();
            static std::optional<Service> read_by_id(long id);
            static std::optional<long> auth(std::string &login, std::string &password);
            static std::vector<Service> read_all();
            static std::vector<Service> search(std::string first_name,std::string last_name);
            static std::vector<Service> login_search(std::string login);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif