#ifndef __QUERIES_H__
#define __QUERIES_H__

#include <string>
#include <vector>

typedef std::pair<std::string, std::string> SQLQuery;

// name, query
static std::vector<SQLQuery> sql_queries = {
  {"Q1", "SELECT * FROM test;"},
  {"Q2", "SELECT a, b AS address FROM (SELECT * FROM test WHERE c < 100 AND b > 3) t1 WHERE a < 10 AND b < 100;"},
  {"Q3", "SELECT \"left\".a, \"left\".b, \"right\".a, \"right\".b FROM table_a AS \"left\" JOIN table_b AS \"right\" ON \"left\".a = \"right\".a;"},
  {"Q4", ""
"SELECT"
"    l_orderkey,"
"    SUM(l_extendedprice * (1 - l_discount)) AS revenue,"
"    o_orderdate,"
"    o_shippriority"
"  FROM"
"    customer,"
"    orders,"
"    lineitem"
"  WHERE"
"    c_mktsegment = '%s'"
"    and c_custkey = o_custkey"
"    and l_orderkey = o_orderkey"
"    and o_orderdate < '%s'"
"    and l_shipdate > '%s'"
"  GROUP BY"
"    l_orderkey,"
"    o_orderdate,"
"    o_shippriority"
"  ORDER BY"
"    revenue DESC,"
"    o_orderdate;"
},

  {"LongSelectList26",    "SELECT a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z FROM test;"},
  {"LongSelectElement26", "SELECT abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxy FROM test;"},
  {"LongSelectList52",    "SELECT a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z FROM test;"},
  {"LongSelectElement52", "SELECT abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxy FROM test;"},
  {"TwoSelects",          "SELECT * FROM test; SELECT age, street AS address FROM data;"},
  {"ExecuteNoParams",     "EXECUTE procedure;"},
  {"Execute2Params",      "EXECUTE procedure(11, 'test');"},
  {"Execute10Params",     "EXECUTE procedure(11, 'test', 5.6, 4.2, 'abc', 6, 7, 8, 9, 10000);"},
  // {"name", "query"},
};

std::string getQueryName(unsigned i);

std::vector<SQLQuery> getQueriesFromDirectory(const std::string& dir_path);

std::vector<SQLQuery> getTPCHQueries();

#endif
