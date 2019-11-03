#include "data_translate.h"

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
	
}

