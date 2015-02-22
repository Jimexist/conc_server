# Concurrent Server

A concurrent server written in C, for CMU course 18-845.

- both servers use ad-hoc threading for client requests
- the baseline server uses `fork`/`exec` for cgi-bins
- the optimized server uses dynamic loading/linking for cgi-bins, and pthread read-write lock for thread safety of code cache
