/** @file visualstring.h
  * this library generates a visual string from a logical string and reshapes\n
  * Perisan/Arabic/English...chars based on Paragrah (RTL/LTR) settings.
  * this library uses fribidi (http://fribidi.org) by Behdad Esfahbod (behdad.org)
  *
  *
  * @copyright (C) 2013, R&D \n
  *   All rights reserved.
  * @date 2013.12.08
  * @version 1.0.0
  * @author amir zamani
  *
 */
#ifndef __VISUALSTRING_H__
#define __VISUALSTRING_H__

#define DONT_HAVE_FRIBIDI_CONFIG_H 1
#define HAVE_FRIBIDI_CUSTOM_H
#define FRIBIDI_INT_TYPES
#define DONT_HAVE_FRIBIDI_UNICODE_VERSION_H
#define FRIBIDI_USE_GLIB 0
#define FRIBIDI_SIZEOF_INT
#include <fribidi/config.h>
#include <fribidi/fribidi-bidi.h>
#include "stdint.h"
#include "stddef.h"



/*****************************************************************************/
__BEGIN_DECLS
/*****************************************************************************/
#define KDefaultStringCapacity      128

/** supported code pages of this library. */
typedef enum _TInputCodePage_ {
    EUtf8,                  /**< UTF8 */
    EArabic1256,            /**< Windows1256 or Arabic1256. */
    EISO8859_6              /**< ISO Arabic 8859_6. */
} TCodePage;

/** paragraph order of processing. */
typedef enum _TParagraph_ {
    ERTL,                   /**< Right To Left as Persian, Hebrew and Arabic. */
    ELTR                    /**< Left To Right as English, French, ... . */
} TParagraph;

/** parameters of visualize conversion. */
typedef struct _VisualString_ {
    const char*         inputString;        /**< source string, could be anything in TCodePage. */
    size_t              inputLength;        /**< length of input string in byte (not actual character). */
    TCodePage           inputCodePage;      /**< source code page. the result code-page is always in Utf8. @see TCodePage */

    TParagraph          outputParagraph;    /**< desired paragraph type. @see TParagraph. */
} VisualString;

/*****************************************************************************/
/** initializes a VisualString structure and set default values.
 * default values are NULL for inputString, 0 for inputLength\n
 * EUtf8 for inputCodePage and ERTL for outputParagraph.
 * @param vs input VisualString structure.
 */
void        bidi_init(VisualString* vs);

/** visualize a single line of characters into a Utf8 output suitable for display devices.
 * @param vs input parameters
 * @param outputStringUtf8  output string in Utf8
 * @param capacity capacity of outputStringUtf8 in bytes.
 * @return the character count of outputStringUtf8 (is not equal to strlen(outputStringUtf8) who return byte length.
 * or returns an negative number as an error. @see TGeneral_Errors
 */
int         bidi_visualize_line(const VisualString* vs, char* outputStringUtf8, size_t capacity);
/*****************************************************************************/
__END_DECLS
/*****************************************************************************/
#endif /* __VISUALSTRING_H__ */
