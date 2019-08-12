#include <stdio.h>
#include <stdbool.h>
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

static void showtime(time * time, bool day)
{
    uint64_t hour = time->h;

    if (time->d > 0) {
        if (day) {
            printf("%llu:%02llu:", (ull) time->d, (ull) time->h);
        } else {
            hour += time->d * 24;

            if (hour > 0) {
                printf("%llu:", (ull) hour);
            }
        }
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
    puts("-h        Hours are the largest time unit printed. For example, 20:00:00 + 5:00:00 prints 25:00:00, not 1:01:00:00");
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
    int first_time = 1;
    bool day = true;
    uint64_t s1, s2;
    int64_t tmp;

    if (1 == argc) {
        usage();
        return 0;
    }

    if (!strcmp(argv[1], "-h")) {
        if (2 == argc) {
            usage();
            exit(EXIT_SUCCESS);
        } else if (argc != 5) {
            fprintf(stderr, "timemath: invalid number of arguments\n");
            usage();
            exit(EXIT_FAILURE);
        }

        first_time = 2;
        day = false;
    } else if ((argc > 2) && (argc != 4)) {
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

    if (!isdigit(argv[first_time][0])) {
        fprintf(stderr, "timemath: invalid option %s\n", argv[1]);
        usage();
        exit(EXIT_FAILURE);
    }

    if ((((1 == first_time) ? 4 : 5) == argc)
        && ((argv[first_time + 1][0] != '+')
            && (argv[first_time + 1][0] != '-'))) {
        fprintf(stderr,
                "timemath: invalid second argument %s, must be + or -\n",
                argv[first_time + 1]);
        usage();
        exit(EXIT_FAILURE);
    }

    if ((time1 = mktime()) == NULL) {
        fprintf(stderr, "timemath: memory error!\n");
        exit(EXIT_FAILURE);
    }

    strtotime(argv[first_time], time1);

    if (((1 == first_time) ? 4 : 5) == argc) {
        if ((time2 = mktime()) == NULL) {
            fprintf(stderr, "timemath: memory error!\n");
            exit(EXIT_FAILURE);
        }

        strtotime(argv[first_time + 2], time2);

        s1 = tosec(time1);
        s2 = tosec(time2);

        if ('+' == argv[first_time + 1][0]) {
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

    showtime(time1, day);
    puts("");
    free(time1);
    free(time2);

    return 0;
}
