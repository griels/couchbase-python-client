//
// Created by Ellis Breen on 25/04/2019.
//

#include "lcb_v4_wrapper.h"
#include "pycbc.h"

// TODO: this should be implemented in LCB V4 API

lcb_STATUS pycbc_crypto_register(lcb_INSTANCE* instance, const char *name, lcbcrypto_PROVIDER *provider){
    (void)instance;
    (void)name;
    (void)provider;
    PYCBC_EXC_WRAP(LCB_ERRTYPE_INTERNAL,
                   LCB_ERRTYPE_INTERNAL,
                   "Not supported in V4 yet");
    return LCB_NOT_SUPPORTED;
}
