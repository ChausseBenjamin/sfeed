#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>

#include "util.h"

/*
 * Taken from OpenBSD.
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t
strlcpy(char *dst, const char *src, size_t siz) {
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}
	/* not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
				*d = '\0'; /* NUL-terminate dst */
		while (*s++)
			;
	}
	return(s - src - 1); /* count does not include NUL */
}

/* print link; if link is relative use baseurl to make it absolute */
void
printlink(const char *link, const char *baseurl, FILE *fp) {
	const char *ebaseproto, *ebasedomain, *p;
	int isrelative;

	/* protocol part */
	for(p = link; *p && (isalpha((int)*p) || isdigit((int)*p) ||
		                *p == '+' || *p == '-' || *p == '.'); p++);
	/* relative link (baseurl is used). */
	isrelative = strncmp(p, "://", strlen("://"));
	if(isrelative) {
		if((ebaseproto = strstr(baseurl, "://"))) {
			ebaseproto += strlen("://");
			fwrite(baseurl, 1, ebaseproto - baseurl, fp);
		} else {
			ebaseproto = baseurl;
			if(*baseurl || (link[0] == '/' && link[1] == '/'))
				fputs("http://", fp);
		}
		if(link[0] == '/') { /* relative to baseurl domain (not path).  */
			if(link[1] == '/') /* absolute url but with protocol from baseurl. */
				link += 2;
			else if((ebasedomain = strchr(ebaseproto, '/')))
				/* relative to baseurl and baseurl path. */
				fwrite(ebaseproto, 1, ebasedomain - ebaseproto, fp);
			else
				fputs(ebaseproto, stdout);
		} else if((ebasedomain = strrchr(ebaseproto, '/'))) {
			/* relative to baseurl and baseurl path. */
			fwrite(ebaseproto, 1, ebasedomain - ebaseproto + 1, fp);
		} else {
			fputs(ebaseproto, fp);
			if(*baseurl && *link)
				fputc('/', fp);
		}
	}
	fputs(link, fp);
}

/* read a field-separated line from 'fp',
 * separated by a character 'separator',
 * 'fields' is a list of pointer with a maximum size of 'maxfields'.
 * 'line' buffer is allocated using malloc, 'size' will contain the
 * allocated buffer size.
 * returns: amount of fields read. */
unsigned int
parseline(char **line, size_t *size, char **fields,
          unsigned int maxfields, int separator, FILE *fp)
{
	unsigned int i = 0;
	char *prev, *s;

	if(getline(line, size, fp) > 0) {
		for(prev = *line; (s = strchr(prev, separator)) && i <= maxfields; i++) {
			*s = '\0';
			fields[i] = prev;
			prev = s + 1;
		}
		fields[i] = prev;
		for(i++; i < maxfields; i++) /* make non-parsed fields empty. */
			fields[i] = "";
	}
	return i;
}

/* print feed name for id; spaces and tabs in string as "-"
 * (spaces in anchors are not valid). */
void
printfeednameid(const char *s, FILE *fp) {
	for(; *s; s++)
		fputc(isspace((int)*s) ? '-' : tolower((int)*s), fp);
}

void
printhtmlencoded(const char *s, FILE *fp) {
	for(; *s; s++) {
		switch(*s) {
		case '<': fputs("&lt;", fp); break;
		case '>': fputs("&gt;", fp); break;
/*		case '&': fputs("&amp;", fp); break;*/
		default:
			fputc(*s, fp);
		}
	}
}

void
feedsfree(struct feed *f) {
	struct feed *next = NULL;

	for(; f; f = next) {
		next = f->next;
		f->next = NULL;
		free(f->name);
		f->name = NULL;
		free(f);
	}
}

void
printutf8pad(FILE *fp, const char *s, size_t len, int pad) {
    size_t n = 0, i;

    for(i = 0; s[i] && n < len; i++) {
        /* start of character */
        if((s[i] & 0xc0) != 0x80)
            n++;
        putc(s[i], fp);
    }
    for(; n < len; n++)
        putc(pad, fp);
}
