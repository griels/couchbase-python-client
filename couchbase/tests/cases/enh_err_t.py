'''
Created on 18 Oct 2017

@author: ellis_breen
'''
import unittest

from couchbase.exceptions import ( CouchbaseError )
from couchbase.tests.base import MockTestCase, ConnectionTestCase,\
    CouchbaseTestCase
from couchbase.auth_domain import AuthDomain
import sys
import couchbase
import logging
logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)
couchbase.enable_logging()

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
        #users=[('writer',('s3cr3t',[('data_reader', 'default'), ('data_writer', 'default')])),
        users=[       ('reader',('s3cr3t',[('data_reader', 'default')]))]
        #self.mockclient._do_request("SET_ENHANCED_ERRORS",{"enabled":True})
        for user in users:
            print str(user)
            (userid, password, roles) = user[0],user[1][0],user[1][1]
            # add user
            self.admin.user_upsert(AuthDomain.Local, userid, password, roles)
            try:
                connection = self.make_connection(username=userid,password=password)
                rv = connection.upsert(self.gen_key(), 'value1')
                # get all users
            except CouchbaseError as e:
                if userid=="writer":
                    raise e
                else:
                    print str(e)
                    self.assertRegexpMatches(e.context,".*auth*","doesn't match")
            finally:
                pass
            #remove user
            #    self.admin.user_remove(AuthDomain.Local, userid)
            
        pass


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()