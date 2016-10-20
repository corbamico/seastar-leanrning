# seastar-leanrning
Play with seastar(www.seastar-project.org)

## seastar-echo
echo tcp server with seastar as one lamda function
based on example in tutorial.md
only work on one core ./bin/seastar -c1

```
	//should be in the lamda, not in main context.
	config = app.configuration();
	
```

## seastar-echo2
show how to fix the network server to work on multiple threads by using distributed<> server 
key code as
```	
	auto server = new distributed<server_echo>;
	server->start()
	server->invoke_on_all(&server_echo::start,port);
```

## ragel-step2
show how to use ragel && seastar::ragel_parser_base for parse 

### how ragel works in seastar

#### use it 
```
   //1. define your parser in *.rl inherit from ragel_parser_base
   class parser:public ragel_parser_base<parser>
   
   //2. call input_stream<char>::consume(parser) 
   //   when you need parser some string orelse.
   input_stream<char>::consume(_parser); 

```

####behind the code of seastar
```
    //iostream_impl.hh
    input_stream<char>::consume(Consumer& consumer)
    {
        consumer(std::move(_buf));
        //which call 
        //future<unconsumed_remainder> 
        //ragel_parser_base::operator()(temporary_buffer<char> buf)
    }
    //ragel.hh
    future<unconsumed_remainder> 
    ragel_parser_base::operator()(temporary_buffer<char> buf)   
    {
        char* parsed = static_cast<ConcreteParser*>(this)->parse(p, pe, eof);
        //which call your defined/implement in *.rl
        //char* parser::parse(char* p, char* pe, char* eof)
    }
```