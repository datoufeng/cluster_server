#include "data_translate.h"
#include<sys/epoll.h>
// #include<exception>
#include<errno.h>
#include "f_exception/fd_exception.h"
void FEN_DEAL::channel_add(channel* cha){
	bool in=cha->init();
	l_channel.push_back(cha);
}

void FEN_DEAL::protocol_add(protocol* pro){
	l_protocol.push_back(pro);
}

void FEN_DEAL::dealer_add(dealer* del){
	l_dealer.push_back(del);
}

void FEN_DEAL::channel_del(channel* cha){
	l_channel.remove(cha);
	cha->fini();
	delete cha;
}

void FEN_DEAL::protocol_del(protocol*pro){
	l_protocol.remove(pro);
	delete pro;
}

void FEN_DEAL::dealer_del(dealer* del){
	l_dealer.remove(del)
	delete del;
}

void FEN_DEAL::run(){
	int wait_fd_num=-1;
	struct epoll_event events[1024];
	while(1){
		wait_fd_num=epoll_wait(epfd,events,1024,4);

		if(wait_fd_num==0)
			continue;
		if(wait_fd_num>0){
			if(events[i].events&EPOLLIN!=0){
				for(int i=0;i<wait_fd_num;i++){
					base_message* trans_msg=new base_message(in);
					events[i].data.ptr->handle(trans_msg);
				}
			}
			else if(events[i].events&EPOLLOUT!=0){
				//	构思中
				while(!events[i].data.ptr.data_buf.empty()){
					events[i].data.ptr.write_fd(events[i].data.ptr.data_buf.front());
					events[i].data.ptr.data_buf.pop_front();
				}
			}

		}

		try{
			if(wait_fd_num<0){
				throw fd_exception();
			}
			catch(fd_exception& f){
				if(errno==EINTR)
					cout<<"EINTER: "<<f.what()<<endl;
				else
					throw;
			}
		}

	}

}

