#ifndef SQLPARSER_COLUMN_TYPE_H
#define SQLPARSER_COLUMN_TYPE_H

#include "sqlparser_win.h"
#include <ostream>
#include <stdint.h>

namespace hsql {
enum class DataType {
  UNKNOWN,
  BIGINT,
  BOOLEAN,
  CHAR,
  DATE,
  DATETIME,
  DECIMAL,
  DOUBLE,
  FLOAT,
  INT,
  INTERVAL,
  LONG,
  REAL,
  SMALLINT,
  TEXT,
  TIME,
  VARCHAR,
};

// A field of a date or time value, as named by EXTRACT and by the qualifier of
// an interval. The ranges after the single fields are the interval qualifiers
// of the SQL standard, e.g. INTERVAL DAY TO SECOND: a range names the leading
// and the trailing field, and every field in between is implied.
enum DatetimeField {
  kDatetimeNone,
  kDatetimeSecond,
  kDatetimeMinute,
  kDatetimeHour,
  kDatetimeDay,
  kDatetimeMonth,
  kDatetimeYear,
  kIntervalYearToMonth,
  kIntervalDayToHour,
  kIntervalDayToMinute,
  kIntervalDayToSecond,
  kIntervalHourToMinute,
  kIntervalHourToSecond,
  kIntervalMinuteToSecond
};

// Represents the type of a column, e.g., FLOAT or VARCHAR(10)
struct SQLParser_API ColumnType {
  ColumnType() = default;
  ColumnType(DataType data_type, int64_t length = 0, int64_t precision = 0, int64_t scale = 0,
             DatetimeField datetime_field = kDatetimeNone);
  DataType data_type;
  int64_t length;     // Used for, e.g., VARCHAR(10)
  int64_t precision;  // Used for, e.g., DECIMAL (6, 4) or TIME (5)
  int64_t scale;      // Used for DECIMAL (6, 4)
  // Used for INTERVAL, e.g. INTERVAL DAY TO SECOND; kDatetimeNone for a bare INTERVAL.
  DatetimeField datetime_field;
};

SQLParser_API bool operator==(const ColumnType& lhs, const ColumnType& rhs);
SQLParser_API bool operator!=(const ColumnType& lhs, const ColumnType& rhs);
SQLParser_API std::ostream& operator<<(std::ostream&, const ColumnType&);
SQLParser_API std::ostream& operator<<(std::ostream&, const DatetimeField&);

}  // namespace hsql

#endif
