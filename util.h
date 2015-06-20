#include <stdio.h>
#include <time.h>

#ifdef COMPAT
#include "compat.h"
#endif

#define ISUTF8(c) (((c) & 0xc0) != 0x80)
#define LEN(x) (sizeof (x) / sizeof *(x))

/* feed info */
struct feed {
	char *         name;     /* feed name */
	unsigned long  totalnew; /* amount of new items per feed */
	unsigned long  total;    /* total items */
	time_t         timenewest;
	char           timenewestformat[64];
};

enum { FieldUnixTimestamp = 0, FieldTimeFormatted, FieldTitle, FieldLink,
       FieldContent, FieldContentType, FieldId, FieldAuthor, FieldFeedType,
       FieldLast };

ssize_t chartoxmlentity(int, char *, size_t);
int     parseline(char **, size_t *, char **, unsigned int, int, FILE *);
void    printcontent(const char *, FILE *);
void    printxmlencoded(const char *, FILE *);
void    printlink(const char *, const char *, FILE *);
void    printurlencoded(const char *, size_t, FILE *);
void    printutf8pad(FILE *, const char *, size_t, int);
int     strtotime(const char *, time_t *);
char   *trimstart(const char *);
char   *trimend(const char *);
char   *xbasename(const char *);


