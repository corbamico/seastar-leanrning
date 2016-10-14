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
