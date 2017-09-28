#!/usr/bin/env python
#
# Copyright 2013, Couchbase, Inc.
# All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
from __builtin__ import str

print "test1"
import argparse
from time import time

from twisted.internet import reactor

from txcouchbase.bucket import RawBucket, Bucket
from couchbase import FMT_BYTES
from couchbase.transcoder import Transcoder
print "test2"
ap = argparse.ArgumentParser()

    
ap.add_argument('-t','--num-threads',type=int,default=1,help='The number of threads to use')

ap.add_argument('-d', '--delay', default=0, type=float,
                help="Number of seconds to wait between each op. "
                "may be a fraction")

ap.add_argument('-C', '--clients', default=1, type=int,
                help="Number of clients (nthreads are per-client)")

ap.add_argument('--deferreds', action='store_true', default=False,
                help="Whether to use Deferreds (or normal callbacks)")
ap.add_argument('-U', '--connstr', default='couchbase://localhost/default',
                help="Connection string")
ap.add_argument('-p', '--password', default=None, type=str)
ap.add_argument('-D', '--duration', default=10, type=int,
                help="Duration of run (in seconds)")

ap.add_argument('-T', '--transcoder', default=False,
                action='store_true',
                help="Use the Transcoder object rather than built-in "
                "conversion routines")

ap.add_argument('--ksize', default=12, type=int,
                help="Key size to use")

ap.add_argument('--vsize', default=128, type=int,
                help="Value size to use")

ap.add_argument('-B','--batch-size', type=int, default=100, help="Batch size to use")


ap.add_argument('-I', '--num-items', type=int, default=1000, help="Set the total number of items the"
                "workload will access within the cluster. This will also determine the working set size at the server and may affect disk latencies if set to a high number.")
ap.add_argument('-r', '--set-pct', type=int, help="The percentage of operations which should be mutations. A value of 100 means only mutations while a value of 0 means only retrievals.")
ap.add_argument('-n', '--no-population', type=bool, help="By default cbc-pillowfight will load all the items (see --num-items) into the cluster and then begin performing the normal workload. Specifying this option bypasses this stage. Useful if the items have already been loaded in a previous run.")


ap.add_argument('-E', '--pause-at-end', type=bool, default= False,help="When the workload completes, do not exit immediately, but wait for user input. This is helpful for analyzing open socket connections and state.")
ap.add_argument('-c', '--num-cycles', type = int, default =1, help="Specify the number of times the workload should cycle. During each cycle an amount of --batch-size operations are executed. Setting this to -1 will cause the workload to run infinitely.")
ap.add_argument('-I','--numItems',type=int,default=1000,help='Number of items to operate on')
ap.add_argument('-p','--key-prefix', type=str, default="", help="Set the prefix to prepend to all keys in the cluster. Useful if you do not wish the items to conflict with existing data."
               ) 

ap.add_argument('-s','--random-seed',type=int,default=0,help='Specify random seed').hide()
ap.add_argument('-m','--min-size',type=int,default=50,help='Set minimum payload size')
ap.add_argument('-M','--max-size',type=int,default=5120,help='Set maximum payload size')
ap.add_argument('-n','--no-population',type=bool,default=True,help='Skip population')
ap.add_argument('-E','--pause-at-end',type=bool,default=False,help='Pause at end of run (holding connections open) until user input')
ap.add_argument('-c','--num-cycles',type=int,default=-1,help='Number of cycles to be run until exiting. Set to -1 to loop infinitely')
ap.add_argument('--sequential',type=int,default=False,help='Use sequential access (instead of random)')
ap.add_argument('--start-at',type=int,default=0,help='For sequential access, set the first item')
ap.add_argument('--rate-limit',type=int,default=0,help='Set operations per second limit (per thread)')
ap.add_argument('--docs',type=bool,default=False,help='User documents to load (overrides --min-size and --max-size')
ap.add_argument('--json',type=bool,default=False,help='Enable writing JSON values (rather than bytes)')
ap.add_argument('--template',type=list,help='Values for templates to be inserted into user documents')
ap.add_argument('--subdoc',type=bool,default=False,help='Use subdoc instead of fulldoc operations')
ap.add_argument('--noop',type=bool,default=False,help='Use NOOP instead of document operations')
ap.add_argument('--pathcount',type=int,default=1,help='Number of subdoc paths per command')
ap.add_argument('--populate-only',type=bool,help='Exit after documents have been populated')
ap.add_argument('e','--expiry',type=bool,help='Set TTL for items')


options = ap.parse_args()

class Runner(object):

    def generate_value(self, rangesize, x):
        return b'V' * (options.min_size + (rangesize / options.batch_size)) * x


    def generate_key(self, x):
        return self.key_prefix + self.key + str(x)

    def __init__(self, cb):
        self.cb = cb
        self.delay = options.delay
        self.key = 'K' * options.ksize
        self.value = b'V' * options.vsize
        self.kv = {}
        rangesize=options.max_size-options.min_size
        for x in range(options.batch_size):
            self.kv[self.generate_key(x)] =  self.generate_value(rangesize, x)
        self.wait_time = 0
        self.opcount = 0
        self.end_time = time() + options.duration
        self._do_stop = False
        self.start()

    def _schedule_raw(self, *args):
        opres = self.cb.upsert_multi(self.kv, format=FMT_BYTES)
        opres.callback = self._schedule_raw
        self.opcount += 1

    def _schedule_deferred(self, *args):
        rv = self.cb.upsertMulti(self.kv, format=FMT_BYTES)
        rv.addCallback(self._schedule_deferred)
        self.opcount += options.batch_size

    def start(self):
        if options.deferreds:
            self._schedule_deferred()
        else:
            self.cb._async_raw = True
            self._schedule_raw()

    def stop(self):
        self._do_stop = True

global_begin = time()
runners = []
clients = []
kwargs = {
    'connstr' : options.connstr,
    'password': options.password,
    'unlock_gil': False
}
if options.transcoder:
    kwargs['transcoder'] = Transcoder()

for _ in range(options.clients):
    cls = Bucket if options.deferreds else RawBucket
    cb = cls(**kwargs)
    clients.append(cb)
    d = cb.connect()

    def _on_connected(unused, client):
        for _ in range(options.threads):
            r = Runner(client)
            runners.append(r)
    d.addCallback(_on_connected, cb)

def stop_all():
    [r.stop() for r in runners]
    reactor.stop()

reactor.callLater(options.duration, stop_all)
reactor.run()


global_duration = time() - global_begin
total_ops = sum([r.opcount for r in runners])
total_time = 0
for r in runners:
    total_time += r.wait_time

print("Total run took an absolute time of %0.2f seconds" % (global_duration,))
print("Did a total of %d operations" % (total_ops,))
print("[ABS] %0.2f ops/second" % (float(total_ops)/float(global_duration),))
