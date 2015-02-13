#!/usr/bin/env python
import re
import os
import subprocess

CGI_PROGRAMS = ('adder', 'multiplier', 'time', 'rand', 'sqrt')
SERVERS = ('./baseline', './optimized')

class Suite:
    
    def __init__(self, conc_level, n_req):
        self.conc_level = conc_level
        self.n_req = n_req
            
    def parse_result(self, result):
        req_count = re.search(r'.*Total: connections \d+ requests (\d+) replies \d+.*', result).group(1)
        assert re.search(r'.*Reply status: 1xx=0 2xx={0} 3xx=0 4xx=0 5xx=0.*'\
            .format(req_count), result), 'reply status count does not look right: ' + result
        assert re.search(r'.*Errors: total 0 .*', result), 'Error occurred: ' + result
        test_duration = re.search(r'.*test-duration ([\d.]+).*', result).group(1)
        conn_rate = re.search(r'.*Connection rate: ([\d.]+).*', result).group(1)
        return req_count, test_duration, conn_rate
    
    def run(self):
        port = 8003
        for server in SERVERS:
            p = subprocess.Popen([server, str(port)])
            try:
                output = subprocess.check_output([
                    'httperf',
                    '--hog',
                    '--server', '127.0.0.1',
                    '--port', str(port),
                    '--wlog', 'y,wlog',
                    '--num-conns', str(self.n_req),
                ])
                print self.parse_result(output)
            finally:
                p.terminate()
                p.poll()
            
def write_wlog_file():
    with open('wlog', 'w') as fout:
        for p in CGI_PROGRAMS:
            fout.write('/cgi-bin/' + p + '\0')

def main():
    write_wlog_file()
    for server in SERVERS:
        assert os.path.exists(server), 'build binaries first'
    
    suite = Suite(500, 5000)
    suite.run()
    
if __name__ == '__main__':
    main()