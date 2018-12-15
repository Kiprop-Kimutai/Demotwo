/******************************************************************************
 *   File: visualstring.c
 *
 *   Copyright(C) 2013, R&D
 *   All rights reserved.
 *
 *   History
 *   2013.12.17  ver 1.0.1
 *
 *   Author:  amir zamani
 *****************************************************************************/
#include "visualstring.h"

#include <memory.h>

#include <fribidi.h>
#include <fribidi-char-sets.h>
#include <fribidi-unicode.h>
/*****************************************************************************/
// removes a character at index from a string and shifts remaining chars to the left.
void
helper_remove_char_at(FriBidiChar* src, int index) {
    int i = index;

    while ( src[i] != 0 ) {
        src[i] = src[i+1];
        i++;
    }
}

// removes all feff chars from a null-terminating string and returns its new length.
int
helper_remove_feff(FriBidiChar* src, int length) {
	int i;
    if ( src == 0    ||    length <= 0 )
        return 0;

    for ( i = 0;    i < length;    i++ ) {
        if ( src[i] == 0xFEFF ) {
            helper_remove_char_at(src, i);
            length--;
        }
    }

    return length;
}
/*****************************************************************************/
void
bidi_init(VisualString *vs) {
    vs->inputString     = 0;
    vs->inputLength     = 0;
    vs->inputCodePage   = EUtf8;

    vs->outputParagraph = ERTL;
}
/*

int
bidi_visualize_line(const VisualString *vs, char *outputStringUtf8, size_t capacity) {

    if ( vs == 0    ||    vs->inputString == 0    ||    vs->inputLength == 0 )
        return -1;

    if ( outputStringUtf8 == 0    ||    capacity <= vs->inputLength )
        return -1;

    if ( vs->inputLength > KDefaultStringCapacity )
        return -1;

    FriBidiCharSet charset = FRIBIDI_CHAR_SET_UTF8;
    if ( vs->inputCodePage == EArabic1256 )
        charset = FRIBIDI_CHAR_SET_CP1256;
    else if ( vs->inputCodePage == EISO8859_6 )
        charset = FRIBIDI_CHAR_SET_CP1256;

    // convert from source charset into Utf32
    FriBidiChar     friSrc[KDefaultStringCapacity+1] = {0};
    FriBidiStrIndex length = fribidi_charset_to_unicode(charset,vs->inputString, vs->inputLength,friSrc);
    if ( length >= KDefaultStringCapacity)
        return -1;

    FriBidiParType parType = FRIBIDI_PAR_RTL;
    if ( vs->outputParagraph == ELTR )
        parType = FRIBIDI_PAR_LTR;

    // make visual for of source
    FriBidiChar     friDes[KDefaultStringCapacity+1] = {0};
    memset(outputStringUtf8, 0, capacity);
    FriBidiLevel level = fribidi_log2vis(friSrc, length,&parType,friDes,0, 0, 0);
    if ( level == 0 )
        return -1;

    // try to remove 0xFEFF chars from string! it's an invalid char in font talbes
    length  = helper_remove_feff(friDes, length);

    // convert back from Utf32 to source charset
    fribidi_unicode_to_charset(charset,friDes, length,outputStringUtf8);
    return length;
}
*/

/*****************************************************************************/
