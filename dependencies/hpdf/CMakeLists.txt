# libharu (HPDF) - PDF creation library
# Version: 2.4.5
# Source: https://github.com/libharu/libharu/releases/tag/v2.4.5

# Sources
file(GLOB SRCS_G "src/*.c")
POCO_SOURCES(SRCS hpdf ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS(SRCS hpdf ${HDRS_G})

# hpdf requires PNG. When PNG::PNG is not available (e.g. unbundled mode
# without system libpng), skip building hpdf. Only warn when the user
# actually requested PDF support — otherwise the missing target is expected.
if (NOT TARGET PNG::PNG)
	if (ENABLE_PDF)
		message(WARNING "PNG not found; hpdf will not be built.")
	endif()
	return()
endif()

add_library(_BUNDLED_HPDF OBJECT EXCLUDE_FROM_ALL ${SRCS})

set_property(TARGET _BUNDLED_HPDF PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(_BUNDLED_HPDF
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if (POCO_UNBUNDLED)
	target_link_libraries(_BUNDLED_HPDF PRIVATE PNG::PNG)
else()
	target_link_libraries(_BUNDLED_HPDF PRIVATE "$<BUILD_LOCAL_INTERFACE:ZLIB::ZLIB>")
	target_link_libraries(_BUNDLED_HPDF PRIVATE "$<BUILD_LOCAL_INTERFACE:PNG::PNG>")
endif()

add_library(HPDF::HPDF ALIAS _BUNDLED_HPDF)
