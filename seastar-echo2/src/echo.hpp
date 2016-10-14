#include "core/seastar.hh"
#include "core/reactor.hh"
#include "core/future-util.hh"
#include "core/shared_ptr.hh"

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
    
    return do_with(engine().listen(make_ipv4_address({port}), lo), [] (auto& listener) {
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


class server_echo
{
private:
    lw_shared_ptr<server_socket> _listener;
public:
    /**
     * @brief functional start_deprecated could be used in main.cpp
     * server->invoke_on_all(&server_echo::start_deprecated,port);
     * as our initial version of multi-thread echo-server.
     * @param port
     * @return 
     */
    future<> start_deprecated(uint16_t port)
    {
        return f(port);
    }
    

    /**
     * @brief we listen at <param>port</param> 
     * and accept connection in endless keep_doing
     * and handle each incoming connected_socket
     * @param port
     */
    void start(uint16_t port)
    {
        listen_options lo;
        lo.reuse_address = true;
        _listener = engine().listen(make_ipv4_address({port}),lo);
        keep_doing([this]{
            return _listener->accept().then([this](connected_socket s, socket_address a)mutable{
                    // Note we ignore, not return, the future returned by
                    // handle_connection(), so we do not wait for one
                    // connection to be handled before accepting the next one.
                    do_handle_connection(std::move(s), std::move(a));
                    
                    //if we do_handle_connection(s,a)
                    //we will got compile error:
                    //	use of deleted function ‘connected_socket::connected_socket(const connected_socket&)’
                    
                });
            
        }).or_terminate();	
    }

    future<> do_handle_connection(connected_socket s, socket_address a) {
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
    
    /**
     * @brief Service a class to be instantiated on each core. Must expose a stop() method that returns a future<>, 
     * to be called when the service is stopped.
     *
     */
    future<> stop(){
        return make_ready_future<>();
    }
};