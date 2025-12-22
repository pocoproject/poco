#ifndef SQLPARSER_COLUMN_TYPE_H
#define SQLPARSER_COLUMN_TYPE_H

#include"sqlparser_win.h"
#include <ostream>

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
  LONG,
  REAL,
  SMALLINT,
  TEXT,
  TIME,
  VARCHAR,
};

// Represents the type of a column, e.g., FLOAT or VARCHAR(10)
struct SQLParser_API ColumnType {
  ColumnType() = default;
  ColumnType(DataType data_type, int64_t length = 0, int64_t precision = 0, int64_t scale = 0);
  DataType data_type;
  int64_t length;     // Used for, e.g., VARCHAR(10)
  int64_t precision;  // Used for, e.g., DECIMAL (6, 4) or TIME (5)
  int64_t scale;      // Used for DECIMAL (6, 4)
};

bool operator==(const ColumnType& lhs, const ColumnType& rhs);
bool operator!=(const ColumnType& lhs, const ColumnType& rhs);
std::ostream& operator<<(std::ostream&, const ColumnType&);

}  // namespace hsql

#endif
