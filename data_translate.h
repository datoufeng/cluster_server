#include<iostream>
#include<string>
#include<list>
#include "thread_pool/thread_pool.h"
#include "memory_pool/memory_pool.h"

using namespace std;
enum IO_DATA{in,out};
struct base_message{
    base_message(){}
    base_message(IO_DATA io){
        this->io_data=io;
	}
    virtual ~base_message(){}
	IO_DATA io_data;
    string value;
    void* ptr;
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
public:
    // string where_data;
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
    void bind_protocol(protocol* i_protocol){
        this->f_protocol=i_protocol;
    }

protected:
    virtual base_message* message_handle(base_message& msg)=0;

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
    
public:
    virtual bind_channel(channel* i_channel){
        this->f_channel=i_channel;
    }
    virtual bind_dealer(dealer* i_dealer){
        this->f_dealer=i_dealer;
    }
    virtual base_message* internel_handle(base_message& input_msg);
    virtual base_handle* get_next_handle(base_message& input_msg);

};

class dealer:base_handle{
    private:
	protocol* f_protocol;
    
    public:
    dealer(){}
    virtual ~dealer(){}
    virtual void init()=0;
    virtual void fini()=0;
    virtual void bind_protocol(protocol* i_protocol){
        this->f_protocol=i_protocol;
    }
    virtual base_message* msg_handle(base_message& msg);

    protected:
    virtual base_message* internel_handle(base_message& input_msg);
    virtual base_handle* get_next_handle(base_message& input_msg);
};

class FEN_DEAL{
    private:
    FEN_DEAL(){
        epfd=epoll_create(1);
    }
    ~FEN_DEAL(){
        close(epfd);
    }
    
    static int epfd=-1;
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
    static void FEN_INIT();
    static void FEN_FINI();

    static void FEN_RUN();

};

FEN_DEAL* FEN_DEAL::l_feng=new FEN_DEAL;
