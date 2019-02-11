#include <iostream>
#include <vector>
#include <QtSql>

int main()
{
    QSqlDatabase dbase;
    dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName("/home/claorisel/workspace/itmo/DB project/sqlite_lesson/db/rzhd.db");

    if (!dbase.open())
    {
        std::cout << "Can't open db" << std::endl;
    }
    else
    {
        std::cout << "opened db" << std::endl;

    }

    /**
     *
     */

    {
        uint32_t line_id = 0;

        QSqlQuery query;

        query.prepare("SELECT ID_GEO_LINE FROM RAILWAY_OBJ_CIRCUIT WHERE ID=?");
        query.addBindValue(100592);
        query.exec();

        if (query.next())
        {
            line_id = query.value(0).toInt();
        }

        std::cout << line_id << std::endl;
    }

    {
        using IdLineList = std::vector<uint32_t>;

        int line_id = 471;
        IdLineList outPoints;
        std::map<int, int> linePoints;

        QSqlQuery query;

        query.prepare("SELECT ID_GEO_POINT, POINT_INDEX FROM GEO_LINE_ADDITIONAL_POINTS "
                      "WHERE ID_GEO_LINE=?");

        query.addBindValue(line_id);
        query.exec();

        while(query.next())
        {
            int id          = query.value(0).toInt();
            int index       = query.value(1).toInt();
            linePoints.insert(linePoints.end(), std::pair<int, int>(index,  id));

            std::cout << "index " << index << " id " << id << std::endl;

        }

    }

    {
        struct DbGeoLine
        {
            uint32_t id;
            uint32_t id_geopoint_start;
            uint32_t id_geopoint_end;
        };


        int id_geopoint_start = 0;
        std::vector<DbGeoLine> geoline_info_list;

        QSqlQuery query;

        query.prepare("SELECT ID, ID_GEO_POINT_START, ID_GEO_POINT_END FROM GEO_LINE WHERE ID_GEO_POINT_START =?");
        query.addBindValue(id_geopoint_start);
        query.exec();

        while(query.next())
        {
            DbGeoLine line_info;
            line_info.id = query.value(0).toInt();
            line_info.id_geopoint_start = query.value(1).toInt();
            line_info.id_geopoint_end = query.value(2).toInt();
            geoline_info_list.push_back(line_info);
        }
    }

    {

        int line_id = 0;
        struct DbSwitch
        {
            uint32_t id = 0;
            uint32_t id_geoline_start;
            uint32_t id_geoline_end;
            uint32_t id_geoline_turn;
            uint8_t turn_type;

            bool has() const { return id != 0; }
            operator bool() const { return has(); }
        };


        DbSwitch switch_info;

        QSqlQuery query;

        query.prepare("SELECT ID, ID_GEO_LINE_START, ID_GEO_LINE_END, ID_GEO_LINE_TURN, TURN_TYPE "
                      "FROM RAILWAY_OBJ_SINGLE_SWITCH "
                      "WHERE ID_GEO_LINE_START =? or ID_GEO_LINE_END =?");

        query.addBindValue(line_id);
        query.addBindValue(line_id);
        query.exec();

        while(query.next())
        {
            switch_info.id = query.value(0).toInt();
            switch_info.id_geoline_start = query.value(1).toInt();
            switch_info.id_geoline_end = query.value(2).toInt();
            switch_info.id_geoline_turn = query.value(3).toInt();
            switch_info.turn_type = query.value(4).toInt();
        }

    }


//    std::cout << "Hello, World!" << std::endl;
    return 0;
}