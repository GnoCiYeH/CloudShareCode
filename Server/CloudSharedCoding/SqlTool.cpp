#include "SqlTool.h"
#include <arpa/inet.h>

SqlTool::SqlTool(std::string host, std::string user, std::string pwd, std::string db_name)
{
    logger = Log::Logger("SqlLogger");
    if ((mysql = mysql_init(NULL)) == NULL)
    {
        FATAL_LOG(logger, "mysql init error");
        return;
    }
    mysql = mysql_real_connect(mysql, host.c_str(), user.c_str(), pwd.c_str(), db_name.c_str(), 3306, NULL, 0);
    if (mysql == NULL)
    {
        FATAL_LOG(logger, "mysql connect error");
        return;
    }
}

SqlTool::~SqlTool()
{
    mysql_close(mysql);
}

MYSQL_RES* SqlTool::exeSql(std::string sql)
{
    if (mysql == NULL)
    {
        ERROR_LOG(logger, "mysql is NULL");
        return NULL;
    }
    if (mysql_query(mysql, sql.c_str()))
    {
        ERROR_LOG(logger, std::string(mysql_error(mysql)));
        return NULL;
    }
    else
    {
        return mysql_store_result(mysql);
    }
}

std::vector<MYSQL_ROW> SqlTool::getRows(MYSQL_RES* result)
{
    std::vector<MYSQL_ROW> rows;
    int num = mysql_num_rows(result);
    for (int i = 0; i < num; i++)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row == NULL)
            break;
        rows.push_back(row);
    }
    return rows;
}

