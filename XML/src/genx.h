/*
 * genx - C-callable library for generating XML documents
 */

/*
 * Copyright (c) 2007-2013 Code Synthesis Tools CC.
 * Copyright (c) 2004 by Tim Bray and Sun Microsystems.
 *
 * For copying permission, see the accompanying COPYING file.
 */

#ifndef GENX_H
#define GENX_H

#include <stddef.h> /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Note on error handling: genx routines mostly return
 *  GENX_SUCCESS (guaranteed to be zero) in normal circumstances, one of
 *  these other GENX_ values on a memory allocation or I/O failure or if the
 *  call would result in non-well-formed output.
 * You can associate an error message with one of these codes explicitly
 *  or with the most recent error using genxGetErrorMessage() and
 *  genxLastErrorMessage(); see below.
 */
typedef enum
{
  GENX_SUCCESS = 0,
  GENX_BAD_UTF8,
  GENX_NON_XML_CHARACTER,
  GENX_BAD_NAME,
  GENX_ALLOC_FAILED,
  GENX_BAD_NAMESPACE_NAME,
  GENX_INTERNAL_ERROR,
  GENX_DUPLICATE_PREFIX,
  GENX_SEQUENCE_ERROR,
  GENX_NO_START_TAG,
  GENX_IO_ERROR,
  GENX_MISSING_VALUE,
  GENX_MALFORMED_COMMENT,
  GENX_XML_PI_TARGET,
  GENX_MALFORMED_PI,
  GENX_DUPLICATE_ATTRIBUTE,
  GENX_ATTRIBUTE_IN_DEFAULT_NAMESPACE,
  GENX_DUPLICATE_NAMESPACE,
  GENX_BAD_DEFAULT_DECLARATION
} genxStatus;

/* character types */
#define GENX_XML_CHAR 1
#define GENX_LETTER 2
#define GENX_NAMECHAR 4

/* The size of the character table. Valid values are 0x100 (first 255
   chars are checked) and 0x10000 (all chars are checked). */
#ifndef GENX_CHAR_TABLE_SIZE
#  define GENX_CHAR_TABLE_SIZE 0x100
#endif

/* a UTF-8 string */
typedef unsigned char * utf8;
typedef const unsigned char * constUtf8;

/*
 * genx's own types
 */
typedef struct genxWriter_rec * genxWriter;
typedef struct genxNamespace_rec * genxNamespace;
typedef struct genxElement_rec * genxElement;
typedef struct genxAttribute_rec * genxAttribute;

typedef void * (*genxAlloc) (void * userData, size_t bytes);
typedef void (*genxDealloc) (void * userData, void* data);

/*
 * Constructors, set/get
 */

/*
 * Create a new writer.  For generating multiple XML documents, it's most
 *  efficient to re-use the same genx object.  However, you can only write
 *  one document at a time with a writer.
 * Returns NULL if it fails, which can only be due to an allocation failure.
 */
genxWriter genxNew(genxAlloc alloc, genxDealloc dealloc, void * userData);

/*
 * Reset the writer object back into usable state after an error or
 * interruption.
 */
genxStatus genxReset (genxWriter w);

/*
 * Dispose of a writer, freeing all associated memory
 */
void genxDispose(genxWriter w);

/*
 * Set/get
 */

/*
 * The userdata pointer will be passed to memory-allocation
 *  and I/O callbacks. If not set, genx will pass NULL
 */
void genxSetUserData(genxWriter w, void * userData);
void * genxGetUserData(genxWriter w);

/*
 * Set/get pretty-printing. If indentation is set to 0, then no pretty-
 * printing is performed.
 */
genxStatus genxSetPrettyPrint(genxWriter w, int indentation);
int genxGetPrettyPrint(genxWriter w);

/*
 * Set/get canonicalization. If true, then output explicit closing
 * tags and sort attributes. Default is false.
 */
genxStatus genxSetCanonical(genxWriter w, int flag);
int genxGetCanonical(genxWriter w);

/*
 * User-provided memory allocator, if desired.  For example, if you were
 *  in an Apache module, you could arrange for genx to use ap_palloc by
 *  making the pool accessible via the userData call.
 * The "dealloc" is to be used to free memory allocated with "alloc".  If
 *  alloc is provided but dealloc is NULL, genx will not attempt to free
 *  the memory; this would be appropriate in an Apache context.
 * If "alloc" is not provided, genx routines use malloc() to allocate memory
 */
void genxSetAlloc(genxWriter w, genxAlloc alloc);
void genxSetDealloc(genxWriter w, genxDealloc dealloc);
genxAlloc genxGetAlloc(genxWriter w);
genxDealloc genxGetDealloc(genxWriter w);

/*
 * Get the prefix associated with a namespace
 */
utf8 genxGetNamespacePrefix(genxNamespace ns);

/*
 * Declaration functions
 */

/*
 * Declare a namespace.  The provided prefix is the default but can be
 *  overridden by genxAddNamespace.  If no default prefiix is provided,
 *  genx will generate one of the form g-%d.
 * On error, returns NULL and signals via statusp
 */
genxNamespace genxDeclareNamespace(genxWriter w,
				   constUtf8 uri, constUtf8 prefix,
				   genxStatus * statusP);

/*
 * Declare an element
 * If something failed, returns NULL and sets the status code via statusP
 */
genxElement genxDeclareElement(genxWriter w,
			       genxNamespace ns, constUtf8 type,
			       genxStatus * statusP);

/*
 * Declare an attribute
 */
genxAttribute genxDeclareAttribute(genxWriter w,
				   genxNamespace ns,
				   constUtf8 name, genxStatus * statusP);

/*
 * Writing XML
 */

/*
 * Caller-provided I/O package.
 * First form is for a null-terminated string.
 * for second, if you have s="abcdef" and want to send "abc", you'd call
 *  sendBounded(userData, s, s + 3)
 */
typedef struct
{
  genxStatus (* send)(void * userData, constUtf8 s);
  genxStatus (* sendBounded)(void * userData, constUtf8 start, constUtf8 end);
  genxStatus (* flush)(void * userData);
} genxSender;

genxStatus genxStartDocSender(genxWriter w, genxSender * sender);

/*
 * End a document.  Calls "flush"
 */
genxStatus genxEndDocument(genxWriter w);

/*
 * Write XML declaration. If encoding or standalone are NULL, then those
 * attributes are omitted.
 */
genxStatus genxXmlDeclaration(genxWriter w,
                              constUtf8 version,
                              constUtf8 encoding,
                              constUtf8 standalone);
/*
 * Write a comment
 */
genxStatus genxComment(genxWriter w, constUtf8 text);

/*
 * Write a PI
 */
genxStatus genxPI(genxWriter w, constUtf8 target, constUtf8 text);

/*
 * Start an element
 */
genxStatus genxStartElementLiteral(genxWriter w,
				   constUtf8 xmlns, constUtf8 type);

/*
 * Start a predeclared element
 * - element must have been declared
 */
genxStatus genxStartElement(genxElement e);

/*
 * Write an attribute
 */
genxStatus genxAddAttributeLiteral(genxWriter w, constUtf8 xmlns,
				   constUtf8 name, constUtf8 value);

/*
 * Start an attribute
 */
genxStatus genxStartAttributeLiteral(genxWriter w,
                                     constUtf8 xmlns, constUtf8 name);

/*
 * Write a predeclared attribute
 */
genxStatus genxAddAttribute(genxAttribute a, constUtf8 value);

/*
 * Start a predeclared attribute
 */
genxStatus genxStartAttribute(genxAttribute a);

/*
 * End an attribute
 */
genxStatus genxEndAttribute(genxWriter w);

/*
 * add a namespace declaration
 */
genxStatus genxAddNamespaceLiteral(genxWriter w,
                                   constUtf8 uri, constUtf8 prefix);

/*
 * add a predefined namespace declaration
 */
genxStatus genxAddNamespace(genxNamespace ns, constUtf8 prefix);

/*
 * Clear default namespace declaration
 */
genxStatus genxUnsetDefaultNamespace(genxWriter w);

/*
 * Write an end tag
 */
genxStatus genxEndElement(genxWriter w);

/*
 * Write some text
 * You can't write any text outside the root element, except with
 * genxComment and genxPI.
 */
genxStatus genxAddText(genxWriter w, constUtf8 start);
genxStatus genxAddCountedText(genxWriter w, constUtf8 start, size_t byteCount);
genxStatus genxAddBoundedText(genxWriter w, constUtf8 start, constUtf8 end);

/*
 * Write one character.  The integer value is the Unicode character
 *  value, as usually expressed in U+XXXX notation.
 */
genxStatus genxAddCharacter(genxWriter w, int c);

/*
 * Utility routines
 */

/*
 * Return the Unicode character encoded by the UTF-8 pointed-to by the
 *  argument, and advance the argument past the encoding of the character.
 * Returns -1 if the UTF-8 is malformed, in which case advances the
 *  argument to point at the first byte past the point past the malformed
 *  ones.
 */
int genxNextUnicodeChar(constUtf8 * sp);

/*
 * Scan a buffer allegedly full of UTF-8 encoded XML characters; return
 *  one of GENX_SUCCESS, GENX_BAD_UTF8, or GENX_NON_XML_CHARACTER
 */
genxStatus genxCheckText(genxWriter w, constUtf8 s);

/*
 * return character status, the OR of GENX_XML_CHAR,
 *  GENX_LETTER, and GENX_NAMECHAR
 */
int genxCharClass(genxWriter w, int c);

/*
 * Silently wipe any non-XML characters out of a chunk of text.
 * If you call this on a string before you pass it addText or
 *  addAttribute, you will never get an error from genx unless
 *  (a) there's a bug in your software, e.g. a malformed element name, or
 *  (b) there's a memory allocation or I/O error
 * The output can never be longer than the input.
 * Returns true if any changes were made.
 */
int genxScrubText(genxWriter w, constUtf8 in, utf8 out);

/*
 * return error messages
 */
char * genxGetErrorMessage(genxWriter w, genxStatus status);
char * genxLastErrorMessage(genxWriter w);

/*
 * return version
 */
char * genxGetVersion();

#ifdef __cplusplus
}
#endif

#endif /* GENX_H */
