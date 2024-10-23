#include "date_type.h"
#include "common/log/log.h"
#include "common/type/char_type.h"
#include "common/value.h"

int DateType::compare(const Value &left, const Value &right) const { 
   ASSERT(left.attr_type() == AttrType::DATES, "left type is not date");
   ASSERT(right.attr_type() == AttrType::DATES, "right type is not date");
   return left.get_int()-right.get_int();
}

// 参考char_type.cpp
RC DateType::set_value_from_str(Value &val, const string &data) const { 
    Value value (data.c_str()); 
    return DataType::type_instance(AttrType::CHARS)->cast_to(value,AttrType::DATES,val);
}

RC DateType::to_string(const Value &val, string &result) const { 
    // YYYY-MM-DD类型
    ASSERT(val.attr_type() == AttrType::DATES, "value is not date");
    int date = val.get_int();

    int year = date/10000;
    int month = (date/100)%100; 
    int day = date % 100;

   // 拼接 注意补位+0
    result += std::to_string(year);
    result += '-';
    if(month<10){
        result += '0';
    }
    result += std::to_string(month);
    result += '-';
    if(day<10){
        result += '0';
    }
    result += std::to_string(day);

    return RC::SUCCESS;
}
