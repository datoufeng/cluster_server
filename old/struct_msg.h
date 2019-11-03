/* struct request_message:public base_message{
    string http_method;
    string url;
    string http_version;

    string host;      //访问的主机名
    int content_length;        //请求信息正文长度
    string user_agent;      //发出请求的用户信息
    string referer;                 //客户端告诉服务器，从哪个资源来访问服务器（url）
    string connection;    //是否长连接，短连接

    string message;
};

struct respond_message:public base_message{
    string http_version;
    string statement;

    string content_encoding;
    int content_length;
    string content_type;
    string date;

    string message;
};

struct client_message:public base_message{
    char type[8];
    char length[8];
    string value;
}; */
