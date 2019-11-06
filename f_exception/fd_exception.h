class fd_exception{
public:
	string exception_str;
	fd_exception() throw();
	fd_exception(const string& ex_str) throw(){
		exception_str=ex_str;
	}
	virtual const char* what() const throw(){
		return exception_str;
	}
}