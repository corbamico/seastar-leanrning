#include "stringstream.hpp"

#include "core/app-template.hh"
#include "core/shared_ptr.hh"
#include "core/reactor.hh"
#include "test-utils.hh"


SEASTAR_TEST_CASE(test_split) {
    return now().then([]{
        std::cerr<<"test works?"<<std::endl;});
}