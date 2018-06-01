# crpc   
expect this is a simple&amp;useful rpc lib for c programmers in linux   
stop and just write your code down if you are interesting. Anyone is welcome here!  
   
current for a function like msg1_t *f(msg2_t *arg)   
please refer to /include/crpc.h, something below is not accurate, and willnot be updated any more  

## lib 
  - server_init(saddr)	generate daemon, with spec addr or default address   
  - client_init(saddr) 	generate connection, with spec addr or default address   
  - reg(func) 	build  funcname - funcpointer relation; here must get ret len   
	  - when server recv a pkt type==func_req && funcname matches, call func by pointer, reply result with ret len to client   
  - sync_func(func, arg, argsize)	// a retsize canbe added here if necessary   
  - async_func(func, arg, argsize, callback)   
	  - when client make an async call, lib build a pkt with type==func_req and funcname and arg with argsize, send request to saddr, wait for reply   
	  - if reply timeout or error, ...   
	  - call callback to process the result   
  - server_run()	wait for client req and send reply   
  - client_run()	wait for server reply for async call   
  - pkt processor 
## pkt  
  - pkt-type | pkt-len | tlv ...   
  - pkt-type: func_req, func_ret   

## server
  - server_init(saddr)   
  - reg(f): add func name f     
## client   
  - client_init(saddr)   
  - result = sync_call(f(arg))	-> sync_func(f, arg, sizeof(arg))   
  - async_call(f(arg), result_handler)	-> async_func(f, arg, sizeof(arg), result_handler)   
    - void result_handler(void *result)   
## future
   - add support for complex function type
   - add support for multiple/complex server/client   
