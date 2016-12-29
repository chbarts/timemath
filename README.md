Trivial time arithmetic code.

Usage:
    % timemath 4:30 + 90
    0:0:6:0
    % timemath 3:0:0:0 - 24:0:0
    2:0:0:0
    % timemath 90
    0:0:1:30

Format: days:hours:minutes:seconds

Everything except seconds is optional, colons side-by-side are taken to
mean a zero in that spot, and no leading colons.
