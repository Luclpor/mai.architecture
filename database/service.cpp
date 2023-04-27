#include "service.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{
    //создает сеанс с бд, создает таблицу
    void Service::init()
    {
        try
        {
            //get() возвращает объект класса Database
            Poco::Data::Session session = database::Database::get().create_session();
            //вызов конструктора
            //for (auto &hint : database::Database::get_all_hints())
            //{
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Service` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`service_name` VARCHAR(256) NOT NULL UNIQUE,"
                        << "`category` VARCHAR(256) NOT NULL,"
                        << "`description_service` VARCHAR(256) NOT NULL,"
                        << "`executor_id` INT NOT NULL,"
                        << "`portfolio` VARCHAR(256) DEFAULT '',"
                        << "`price` INT NOT NULL,"
                        << "PRIMARY KEY (`id`),KEY `sn` (`service_name`),KEY `ct` (`category`),"
                        << "FOREIGN KEY (`executor_id`) REFERENCES `Test`(`id`) ON DELETE CASCADE);",
                        //<<  hint,
                now;
            //std::cout << create_stmt.toString() << std::endl;
            //}
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl<< e.displayText() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Service::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("service_name", _service_name);
        root->set("category", _category);
        root->set("description_service", _description_service);
        root->set("executor_id", _executor_id);
        root->set("portfolio", _portfolio);
        root->set("price",_price);

        return root;
    }

    Service Service::fromJSON(const std::string &str)
    {
        Service service;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        service.id() = object->getValue<long>("id");
        service.service_name() = object->getValue<std::string>("service_name");
        service.category() = object->getValue<std::string>("category");
        service.description_service() = object->getValue<std::string>("description_service");
        service.executor_id() = object->getValue<long>("executor_id");
        service.portfolio() = object->getValue<std::string>("portfolio");
        service.price() = object->getValue<long>("price");

        return service;
    }

    std::optional<Service> Service::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Service a;
            select << "SELECT id, service_name, category, description_service, executor_id,portfolio FROM Test where id=?",
                into(a._id),
                into(a._service_name),
                into(a._category),
                into(a._description_service),
                into(a._executor_id),
                into(a._portfolio),
                into(a._price),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl<< e.displayText() << std::endl;
            
        }
        return {};
    }

    std::vector<Service> Service::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Service> result;
            Service a;
            select <<  "SELECT id, service_name, category, description_service, executor_id, portfolio, price FROM Service",
            //for(int i=0;i<2;++i){
            //Statement select(session);
            //select << select_str,
                into(a._id),
                into(a._service_name),
                into(a._category),
                into(a._description_service),
                into(a._executor_id),
                into(a._portfolio),
                into(a._price),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            
            {
                if (select.execute())
                std::cout<<"read_all"<< std::endl;
                    result.push_back(a);
            }
            //std::string test = "-- sharding:1";
            // select_str += test;
           // }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl<< e.displayText() << std::endl;
            throw;
        }
    }

    /*std::vector<Service> Service::search(std::string first_name, std::string last_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Service> result;
            Service a;
            first_name += "%";
            last_name += "%";
            select << "SELECT id, first_name, last_name, email, title, login, password FROM Test where first_name LIKE ? and last_name LIKE ?",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._title),
                into(a._login),
                into(a._password),
                use(first_name),
                use(last_name),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }*/

    /*std::vector<Service> Service::login_search(std::string login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Service> result;
            Service a;
            login += "%";
            select << "SELECT id, first_name, last_name, email, title FROM Test where login LIKE ?",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._title),
                //into(a._login),
                //into(a._password),
                use(login),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }*/



    void Service::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            //std::string sharding_hint = database::Database::sharding_hint_service(_executor_id);

            insert << "INSERT INTO Service (service_name,category,description_service, executor_id, portfolio, price) VALUES(?, ?, ?, ?, ?, ?)",
            //select_str += sharding_hint;
            //std::cout << select_str << std::endl;
            //insert << select_str,
                use(_service_name),
                use(_category),
                use(_description_service),
                use(_executor_id),
                use(_portfolio),
                use(_price);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }



    long Service::get_id() const
    {
        return _id;
    }

    const std::string &Service::get_portfolio() const {
        return _portfolio;
    }

    const std::string &Service::get_service_name() const
    {
        return _service_name;
    }

    const std::string &Service::get_category() const
    {
        return _category;
    }

    const std::string &Service::get_description_service() const
    {
        return _description_service;
    }

    long Service::get_executor_id() const
    {
        return _executor_id;
    }

    long  Service::get_price() const
    {
        return _price;
    }

    long &Service::id()
    {
        return _id;
    }

    std::string &Service::service_name()
    {
        return _service_name;
    }

    std::string &Service::category()
    {
        return _category;
    }

    std::string &Service::description_service()
    {
        return _description_service;
    }

    long &Service::executor_id()
    {
        return _executor_id;
    }

    std::string &Service::portfolio()
    {
        return _portfolio;
    }

    long &Service::price()
    {
        return _price;
    }
}