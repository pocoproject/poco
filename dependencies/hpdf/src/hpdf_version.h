#define HPDF_MAJOR_VERSION 2
#define HPDF_MINOR_VERSION 4
#define HPDF_BUGFIX_VERSION 5
#define HPDF_EXTRA_VERSION ""

#define hpdf_vquote(x) #x
#define hpdf_vexp(x) hpdf_vquote(x)

#define HPDF_VERSION_TEXT (hpdf_vexp(HPDF_MAJOR_VERSION) "." hpdf_vexp(HPDF_MINOR_VERSION) "." hpdf_vexp(HPDF_BUGFIX_VERSION) HPDF_EXTRA_VERSION)
#define HPDF_VERSION_ID ((HPDF_MAJOR_VERSION) * 10000 + (HPDF_MINOR_VERSION) * 100 + (HPDF_BUGFIX_VERSION))
