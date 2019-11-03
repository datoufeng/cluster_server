#include<iostream>
#include<string>
#include<list>
#include<sys/epoll.h>
#include "thread_pool/thread_pool.h"
#include "memory_pool/memory_pool.h"
using namespace std;

struct base_message{
    base_message(){
		if(epfd!=-1){
			epfd=epoll_create(1);
		}
	}
    virtual ~base_message(){
		// close(epfd);
	}
	static int epfd=-1;
    // enum TYPE{web,client,other_type}type;
	enum IO_DATA{in,out}io_data;
	// char type[8];
    // char length[8];
    string value;
};

class base_handle{
    public:
    base_handle(){}
    virtual ~base_handle(){}
    void handle(base_message& input);
    
    virtual base_message* internel_handle(base_message& input_msg)=0;
    virtual base_handle* get_next_handle(base_message& input_msg)=0;
};

class channel:public base_handle{
private:
    list<string> data_buf;
    int data_fd=-1;
    bool need_close=false;
	protocol* f_protocol;
public:
    channel(){}
    virtual ~channel(){}

    virtual bool init()=0;
    virtual bool read_fd(string& data)=0;
    virtual bool write_fd(string& data)=0;
    virtual void fini()=0;
    // string& convert_to_printable(memory_pool& data);
    bool get_close_statement(){
        return need_close;
    }
    void set_close(bool state){
        this->need_close=state;
    }
    int get_fd(){
        return data_fd;
    }
    bool if_empty(){
        return true==data_buf.empty();
    }
	// base_message* type_self_deal()=0;

protected:
    virtual base_message* get_next_stage(base_message& msg)=0;

private:
    virtual base_message* internel_handle(base_message& input_msg);
    virtual base_handle* get_next_handle(base_message& input_msg);
};

class protocol:public base_handle{
public:
    protocol(){}
    virtual ~protocol(){}
	channel* f_channel;
	dealer* f_dealer;
protected:

    virtual base_message* raw_to_struct(base_message & msg)=0;
    virtual base_message* struct_to_raw(base_message& msg)=0;
    
private:
	
    // virtual base_handle* get_next_role();
    // virtual base_handle* get_next_channel();

    virtual base_message* internel_handle(base_message& input_msg);
    virtual base_handle* get_next_handle(base_message& input_msg);

};

class dealer:base_handle{
    private:
	protocol* f_protocol;
    public:
    dealer(){}
    virtual ~dealer(){}
    virtual base_message* msg_handle(base_message& msg);

    protected:
    virtual base_message* internel_handle(base_message& input_msg);
    virtual base_handle* get_next_handle(base_message& input_msg);
};

class FEN_DEAL{
    private:
    FEN_DEAL(){
        // l_feng=new Feng_Deal;
    }
    static FEN_DEAL* l_feng;
    list<channel*> l_channel;
    list<protocol*> l_protocol;
    list<dealer*> l_dealer;

    void channel_add(channel* cha);
    void protocol_add(protocol* pro);
    void dealer_add(dealer* del);
    
    void channel_del(channel* cha);
    void protocol_del(protocol*pro);
    void dealer_del(dealer* del);
	void run();
	
    public:
    static void FEN_ADD_PROTOCOL(protocol* pro);
    static void FEN_ADD_CHANNEL(channel* cha);
    static void FEN_ADD_DEALER(dealer* del);
    static void FEN_DEL_PROTOCOL(protocol*pro);
    static void FEN_DEL_CHANNEL(channel* cha);
    static void FEN_DEL_DEALER(dealer*del);

    static void FEN_RUN();

};

FEN_DEAL* FEN_DEAL::l_feng=new FEN_DEAL;
