'''
Created on 19 Oct 2017

@author: ellis_breen
'''
import sys
import re
from nose import run_exit

#sys.argv[0] = re.sub(r'(-script\.pyw?|\.exe)?$', '', sys.argv[0])

#sys.argv.append("-v")
#sys.argv.append("couchbase.tests.cases.enh_err_t")
def main():
    run_exit(argv=['-v','couchbase.tests.cases.enh_err_t'])    
if __name__ == '__main__':    
    main()

