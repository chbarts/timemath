#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

typedef unsigned long long int ull;

typedef struct {
    uint64_t d, h, m, s;
} time;

static time *mktime(void)
{
    time *time;

    if ((time = malloc(sizeof(*time))) == NULL)
        return NULL;

    time->d = 0;
    time->h = 0;
    time->m = 0;
    time->s = 0;

    return time;
}

static uint64_t tosec(time * time)
{
    return (((time->d * 24) + time->h) * 60 + time->m) * 60 + time->s;
}

static void totime(uint64_t sec, time * time)
{
    time->s = sec % 60;
    sec /= 60;
    time->m = sec % 60;
    sec /= 60;
    time->h = sec % 24;
    sec /= 24;
    time->d = sec;
}

/* day:hour:min:sec, all except sec optional */
static void strtotime(char str[], time * time)
{
    char *endptr = str - 1;
    uint64_t tmps[4], res;
    int i = 0;

    do {
        tmps[i++] = strtoull(endptr + 1, &endptr, 10);
    } while ((*endptr != '\0') && (i < 4));

    switch (i) {
    case 1:
        res = tmps[0];
        break;
    case 2:
        res = tmps[0] * 60;
        res += tmps[1];
        break;
    case 3:
        res = tmps[0] * 3600;
        res += tmps[1] * 60;
        res += tmps[2];
        break;
    case 4:
        res = tmps[0] * 24 * 3600;
        res += tmps[1] * 3600;
        res += tmps[2] * 60;
        res += tmps[3];
        break;
    }

    totime(res, time);
}

static void showtime(time * time)
{
    if (time->d > 0) {
        printf("%llu:%02llu:", (ull) time->d, (ull) time->h);
    } else if (time->h > 0) {
        printf("%02llu:", (ull) time->h);
    }

    printf("%02llu:%02llu", (ull) time->m, (ull) time->s);
}

static void usage(void)
{
    puts("timemath time1 [[+-] time2]");
    puts("Add or subtract two times, or just do some time conversion");
    puts("A time is days:hours:minutes:seconds, with all but the seconds optional");
    puts("Two colons side-by-side are taken to mean a zero in that spot, and extra numbers are ignored");
    puts("No leading colons");
    puts("");
    puts("--help    This help");
    puts("--version Version information");
}

static void version(void)
{
    puts("timemath version 1.0");
    puts("Copyright Chris Barts, 2016");
    puts("This is released under the GPL version 3.0");
    puts("or, at your option, any later version");
    puts("Mail <chbarts@gmail.com> for more information");
}

int main(int argc, char *argv[])
{
    time *time1 = NULL, *time2 = NULL;
    uint64_t s1, s2;
    int64_t tmp;

    if (1 == argc) {
        usage();
        return 0;
    }

    if ((argc > 2) && (argc != 4)) {
        fprintf(stderr, "timemath: invalid number of arguments\n");
        usage();
        exit(EXIT_FAILURE);
    }

    if (!strcmp(argv[1], "--help")) {
        usage();
        return 0;
    }

    if (!strcmp(argv[1], "--version")) {
        version();
        return 0;
    }

    if (!isdigit(argv[1][0])) {
        fprintf(stderr, "timemath: invalid option %s\n", argv[1]);
        usage();
        exit(EXIT_FAILURE);
    }

    if ((4 == argc) && ((argv[2][0] != '+') && (argv[2][0] != '-'))) {
        fprintf(stderr,
                "timemath: invalid second argument %s, must be + or -\n",
                argv[2]);
        usage();
        exit(EXIT_FAILURE);
    }

    if ((time1 = mktime()) == NULL) {
        fprintf(stderr, "timemath: memory error!\n");
        exit(EXIT_FAILURE);
    }

    strtotime(argv[1], time1);

    if (4 == argc) {
        if ((time2 = mktime()) == NULL) {
            fprintf(stderr, "timemath: memory error!\n");
            exit(EXIT_FAILURE);
        }

        strtotime(argv[3], time2);

        s1 = tosec(time1);
        s2 = tosec(time2);

        if ('+' == argv[2][0]) {
            tmp = s1 + s2;
        } else {
            tmp = s1 - s2;
        }

        if (tmp < 0) {
            tmp = -tmp;
            totime((uint64_t) tmp, time1);
            printf("-");
        } else {
            totime((uint64_t) tmp, time1);
        }
    }

    showtime(time1);
    puts("");
    free(time1);
    free(time2);

    return 0;
}
