Trivial time arithmetic code.

Usage:

    % timemath 4:30 + 90

    06:00

    % timemath 3:0:0:0 - 24:0:0

    2:00:00:00

    % timemath 90

    01:30

Output Format: days:hours:minutes:seconds or minutes:seconds if days and hours are both zero

Input Format: days:hours:minutes:seconds

Everything except seconds is optional, colons side-by-side are taken to
mean a zero in that spot, and no leading colons.
