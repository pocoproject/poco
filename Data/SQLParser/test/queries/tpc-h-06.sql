-- http://www.sqlserver-dba.com/2011/09/this-is-a-followup-on-my-earlier-post-of-sql-server-test-data-generation-testing-tools-i-had-some-requests-for-my-set-up-pr.html
SELECT SUM(L_EXTENDEDPRICE*L_DISCOUNT) AS REVENUE
FROM LINEITEM
WHERE L_SHIPDATE >= '1994-01-01' AND L_SHIPDATE < dateadd(yy, 1, cast('1994-01-01' as datetime))
AND L_DISCOUNT BETWEEN .06 - 0.01 AND .06 + 0.01 AND L_QUANTITY < 24