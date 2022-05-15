#!/bin/bash
#apt-get install mariadb-server
USER=pocotest
PASS=pocotest

mysql -uroot <<MYSQL_SCRIPT
CREATE DATABASE pocotest;
CREATE USER '$USER'@'localhost' IDENTIFIED BY '$PASS';
GRANT ALL PRIVILEGES ON pocotest.* TO '$USER'@'localhost';
FLUSH PRIVILEGES;
MYSQL_SCRIPT

echo "MySQL user created."
echo "Username:   $USER"
echo "Password:   $PASS"
