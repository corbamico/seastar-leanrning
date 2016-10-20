/*
 * This file is open source software, licensed to you under the terms
 * of the Apache License, Version 2.0 (the "License").  See the NOTICE file
 * distributed with this work for additional information regarding copyright
 * ownership.  You may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (C) 2015 Cloudius Systems, Ltd.
 */

#include "core/ragel.hh"
#include "core/iostream.hh"
#include "core/temporary_buffer.hh"
#include "core/app-template.hh"

#include <memory>
#include <iostream>
#include <unordered_map>


%% machine http_response;

%%{

access _fsm_;

action mark {
    g.mark_start(p);
}

action store_version {
    std::cerr<<str()<<std::endl;
    //<<std::flush(cout)
}


action done {
    done = true;
    fbreak;
}

cr = '\r';
lf = '\n';
crlf = '\r\n';
tchar = alpha | digit | '-' | '!' | '#' | '$' | '%' | '&' | '\'' | '*'
        | '+' | '.' | '^' | '_' | '`' | '|' | '~';

sp = ' ';
ht = '\t';

sp_ht = sp | ht;

http_version = 'HTTP/' (digit '.' digit) >mark %store_version;

start_line = http_version space digit digit digit space (any - cr - lf)* crlf;
main := start_line  :> (crlf @done);

}%%

class http_response_parser : public ragel_parser_base<http_response_parser> {
    %% write data nofinal noprefix;
public:
    enum class state {
        error,
        eof,
        done,
    };


    state _state;
public:
    void init() {
        init_base();

        _state = state::eof;
        %% write init;
    }
    char* parse(char* p, char* pe, char* eof) {
        sstring_builder::guard g(_builder, p, pe);
        auto str = [this, &g, &p] { g.mark_end(p); return get_str(); };
        bool done = false;
        if (p != pe) {
            _state = state::error;
        }
        %% write exec;
        if (!done) {
            p = nullptr;
        } else {
            _state = state::done;
        }
        return p;
    }

    bool eof() const {
        return _state == state::eof;
    }
};


class string_data_source_impl: public data_source_impl 
{
private:
    temporary_buffer<char> _string_buf;
public:
    explicit string_data_source_impl(sstring ss):_string_buf(ss.c_str(),ss.size()){}
    //explicit string_data_source_impl(sstring ss){
    //    _string_buf = std::move(ss).release();
    //}

    virtual future<temporary_buffer<char>> get()
    {
        return make_ready_future<temporary_buffer<char> >(std::move(_string_buf));
    }
};

class string_data_srouce:public data_source
{
public:
    string_data_srouce(sstring ss)
        :data_source(std::make_unique<string_data_source_impl>(ss)){}
};

input_stream<char> make_string_input_stream(sstring ss) {
    return input_stream<char>(string_data_srouce(ss));
}



int main( int argc, char **argv )
{
    app_template app;
    //http_response_parser _parser;
    //sstring ss("HTTP/1.1");
    //auto input_string=make_string_input_stream(ss);
    
            http_response_parser _parser;
            sstring ss("HTTP/1.1");
            auto input_string=make_string_input_stream(ss);
            
            std::cout<< ss.c_str() <<std::endl;
            
            input_string.consume(_parser);
    
    app.run_deprecated(argc,argv,[&]{

        });
    return 0;
}

