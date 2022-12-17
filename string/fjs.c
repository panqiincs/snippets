/*
   fjs.c

   FJS is a very fast algorithm for finding every occurrence
   of a string p of length m in a string x of length n.
   For details see <https://cgjennings.ca/articles/fjs/>.

   Christopher G. Jennings.
   See LICENSE.md for license details (MIT license).
*/

#include <stdio.h>
#include <string.h>

typedef unsigned char CTYPE;   // type of alphabet letters

// For large alphabets, such as Unicode, see the Web page above
// for techniques to improve performance

#define ALPHA      (256)       // alphabet size
#define MAX_PATLEN (100)       // maximum pattern length

int betap[MAX_PATLEN+1];
int delta[ALPHA];

void output(int pos) {
    static int matches = 0;
    printf("match %d found at position %d\n", ++matches, pos);
}

void make_betap(const CTYPE *pat, int pat_len) {
    int i = 0;
    int j = -1;
    betap[0] = -1;

    while (i < pat_len) {
        while ((j > -1) && (pat[i] != pat[j])) {
            j = betap[j];
        }
        if (pat[++i] == pat[++j]) {
            betap[i] = betap[j];
        } else {
            betap[i] = j;
        }
    }
}

void make_delta(const CTYPE *pat, int pat_len) {
    int i;

    for (i = 0; i < ALPHA; ++i) {
        delta[i] = pat_len + 1;
    }
    for (i = 0; i < pat_len; ++i) {
        delta[pat[i]] = pat_len - i;
    }
}

void FJS(const CTYPE *text, int text_len, const CTYPE *pat, int pat_len) {
    if (pat_len < 1) {
        return;
    }
    make_betap(pat, pat_len);
    make_delta(pat, pat_len);

    int i = 0, j = 0, mp = pat_len - 1, ip = mp;
    while (ip < text_len) {
        if (j <= 0) {
            while (pat[mp] != text[ip]) {
                ip += delta[text[ip+1]];
                if (ip >= text_len) {
                    return;
                }
            }
            j = 0;
            i = ip - mp;
            while ((j < mp) && (text[i] == pat[j])) {
                ++i;
                ++j;
            }
            if (j == mp) {
                output(i-mp);
                ++i;
                ++j;
            }
            if (j <= 0) {
                ++i;
            } else {
                j = betap[j];
            }
        } else {
            while ((j < pat_len) && (text[i] == pat[j])) {
                ++i;
                ++j;
            }
            if (j == pat_len) {
                output(i - pat_len);
            }
            j = betap[j];
        }
        ip = i + mp - j;
    }
}

int main(int argc, char **argv)
{
    if (argc == 3) {
        if (strlen(argv[2]) <= MAX_PATLEN) {
            FJS((CTYPE *)argv[1], strlen(argv[1]), (CTYPE *)argv[2], strlen(argv[2]));
        } else {
            printf("Recompile with MAX_PATLEN >= %d\n", strlen(argv[2]));
        }
    } else {
        printf("Usage: %s text pattern\n", argv[0]);
    }

    return 0;
}
