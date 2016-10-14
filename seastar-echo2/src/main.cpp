#include "core/app-template.hh"
#include "core/distributed.hh"
#include "core/future-util.hh"
#include "echo.hpp"




namespace bpo = boost::program_options;

int main(int argc, char** argv) {
	
	try {
		
		app_template app;
		app.add_options()
			("port,p",bpo::value<uint16_t>()->default_value(10007),"echo server port")
			("server",bpo::value<std::string>()->default_value("localhost"),"echo server address")
			;

        app.run_deprecated(argc, argv, [&](){
				auto&&   config = app.configuration();
				uint16_t port = config["port"].as<uint16_t>();	
				auto     server = new distributed<server_echo>;
				
				server->start()
				       .then([server = std::move(server),port] () mutable {
						 server->invoke_on_all(&server_echo::start,port);
					 
					 }).then([port]{
						 std::cout << "echo server listening on port " << port << " ..." << std::endl;
					 
					 });
				
				
				//return f(port);
			});
		
    
	} catch(...) {
        std::cerr << "Couldn't start application: "
                  << std::current_exception() << "\n";
        return 1;
    }
    return 0;
}