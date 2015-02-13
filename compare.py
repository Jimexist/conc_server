#!/usr/bin/env python3
import re
import os
import subprocess

CGI_PROGRAMS = ('adder', 'multiplier', 'time', 'rand', 'sqrt')
SERVERS = ('./baseline', './optimized')

class Suite:

    def __init__(self, conc_level, n_req):
        self.conc_level = conc_level
        self.n_req = n_req

    def run(self):
        port = 10003
        for server in SERVERS:
            p = subprocess.Popen([server, str(port)])
            try:
                output = subprocess.check_output([
                    'ab',
                    '-n', str(self.n_req),
                    '-c', str(self.conc_level),
                    '-r',
                    '-e', '{0}_n{1}_c{2}_result.csv'.format(server[2:], self.n_req, self.conc_level),
                    'http://{0}:{1}/cgi-bin/adder'.format('127.0.0.1', port),
                ])
                print(output)
            finally:
                p.terminate()
                p.poll()

def main():
    for server in SERVERS:
        assert os.path.exists(server), 'build binaries first'

    for i in [1, 10, 100, 500, 1000]:
        suite = Suite(i, 10000)
        suite.run()

if __name__ == '__main__':
    main()
