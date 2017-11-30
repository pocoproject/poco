#
# Makefile
#
# Makefile for Poco PDF
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I $(POCO_BASE)/PDF/include/Poco/PDF

objects = Destination Document Encoder Font Image Outline \
	LinkAnnotation Page PDFException TextAnnotation \
	hpdf_3dmeasure hpdf_annotation hpdf_array hpdf_binary hpdf_boolean \
	hpdf_catalog hpdf_destination hpdf_dict hpdf_doc hpdf_doc_png \
	hpdf_encoder hpdf_encoder_cns hpdf_encoder_cnt hpdf_encoder_jp \
	hpdf_encoder_kr hpdf_encoder_utf hpdf_encrypt hpdf_encryptdict hpdf_error \
	hpdf_exdata hpdf_ext_gstate hpdf_font hpdf_font_cid hpdf_font_tt \
	hpdf_font_type1 hpdf_fontdef hpdf_fontdef_base14 hpdf_fontdef_cid \
	hpdf_fontdef_cns hpdf_fontdef_cnt hpdf_fontdef_jp hpdf_fontdef_kr \
	hpdf_fontdef_tt hpdf_fontdef_type1 hpdf_gstate hpdf_image hpdf_image_ccitt \
	hpdf_image_png hpdf_info hpdf_list hpdf_mmgr hpdf_name hpdf_namedict hpdf_null \
	hpdf_number hpdf_objects hpdf_outline hpdf_page_label hpdf_page_operator \
	hpdf_pages hpdf_pdfa hpdf_real hpdf_streams hpdf_string hpdf_u3d hpdf_utils hpdf_xref \
	adler32 compress crc32 deflate gzio \
	infback inffast inflate inftrees trees zutil \
	png pngerror pnggccrd pngget pngmem pngpread \
	pngread pngrio pngrtran pngrutil pngset pngtest \
	pngtrans pngvcrd pngwio pngwrite pngwtran pngwutil

target         = PocoPDF
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
