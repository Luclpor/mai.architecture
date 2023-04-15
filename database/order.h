#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

using Poco::DateTime;

namespace database
{
    class Order{
        private:
            long _id;
            std::string _order_name;
            std::string _category;
            std::string _order_date;
            long _service_id;

        public:

            static Order fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_order_name() const;
            const std::string &get_category() const;
            const std::string &get_order_date() const;
            long             get_service_id() const;

            long&        id();
            std::string &order_name();
            std::string &category();
            std::string &order_date();
            long&        service_id();

            static void init();
            static std::optional<Order> read_by_id(long id);
            static std::optional<long> auth(std::string &login, std::string &password);
            static std::vector<Order> read_all();
            static std::vector<Order> search(std::string order_name);
            static std::optional<long>  search_name(std::string &order_name,long &executor_id);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif