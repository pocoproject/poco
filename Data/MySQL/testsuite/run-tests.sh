#!/bin/bash
MYSQL_DOCKER_VER=latest
MYSQL_DB_START_WAIT=10
echo "running mysql docker container"
docker run -p 3306:3306 --name poco-test-mysql -e MYSQL_ROOT_PASSWORD=poco -e MYSQL_DATABASE=pocotestdb -d mysql:$MYSQL_DOCKER_VER
echo "container up and running, sleeping $MYSQL_DB_START_WAIT seconds waiting for mysql db to start ..."
sleep $MYSQL_DB_START_WAIT
./bin/Linux/x86_64/testrunner -all
echo "stopping mysql docker container"
docker stop poco-test-mysql
echo "removing mysql docker container"
docker rm poco-test-mysql
