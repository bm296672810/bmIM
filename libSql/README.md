# libSql

## 1. 下载并安装mysql server
## 2. 然后设置CMakeLists.txt:
###  a.指定MYSQL_ROOT_PATH为mysql server安装的根目录 
###  b.设置RELEASE_LIBS:
    windows下设为 ${MYSQL_ROOT_PATH}/lib/libmysql.lib
    mac下设为 ${MYSQL_ROOT_PATH}/lib/libmysql.a
    unix/linux下设为 ${MYSQL_ROOT_PATH}/lib/libmysql.a
## 3. 将mysql依赖的动态库拷贝到可执行程序同级目录:
###  a.windows 
    ${MYSQL_ROOT_PATH}/lib/libmysql.dll
    ${MYSQL_ROOT_PATH}/bin/ssleay32.dll
    ${MYSQL_ROOT_PATH}/bin/libeay32.dll
###  b.mac
###  c.unix/linux