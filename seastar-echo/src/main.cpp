#include "core/app-template.hh"
#include "core/distributed.hh"
#include "core/future-util.hh"
#include "echo.hpp"


#include "core/seastar.hh"
#include "core/reactor.hh"
#include "core/future-util.hh"

future<> handle_connection(connected_socket s, socket_address a) {
    auto out = s.output();
    auto in = s.input();
    return do_with(std::move(s), std::move(out), std::move(in),
        [] (auto& s, auto& out, auto& in) {
            return repeat([&out, &in] {
                return in.read().then([&out] (auto buf) {
                    if (buf) {
                        return out.write(std::move(buf)).then([] {
                            return stop_iteration::no;
                        });
                    } else {
                        return make_ready_future<stop_iteration>(stop_iteration::yes);
                    }
                });
            }).then([&out] {
                return out.close();
            });
        });
}

future<> f(uint16_t port) {
    listen_options lo;
    lo.reuse_address = true;
    return do_with(listen(make_ipv4_address({port}), lo), [] (auto& listener) {
        return keep_doing([&listener] () {
            return listener.accept().then(
                [] (connected_socket s, socket_address a) {
                    // Note we ignore, not return, the future returned by
                    // handle_connection(), so we do not wait for one
                    // connection to be handled before accepting the next one.
                    handle_connection(std::move(s), std::move(a));
                });
        });
    });
}


namespace bpo = boost::program_options;

int main(int argc, char** argv) {
	
	try {
		app_template app;
		app.add_options()
			("port,p",bpo::value<uint16_t>()->default_value(10007),"echo server port")
			("server",bpo::value<std::string>()->default_value("localhost"),"echo server address")
			;

        app.run_deprecated(argc, argv, [&](){
				auto& config = app.configuration();
				auto& port = config["port"].as<uint16_t>();			
				return f(port);
			});
		
    
	} catch(...) {
        std::cerr << "Couldn't start application: "
                  << std::current_exception() << "\n";
        return 1;
    }
    return 0;
}