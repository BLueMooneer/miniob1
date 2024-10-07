#include "common/log/log.h"
#include "sql/executor/create_table_executor.h"


#include "event/sql_event.h"
#include "sql/stmt/drop_table_stmt.h"
#include "drop_table_executor.h"
#include "event/session_event.h"
#include "session/session.h"
#include "storage/db/db.h"


RC DropTableExecutor::execute(SQLStageEvent *sql_event)
{
    // 将刚刚创建好的stmt保存下来
    DropTableStmt *stmt = (DropTableStmt *)sql_event->stmt();
    Db *db = sql_event->session_event()->session()->get_current_db();
    
    return db->drop_table(stmt->table_name().c_str());
    
}