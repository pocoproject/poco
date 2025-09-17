#!/bin/bash -eu

mkdir -p cmake-build
cd cmake-build

cmake -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTS=OFF -DENABLE_FUZZING=ON \
      -DENABLE_ACTIVERECORD=OFF -DENABLE_PAGECOMPILER=OFF \
      -DENABLE_PAGECOMPILER_FILE2PAGE=OFF -DENABLE_DATA_SQLITE=OFF \
      -DENABLE_REDIS=OFF -DENABLE_MONGODB=OFF -DENABLE_PROMETHEUS=OFF \
      -DENABLE_ACTIVERECORD_COMPILER=OFF \
      ..

make -j$(nproc)

# Copy binaries with preserving historical names
cp bin/JSON-parse-fuzzer $OUT/json_parser_fuzzer
cp bin/XML-parse-fuzzer $OUT/xml_parser_fuzzer
cp bin/Foundation-datetime-fuzzer $OUT/date_time_fuzzer
cp bin/Foundation-misc-fuzzer $OUT/foundation_misc_fuzzer
cp bin/JWT-decode-fuzzer $OUT/jwt_decode_fuzzer
cp bin/Net-mail-parser-fuzzer $OUT/mail_message_fuzzer
cp bin/Net-http-parser-fuzzer $OUT/http_message_fuzzer

# Seed corpus
zip -q -r -j $OUT/mail_message_fuzzer_seed_corpus.zip $SRC/poco/Net/fuzzing/seed/Mail
zip -q -r -j $OUT/http_message_fuzzer_seed_corpus.zip $SRC/poco/Net/fuzzing/seed/HTTP

# Dictionaries
cp $SRC/poco/XML/fuzzing/xml.dict $OUT/xml_parser_fuzzer.dict
