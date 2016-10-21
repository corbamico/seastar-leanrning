
#include "core/iostream.hh"
#include "core/temporary_buffer.hh"

class string_data_source_impl: public data_source_impl 
{
private:
    temporary_buffer<char> _string_buf;
    sstring _ss;
public:
    explicit string_data_source_impl(sstring ss):_string_buf(ss.c_str(),ss.size()),_ss(ss){}
    //explicit string_data_source_impl(sstring ss){
    //    _string_buf = std::move(ss).release();
    //}

    virtual future<temporary_buffer<char>> get()
    {
        temporary_buffer<char> * tmp_buf = new temporary_buffer<char>(_ss.c_str(),_ss.size());
        return make_ready_future<temporary_buffer<char> >(std::move(*tmp_buf));
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