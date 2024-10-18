/***************************************************************************
 *
 * @file hello_world.cpp
 * @author cswuyg
 * @date 2019/01
 * @brief  xapian first demo
 *
 **************************************************************************/
#include <iostream>
#include "xapian.h"

const char* const K_DB_PATH = "index_data";
const char* const K_DOC_UNIQUE_ID = "007";

/// 创建索引
void createIndex() {
    std::cout << "--index start--" << std::endl;
    Xapian::WritableDatabase db(K_DB_PATH, Xapian::DB_CREATE_OR_OPEN);

    Xapian::Document doc;
    doc.add_posting("T中国", 1);
    doc.add_posting("T篮球", 1);
    doc.add_posting("T比赛", 1);
    doc.set_data("中国篮球比赛");
    doc.add_boolean_term(K_DOC_UNIQUE_ID);

    Xapian::docid innerId = db.replace_document(K_DOC_UNIQUE_ID, doc);

    std::cout << "add doc innerId=" << innerId << std::endl;

    db.commit();

    std::cout << "--index finish--" << std::endl;
}

/// 检索索引
void queryIndex() {
    std::cout << "--search start--" << std::endl;
    Xapian::Query termOne = Xapian::Query("");
    Xapian::Query termTwo = Xapian::Query("");
    Xapian::Query termThree = Xapian::Query("中国台湾");
    auto query = Xapian::Query(Xapian::Query::OP_AND, Xapian::Query(Xapian::Query::OP_OR, termOne, termTwo), termThree);
    std::cout << "query=" << query.get_description() << std::endl;

    Xapian::Database db(K_DB_PATH);
    Xapian::Enquire enquire(db);
    enquire.set_query(query);
    Xapian::MSet result = enquire.get_mset(0, 100);
    std::cout << "find results count=" << result.get_matches_estimated() << std::endl;

    for (auto it = result.begin(); it != result.end(); ++it) {
        Xapian::Document doc = it.get_document();
        std::string data = doc.get_data();
        double docScoreWeight = it.get_weight();
        int docScorePercent = it.get_percent();

        std::cout << "doc=" << data << ",weight=" << docScoreWeight << ",percent=" << docScorePercent << std::endl;
    }

    std::cout << "--search finish--" << std::endl;
}

int main() {
    /* createIndex(); */
    queryIndex();
    return 0;
}
