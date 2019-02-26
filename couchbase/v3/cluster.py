from couchbase.cluster import Cluster as SDK2Cluster
from couchbase.v3 import OptionBlock, QueryOptions, Bucket
from couchbase.v3.bucket import BucketOptions


class ClusterOptions(OptionBlock):
    pass

class Cluster(SDK2Cluster):
    def __init__(self,
                 conn,  # type: str
                 options  # type: ClusterOptions
                 ):
        pass
    def bucket(self,
               name,  # type: str,
               options=None  # type: BucketOptions
               ):
        # type: (...)->Bucket
        pass

    def query(self,
              statement,  # type: str,
              options=None  # type: QueryOptions
              ):
        # type: (...)->IQueryResult
        """
        Executes a N1QL query against the remote cluster returning a IQueryResult with the results of the query.
        :param statement: N1QL query
        :param options: the optional parameters that the Query service takes. See The N1QL Query API for details or a SDK 2.0 implementation for detail.
        :return: An IQueryResult object with the results of the query or error message if the query failed on the server.
        :except Any exceptions raised by the underlying platform - HTTP_TIMEOUT for example.
        :except ServiceNotFoundException - service does not exist or cannot be located.

        """
        pass

    def analytics_query(self,
                        statement,  # type: str,
                        options=None  # type: AnalyticsOptions
                        ):
        # type: (...)->IAnalyticsResult
        """
        Executes an Analytics query against the remote cluster and returns a IAnalyticsResult with the results of the query.
        :param statement: the analytics statement to execute
        :param options: the optional parameters that the Analytics service takes based on the Analytics RFC.
        :return: An IAnalyticsResult object with the results of the query or error message if the query failed on the server.
        Throws Any exceptions raised by the underlying platform - HTTP_TIMEOUT for example.
        :except ServiceNotFoundException - service does not exist or cannot be located.
        """
        pass

    def search_query(self,
                     query,  # type: ISearchQuery
                     options=None  # type: SearchOptions
                     ):
        # type: (...)->ISearchResult
        """
        Executes a Search or FTS query against the remote cluster and returns a ISearchResult implementation with the results of the query.

        :param query: the fluent search API to construct a query for FTS
        :param options: the options to pass to the cluster with the query based off the FTS/Search RFC
        :return: An ISearchResult object with the results of the query or error message if the query failed on the server.
        Any exceptions raised by the underlying platform - HTTP_TIMEOUT for example.
        :except    ServiceNotFoundException - service does not exist or cannot be located.

        """
        pass

    def diagnostics(self,
                    reportId=None  # type: str
                    ):
        # type: (...)->IDiagnosticsResult
        """
        Creates a diagnostics report that can be used to determine the healthfulness of the Cluster.
        :param reportId - an optional string name for the generated report.
        :return:A IDiagnosticsResult object with the results of the query or error message if the query failed on the server.

        """
        pass

    def users(self):
        # type: (...)->IUserManager
        pass

    def indexes(self):
        # type: (...)->IIndexManager
        pass

    def nodes(self):
        # type: (...)->INodeManager
        pass

    def buckets(self):
        # type: (...)->IBucketManager
        pass

    def disconnect(self,
                   options=None  # type: DisconnectOptions
                   ):
        # type: (...)->None
        """
        Closes and cleans up any resources used by the Cluster and any objects it owns. Note the name is platform idiomatic.

        :param options - TBD
        :return: None
        :except Any exceptions raised by the underlying platform

        """
        pass

    def manager(self):
        # type: (...)->ClusterManager
        """

        Manager
        Returns a ClusterManager object for managing resources at the Cluster level.

        Caveats and notes:
        It is acceptable for a Cluster object to have a static Connect method which takes a Configuration for ease of use.
        To facilitate testing/mocking, it's acceptable for this structure to derive from an interface at the implementers discretion.
        The "Get" and "Set" prefixes are considered platform idiomatic and can be adjusted to various platform idioms.
        The Configuration is passed in via the ctor; overloads for connection strings and various other platform specific configuration are also passed this way.
        If a language does not support ctor overloading, then an equivalent method can be used on the object.

        :return:
        """
        pass