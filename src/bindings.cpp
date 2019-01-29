#include "boost/python.hpp"

#include "boost/python/suite/indexing/vector_indexing_suite.hpp"

#include "boost/python/suite/indexing/map_indexing_suite.hpp"

#include "../cmake-build-release/libcouchbase-cxx-prefix/src/libcouchbase-cxx/include/libcouchbase/couchbase++.h"

namespace bp = boost::python;

struct Handler_wrapper : Couchbase::Handler, bp::wrapper< Couchbase::Handler > {

    Handler_wrapper()
    : Couchbase::Handler()
      , bp::wrapper< Couchbase::Handler >(){
        // null constructor
        
    }

    virtual bool done(  ) const {
        bp::override func_done = this->get_override( "done" );
        return func_done(  );
    }

    virtual void handle_response( ::Couchbase::Client & client, int cbtype, ::lcb_RESPBASE const * rb ){
        bp::override func_handle_response = this->get_override( "handle_response" );
        func_handle_response( boost::ref(client), cbtype, boost::python::ptr(rb) );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Counter__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Counter >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Counter > > {

    Response_less__Couchbase_scope_OpInfo_scope_Counter__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Counter> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Counter>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Counter > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Counter__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Counter>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Counter > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Counter >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Counter >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Counter >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Counter >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct CounterResponse_wrapper : Couchbase::CounterResponse, bp::wrapper< Couchbase::CounterResponse > {

    CounterResponse_wrapper(Couchbase::CounterResponse const & arg )
    : Couchbase::CounterResponse( arg )
      , bp::wrapper< Couchbase::CounterResponse >(){
        // copy constructor
        
    }

    CounterResponse_wrapper()
    : Couchbase::CounterResponse()
      , bp::wrapper< Couchbase::CounterResponse >(){
        // null constructor
        
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::CounterResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::CounterResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Counter >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Counter >::done( );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Endure__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Endure >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Endure > > {

    Response_less__Couchbase_scope_OpInfo_scope_Endure__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Endure> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Endure>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Endure > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Endure__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Endure>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Endure > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Endure >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Endure >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Endure >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Endure >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct EndureResponse_wrapper : Couchbase::EndureResponse, bp::wrapper< Couchbase::EndureResponse > {

    EndureResponse_wrapper(Couchbase::EndureResponse const & arg )
    : Couchbase::EndureResponse( arg )
      , bp::wrapper< Couchbase::EndureResponse >(){
        // copy constructor
        
    }

    EndureResponse_wrapper()
    : Couchbase::EndureResponse()
      , bp::wrapper< Couchbase::EndureResponse >(){
        // null constructor
        
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::EndureResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::EndureResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Endure >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Endure >::done( );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Get__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Get >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Get > > {

    Response_less__Couchbase_scope_OpInfo_scope_Get__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Get> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Get>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Get > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Get__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Get>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Get > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Get >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Get >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Get >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Get >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct GetResponse_wrapper : Couchbase::GetResponse, bp::wrapper< Couchbase::GetResponse > {

    GetResponse_wrapper( )
    : Couchbase::GetResponse( )
      , bp::wrapper< Couchbase::GetResponse >(){
        // null constructor
    
    }

    GetResponse_wrapper(::Couchbase::GetResponse const & other )
    : Couchbase::GetResponse( boost::ref(other) )
      , bp::wrapper< Couchbase::GetResponse >(){
        // copy constructor
    
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * arg2 ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(arg2) );
        else{
            this->Couchbase::GetResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(arg2) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * arg2 ) {
        Couchbase::GetResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(arg2) );
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Get >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Get >::done( );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Observe__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Observe >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Observe > > {

    Response_less__Couchbase_scope_OpInfo_scope_Observe__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Observe> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Observe>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Observe > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Observe__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Observe>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Observe > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Observe >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Observe >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Observe >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Observe >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct ObserveResponse_wrapper : Couchbase::ObserveResponse, bp::wrapper< Couchbase::ObserveResponse > {

    ObserveResponse_wrapper(Couchbase::ObserveResponse const & arg )
    : Couchbase::ObserveResponse( arg )
      , bp::wrapper< Couchbase::ObserveResponse >(){
        // copy constructor
        
    }

    ObserveResponse_wrapper( )
    : Couchbase::ObserveResponse( )
      , bp::wrapper< Couchbase::ObserveResponse >(){
        // null constructor
    
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::ObserveResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::ObserveResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Observe >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Observe >::done( );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Remove__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Remove >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Remove > > {

    Response_less__Couchbase_scope_OpInfo_scope_Remove__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Remove> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Remove>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Remove > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Remove__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Remove>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Remove > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Remove >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Remove >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Remove >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Remove >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Stats__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Stats >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Stats > > {

    Response_less__Couchbase_scope_OpInfo_scope_Stats__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Stats> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Stats>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Stats > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Stats__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Stats>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Stats > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Stats >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Stats >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Stats >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Stats >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Store__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Store >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Store > > {

    Response_less__Couchbase_scope_OpInfo_scope_Store__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Store> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Store>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Store > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Store__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Store>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Store > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Store >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Store >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Store >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Store >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Touch__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Touch >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Touch > > {

    Response_less__Couchbase_scope_OpInfo_scope_Touch__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Touch> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Touch>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Touch > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Touch__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Touch>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Touch > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Touch >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Touch >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Touch >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Touch >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct Response_less__Couchbase_scope_OpInfo_scope_Unlock__greater__wrapper : Couchbase::Response< Couchbase::OpInfo::Unlock >, bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Unlock > > {

    Response_less__Couchbase_scope_OpInfo_scope_Unlock__greater__wrapper(Couchbase::Response<Couchbase::OpInfo::Unlock> const & arg )
    : Couchbase::Response<Couchbase::OpInfo::Unlock>( arg )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Unlock > >(){
        // copy constructor
        
    }

    Response_less__Couchbase_scope_OpInfo_scope_Unlock__greater__wrapper( )
    : Couchbase::Response<Couchbase::OpInfo::Unlock>( )
      , bp::wrapper< Couchbase::Response< Couchbase::OpInfo::Unlock > >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::Response< Couchbase::OpInfo::Unlock >::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::Response< Couchbase::OpInfo::Unlock >::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        else{
            this->Couchbase::Response< Couchbase::OpInfo::Unlock >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * res ) {
        Couchbase::Response< Couchbase::OpInfo::Unlock >::handle_response( boost::ref(arg0), arg1, boost::python::ptr(res) );
    }

};

struct StatsResponse_wrapper : Couchbase::StatsResponse, bp::wrapper< Couchbase::StatsResponse > {

    StatsResponse_wrapper(Couchbase::StatsResponse const & arg )
    : Couchbase::StatsResponse( arg )
      , bp::wrapper< Couchbase::StatsResponse >(){
        // copy constructor
        
    }

    StatsResponse_wrapper( )
    : Couchbase::StatsResponse( )
      , bp::wrapper< Couchbase::StatsResponse >(){
        // null constructor
    
    }

    virtual bool done(  ) const  {
        if( bp::override func_done = this->get_override( "done" ) )
            return func_done(  );
        else{
            return this->Couchbase::StatsResponse::done(  );
        }
    }
    
    bool default_done(  ) const  {
        return Couchbase::StatsResponse::done( );
    }

    virtual void handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * resp ) {
        if( bp::override func_handle_response = this->get_override( "handle_response" ) )
            func_handle_response( boost::ref(arg0), arg1, boost::python::ptr(resp) );
        else{
            this->Couchbase::StatsResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(resp) );
        }
    }
    
    void default_handle_response( ::Couchbase::Client & arg0, int arg1, ::lcb_RESPBASE const * resp ) {
        Couchbase::StatsResponse::handle_response( boost::ref(arg0), arg1, boost::python::ptr(resp) );
    }

};

BOOST_PYTHON_MODULE(couchbase){
    { //::std::vector< char >
        typedef bp::class_< std::vector< char > > __self_exposer_t;
        __self_exposer_t __self_exposer = __self_exposer_t( "__self" );
        bp::scope __self_scope( __self_exposer );
        __self_exposer.def( bp::vector_indexing_suite< ::std::vector< char >, true >() );
    }

    { //::std::vector< Couchbase::ObserveResponse::ServerReply >
        typedef bp::class_< std::vector< Couchbase::ObserveResponse::ServerReply > > __self_exposer_t;
        __self_exposer_t __self_exposer = __self_exposer_t( "__self" );
        bp::scope __self_scope( __self_exposer );
        //WARNING: the next line of code will not compile, because "::Couchbase::ObserveResponse::ServerReply" does not have operator== !
        __self_exposer.def( bp::vector_indexing_suite< ::std::vector< Couchbase::ObserveResponse::ServerReply > >() );
    }

    { //::std::map< std::string, std::map< std::string, std::string > >
        typedef bp::class_< std::map< std::string, std::map< std::string, std::string > > > map_less__std_scope_string_comma__std_scope_map_less__std_scope_string_comma__std_scope_string__greater___greater__exposer_t;
        map_less__std_scope_string_comma__std_scope_map_less__std_scope_string_comma__std_scope_string__greater___greater__exposer_t map_less__std_scope_string_comma__std_scope_map_less__std_scope_string_comma__std_scope_string__greater___greater__exposer = map_less__std_scope_string_comma__std_scope_map_less__std_scope_string_comma__std_scope_string__greater___greater__exposer_t( "map_less__std_scope_string_comma__std_scope_map_less__std_scope_string_comma__std_scope_string__greater___greater_" );
        bp::scope map_less__std_scope_string_comma__std_scope_map_less__std_scope_string_comma__std_scope_string__greater___greater__scope( map_less__std_scope_string_comma__std_scope_map_less__std_scope_string_comma__std_scope_string__greater___greater__exposer );
        //WARNING: the next line of code will not compile, because "::std::map<std::basic_string<char>, std::basic_string<char>, std::less<std::basic_string<char> >, std::allocator<std::pair<const std::basic_string<char>, std::basic_string<char> > > >" does not have operator== !
        map_less__std_scope_string_comma__std_scope_map_less__std_scope_string_comma__std_scope_string__greater___greater__exposer.def( bp::map_indexing_suite< ::std::map< std::string, std::map< std::string, std::string > > >() );
    }

    bp::enum_< Couchbase::PersistTo>("PersistTo")
        .value("NONE", Couchbase::PersistTo::NONE)
        .value("MASTER", Couchbase::PersistTo::MASTER)
        .value("TWO", Couchbase::PersistTo::TWO)
        .value("THREE", Couchbase::PersistTo::THREE)
        .value("FOUR", Couchbase::PersistTo::FOUR)
        .export_values()
        ;

    bp::enum_< Couchbase::ReplicateTo>("ReplicateTo")
        .value("NONE", Couchbase::ReplicateTo::NONE)
        .value("ONE", Couchbase::ReplicateTo::ONE)
        .value("TWO", Couchbase::ReplicateTo::TWO)
        .value("THREE", Couchbase::ReplicateTo::THREE)
        .export_values()
        ;

    bp::class_< Couchbase::Buffer >( "Buffer", bp::init< Couchbase::Buffer const & >(( bp::arg("other") )) )    
        .def( bp::init< void const *, size_t >(( bp::arg("buf"), bp::arg("len") )) )    
        .def( bp::init< >() )    
        .def( 
            "begin"
            , (::Couchbase::Buffer::const_iterator ( ::Couchbase::Buffer::* )(  )const)( &::Couchbase::Buffer::begin ) )    
        .def( 
            "data"
            , (char const * ( ::Couchbase::Buffer::* )(  )const)( &::Couchbase::Buffer::data ) )    
        .def( 
            "empty"
            , (bool ( ::Couchbase::Buffer::* )(  )const)( &::Couchbase::Buffer::empty ) )    
        .def( 
            "end"
            , (::Couchbase::Buffer::const_iterator ( ::Couchbase::Buffer::* )(  )const)( &::Couchbase::Buffer::end ) )    
        .def( "as__scope_std_scope_string", &Couchbase::Buffer::operator ::std::string  )    
        .def( 
            "size"
            , (::size_t ( ::Couchbase::Buffer::* )(  )const)( &::Couchbase::Buffer::size ) )    
        .def( 
            "to_string"
            , (::std::string ( ::Couchbase::Buffer::* )(  )const)( &::Couchbase::Buffer::to_string ) )    
        .def( bp::self_ns::str( bp::self ) );

    bp::implicitly_convertible< Couchbase::Buffer, std::string >();

    { //::Couchbase::DurabilityOptions
        typedef bp::class_< Couchbase::DurabilityOptions > DurabilityOptions_exposer_t;
        DurabilityOptions_exposer_t DurabilityOptions_exposer = DurabilityOptions_exposer_t( "DurabilityOptions", bp::init< bp::optional< Couchbase::PersistTo, Couchbase::ReplicateTo, bool > >(( bp::arg("n_persist")=::Couchbase::PersistTo::NONE, bp::arg("n_replicate")=::Couchbase::ReplicateTo::NONE, bp::arg("cap_max")=(bool)(true) )) );
        bp::scope DurabilityOptions_scope( DurabilityOptions_exposer );
        bp::implicitly_convertible< Couchbase::PersistTo, Couchbase::DurabilityOptions >();
        { //::Couchbase::DurabilityOptions::cap_max
        
            typedef void ( ::Couchbase::DurabilityOptions::*cap_max_function_type)( bool ) ;
            
            DurabilityOptions_exposer.def( 
                "cap_max"
                , cap_max_function_type( &::Couchbase::DurabilityOptions::cap_max )
                , ( bp::arg("enabled") ) );
        
        }
        { //::Couchbase::DurabilityOptions::enabled
        
            typedef bool ( ::Couchbase::DurabilityOptions::*enabled_function_type)(  ) const;
            
            DurabilityOptions_exposer.def( 
                "enabled"
                , enabled_function_type( &::Couchbase::DurabilityOptions::enabled ) );
        
        }
        { //::Couchbase::DurabilityOptions::persist_to
        
            typedef void ( ::Couchbase::DurabilityOptions::*persist_to_function_type)( ::Couchbase::PersistTo ) ;
            
            DurabilityOptions_exposer.def( 
                "persist_to"
                , persist_to_function_type( &::Couchbase::DurabilityOptions::persist_to )
                , ( bp::arg("n") ) );
        
        }
        { //::Couchbase::DurabilityOptions::replicate_to
        
            typedef void ( ::Couchbase::DurabilityOptions::*replicate_to_function_type)( ::Couchbase::ReplicateTo ) ;
            
            DurabilityOptions_exposer.def( 
                "replicate_to"
                , replicate_to_function_type( &::Couchbase::DurabilityOptions::replicate_to )
                , ( bp::arg("n") ) );
        
        }
    }

    { //::Couchbase::Client
        typedef bp::class_< Couchbase::Client, boost::noncopyable > Client_exposer_t;
        Client_exposer_t Client_exposer = Client_exposer_t( "Client", bp::init< bp::optional< std::string const &, std::string const &, std::string const & > >(( bp::arg("connstr")="couchbase://localhost/default", bp::arg("passwd")="", bp::arg("username")="" )) );
        bp::scope Client_scope( Client_exposer );
        bp::implicitly_convertible< std::string const &, Couchbase::Client >();
        { //::Couchbase::Client::_dispatch
        
            typedef void ( ::Couchbase::Client::*_dispatch_function_type)( int,::lcb_RESPBASE const * ) ;
            
            Client_exposer.def( 
                "_dispatch"
                , _dispatch_function_type( &::Couchbase::Client::_dispatch )
                , ( bp::arg("arg0"), bp::arg("arg1") ) );
        
        }
        { //::Couchbase::Client::breakout
        
            typedef void ( ::Couchbase::Client::*breakout_function_type)( bool ) ;
            
            Client_exposer.def( 
                "breakout"
                , breakout_function_type( &::Couchbase::Client::breakout )
                , ( bp::arg("force")=(bool)(false) ) );
        
        }
        { //::Couchbase::Client::connect
        
            typedef ::Couchbase::Status ( ::Couchbase::Client::*connect_function_type)(  ) ;
            
            Client_exposer.def( 
                "connect"
                , connect_function_type( &::Couchbase::Client::connect ) );
        
        }
        { //::Couchbase::Client::counter
        
            typedef ::Couchbase::CounterResponse ( ::Couchbase::Client::*counter_function_type)( ::Couchbase::CounterCommand const & ) ;
            
            Client_exposer.def( 
                "counter"
                , counter_function_type( &::Couchbase::Client::counter )
                , ( bp::arg("arg0") ) );
        
        }
        { //::Couchbase::Client::endure
        
            typedef ::Couchbase::EndureResponse ( ::Couchbase::Client::*endure_function_type)( ::Couchbase::EndureCommand const &,::Couchbase::DurabilityOptions const * ) ;
            
            Client_exposer.def( 
                "endure"
                , endure_function_type( &::Couchbase::Client::endure )
                , ( bp::arg("cmd"), bp::arg("options")=__null ) );
        
        }
        { //::Couchbase::Client::endure
        
            typedef ::Couchbase::EndureResponse ( ::Couchbase::Client::*endure_function_type)( ::Couchbase::EndureCommand const &,::Couchbase::DurabilityOptions const & ) ;
            
            Client_exposer.def( 
                "endure"
                , endure_function_type( &::Couchbase::Client::endure )
                , ( bp::arg("cmd"), bp::arg("options") ) );
        
        }
        { //::Couchbase::Client::enter
        
            typedef void ( ::Couchbase::Client::*enter_function_type)(  ) ;
            
            Client_exposer.def( 
                "enter"
                , enter_function_type( &::Couchbase::Client::enter ) );
        
        }
        { //::Couchbase::Client::fail
        
            typedef void ( ::Couchbase::Client::*fail_function_type)(  ) ;
            
            Client_exposer.def( 
                "fail"
                , fail_function_type( &::Couchbase::Client::fail ) );
        
        }
        { //::Couchbase::Client::get
        
            typedef ::Couchbase::GetResponse ( ::Couchbase::Client::*get_function_type)( ::Couchbase::GetCommand const & ) ;
            
            Client_exposer.def( 
                "get"
                , get_function_type( &::Couchbase::Client::get )
                , ( bp::arg("arg0") ) );
        
        }
        { //::Couchbase::Client::get
        
            typedef ::Couchbase::GetResponse ( ::Couchbase::Client::*get_function_type)( ::Couchbase::GetCommand ) ;
            
            Client_exposer.def( 
                "get"
                , get_function_type( &::Couchbase::Client::get )
                , ( bp::arg("params") ) );
        
        }
#ifdef PYCBC_BOOST_RAW
        { //::Couchbase::Client::getLcb
            typedef ::lcb_t ( ::Couchbase::Client::*getLcb_function_type)(  ) const;
            
            Client_exposer.def( 
                "getLcb"
                , getLcb_function_type( &::Couchbase::Client::getLcb )
                    /* undefined call policies */ );
        }
        { //::Couchbase::Client::handle
        
            typedef ::lcb_t ( ::Couchbase::Client::*handle_function_type)(  ) const;
            
            Client_exposer.def( 
                "handle"
                , handle_function_type( &::Couchbase::Client::handle )
                    /* undefined call policies */ );
        
        }
#endif
        { //::Couchbase::Client::leave
        
            typedef void ( ::Couchbase::Client::*leave_function_type)(  ) ;
            
            Client_exposer.def( 
                "leave"
                , leave_function_type( &::Couchbase::Client::leave ) );
        
        }
        { //::Couchbase::Client::mctx_endure
        
            typedef ::Couchbase::Status ( ::Couchbase::Client::*mctx_endure_function_type)( ::Couchbase::DurabilityOptions const &,::Couchbase::Handler *,::Couchbase::Internal::MultiDurContext & ) ;
            
            Client_exposer.def( 
                "mctx_endure"
                , mctx_endure_function_type( &::Couchbase::Client::mctx_endure )
                , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("arg2") ) );
        
        }
        { //::Couchbase::Client::mctx_observe
        
            typedef ::Couchbase::Status ( ::Couchbase::Client::*mctx_observe_function_type)( ::Couchbase::Handler *,::Couchbase::Internal::MultiObsContext & ) ;
            
            Client_exposer.def( 
                "mctx_observe"
                , mctx_observe_function_type( &::Couchbase::Client::mctx_observe )
                , ( bp::arg("arg0"), bp::arg("arg1") ) );
        
        }
        { //::Couchbase::Client::remove
        
            typedef ::Couchbase::RemoveResponse ( ::Couchbase::Client::*remove_function_type)( ::Couchbase::RemoveCommand const & ) ;
            
            Client_exposer.def( 
                "remove"
                , remove_function_type( &::Couchbase::Client::remove )
                , ( bp::arg("arg0") ) );
        
        }
        { //::Couchbase::Client::remove
        
            typedef ::Couchbase::RemoveResponse ( ::Couchbase::Client::*remove_function_type)( ::Couchbase::RemoveCommand ) ;
            
            Client_exposer.def( 
                "remove"
                , remove_function_type( &::Couchbase::Client::remove )
                , ( bp::arg("params") ) );
        
        }
        { //::Couchbase::Client::stats
        
            typedef ::Couchbase::StatsResponse ( ::Couchbase::Client::*stats_function_type)( ::std::string const & ) ;
            
            Client_exposer.def( 
                "stats"
                , stats_function_type( &::Couchbase::Client::stats )
                , ( bp::arg("key") ) );
        
        }
        { //::Couchbase::Client::store
        
            typedef ::Couchbase::StoreResponse ( ::Couchbase::Client::*store_function_type)( ::Couchbase::StoreCommand< LCB_SET > const & ) ;
            
            Client_exposer.def( 
                "store"
                , store_function_type( &::Couchbase::Client::store )
                , ( bp::arg("arg0") ) );
        
        }
        { //::Couchbase::Client::store
        
            typedef ::Couchbase::StoreResponse ( ::Couchbase::Client::*store_function_type)( ::Couchbase::StoreCommand< LCB_ADD > const & ) ;
            
            Client_exposer.def( 
                "store"
                , store_function_type( &::Couchbase::Client::store )
                , ( bp::arg("arg0") ) );
        
        }
        { //::Couchbase::Client::store
        
            typedef ::Couchbase::StoreResponse ( ::Couchbase::Client::*store_function_type)( ::Couchbase::StoreCommand< LCB_REPLACE > const & ) ;
            
            Client_exposer.def( 
                "store"
                , store_function_type( &::Couchbase::Client::store )
                , ( bp::arg("arg0") ) );
        
        }
        { //::Couchbase::Client::touch
        
            typedef ::Couchbase::TouchResponse ( ::Couchbase::Client::*touch_function_type)( ::Couchbase::TouchCommand const & ) ;
            
            Client_exposer.def( 
                "touch"
                , touch_function_type( &::Couchbase::Client::touch )
                , ( bp::arg("arg0") ) );
        
        }
        { //::Couchbase::Client::unlock
        
            typedef ::Couchbase::UnlockResponse ( ::Couchbase::Client::*unlock_function_type)( ::Couchbase::UnlockCommand const & ) ;
            
            Client_exposer.def( 
                "unlock"
                , unlock_function_type( &::Couchbase::Client::unlock )
                , ( bp::arg("cmd") ) );
        
        }
        { //::Couchbase::Client::wait
        
            typedef void ( ::Couchbase::Client::*wait_function_type)(  ) ;

            Client_exposer.def(
                "wait"
                , wait_function_type( &::Couchbase::Client::wait ) );
        
        }
    }

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Counter > >( "Command_less__Couchbase_scope_OpInfo_scope_Counter__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Counter >::as_basecmd )
                /* undefined call policies */ )    
#endif
        .def(
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Counter >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Counter >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Counter >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Counter >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Counter >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Counter >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Counter >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Counter >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Counter >::keylen ) )
        ;//.def( "self", &bp::self );

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Endure > >( "Command_less__Couchbase_scope_OpInfo_scope_Endure__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Endure >::as_basecmd )
                /* undefined call policies */ )
#endif
        .def( 
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Endure >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Endure >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Endure >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Endure >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Endure >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Endure >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Endure >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Endure >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Endure >::keylen ) )
        ;//.def( "self",&bp::self);

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Get > >( "Command_less__Couchbase_scope_OpInfo_scope_Get__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Get >::as_basecmd )
                /* undefined call policies */ )
                #endif
        .def( 
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Get >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Get >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Get >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Get >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Get >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Get >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Get >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Get >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Get >::keylen ) )    
        ;//.def( "self", &bp::self );

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Observe > >( "Command_less__Couchbase_scope_OpInfo_scope_Observe__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Observe >::as_basecmd )
                /* undefined call policies */ )    
#endif
        .def(
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Observe >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Observe >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Observe >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Observe >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Observe >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Observe >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Observe >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Observe >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Observe >::keylen ) )    
        ;//.def( "self", &bp::self );

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Remove > >( "Command_less__Couchbase_scope_OpInfo_scope_Remove__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Remove >::as_basecmd )
                /* undefined call policies */ )    
#endif
        .def(
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Remove >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Remove >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Remove >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Remove >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Remove >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Remove >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Remove >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Remove >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Remove >::keylen ) )    
        ;//.def( "self", &bp::self );

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Stats > >( "Command_less__Couchbase_scope_OpInfo_scope_Stats__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Stats >::as_basecmd )
                /* undefined call policies */ )
#endif
        .def( 
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Stats >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Stats >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Stats >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Stats >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Stats >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Stats >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Stats >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Stats >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Stats >::keylen ) )    
        ;//.def( "self", &bp::self );

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Store > >( "Command_less__Couchbase_scope_OpInfo_scope_Store__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Store >::as_basecmd )
                /* undefined call policies */ )    
#endif
        .def(
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Store >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Store >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Store >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Store >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Store >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Store >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Store >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Store >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Store >::keylen ) )    
        ;//.def( "self", &bp::self );

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Touch > >( "Command_less__Couchbase_scope_OpInfo_scope_Touch__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Touch >::as_basecmd )
                /* undefined call policies */ )
#endif
        .def( 
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Touch >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Touch >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Touch >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Touch >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Touch >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Touch >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Touch >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Touch >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Touch >::keylen ) )    
       ;//.def( "self", &bp::self );

    bp::class_< Couchbase::Command< Couchbase::OpInfo::Unlock > >( "Command_less__Couchbase_scope_OpInfo_scope_Unlock__greater_", bp::init< >() )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_basecmd"
            , (::lcb_CMDBASE const * ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::as_basecmd )
                /* undefined call policies */ )
#endif
        .def( 
            "cas"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )( ::uint64_t ))( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::cas )
            , ( bp::arg("casval") ) )    
        .def( 
            "expiry"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )( unsigned int ))( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::expiry )
            , ( bp::arg("n") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )( char const *,::size_t ))( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::key )
            , ( bp::arg("buf"), bp::arg("len") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )( char const * ))( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )( ::std::string const & ))( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::key )
            , ( bp::arg("s") ) )    
        .def( 
            "key"
            , (void ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )( ::Couchbase::Buffer const & ))( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::key )
            , ( bp::arg("buf") ) )    
        .def( 
            "key"
            , (::Couchbase::Buffer ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Command<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Command< Couchbase::OpInfo::Unlock >::keylen ) )    
       ;//.def( "self", &bp::self );

    { //::Couchbase::Context
        typedef bp::class_< Couchbase::Context, boost::noncopyable > Context_exposer_t;
        Context_exposer_t Context_exposer = Context_exposer_t( "Context", bp::init< Couchbase::Client & >(( bp::arg("client") )) );
        bp::scope Context_scope( Context_exposer );
        bp::implicitly_convertible< Couchbase::Client &, Couchbase::Context >();
        { //::Couchbase::Context::bail
        
            typedef void ( ::Couchbase::Context::*bail_function_type)(  ) ;
            
            Context_exposer.def( 
                "bail"
                , bail_function_type( &::Couchbase::Context::bail ) );
        
        }
        { //::Couchbase::Context::reset
        
            typedef void ( ::Couchbase::Context::*reset_function_type)(  ) ;
            
            Context_exposer.def( 
                "reset"
                , reset_function_type( &::Couchbase::Context::reset ) );
        
        }
        { //::Couchbase::Context::submit
        
            typedef void ( ::Couchbase::Context::*submit_function_type)(  ) ;
            
            Context_exposer.def( 
                "submit"
                , submit_function_type( &::Couchbase::Context::submit ) );
        
        }
    }

    { //::Couchbase::CounterCommand
        typedef bp::class_< Couchbase::CounterCommand, bp::bases< Couchbase::Command< Couchbase::OpInfo::Counter > > > CounterCommand_exposer_t;
        CounterCommand_exposer_t CounterCommand_exposer = CounterCommand_exposer_t( "CounterCommand", bp::init< bp::optional< int64_t > >(( bp::arg("delta")=(::int64_t)(1) )) );
        bp::scope CounterCommand_scope( CounterCommand_exposer );
        bp::implicitly_convertible< int64_t, Couchbase::CounterCommand >();
        { //::Couchbase::CounterCommand::deflval
        
            typedef void ( ::Couchbase::CounterCommand::*deflval_function_type)( ::uint64_t ) ;
            
            CounterCommand_exposer.def( 
                "deflval"
                , deflval_function_type( &::Couchbase::CounterCommand::deflval )
                , ( bp::arg("d") ) );
        
        }
        { //::Couchbase::CounterCommand::delta
        
            typedef void ( ::Couchbase::CounterCommand::*delta_function_type)( ::int64_t ) ;
            
            CounterCommand_exposer.def( 
                "delta"
                , delta_function_type( &::Couchbase::CounterCommand::delta )
                , ( bp::arg("delta") ) );
        
        }
    }

    bp::class_< Handler_wrapper, boost::noncopyable >( "Handler" )
#ifdef PYCBC_BOOST_RAW
        .def(
            "as_cookie"
            , (::Couchbase::Handler * ( ::Couchbase::Handler::* )(  ))( &::Couchbase::Handler::as_cookie )
                /* undefined call policies */ )
                #endif
        .def( 
            "done"
            , bp::pure_virtual( (bool ( ::Couchbase::Handler::* )(  )const)(&::Couchbase::Handler::done) ) )    
        .def( 
            "handle_response"
            , bp::pure_virtual( (void ( ::Couchbase::Handler::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Handler::handle_response) )
            , ( bp::arg("client"), bp::arg("cbtype"), bp::arg("rb") ) );

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Counter__greater__wrapper, bp::bases< Couchbase::Handler > >( "Response_less__Couchbase_scope_OpInfo_scope_Counter__greater_", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Counter >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Counter >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Counter__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Counter__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Counter >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Counter >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Counter__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Counter__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Counter >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Counter >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Counter >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Counter >::set_key )
            , ( bp::arg("rb") ) )    
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Counter >::status ) );

    bp::class_< CounterResponse_wrapper, bp::bases< Couchbase::Response< Couchbase::OpInfo::Counter > > >( "CounterResponse" )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::CounterResponse::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::CounterResponse::handle_response)
            , (void ( CounterResponse_wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&CounterResponse_wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "value"
            , (::uint64_t ( ::Couchbase::CounterResponse::* )(  )const)( &::Couchbase::CounterResponse::value ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Counter>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Counter >::done)
            , (bool ( CounterResponse_wrapper::* )(  )const)(&CounterResponse_wrapper::default_done) );

    bp::class_< Couchbase::EndureCommand, bp::bases< Couchbase::Command< Couchbase::OpInfo::Endure > > >( "EndureCommand", bp::init< char const *, uint64_t >(( bp::arg("key"), bp::arg("cas") )) )    
        .def( bp::init< char const *, size_t, uint64_t >(( bp::arg("key"), bp::arg("nkey"), bp::arg("cas") )) )    
        .def( bp::init< std::string const &, uint64_t >(( bp::arg("key"), bp::arg("cas") )) )
#ifdef PYCBC_BOOST_RAW
        .def(
            "options"
            , (::Couchbase::DurabilityOptions const * ( ::Couchbase::EndureCommand::* )(  )const)( &::Couchbase::EndureCommand::options )
                /* undefined call policies */ )
#endif
        .def( 
            "options"
            , (void ( ::Couchbase::EndureCommand::* )( ::Couchbase::DurabilityOptions const * ))( &::Couchbase::EndureCommand::options )
            , ( bp::arg("o") ) );

    bp::class_< Couchbase::EndureContext, boost::noncopyable >( "EndureContext", bp::init< Couchbase::Client &, Couchbase::DurabilityOptions const &, Couchbase::Handler *, Couchbase::Status & >(( bp::arg("arg0"), bp::arg("arg1"), bp::arg("h"), bp::arg("arg3") )) )    
        .def( 
            "add"
            , (::Couchbase::Status ( ::Couchbase::EndureContext::* )( ::Couchbase::EndureCommand const & ))( &::Couchbase::EndureContext::add )
            , ( bp::arg("arg0") ) )    
        .def( 
            "bail"
            , (void ( ::Couchbase::EndureContext::* )(  ))( &::Couchbase::EndureContext::bail ) )    
        .def( 
            "submit"
            , (::Couchbase::Status ( ::Couchbase::EndureContext::* )(  ))( &::Couchbase::EndureContext::submit ) );

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Endure__greater__wrapper, bp::bases< Couchbase::Handler > >( "Response_less__Couchbase_scope_OpInfo_scope_Endure__greater_", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Endure >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Endure >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Endure__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Endure__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Endure >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Endure >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Endure__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Endure__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Endure >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Endure >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Endure >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Endure >::set_key )
            , ( bp::arg("rb") ) )
#ifdef PYCBC_BOOST_RAW
        .def(
            "setcode"
            , (::Couchbase::EndureResponse & (*)( ::Couchbase::EndureResponse &,::Couchbase::Status const & ))( &::Couchbase::Response< Couchbase::OpInfo::Endure >::setcode )
            , ( bp::arg("r"), bp::arg("c") )
                /* undefined call policies */ )
#endif
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Endure >::status ) )    
        .staticmethod( "setcode" );

    bp::class_< EndureResponse_wrapper, bp::bases< Couchbase::Response< Couchbase::OpInfo::Endure > > >( "EndureResponse" )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::EndureResponse::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::EndureResponse::handle_response)
            , (void ( EndureResponse_wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&EndureResponse_wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "on_master_ram"
            , (bool ( ::Couchbase::EndureResponse::* )(  )const)( &::Couchbase::EndureResponse::on_master_ram ) )    
        .def( 
            "on_master_storage"
            , (bool ( ::Couchbase::EndureResponse::* )(  )const)( &::Couchbase::EndureResponse::on_master_storage ) )    
        .def( 
            "persisted"
            , (::size_t ( ::Couchbase::EndureResponse::* )(  )const)( &::Couchbase::EndureResponse::persisted ) )    
        .def( 
            "probes"
            , (::size_t ( ::Couchbase::EndureResponse::* )(  )const)( &::Couchbase::EndureResponse::probes ) )    
        .def( 
            "replicate"
            , (::size_t ( ::Couchbase::EndureResponse::* )(  )const)( &::Couchbase::EndureResponse::replicate ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Endure>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Endure >::done)
            , (bool ( EndureResponse_wrapper::* )(  )const)(&EndureResponse_wrapper::default_done) );

    { //::Couchbase::GetCommand
        typedef bp::class_< Couchbase::GetCommand, bp::bases< Couchbase::Command< Couchbase::OpInfo::Get > > > GetCommand_exposer_t;
        GetCommand_exposer_t GetCommand_exposer = GetCommand_exposer_t( "GetCommand", bp::init< >() );
        bp::scope GetCommand_scope( GetCommand_exposer );
        GetCommand_exposer.def( bp::init< char const * >(( bp::arg("k") )) );
        bp::implicitly_convertible< char const *, Couchbase::GetCommand >();
        GetCommand_exposer.def( bp::init< char const *, size_t >(( bp::arg("k"), bp::arg("n") )) );
        GetCommand_exposer.def( bp::init< std::string const & >(( bp::arg("s") )) );
        bp::implicitly_convertible< std::string const &, Couchbase::GetCommand >();
        { //::Couchbase::GetCommand::locktime
        
            typedef void ( ::Couchbase::GetCommand::*locktime_function_type)( unsigned int ) ;
            
            GetCommand_exposer.def( 
                "locktime"
                , locktime_function_type( &::Couchbase::GetCommand::locktime )
                , ( bp::arg("n") ) );
        
        }
    }

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Get__greater__wrapper, bp::bases< Couchbase::Handler > >( "Response_less__Couchbase_scope_OpInfo_scope_Get__greater_", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Get >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Get >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Get__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Get__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Get >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Get >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Get__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Get__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Get >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Get >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Get >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Get >::set_key )
            , ( bp::arg("rb") ) )    
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Get>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Get >::status ) );

    { //::Couchbase::GetResponse
        typedef bp::class_< GetResponse_wrapper, bp::bases< Couchbase::Response< Couchbase::OpInfo::Get > > > GetResponse_exposer_t;
        GetResponse_exposer_t GetResponse_exposer = GetResponse_exposer_t( "GetResponse", bp::init< >() );
        bp::scope GetResponse_scope( GetResponse_exposer );
        GetResponse_exposer.def( bp::init< Couchbase::GetResponse const & >(( bp::arg("other") )) );
        { //::Couchbase::GetResponse::clear
        
            typedef void ( ::Couchbase::GetResponse::*clear_function_type)(  ) ;
            
            GetResponse_exposer.def( 
                "clear"
                , clear_function_type( &::Couchbase::GetResponse::clear ) );
        
        }
        { //::Couchbase::GetResponse::handle_response
        
            typedef void ( ::Couchbase::GetResponse::*handle_response_function_type)( ::Couchbase::Client &,int,::lcb_RESPBASE const * ) ;
            typedef void ( GetResponse_wrapper::*default_handle_response_function_type)( ::Couchbase::Client &,int,::lcb_RESPBASE const * ) ;
            
            GetResponse_exposer.def( 
                "handle_response"
                , handle_response_function_type(&::Couchbase::GetResponse::handle_response)
                , default_handle_response_function_type(&GetResponse_wrapper::default_handle_response)
                , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("arg2") ) );
        
        }
        { //::Couchbase::GetResponse::itemflags
        
            typedef ::uint32_t ( ::Couchbase::GetResponse::*itemflags_function_type)(  ) const;
            
            GetResponse_exposer.def( 
                "itemflags"
                , itemflags_function_type( &::Couchbase::GetResponse::itemflags ) );
        
        }
        { //::Couchbase::GetResponse::operator=
        
            typedef ::Couchbase::GetResponse & ( ::Couchbase::GetResponse::*assign_function_type)( ::Couchbase::GetResponse const & ) ;
            
            GetResponse_exposer.def( 
                "assign"
                , assign_function_type( &::Couchbase::GetResponse::operator= )
                , ( bp::arg("arg0") )
                , bp::return_self< >() );
        
        }
        { //::Couchbase::GetResponse::value
        
            typedef void ( ::Couchbase::GetResponse::*value_function_type)( ::std::string & ) const;
            
            GetResponse_exposer.def( 
                "value"
                , value_function_type( &::Couchbase::GetResponse::value )
                , ( bp::arg("s") ) );
        
        }
        { //::Couchbase::GetResponse::value
        
            typedef void ( ::Couchbase::GetResponse::*value_function_type)( ::std::vector< char > & ) const;
            
            GetResponse_exposer.def( 
                "value"
                , value_function_type( &::Couchbase::GetResponse::value )
                , ( bp::arg("v") ) );
        
        }
        { //::Couchbase::GetResponse::value
        
            typedef ::Couchbase::Buffer ( ::Couchbase::GetResponse::*value_function_type)(  ) const;
            
            GetResponse_exposer.def( 
                "value"
                , value_function_type( &::Couchbase::GetResponse::value ) );
        
        }
        { //::Couchbase::GetResponse::valuebuf
        
            typedef char const * ( ::Couchbase::GetResponse::*valuebuf_function_type)(  ) const;
            
            GetResponse_exposer.def( 
                "valuebuf"
                , valuebuf_function_type( &::Couchbase::GetResponse::valuebuf ) );
        
        }
        { //::Couchbase::GetResponse::valueflags
        
            typedef ::uint32_t ( ::Couchbase::GetResponse::*valueflags_function_type)(  ) const;
            
            GetResponse_exposer.def( 
                "valueflags"
                , valueflags_function_type( &::Couchbase::GetResponse::valueflags ) );
        
        }
        { //::Couchbase::GetResponse::valuesize
        
            typedef ::size_t ( ::Couchbase::GetResponse::*valuesize_function_type)(  ) const;
            
            GetResponse_exposer.def( 
                "valuesize"
                , valuesize_function_type( &::Couchbase::GetResponse::valuesize ) );
        
        }
        { //::Couchbase::Response< Couchbase::OpInfo::Get >::done
        
            typedef Couchbase::GetResponse exported_class_t;
            typedef bool ( exported_class_t::*done_function_type)(  ) const;
            typedef bool ( GetResponse_wrapper::*default_done_function_type)(  ) const;
            
            GetResponse_exposer.def( 
                "done"
                , done_function_type(&::Couchbase::Response< Couchbase::OpInfo::Get >::done)
                , default_done_function_type(&GetResponse_wrapper::default_done) );
        
        }
    }

    bp::class_< Couchbase::Internal::MultiContextT< lcb_CMDENDURE >, boost::noncopyable >( "MultiDurContext", bp::init< >() )    
        .def( bp::init< lcb_MULTICMD_CTX *, Couchbase::Handler *, Couchbase::Client * >(( bp::arg("c"), bp::arg("handler"), bp::arg("cli") )) )    
        .def( 
            "add"
            , (::Couchbase::Status ( ::Couchbase::Internal::MultiContextT<lcb_CMDENDURE>::* )( ::lcb_CMDENDURE const * ))( &::Couchbase::Internal::MultiContextT< lcb_CMDENDURE >::add )
            , ( bp::arg("cmd") ) )    
        .def( 
            "bail"
            , (void ( ::Couchbase::Internal::MultiContextT<lcb_CMDENDURE>::* )(  ))( &::Couchbase::Internal::MultiContextT< lcb_CMDENDURE >::bail ) )    
        .def( 
            "done"
            , (::Couchbase::Status ( ::Couchbase::Internal::MultiContextT<lcb_CMDENDURE>::* )(  ))( &::Couchbase::Internal::MultiContextT< lcb_CMDENDURE >::done ) );

    bp::class_< Couchbase::Internal::MultiContextT< lcb_CMDOBSERVE >, boost::noncopyable >( "MultiObsContext", bp::init< >() )    
        .def( bp::init< lcb_MULTICMD_CTX *, Couchbase::Handler *, Couchbase::Client * >(( bp::arg("c"), bp::arg("handler"), bp::arg("cli") )) )    
        .def( 
            "add"
            , (::Couchbase::Status ( ::Couchbase::Internal::MultiContextT<lcb_CMDOBSERVE>::* )( ::lcb_CMDOBSERVE const * ))( &::Couchbase::Internal::MultiContextT< lcb_CMDOBSERVE >::add )
            , ( bp::arg("arg0") ) )    
        .def( 
            "bail"
            , (void ( ::Couchbase::Internal::MultiContextT<lcb_CMDOBSERVE>::* )(  ))( &::Couchbase::Internal::MultiContextT< lcb_CMDOBSERVE >::bail ) )    
        .def( 
            "done"
            , (::Couchbase::Status ( ::Couchbase::Internal::MultiContextT<lcb_CMDOBSERVE>::* )(  ))( &::Couchbase::Internal::MultiContextT< lcb_CMDOBSERVE >::done ) );

    { //::Couchbase::ObserveCommand
        typedef bp::class_< Couchbase::ObserveCommand, bp::bases< Couchbase::Command< Couchbase::OpInfo::Observe > > > ObserveCommand_exposer_t;
        ObserveCommand_exposer_t ObserveCommand_exposer = ObserveCommand_exposer_t( "ObserveCommand", bp::init< >() );
        bp::scope ObserveCommand_scope( ObserveCommand_exposer );
        ObserveCommand_exposer.def( bp::init< char const * >(( bp::arg("k") )) );
        bp::implicitly_convertible< char const *, Couchbase::ObserveCommand >();
        ObserveCommand_exposer.def( bp::init< char const *, size_t >(( bp::arg("k"), bp::arg("n") )) );
        ObserveCommand_exposer.def( bp::init< std::string const & >(( bp::arg("s") )) );
        bp::implicitly_convertible< std::string const &, Couchbase::ObserveCommand >();
        { //::Couchbase::ObserveCommand::master_only
        
            typedef void ( ::Couchbase::ObserveCommand::*master_only_function_type)( bool ) ;
            
            ObserveCommand_exposer.def( 
                "master_only"
                , master_only_function_type( &::Couchbase::ObserveCommand::master_only )
                , ( bp::arg("val")=(bool)(true) ) );
        
        }
    }

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Observe__greater__wrapper, bp::bases< Couchbase::Handler > >( "Response_less__Couchbase_scope_OpInfo_scope_Observe__greater_", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Observe >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Observe >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Observe__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Observe__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Observe >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Observe >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Observe__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Observe__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Observe >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Observe >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Observe >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Observe >::set_key )
            , ( bp::arg("rb") ) )    
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Observe>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Observe >::status ) );

    { //::Couchbase::ObserveResponse
        typedef bp::class_< ObserveResponse_wrapper, bp::bases< Couchbase::Response< Couchbase::OpInfo::Observe > > > ObserveResponse_exposer_t;
        ObserveResponse_exposer_t ObserveResponse_exposer = ObserveResponse_exposer_t( "ObserveResponse", bp::init< >() );
        bp::scope ObserveResponse_scope( ObserveResponse_exposer );
        bp::class_< Couchbase::ObserveResponse::ServerReply >( "ServerReply", bp::init< >() )    
            .def( 
                "exists"
                , (bool ( ::Couchbase::ObserveResponse::ServerReply::* )(  )const)( &::Couchbase::ObserveResponse::ServerReply::exists ) )    
            .def( 
                "persisted"
                , (bool ( ::Couchbase::ObserveResponse::ServerReply::* )(  )const)( &::Couchbase::ObserveResponse::ServerReply::persisted ) )    
            .def_readwrite( "cas", &Couchbase::ObserveResponse::ServerReply::cas )    
            .def_readwrite( "master", &Couchbase::ObserveResponse::ServerReply::master )    
            .def_readwrite( "status", &Couchbase::ObserveResponse::ServerReply::status );
        { //::Couchbase::ObserveResponse::all_replies
        
            typedef ::std::vector< Couchbase::ObserveResponse::ServerReply > const & ( ::Couchbase::ObserveResponse::*all_replies_function_type)(  ) const;
            
            ObserveResponse_exposer.def( 
                "all_replies"
                , all_replies_function_type( &::Couchbase::ObserveResponse::all_replies )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Couchbase::ObserveResponse::handle_response
        
            typedef void ( ::Couchbase::ObserveResponse::*handle_response_function_type)( ::Couchbase::Client &,int,::lcb_RESPBASE const * ) ;
            typedef void ( ObserveResponse_wrapper::*default_handle_response_function_type)( ::Couchbase::Client &,int,::lcb_RESPBASE const * ) ;
            
            ObserveResponse_exposer.def( 
                "handle_response"
                , handle_response_function_type(&::Couchbase::ObserveResponse::handle_response)
                , default_handle_response_function_type(&ObserveResponse_wrapper::default_handle_response)
                , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) );
        
        }
        { //::Couchbase::ObserveResponse::master_reply
        
            typedef ::Couchbase::ObserveResponse::ServerReply const & ( ::Couchbase::ObserveResponse::*master_reply_function_type)(  ) const;
            
            ObserveResponse_exposer.def( 
                "master_reply"
                , master_reply_function_type( &::Couchbase::ObserveResponse::master_reply )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Couchbase::Response< Couchbase::OpInfo::Observe >::done
        
            typedef Couchbase::ObserveResponse exported_class_t;
            typedef bool ( exported_class_t::*done_function_type)(  ) const;
            typedef bool ( ObserveResponse_wrapper::*default_done_function_type)(  ) const;
            
            ObserveResponse_exposer.def( 
                "done"
                , done_function_type(&::Couchbase::Response< Couchbase::OpInfo::Observe >::done)
                , default_done_function_type(&ObserveResponse_wrapper::default_done) );
        
        }
    }

    { //::Couchbase::RemoveCommand
        typedef bp::class_< Couchbase::RemoveCommand, bp::bases< Couchbase::Command< Couchbase::OpInfo::Remove > > > RemoveCommand_exposer_t;
        RemoveCommand_exposer_t RemoveCommand_exposer = RemoveCommand_exposer_t( "RemoveCommand", bp::init< >() );
        bp::scope RemoveCommand_scope( RemoveCommand_exposer );
        RemoveCommand_exposer.def( bp::init< char const * >(( bp::arg("k") )) );
        bp::implicitly_convertible< char const *, Couchbase::RemoveCommand >();
        RemoveCommand_exposer.def( bp::init< char const *, size_t >(( bp::arg("k"), bp::arg("n") )) );
        RemoveCommand_exposer.def( bp::init< std::string const & >(( bp::arg("s") )) );
        bp::implicitly_convertible< std::string const &, Couchbase::RemoveCommand >();
    }

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Remove__greater__wrapper, bp::bases< Couchbase::Handler > >( "RemoveResponse", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Remove >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Remove >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Remove__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Remove__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Remove >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Remove >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Remove__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Remove__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Remove >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Remove >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Remove >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Remove >::set_key )
            , ( bp::arg("rb") ) )    
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Remove>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Remove >::status ) );

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Stats__greater__wrapper, bp::bases< Couchbase::Handler > >( "Response_less__Couchbase_scope_OpInfo_scope_Stats__greater_", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Stats >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Stats >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Stats__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Stats__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Stats >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Stats >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Stats__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Stats__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Stats >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Stats >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Stats >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Stats >::set_key )
            , ( bp::arg("rb") ) )    
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Stats>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Stats >::status ) );

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Store__greater__wrapper, bp::bases< Couchbase::Handler > >( "StoreResponse", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Store >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Store >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Store__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Store__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Store >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Store >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Store__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Store__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Store >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Store >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Store >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Store >::set_key )
            , ( bp::arg("rb") ) )    
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Store>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Store >::status ) );

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Touch__greater__wrapper, bp::bases< Couchbase::Handler > >( "TouchResponse", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Touch >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Touch >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Touch__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Touch__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Touch >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Touch >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Touch__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Touch__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Touch >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Touch >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Touch >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Touch >::set_key )
            , ( bp::arg("rb") ) )    
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Touch>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Touch >::status ) );

    bp::class_< Response_less__Couchbase_scope_OpInfo_scope_Unlock__greater__wrapper, bp::bases< Couchbase::Handler > >( "UnlockResponse", bp::init< >() )    
        .def( 
            "cas"
            , (::uint64_t ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Unlock >::cas ) )    
        .def( 
            "done"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )(  )const)(&::Couchbase::Response< Couchbase::OpInfo::Unlock >::done)
            , (bool ( Response_less__Couchbase_scope_OpInfo_scope_Unlock__greater__wrapper::* )(  )const)(&Response_less__Couchbase_scope_OpInfo_scope_Unlock__greater__wrapper::default_done) )    
        .def( 
            "final"
            , (bool ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Unlock >::final ) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::Response< Couchbase::OpInfo::Unlock >::handle_response)
            , (void ( Response_less__Couchbase_scope_OpInfo_scope_Unlock__greater__wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&Response_less__Couchbase_scope_OpInfo_scope_Unlock__greater__wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("res") ) )    
        .def( 
            "key"
            , (::std::string ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Unlock >::key ) )    
        .def( 
            "keybuf"
            , (char const * ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Unlock >::keybuf ) )    
        .def( 
            "keylen"
            , (::size_t ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Unlock >::keylen ) )    
        .def( 
            "set_key"
            , (void ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )( ::lcb_RESPBASE const * ))( &::Couchbase::Response< Couchbase::OpInfo::Unlock >::set_key )
            , ( bp::arg("rb") ) )    
        .def( 
            "status"
            , (::Couchbase::Status ( ::Couchbase::Response<Couchbase::OpInfo::Unlock>::* )(  )const)( &::Couchbase::Response< Couchbase::OpInfo::Unlock >::status ) );

    { //::Couchbase::StatsCommand
        typedef bp::class_< Couchbase::StatsCommand, bp::bases< Couchbase::Command< Couchbase::OpInfo::Stats > > > StatsCommand_exposer_t;
        StatsCommand_exposer_t StatsCommand_exposer = StatsCommand_exposer_t( "StatsCommand", bp::init< >() );
        bp::scope StatsCommand_scope( StatsCommand_exposer );
        StatsCommand_exposer.def( bp::init< char const * >(( bp::arg("k") )) );
        bp::implicitly_convertible< char const *, Couchbase::StatsCommand >();
        StatsCommand_exposer.def( bp::init< char const *, size_t >(( bp::arg("k"), bp::arg("n") )) );
        StatsCommand_exposer.def( bp::init< std::string const & >(( bp::arg("s") )) );
        bp::implicitly_convertible< std::string const &, Couchbase::StatsCommand >();
    }

    bp::class_< StatsResponse_wrapper, bp::bases< Couchbase::Response< Couchbase::OpInfo::Stats > > >( "StatsResponse", bp::init< >() )    
        .def( 
            "done"
            , (bool ( ::Couchbase::StatsResponse::* )(  )const)(&::Couchbase::StatsResponse::done)
            , (bool ( StatsResponse_wrapper::* )(  )const)(&StatsResponse_wrapper::default_done) )    
        .def( 
            "handle_response"
            , (void ( ::Couchbase::StatsResponse::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&::Couchbase::StatsResponse::handle_response)
            , (void ( StatsResponse_wrapper::* )( ::Couchbase::Client &,int,::lcb_RESPBASE const * ))(&StatsResponse_wrapper::default_handle_response)
            , ( bp::arg("arg0"), bp::arg("arg1"), bp::arg("resp") ) )    
        .def_readwrite( "stats", &Couchbase::StatsResponse::stats );

    bp::class_< Couchbase::StoreCommand< LCB_ADD >, bp::bases< Couchbase::Command< Couchbase::OpInfo::Store > > >( "InsertCommand", bp::init< >() )    
        .def( bp::init< std::string const &, std::string const & >(( bp::arg("key"), bp::arg("value") )) )    
        .def( bp::init< char const *, char const * >(( bp::arg("key"), bp::arg("value") )) )    
        .def( bp::init< char const *, size_t, char const *, size_t >(( bp::arg("key"), bp::arg("nkey"), bp::arg("value"), bp::arg("nvalue") )) )    
        .def( 
            "itemflags"
            , (void ( ::Couchbase::StoreCommand<LCB_ADD>::* )( ::uint32_t ))( &::Couchbase::StoreCommand< LCB_ADD >::itemflags )
            , ( bp::arg("f") ) )    
        .def( 
            "mode"
            , (void ( ::Couchbase::StoreCommand<LCB_ADD>::* )( ::Couchbase::StoreMode ))( &::Couchbase::StoreCommand< LCB_ADD >::mode )
            , ( bp::arg("op") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_ADD>::* )( void const *,::size_t ))( &::Couchbase::StoreCommand< LCB_ADD >::value )
            , ( bp::arg("b"), bp::arg("n") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_ADD>::* )( char const * ))( &::Couchbase::StoreCommand< LCB_ADD >::value )
            , ( bp::arg("s") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_ADD>::* )( ::std::string const & ))( &::Couchbase::StoreCommand< LCB_ADD >::value )
            , ( bp::arg("s") ) );

    bp::class_< Couchbase::StoreCommand< LCB_REPLACE >, bp::bases< Couchbase::Command< Couchbase::OpInfo::Store > > >( "ReplaceCommand", bp::init< >() )    
        .def( bp::init< std::string const &, std::string const & >(( bp::arg("key"), bp::arg("value") )) )    
        .def( bp::init< char const *, char const * >(( bp::arg("key"), bp::arg("value") )) )    
        .def( bp::init< char const *, size_t, char const *, size_t >(( bp::arg("key"), bp::arg("nkey"), bp::arg("value"), bp::arg("nvalue") )) )    
        .def( 
            "itemflags"
            , (void ( ::Couchbase::StoreCommand<LCB_REPLACE>::* )( ::uint32_t ))( &::Couchbase::StoreCommand< LCB_REPLACE >::itemflags )
            , ( bp::arg("f") ) )    
        .def( 
            "mode"
            , (void ( ::Couchbase::StoreCommand<LCB_REPLACE>::* )( ::Couchbase::StoreMode ))( &::Couchbase::StoreCommand< LCB_REPLACE >::mode )
            , ( bp::arg("op") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_REPLACE>::* )( void const *,::size_t ))( &::Couchbase::StoreCommand< LCB_REPLACE >::value )
            , ( bp::arg("b"), bp::arg("n") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_REPLACE>::* )( char const * ))( &::Couchbase::StoreCommand< LCB_REPLACE >::value )
            , ( bp::arg("s") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_REPLACE>::* )( ::std::string const & ))( &::Couchbase::StoreCommand< LCB_REPLACE >::value )
            , ( bp::arg("s") ) );

    bp::class_< Couchbase::StoreCommand< LCB_SET >, bp::bases< Couchbase::Command< Couchbase::OpInfo::Store > > >( "UpsertCommand", bp::init< >() )    
        .def( bp::init< std::string const &, std::string const & >(( bp::arg("key"), bp::arg("value") )) )    
        .def( bp::init< char const *, char const * >(( bp::arg("key"), bp::arg("value") )) )    
        .def( bp::init< char const *, size_t, char const *, size_t >(( bp::arg("key"), bp::arg("nkey"), bp::arg("value"), bp::arg("nvalue") )) )    
        .def( 
            "itemflags"
            , (void ( ::Couchbase::StoreCommand<LCB_SET>::* )( ::uint32_t ))( &::Couchbase::StoreCommand< LCB_SET >::itemflags )
            , ( bp::arg("f") ) )    
        .def( 
            "mode"
            , (void ( ::Couchbase::StoreCommand<LCB_SET>::* )( ::Couchbase::StoreMode ))( &::Couchbase::StoreCommand< LCB_SET >::mode )
            , ( bp::arg("op") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_SET>::* )( void const *,::size_t ))( &::Couchbase::StoreCommand< LCB_SET >::value )
            , ( bp::arg("b"), bp::arg("n") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_SET>::* )( char const * ))( &::Couchbase::StoreCommand< LCB_SET >::value )
            , ( bp::arg("s") ) )    
        .def( 
            "value"
            , (void ( ::Couchbase::StoreCommand<LCB_SET>::* )( ::std::string const & ))( &::Couchbase::StoreCommand< LCB_SET >::value )
            , ( bp::arg("s") ) );

    { //::Couchbase::TouchCommand
        typedef bp::class_< Couchbase::TouchCommand, bp::bases< Couchbase::Command< Couchbase::OpInfo::Store > > > TouchCommand_exposer_t;
        TouchCommand_exposer_t TouchCommand_exposer = TouchCommand_exposer_t( "TouchCommand", bp::init< >() );
        bp::scope TouchCommand_scope( TouchCommand_exposer );
        TouchCommand_exposer.def( bp::init< char const * >(( bp::arg("k") )) );
        bp::implicitly_convertible< char const *, Couchbase::TouchCommand >();
        TouchCommand_exposer.def( bp::init< char const *, size_t >(( bp::arg("k"), bp::arg("n") )) );
        TouchCommand_exposer.def( bp::init< std::string const & >(( bp::arg("s") )) );
        bp::implicitly_convertible< std::string const &, Couchbase::TouchCommand >();
    }

    { //::Couchbase::UnlockCommand
        typedef bp::class_< Couchbase::UnlockCommand, bp::bases< Couchbase::Command< Couchbase::OpInfo::Unlock > > > UnlockCommand_exposer_t;
        UnlockCommand_exposer_t UnlockCommand_exposer = UnlockCommand_exposer_t( "UnlockCommand", bp::init< >() );
        bp::scope UnlockCommand_scope( UnlockCommand_exposer );
        UnlockCommand_exposer.def( bp::init< char const * >(( bp::arg("k") )) );
        bp::implicitly_convertible< char const *, Couchbase::UnlockCommand >();
        UnlockCommand_exposer.def( bp::init< char const *, size_t >(( bp::arg("k"), bp::arg("n") )) );
        UnlockCommand_exposer.def( bp::init< std::string const & >(( bp::arg("s") )) );
        bp::implicitly_convertible< std::string const &, Couchbase::UnlockCommand >();
    }
}