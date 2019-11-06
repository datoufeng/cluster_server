#include "data_translate.h"
void base_handle::handle(base_message& input){
    base_message* next_msg=internel_handle(input);
	if(next_msg!=NULL){
		base_handle* next_handle=get_next_handle(*next_msg);
		if(next_handle!=nullptr){
			next_handle->handle(*next_msg);
		}
	}
}

base_message* channel::internel_handle(base_message& input_msg){
	// this->get_next_stage(input_msg);
	string tmp;
	read_fd(tmp);
	input_msg=this->get_next_stage(tmp);
	return input_msg;
}

base_handle* channel::get_next_handle(base_message& input_msg){
	if(input_msg.type!=web||input_msg.type!=client||input_msg.type!=other_type)
		return NULL;
	if(input_msg.io_data==in){
		return this->f_protocol;
	}
	else{
		// string ret_string=value;
		data_buf.push_back(input_msg.value);
		return NULL;
	}
}

base_message* protocol::internel_handle(base_message& input_msg){
	// return this->raw_to_struct(input_msg);
	if(input_msg.io_data==in){
		return this->raw_to_struct(input_msg);
	}
	else{
		return this->struct_to_raw(input_msg);
	}
}

base_handle* protocol::get_next_handle(base_message& input_msg){
	// return this->raw_to_struct(input_msg);	//message结构体判断是否进行下一步
	if(input_msg.io_data==in){
		return this->f_dealer;
	}
	else{
		return this->f_channel;
	}
}

base_message* dealer::internel_handle(base_message& input_msg){
	return this->msg_handle(input_msg);//通过标志位，判断是否进行下一步
}

base_handle* dealer::get_next_handle(base_message& input_msg){
	return this->f_protocol;//通过标志位判断，下一步是给dealer ， 还是protocol
}

