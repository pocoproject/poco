#ifndef SQLPARSER_IMPORT_EXPORT_OPTIONS_H
#define SQLPARSER_IMPORT_EXPORT_OPTIONS_H

#include <utility>

namespace hsql {

// Name unchanged for compatibility. Historically, this was only used for import statements before we introduced export
// statements (`COPY ... TO`). We did not change the enum name to accomodate forks. In the grammar, however, we call the
// corresponding terminal symbol `file_type` and use it for both `ImportStatement` and `ExportStatement`.
enum ImportType {
  kImportCSV,
  kImportTbl,  // Hyrise file format.
  kImportBinary,
  kImportAuto
};

enum CsvOptionType {
  Delimiter,
  Null,
  Quote,
};

struct CsvOptions {
  CsvOptions();
  ~CsvOptions();

  char* delimiter;
  char* null;
  char* quote;

  bool accept_csv_option(std::pair<CsvOptionType, char*>* option);
};

struct ImportExportOptions {
  ImportExportOptions();
  ~ImportExportOptions();

  ImportType format;
  char* encoding;
  CsvOptions* csv_options;
};

}  // namespace hsql

#endif
