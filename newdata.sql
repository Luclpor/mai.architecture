use archdb;
INSERT INTO Test (first_name,last_name,login,password,email,title) VALUES("Ivanw", "Konstantinovw","luclpor1337w", 12345, "luclpor1337w@gmail.com" ,"thisw");
INSERT INTO Test (first_name,last_name,login,password,email,title) VALUES("Ivanq", "Konstantinovq","luclpor1337q", 12345, "luclpor1337q@gmail.com" ,"thisq");
INSERT INTO Test (first_name,last_name,login,password,email,title) VALUES("Ivane", "Konstantinove","luclpor1337e", 12345, "luclpor1337e@gmail.com" ,"thise");
INSERT INTO Test (first_name,last_name,login,password,email,title) VALUES("Ivanr", "Konstantinovr","luclpor1337r", 12345, "luclpor1337r@gmail.com" ,"thisr");
INSERT INTO Test (first_name,last_name,login,password,email,title) VALUES("Ivant", "Konstantinovt","luclpor1337t", 12345, "luclpor1337t@gmail.com" ,"thist");
INSERT INTO Service (service_name,category,description_service, executor_id, portfolio, price) VALUES("read", "learn", "readtext",1,"d" ,1000);
INSERT INTO Service (service_name,category,description_service, executor_id, portfolio, price) VALUES("write", "learn", "writetext",1,"yes",1000);
INSERT INTO Service (service_name,category,description_service, executor_id, portfolio, price) VALUES("wash", "learn", "read text",3,"d",1000);
INSERT INTO Service (service_name,category,description_service, executor_id, portfolio, price) VALUES("clean", "learn", "read text",4,"yes",1000);
INSERT INTO Service (service_name,category,description_service, executor_id, portfolio, price) VALUES("play", "learn", "read text",5,"d",1000);
INSERT INTO Service (service_name,category,description_service, executor_id, portfolio, price) VALUES("sing", "learn", "read text",2,"",1000);
INSERT INTO Service (service_name,category,description_service, executor_id, portfolio, price) VALUES("jump", "learn", "read text",2,"",1000);
INSERT INTO Orders (order_name, category, service_id) VALUES("one","pets",1);
INSERT INTO Orders (order_name, category, service_id) VALUES("one", "pets", 2);
INSERT INTO Orders (order_name, category, service_id) VALUES("two","home", 3);
INSERT INTO Orders (order_name, category, service_id) VALUES("three", "nohome", 4);
INSERT INTO Orders (order_name, category, service_id) VALUES("four", "game",5);


