from couchbase.exceptions import CouchbaseError


class KeyValueException(CouchbaseError):
    """
    A server error occurred while executing a K/V operation. Assumes that the service has returned a response.
    Message
    The XError message returned by the memcached server
    Properties
    The memcached response status
    XError and Enhanced error message information
    The document id
    The opaque used in the request"""
    pass

"""
Derived Exceptions
Expected to be handled specifically by the application to perform an additional action such as retrying to check if the key has become unlocked.
"""

class KeyNotFoundException(KeyValueException):
    pass
class KeyExistsException(KeyValueException):
    pass
class ValueTooBigException(KeyValueException):
    pass
class KeyLockedException(KeyValueException):
    pass
class TempFailException(KeyValueException):
    pass
class PathNotFoundException(KeyValueException):
    pass
class PathExistsException(KeyValueException):
    pass
class InvalidRangeException(KeyValueException):
    pass
class KeyDeletedException(KeyValueException):
    pass
class QueryException(KeyValueException):
    """
    A server error occurred while executing a N1QL query. Assumes that that the service has returned a response.
    Message
    The error message returned by the Query service
    Properties
    The error(s) returned by response from the server by the Query/N1QL service
    Any additional information returned by the server, the node it executed on, payload, HTTP status
    """
    pass
"""
Derived Exceptions
TBD? May be nothing to extend...
"""
class SearchException(CouchbaseError):
    pass

    """Message
    The error message returned by the Search service
    Properties
    The error(s) returned by response from the server by the FTS Service
    Any additional information returned by the server, the node it executed on, payload, HTTP status
    """

"""Derived Exceptions
TBD? May be nothing to extend...
"""
class AnalyticsException(CouchbaseError):
    pass
    """A server error occurred while executing an Analytics query. Assumes that that the service has returned a response
    Message
    The error message returned by the Analytics service
    Properties
    The error(s) returned by response from the server, contextId, any additional information returned by the server, the node it executed on, payload, HTTP status.
    """
"""
Derived Exceptions
TBD? May be nothing to extend...
"""
class ViewException(CouchbaseError):
    """A server error occurred while executing a View query.  Assumes that that the service has returned a response.
    Message
    The error message returned by the View service
    Properties
    The error(s) returned by response from the server, contextId, any additional information returned by the server, the node it executed on, payload, HTTP status.
    """
    pass

class PartialViewResultException(ViewException):
    #? (returns rows that it did get)
    pass

class InvalidConfigurationException(CouchbaseError):
    """An invalid configuration was supplied to the client.
    Message
    "A configuration error has occurred." details and inner exceptions, any stacktrace info.
    Properties
    TBD
    """
    pass
class BootstrappingException(CouchbaseError):
    """The client cannot initiate or fails while performing the bootstrap process.
    Message
    "A bootstrapping error has occurred." details and inner exceptions, any stacktrace info.
    Properties
    TBD"""
    pass
class ServiceNotFoundException(CouchbaseError):
    """The client requests or queries a service that is not enabled or available in the cluster.
    Message
    "The service requested is not enabled or cannot be found on the node requested.." details and inner exceptions, any stacktrace info.
    Properties
    TBD"""

class TimeoutException(CouchbaseError):
    """---
    Message
    Properties
    Reason: (Exception) Explains the underlying reason we expect this was caused.
    """
    pass
class NetworkException(CouchbaseError):
    """A network error happens while executing a query or performing the K/V operation; the query or operation does not succeed with a valid service response.
    Message
    Properties
    Bubble up any platform idiomatic information."""
    pass
class NodeUnavailableException(CouchbaseError):
    """The client attempts to use a node which is either offline or cannot fulfill a request.
    Message
    "The node that the operation has been requested on is down or not available". details and inner exceptions, any stacktrace info.
    Properties
    TBD"""
    pass
class BucketMissingException(CouchbaseError):
    """The application attempts to open or use a bucket which does exist or is not available at that time.
    Message
    "The requested bucket '{bucketname}' cannot be found."
    Properties
    TBD"""
    pass
class CollectionMissingException(CouchbaseError):
    """The application attempts to open or use a collection which does not exist or is not available at that time.
    Message
    "The requested collection '{collectionname}' cannot be found."
    Properties
    TBD
    """
    pass

class AuthenticationException(CouchbaseError):
    """An authorization failure is returned by the server for given resource and credentials.
    Message
    "An authorization error has occurred"
    Properties
    TBD"""
class AccessDeniedException(CouchbaseError):
    pass


# previous exceptions


class CASMismatchException(CouchbaseError):
    pass


class ReplicaNotConfiguredException(CouchbaseError):
    pass


class DurabilityAmbiguous(CouchbaseError):
    pass


class DocumentConcurrentlyModifiedException(CouchbaseError):
    pass


class DocumentMutationLostException(CouchbaseError):
    pass


class ReplicaNotAvailableException(CouchbaseError):
    pass
