'''
Created on 18 Oct 2017

@author: ellis_breen
'''
import unittest

from couchbase.admin import Admin
from couchbase.bucket import Bucket
from couchbase.result import HttpResult
from couchbase.connstr import ConnectionString
from couchbase.exceptions import (
    ArgumentError, AuthError, CouchbaseError,
    CouchbaseNetworkError, HTTPError)
from couchbase.tests.base import CouchbaseTestCase, SkipTest
from couchbase.auth_domain import AuthDomain
import sys

class EnhancedErrorTest(CouchbaseTestCase):

    def setUp(self):
        super(EnhancedErrorTest, self).setUp()
        self.admin = self.make_admin_connection()

    def tearDown(self):
        super(EnhancedErrorTest, self).tearDown()
        if self.should_check_refcount:
            rc = sys.getrefcount(self.admin)
            #TODO: revise GC handling - broken on Mac
            #self.assertEqual(rc, 2)

        del self.admin


    def test_enhanced_err_present_authorisation(self):
        users=[('writer',('s3cr3t',[('data_reader', 'default'), ('data_writer', 'default')])),
               ('reader',('s3cr3t',[('data_reader', 'default')]))]
 
        for user in users:
            (userid, password, roles) = user[0],user[1][0],user[1][1]
            # add user
            self.admin.user_upsert(AuthDomain.Local, userid, password, roles)
            try:
                
                print
                # get all users
            except CouchbaseError as e:
                if userid=="writer":
                    raise e
                else:
                    self.assertRegexpMatches(e.context,".*auth*","doesn't match")
            finally:
    
            # remove user
                self.admin.user_remove(AuthDomain.Local, userid)
            
        pass


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()