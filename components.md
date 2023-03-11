# Компонентная архитектура
<!-- Состав и взаимосвязи компонентов системы между собой и внешними системами с указанием протоколов, ключевые технологии, используемые для реализации компонентов.
Диаграмма контейнеров C4 и текстовое описание. 
-->
## Компонентная диаграмма

```plantuml
@startuml
!include  https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml

AddElementTag("microService", $shape=EightSidedShape(), $bgColor="CornflowerBlue", $fontColor="white", $legendText="microservice")
AddElementTag("storage", $shape=RoundedBoxShape(), $bgColor="lightSkyBlue", $fontColor="white")

Person(admin, "Администратор")
Person(moderator, "Модератор")
Person(user, "Пользователь")

System_Ext(web_site, "Клиентский веб-сайт", "HTML, CSS, JavaScript", $tags = "web_site")

System_Boundary(service_site, "Сайт заказа услуг") {
   Container(client_service, "Сервис авторизации", "C++", "Сервис управления пользователями", $tags = "microService")    
   Container(post_service, "Сервис услуг", "C++", "Сервис управления услугами", $tags = "microService") 
   Container(blog_service, "Сервис зазаков", "C++", "Сервис управления заказами", $tags = "microService")   
   ContainerDb(db, "База данных", "MySQL", "Хранение данных о услугах, заказах  и пользователях", $tags = "storage")
   
}

Rel(admin, web_site, "Просмотр, добавление и редактирование информации о пользователях, услугах и заказах")
Rel(moderator, web_site, "Модерация услуг, заказов и пользователей")
Rel(user, web_site, "Регистрация, просмотр услуг , создание заказа и услуги")

Rel(web_site, client_service, "Работа с пользователями", "localhost/person")
Rel(client_service, db, "INSERT/SELECT/UPDATE", "SQL")

Rel(web_site, post_service, "Работа с услугами", "localhost/pres")
Rel(post_service, db, "INSERT/SELECT/UPDATE", "SQL")

Rel(web_site, blog_service, "Работа с заказами ", "localhost/conf")
Rel(blog_service, db, "INSERT/SELECT/UPDATE", "SQL")

@enduml
```
## Список компонентов  

### Сервис авторизации
**API**:
-	Создание нового пользователя
      - входные параметры: login, пароль, имя, фамилия, email, обращение (г-н/г-жа)
      - выходные параметры: отсутствуют
-	Поиск пользователя по логину
     - входные параметры:  login
     - выходные параметры: имя, фамилия, email, обращение (г-н/г-жа)
-	Поиск пользователя по маске имени и фамилии
     - входные параметры: маска фамилии, маска имени
     - выходные параметры: login, имя, фамилия, email, обращение (г-н/г-жа)

### Сервис услуг
**API**:
- Создание услуги
  - Входные параметры: название услуги, категория, описание услуги, исполнитель, портфолио(если имеется) и цена
  - Выходыне параметры: идентификатор услуги
- Получение списка всех услуг
  - Входные параметры: отсутствуют
  - Выходные параметры: массив услуг, где для каждой  услуги указан идентификатор, название, категория, аннотация, исполнитель и цена

### Сервис зазказов
**API**:
- Добавление услуг в заказ
  - Входные параметры: название, категория, исполнитель
  - Выходные параметры: идентификатор заказа
- Получение заказа для пользователя
  - Входные параметры: логин пользователя, дата создания заказа
  - Выходные параметры: идентификатор заказа