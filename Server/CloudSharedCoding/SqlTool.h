#ifndef SQLTOOL_H
#define SQLTOOL_H

#include<mysql/mysql.h>
#include"Log.h"

class SqlTool
{
public:
    SqlTool() {};
    SqlTool(std::string host, std::string user, std::string pwd, std::string db_name);
    virtual ~SqlTool();

public:
    MYSQL_RES* exeSql(std::string sql);
    std::vector<MYSQL_ROW> getRows(MYSQL_RES* result);

private:
    Log::Logger logger;
    MYSQL* mysql;
};

#endif // SQLTOOL_H
