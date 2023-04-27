#include "order.h"
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
using Poco::DateTime;

namespace database
{

    void Order::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            //for (auto &hint : database::Database::get_all_hints())
            //{
                //Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Orders` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`order_name` VARCHAR(256) NOT NULL,"
                        << "`category` VARCHAR(256) NOT NULL,"
                        << "`service_id` INT NOT NULL,"
                        << "`order_date` DATETIME NOT NULL DEFAULT NOW(),"
                        << "PRIMARY KEY (`id`),"
						<< "FOREIGN KEY(`service_id`) REFERENCES `Service`(`id`));",
                       // <<  hint,

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

    Poco::JSON::Object::Ptr Order::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("order_name", _order_name);
        root->set("category", _category);
        root->set("service_id", _service_id);
        root->set("order_date",_order_date);
		
        return root;
    }

    Order Order::fromJSON(const std::string &str)
    {
        Order order;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        order.id() = object->getValue<long>("id");
        order.order_name() = object->getValue<std::string>("order_name");
        order.category() = object->getValue<std::string>("category");
        order.service_id() = object->getValue<long>("service_id");
        order.order_date() = object->getValue<long>("order_date");

        return order;
    }

    
    std::optional<Order> Order::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Order a;
            select << "SELECT id, order_name, category, service_id password FROM Orders where id=?",
                into(a._id),
                into(a._order_name),
                into(a._category),
                into(a._service_id),
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
    std::optional<long> Order::auth(std::string &login, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            //вызывается конструктор для класса statement
            Poco::Data::Statement select(session);
            long id;
            select << "SELECT id FROM Test where login=? and password=?",
                into(id),
                use(login),
                use(password),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return id;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }

    std::vector<Order> Order::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Order> result;
            Order a;
            select << "SELECT id, order_name, category, service_id FROM Orders",
                into(a._id),
                into(a._order_name),
                into(a._category),
                into(a._service_id),
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

            std::cout << "statement:" << e.what() << std::endl<< e.displayText() << std::endl;
            throw;
        }
    }

    std::vector<Order> Order::search(std::string order_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Order> result;
            Order a;
            //order_name += "%";
            select << "SELECT `id`, `order_name`, `category`, `service_id`,  CAST(`order_date` AS VARCHAR(30)) FROM `Orders` where `order_name`=?",
               into(a._id),
                into(a._order_name),
                into(a._category),
                into(a._service_id),
                into(a._order_date),
                use(order_name),
                range(0, 1); //  iterate over result set one row at a time
            std::cout << "here" << std::endl;
            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            std::cout << "here2" << std::endl;
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
        catch (std::exception &e)
        {
            std::cout << "exception: " << e.what() << std::endl;
            throw;
        }
    }

    std::optional<long> Order::search_name(std::string &order_name,long &executor_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Order> result;
            Order a;
            long id;
            select << "SELECT `id` FROM `Service` where `service_name` LIKE ? AND `service_id` = ?",
               into(id),
               use(order_name),
               use(executor_id),
               range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return id;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }

   void Order::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            std::cout << "save"<< std::endl;
            insert << "INSERT INTO `Orders` (`order_name`, `category`, `service_id`, `order_date`) VALUES(?, ?, ?, NOW())",
                use(_order_name),
                use(_category),
                use(_service_id);
                

            insert.execute();
            std::cout << "aftersave"<< std::endl;

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

            std::cout << "statement:" << e.what() << std::endl<< e.displayText() << std::endl;
            throw;
        }
    }

    const std::string &Order::get_order_name() const
    {
        return _order_name;
    }

    const std::string &Order::get_category() const
    {
        return _category;
    }

    const std::string &Order::get_order_date() const
    {
        return _order_date;
    }


    long Order::get_id() const
    {
        return _id;
    }

    long Order::get_service_id() const
    {
        return _service_id;
    }

    long &Order::id()
    {
        return _id;
    }

    std::string &Order::order_name()
    {
        return _order_name;
    }

    std::string &Order::category()
    {
        return _category;
    }

    std::string &Order::order_date()
    {
        return _order_date;
    }

    long &Order::service_id()
    {
        return _service_id;
    }
}