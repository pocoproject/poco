#ifndef SQLPARSER_SELECT_STATEMENT_H
#define SQLPARSER_SELECT_STATEMENT_H

#include "Expr.h"
#include "SQLStatement.h"
#include "Table.h"

namespace hsql {
enum OrderType { kOrderAsc, kOrderDesc };

enum SetType { kSetUnion, kSetIntersect, kSetExcept };

enum RowLockMode { ForUpdate, ForNoKeyUpdate, ForShare, ForKeyShare };
enum RowLockWaitPolicy { NoWait, SkipLocked, None };

// Description of the order by clause within a select statement.
struct SQLParser_API OrderDescription {
  OrderDescription(OrderType type, Expr* expr);
  virtual ~OrderDescription();

  OrderType type;
  Expr* expr;
};

// Description of the limit clause within a select statement.
struct SQLParser_API LimitDescription {
  LimitDescription(Expr* limit, Expr* offset);
  virtual ~LimitDescription();

  Expr* limit;
  Expr* offset;
};

// Description of the group-by clause within a select statement.
struct SQLParser_API GroupByDescription {
  GroupByDescription();
  virtual ~GroupByDescription();

  std::vector<Expr*>* columns;
  Expr* having;
};

struct SQLParser_API WithDescription {
  ~WithDescription();

  char* alias;
  SelectStatement* select;
};

struct SQLParser_API SetOperation {
  SetOperation();
  virtual ~SetOperation();

  SetType setType;
  bool isAll;

  SelectStatement* nestedSelectStatement;
  std::vector<OrderDescription*>* resultOrder;
  LimitDescription* resultLimit;
};

struct SQLParser_API LockingClause {
  RowLockMode rowLockMode;
  RowLockWaitPolicy rowLockWaitPolicy;
  std::vector<char*>* tables;
};

// Representation of a full SQL select statement.
struct SQLParser_API SelectStatement : SQLStatement {
  SelectStatement();
  ~SelectStatement() override;

  TableRef* fromTable;
  bool selectDistinct;
  std::vector<Expr*>* selectList;
  Expr* whereClause;
  GroupByDescription* groupBy;

  // Note that a SetOperation is always connected to a
  // different SelectStatement. This statement can itself
  // have SetOperation connections to other SelectStatements.
  // To evaluate the operations in the correct order:
  //    Iterate over the setOperations vector:
  //      1. Fully evaluate the nestedSelectStatement within the SetOperation
  //      2. Connect the original statement with the
  //         evaluated nestedSelectStatement
  //      3. Apply the resultOrder and the resultLimit
  //      4. The result now functions as the the original statement
  //         for the next iteration
  //
  // Example:
  //
  //   (SELECT * FROM students INTERSECT SELECT * FROM students_2) UNION SELECT * FROM students_3 ORDER BY grade ASC;
  //
  //   1. We evaluate `Select * FROM students`
  //   2. Then we iterate over the setOperations vector
  //   3. We evalute the nestedSelectStatement of the first entry, which is: `SELECT * FROM students_2`
  //   4. We connect the result of 1. with the results of 3. using the setType, which is INTERSECT
  //   5. We continue the iteration of the setOperations vector
  //   6. We evaluate the new nestedSelectStatement which is: `SELECT * FROM students_3`
  //   7. We apply a Union-Operation to connect the results of 4. and 6.
  //   8. Finally, we apply the resultOrder of the last SetOperation (ORDER BY grade ASC)
  std::vector<SetOperation*>* setOperations;

  std::vector<OrderDescription*>* order;
  std::vector<WithDescription*>* withDescriptions;
  LimitDescription* limit;
  std::vector<LockingClause*>* lockings;
};

}  // namespace hsql

#endif
