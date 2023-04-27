#mkdir build && cd build && cmake .. && make && cd ..
#cp -r build service_service
#cp -r build auth_service
#cp -r build order_service
docker-compose build && docker-compose up