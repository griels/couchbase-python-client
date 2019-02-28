"""Cluster Creation and Resource Allocation"""
import couchbase.v3.cluster
from couchbase.v3 import Durations, QueryOptions, QueryParameters
from couchbase.v3.collection import UpsertOptions
from couchbase.v3.durability import Durability
from couchbase.cluster import PasswordAuthenticator

cluster = couchbase.v3.cluster.Cluster("couchbase://127.0.0.1/default")
cluster.authenticate(PasswordAuthenticator("default", "password"))
bucket = cluster.bucket("default")
collection = bucket.default_collection()

"""Basic Key/Value Operations (CRUD)
Insert a JSON document representing a Person type into Couchbase server and retrieve it.



Basic Key/Value Operations
"""


class AttrDict(dict):
    def __init__(self, *args, **kwargs):
        super(AttrDict, self).__init__(*args, **kwargs)

    def __getattr__(self, item):
        return dict.get(self, item)


class Person(AttrDict):
    def __init__(self, name=None, age=None, animals=None, attributes=None):
        super(Person, self).__init__(name=name, age=age, animals=animals, attributes=attributes)


class Attributes(AttrDict):
    def __init__(self, dimensions=None, hair=None, hobbies=None):
        super(Attributes, self).__init__(dimensions=dimensions, hair=hair, hobbies=hobbies)


class Dimensions(AttrDict):
    def __init__(self, height=None, weight=None):
        super(Dimensions, self).__init__(height=height, weight=weight)


class Hobby(AttrDict):
    def __init__(self, name=None, type=None, details=None):
        super(Hobby, self).__init__(name=name, type=type, details=details)


# class Details:
#    def __init__(self, ):
Details = dict
Location = dict
item = Person(name="Joan Deere", age=28, animals=["kitty", "puppy"],
              attributes=Attributes(dimensions=Dimensions(height=65,
                                                          weight=120
                                                          ),
                                    hair="brown",
                                    hobbies=[Hobby(
                                        name="Curling",
                                        type="winter",
                                        details=Details(location=Location(
                                            long=-121.886330,
                                            lat=37.338207
                                        )
                                        ))]
                                    )
              )
bucket = cluster.bucket("default")

id = "p01"
result = collection.upsert(id, item, UpsertOptions(expiration=Durations.days(1)))
get = collection.get(id, timeout=Durations.seconds(10))

person = get.content_as[Person]

del person, result
"""Basic Key/Value Projections
Retrieve a projection for the fields "age", "name", and "attributes.hair" for an existing JSON document in Couchbase server.

Basic K/V Projections
"""
id = "p01"

result = collection.get(id, couchbase.v3.GetOptions.project("name", "age", "attributes.hair"))

person = result.content_as[Person]
print("Age={age}, Name={name}, Hair{attributes.hair}".format(**person))

"""
Basic Durability using Syncronous Replication
Insert a document into Couchbase server using the new durability feature Synchronous Replication.

Basic Synchronous Replication Durability
"""
id = "p02";
result = collection.insert(id, Person
    (
    name="Jon Henry",
    age=34
), durability_level=Durability.MajorityAndPersistActive, timeout=Durations.seconds(30))

"""Basic N1QL Query
Select all JSON documents of type "person" from Couchbase Server.

Basic N1QL Query
"""
statement = "SELECT * FROM `default` WHERE type=$type"

results = cluster.query(
    statement,
    parameters=QueryParameters(type="person"), timeout=Durations.seconds(75))

for result in results:
    print(result)
