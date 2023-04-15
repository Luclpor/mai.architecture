#include "auth.h"
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
    void Auth::init()
    {
        try
        {
            //get() возвращает объект класса Database
            Poco::Data::Session session = database::Database::get().create_session();
            //вызов конструктора
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Test` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`first_name` VARCHAR(256) NOT NULL,"
                        << "`last_name` VARCHAR(256) NOT NULL,"
                        << "`login` VARCHAR(256) NOT NULL UNIQUE,"
                        << "`password` VARCHAR(256) NOT NULL,"
                        << "`email` VARCHAR(256) NULL,"
                        << "`title` VARCHAR(1024) NULL,"
                        << "PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));",
                now;
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

    Poco::JSON::Object::Ptr Auth::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("first_name", _first_name);
        root->set("last_name", _last_name);
        root->set("email", _email);
        root->set("title", _title);
        root->set("login", _login);
        root->set("password", _password);

        return root;
    }

    Auth Auth::fromJSON(const std::string &str)
    {
        Auth auth;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        auth.id() = object->getValue<long>("id");
        auth.first_name() = object->getValue<std::string>("first_name");
        auth.last_name() = object->getValue<std::string>("last_name");
        auth.email() = object->getValue<std::string>("email");
        auth.title() = object->getValue<std::string>("title");
        auth.login() = object->getValue<std::string>("login");
        auth.password() = object->getValue<std::string>("password");

        return auth;
    }

    std::optional<long> Auth::auth(std::string &login, std::string &password)
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
    std::optional<Auth> Auth::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Auth a;
            select << "SELECT id, first_name, last_name, email, title,login,password FROM Test where id=?",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._title),
                into(a._login),
                into(a._password),
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

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    std::vector<Auth> Auth::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Auth> result;
            Auth a;
            select << "SELECT id, first_name, last_name, email, title, login, password FROM Test",
                into(a._id),
                into(a._first_name),
                into(a._last_name),
                into(a._email),
                into(a._title),
                into(a._login),
                into(a._password),
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
    }

    std::vector<Auth> Auth::search(std::string first_name, std::string last_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Auth> result;
            Auth a;
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
    }

    std::vector<Auth> Auth::login_search(std::string login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Auth> result;
            Auth a;
            select << "SELECT id, first_name, last_name, email, title FROM Test where login=?",
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
    }



    void Auth::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Test (first_name,last_name,email,title,login,password) VALUES(?, ?, ?, ?, ?, ?)",
                use(_first_name),
                use(_last_name),
                use(_email),
                use(_title),
                use(_login),
                use(_password);

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

    const std::string &Auth::get_login() const
    {
        return _login;
    }

    const std::string &Auth::get_password() const
    {
        return _password;
    }

    std::string &Auth::login()
    {
        return _login;
    }

    std::string &Auth::password()
    {
        return _password;
    }

    long Auth::get_id() const
    {
        return _id;
    }

    const std::string &Auth::get_first_name() const
    {
        return _first_name;
    }

    const std::string &Auth::get_last_name() const
    {
        return _last_name;
    }

    const std::string &Auth::get_email() const
    {
        return _email;
    }

    const std::string &Auth::get_title() const
    {
        return _title;
    }

    long &Auth::id()
    {
        return _id;
    }

    std::string &Auth::first_name()
    {
        return _first_name;
    }

    std::string &Auth::last_name()
    {
        return _last_name;
    }

    std::string &Auth::email()
    {
        return _email;
    }

    std::string &Auth::title()
    {
        return _title;
    }
}