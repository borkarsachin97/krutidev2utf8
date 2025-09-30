#include <glib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <stdio.h>



// Helper function to replace all occurrences of a substring
gchar* string_replace(const gchar* str, const gchar* old, const gchar* new) {
    if (!str || !old || !*old) return g_strdup(str);

    GString *result = g_string_new("");
    const gchar *start = str;
    gsize old_len = strlen(old);

    while (*start) {
        const gchar *found = strstr(start, old);
        if (!found) {
            g_string_append(result, start);
            break;
        }
        
        g_string_append_len(result, start, found - start);
        g_string_append(result, new);
        start = found + old_len;
    }

    return g_string_free(result, FALSE);
}

// Main conversion function
gchar* kruti_to_unicode(const gchar* input) {
    // Define mapping tables (partial for brevity)
    const gchar *kruti_list[] = {
        "ñ", "Q+Z", "sas", "aa", ")Z", "ZZ", "‘", "’", "“", "”", "å", "ƒ", "„", "…", 
        "†", "‡", "ˆ", "‰", "Š", "‹", "¶+", "d+", "[+k", "[+", "x+", "T+", "t+", "M+", 
        "<+", "Q+", ";+", "j+", "u+", "Ùk", "Ù", "ä", "–", "—", "é", "™", "=kk", "f=k", 
        "à", "á", "â", "ã", "ºz", "º", "í", "{k", "{", "=", "«", "Nî", "Vî", "Bî", "Mî", 
        "<î", "|", "K", "}", "J", "Vª", "Mª", "<ªª", "Nª", "Ø", "Ý", "nzZ", "æ", "ç", "Á", 
        "xz", "#", ":", "v‚", "vks", "vkS", "vk", "v", "b±", "Ã", "bZ", "b", "m", "Å", ",s", 
        ",", "_", "ô", "d", "Dk", "D", "[k", "[", "x", "Xk", "X", "Ä", "?k", "?", "³", "pkS", 
        "p", "Pk", "P", "N", "t", "Tk", "T", ">", "÷", "¥", "ê", "ë", "V", "B", "ì", "ï", 
        "M", "<", ".k", ".", "r", "Rk", "R", "Fk", "F", ")", "n", "/k", "èk", "/", "Ë", "è", 
        "u", "Uk", "U", "i", "Ik", "I", "Q", "¶", "c", "Ck", "C", "Hk", "H", "e", "Ek", "E", 
        ";", "¸", "j", "y", "Yk", "Y", "G", "o", "Ok", "O", "'k", "'", "\"k", "\"", "l", "Lk", 
        "L", "g", "È", "z", "Ì", "Í", "Î", "Ï", "Ñ", "Ò", "Ó", "Ô", "Ö", "Ük", "Ü", "‚", "ks", 
        "kS", "k", "h", "q", "w", "`", "s", "S", "a", "¡", "%", "W", "•", "·", "∙", "~j", "~", 
        "\\", "+", " ः", "^", "*", "Þ", "ß", "(", "¼", "½", "¿", "À", "¾", "A", "-", "&", "Œ", 
        "]", "~ ", "@", NULL
    };
    
		
    const gchar *unicode_list[] = {
        "॰", "QZ+", "sa", "a", "र्द्ध", "Z", "\"", "\"", "'", "'", "०", "१", "२", "३", 
        "४", "५", "६", "७", "८", "९", "फ़्", "क़", "ख़", "ख़्", "ग़", "ज़्", "ज़", "ड़", 
        "ढ़", "फ़", "य़", "ऱ", "ऩ", "त्त", "त्त्", "क्त", "दृ", "कृ", "न्न", "न्न्", "=k", "f=", 
        "ह्न", "ह्य", "हृ", "ह्म", "ह्र", "ह्", "द्द", "क्ष", "क्ष्", "त्र", "त्र्", "छ्य", 
        "ट्य", "ठ्य", "ड्य", "ढ्य", "द्य", "ज्ञ", "द्व", "श्र", "ट्र", "ड्र", "ढ्र", "छ्र", 
        "क्र", "फ्र", "र्द्र", "द्र", "प्र", "प्र", "ग्र", "रु", "रू", "ऑ", "ओ", "औ", "आ", 
        "अ", "ईं", "ई", "ई", "इ", "उ", "ऊ", "ऐ", "ए", "ऋ", "क्क", "क", "क", "क्", "ख", 
        "ख्", "ग", "ग", "ग्", "घ", "घ", "घ्", "ङ", "चै", "च", "च", "च्", "छ", "ज", "ज", 
        "ज्", "झ", "झ्", "ञ", "ट्ट", "ट्ठ", "ट", "ठ", "ड्ड", "ड्ढ", "ड", "ढ", "ण", "ण्", 
        "त", "त", "त्", "थ", "थ्", "द्ध", "द", "ध", "ध", "ध्", "ध्", "ध्", "न", "न", "न्", 
        "प", "प", "प्", "फ", "फ्", "ब", "ब", "ब्", "भ", "भ्", "म", "म", "म्", "य", "य्", 
        "र", "ल", "ल", "ल्", "ळ", "व", "व", "व्", "श", "श्", "ष", "ष्", "स", "स", "स्", 
        "ह", "ीं", "्र", "द्द", "ट्ट", "ट्ठ", "ड्ड", "कृ", "भ", "्य", "ड्ढ", "झ्", "श", "श्", 
        "ॉ", "ो", "ौ", "ा", "ी", "ु", "ू", "ृ", "े", "ै", "ं", "ँ", "ः", "ॅ", "ऽ", "ऽ", 
        "ऽ", "्र", "्", "?", "़", ":", "‘", "’", "“", "”", ";", "(", ")", "{", "}", "=", 
        "।", ".", "-", "॰", ",", "् ", "/", NULL
    };

    gchar *converted = g_strdup(input);
    
    if (!converted) return NULL;

    // Apply main symbol replacements
    for (int i = 0; kruti_list[i]; i++) {
        gchar *new_str = string_replace(converted, kruti_list[i], unicode_list[i]);
        g_free(converted);
        converted = new_str;
        if (!converted) return NULL;
    }


    // Apply special substitutions
    gchar *temp = string_replace(converted, "±", "Zं");
    g_free(converted);
    converted = temp;

    temp = string_replace(converted, "Æ", "र्f");
    g_free(converted);
    converted = temp;

    // Convert to wide characters for efficient processing
    glong items_read = 0, items_written = 0;

    gunichar *ucs4 = g_utf8_to_ucs4(converted, -1, &items_read, &items_written, NULL);
    gsize len = items_written;
    if (!ucs4) {
        g_free(converted);
        return NULL;
    }

    // Process "fa" sequences
    for (gsize i = 0; i < len; i++) {
        if (i + 2 < len && ucs4[i] == 'f' && ucs4[i+1] == 'a') {
            ucs4[i] = ucs4[i+2];
            ucs4[i+1] = 0x093F;
            for (gsize j = i+2; j < len - 1; j++) {
                ucs4[j] = ucs4[j+1];
            }
            len--;
        }
    }
    
    // Process 'f' characters
		for (gsize i = 0; i < len; i++) {
			if (ucs4[i] == 'f' && i + 1 < len) {
				gunichar next_char = ucs4[i+1];
				ucs4[i] = next_char;
				ucs4[i+1] = 0x093F; // Devanagari vowel sign i
			}
		}

    // Process "ि्" sequences
    for (gsize i = 0; i < len; i++) {
        if (i + 2 < len && 
            ucs4[i] == 0x093F && // Devanagari vowel sign i
            ucs4[i+1] == 0x094D) { // Devanagari sign virama
            ucs4[i] = 0x094D;
            gunichar temp_char = ucs4[i+2];
            ucs4[i+2] = 0x093F;
            ucs4[i+1] = temp_char;
        }
    }
    

    // Handle 'Z' characters (Reph/Punarmudran)
for (gsize i = 0; i < len; i++) {
    if (ucs4[i] == 'Z' && i > 0) {
        gsize pos = i - 1;
const gunichar matras[] = {
    // Independent vowels
    0x0905, // अ
    0x0906, // आ
    0x0907, // इ
    0x0908, // ई
    0x0909, // उ
    0x090A, // ऊ
    0x090F, // ए
    0x0910, // ऐ
    0x0913, // ओ
    0x0914, // औ
    
    // Dependent vowel signs (matras)
    0x093E, // ा (AA)
    0x093F, // ि (I)
    0x0940, // ी (II)
    0x0941, // ु (U)
    0x0942, // ू (UU)
    0x0943, // ृ (RI)
    0x0947, // े (E)
    0x0948, // ै (AI)
    0x094B, // ो (O)
    0x094C, // ौ (AU)
    
    // Diacritical marks
    0x0902, // ं (Anusvara)
    0x0901, // ँ (Chandrabindu)
    0x0945, // ॅ (Candral E)
    // Terminator
    0
};        
        // Skip over matras (vowel signs) to find the base consonant
        while (pos > 0) {
            gboolean is_matra = FALSE;
            for (int m = 0; matras[m]; m++) {
                if (ucs4[pos] == matras[m]) {
                    is_matra = TRUE;
                    break;
                }
            }
            if (!is_matra) break;
            pos--;
        }

        // Extract the syllable (consonant + matras)
        gsize syllable_len = i - pos;
        gunichar *syllable = g_new(gunichar, syllable_len);
        for (gsize j = 0; j < syllable_len; j++) {
            syllable[j] = ucs4[pos + j];
        }

        // Replace 'Z' with RA + Virama (र्)
        ucs4[pos] = 0x0930;  // RA (र)
        ucs4[pos + 1] = 0x094D;  // Virama (्)

        // Shift remaining characters to the right
        gsize new_len = len + 1;  // 'Z' is replaced by RA + Virama (no extra length)
        ucs4 = g_renew(gunichar, ucs4, new_len);

        // Move characters after the syllable to make space
        for (gsize j = len; j > pos + syllable_len; j--) {
            ucs4[j] = ucs4[j - 1];
        }

        // Reinsert the syllable after RA + Virama
        for (gsize j = 0; j < syllable_len; j++) {
            ucs4[pos + 2 + j] = syllable[j];
        }

        len = new_len;
        g_free(syllable);
        i = pos + 1;  // Skip ahead to avoid reprocessing
    }
} 

/*
 * I thought this was kruti dev rules
 * because improper rendering, but
 * Important: It turned out to be
 * 			bug in poppler pdf rendering
 * 			library
// fix for chandrabindu after comma and dandaa
for (gsize i = 0; i + 1 < len; i++) {
    if ((ucs4[i] == 0x002C || ucs4[i] == 0x0964) && (ucs4[i+1] == 0x0902 || ucs4[i+1] == 0x0901)) {
        gunichar t = ucs4[i+1];
        ucs4[i+1] = ucs4[i];
        ucs4[i] = t;
    }
}

// fix for ae 
for (gsize i = 0; i + 1 < len; i++) {
    if (ucs4[i+1] == 0x0020 && ucs4[i+2] == 0x0947 && ucs4[i+3] == 0x0020) {
        gunichar t = ucs4[i+2];
        ucs4[i+1] = ucs4[i];
        ucs4[i] = t;
        ucs4[i+2] = 0x0020;
    }
}

	// pwfa d // pawfd
// fix for ae pwaf d to pawfd
// 'pwaf d' pawfd pawf d
// we got 'fa ','pwfa d', pawfd, 
for (gsize i = 0; i + 1 < len; i++) {
    if (ucs4[i+1] == 0x0902 && ucs4[i+2] == 0x093F && ucs4[i+3] == 0x0020) {
        gunichar w = ucs4[i];
        gunichar f = ucs4[i+1];
        gunichar a = ucs4[i+2];
        ucs4[i] = w;
        ucs4[i+1] = f;
        ucs4[i+2] = ucs4[i+4];
        ucs4[i+3] = a;
        ucs4[i+4] = ' ';
        
    }
}

// lacèa k, 

*/
    // Convert back to UTF-8
    gchar *result = g_ucs4_to_utf8(ucs4, len, NULL, NULL, NULL);
    g_free(ucs4);
    g_free(converted);

    return result;

}

int main() {
    printf("KrutiDev to Unicode Converter - Paragraph Test\n");
    printf("============================================\n");

    // The KrutiDev paragraph to test
const gchar *kruti_paragraph = 
"vuqlkj] Hkkjrh; lekt esa vk;ksZa] nzfoMksa] eaxksfy;ksa vkSj 'kdksa dk lfEeJ.k gSA ;s ";
    printf("Original KrutiDev Text:\n");
    printf("-----------------------\n");
    printf("%s\n\n", kruti_paragraph);

    // Convert the paragraph
    gchar *converted_paragraph = kruti_to_unicode(kruti_paragraph);
    
    if (converted_paragraph) {
        printf("Converted Unicode Text:\n");
        printf("-----------------------\n");
        printf("%s\n\n", converted_paragraph);

        printf("Conversion Statistics:\n");
        printf("----------------------\n");
        printf("Original length: %ld characters\n", strlen(kruti_paragraph));
        printf("Converted length: %ld characters\n", strlen(converted_paragraph));
        printf("Unicode code points: %ld\n", g_utf8_strlen(converted_paragraph, -1));
        
        g_free(converted_paragraph);
    } else {
        printf("Conversion failed!\n");
    }

    return 0;
}

