#
# MongoDB & Redis does not work on Travis OSX
# should be restored later on when the OSX environment would have
# been fixed by Travis
#
export EXCLUDE_TESTS="Data/ODBC Data/MySQL Data/PostgreSQL MongoDB Redis PDF"

