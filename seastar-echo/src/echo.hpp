#include "core/seastar.hh"
#include "core/reactor.hh"
#include "core/future-util.hh"
#include "core/shared_ptr.hh"

class server_echo
{
public:
	lw_shared_ptr<server_socket> _listener;
public:
	void start(ipv4_addr server_addr)
	{/*
		try{
			listen_options lo;
			lo.reuse_address = true;
			_listener = engine().listen(make_ipv4_address({server_addr.port}),lo);
			
			
			keep_doing([this]{
					return _listener->accept()
							  .then([this](connected_socket fd , socket_address addr) mutable {
								  //return fd.output().write("welcome echo server\n");
							   })
							  .finally([]{});
							  
				}).or_terminate();
			
		}
		catch(...){
			std::cerr << std::current_exception() <<std::endl;
		}
	*/			
	}
	
	/**
	 * @brief Service a class to be instantiated on each core. Must expose a stop() method that returns a future<>, 
	 * to be called when the service is stopped.
	 *
	 */
	future<> stop(){
		return make_ready_future<>();
	}
};