import copy

from couchbase_v2 import GetFullDocumentOperation, GetOperation, ExistsOperation
    #, GetStringOperation, GetIntOperation, ExistsOperation


class GetSpec(list):
    def get_full_document(self):
        # type: ()->GetSpec
        result = copy.deepcopy(self)
        result.append(GetFullDocumentOperation())
        return result

    def get(self, *path  # type: Tuple[str,...]
            ):
        # type: (...) -> GetSpec
        if not hasattr(path, "__iter__"):
            return self.get_single(path)
        result = copy.deepcopy(self)
        result += map(lambda v: GetOperation(v, False), path)
        return result

    def get_single(self, path,  # type: str
                   xattr=False  # type: bool
                   ):
        # type (...) -> ReadSpec
        result = copy.deepcopy(self)
        result.append(GetOperation(path, xattr))
    #
    # def get_str(self, path,  # type: str
    #             xattr=False  # type: bool
    #             ):
    #     result = copy.deepcopy(self)
    #     result.append(GetStringOperation(path, xattr))
    #
    # def get_int(self, path,  # type: str
    #             xattr=False  # type: bool
    #             ):
    #     result = copy.deepcopy(self)
    #     result.append(GetIntOperation(path, xattr))
    #
    def exists(self, path,  # type: str
               xattr=False  # type: bool
               ):
        result = copy.deepcopy(self)
        result.append(ExistsOperation(path, xattr))