//
// Created by Ellis Breen on 25/04/2019.
//

#include "pycbc.h"
#include "lcb_v4_backport.h"

lcb_STATUS pycbc_crypto_register(lcb_INSTANCE instance, const char *name, lcbcrypto_PROVIDER *provider){
    lcbcrypto_register(instance,name,provider);
    return LCB_SUCCESS;
}