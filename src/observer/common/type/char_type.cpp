/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/char_type.h"
#include "common/value.h"

static const int day_of_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
static bool is_lunar_year(int year){
  return (year%4==0&&year%100!=0 ) || year%400==0;
}

// 单独定义一个字符串转date的函数 length不包含\0
static RC chars_to_dates(const char *str,int n,int& date){
    int year = 0;
    int month = 0;
    int day = 0;
    if(3!=sscanf(str,"%d-%d-%d",&year,&month,&day)){
      return RC::INVALID_DATE;
    }
    
    if(year<0){
      return RC::INVALID_DATE;
    }


    if(month<=0 || month >12){
      return RC::INVALID_DATE;
    }

    if(day <=0 || day > day_of_month[month-1]+(month == 2 && is_lunar_year(year)?1:0)){
      return RC::INVALID_DATE;
    }

    date = year*10000+month*100+day;
    return RC::SUCCESS;

}

int CharType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::CHARS && right.attr_type() == AttrType::CHARS, "invalid type");
  return common::compare_string(
      (void *)left.value_.pointer_value_, left.length_, (void *)right.value_.pointer_value_, right.length_);
}

RC CharType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  return RC::SUCCESS;
}

RC CharType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    case AttrType::DATES:{
      int date = 0;
      RC rc = chars_to_dates(val.data(),val.length(),date);
      if (OB_FAIL(rc)){
        LOG_WARN("Invalid Date format, rc = %s",strrc(rc));
        return rc;
      }
      result.set_type(AttrType::DATES);
      //result.set_data((char*)&date,sizeof(date));
      result.set_date(date);
    }break;
    default: return RC::UNIMPLEMENTED;
  }
  return RC::SUCCESS;
}

int CharType::cast_cost(AttrType type)
{
  if (type == AttrType::CHARS) {
    return 0;
  }
  else if(type == AttrType::DATES){
    return 1;
  }
  return INT32_MAX;
}

RC CharType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.pointer_value_;
  result = ss.str();
  return RC::SUCCESS;
}