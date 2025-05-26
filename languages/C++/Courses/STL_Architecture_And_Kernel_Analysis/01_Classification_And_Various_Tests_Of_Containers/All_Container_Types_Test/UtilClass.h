#ifndef UTILCLASS_H
#define UTILCLASS_H

#include <cstring>
#include <functional>

using std::hash;

//以下 MyString 是為了測試 containers with moveable elements 效果.  
class MyString {
public:
    static size_t DCtor;  	//累計 default-ctor 的呼叫次數 
    static size_t Ctor;  	//累計 ctor      的呼叫次數 
    static size_t CCtor;  	//累計 copy-ctor 的呼叫次數 
    static size_t CAsgn;  	//累計 copy-asgn 的呼叫次數 
    static size_t MCtor;  	//累計 move-ctor 的呼叫次數 
    static size_t MAsgn;  	//累計 move-asgn 的呼叫次數 		    
    static size_t Dtor;	//累計 dtor 的呼叫次數 
private:
    char* _data;
    size_t _len;
    void _init_data(const char* s) {
        _data = new char[_len + 1];
        memcpy(_data, s, _len);
        _data[_len] = '\0';
    }
public:
    //default ctor
    MyString() : _data(NULL), _len(0) { ++DCtor; }

    //ctor
    MyString(const char* p) : _len(strlen(p)) {
        ++Ctor;
        _init_data(p);
    }

    // copy ctor
    MyString(const MyString& str) : _len(str._len) {
        ++CCtor;
        _init_data(str._data); 	//COPY
    }

    //move ctor, with "noexcept"
    MyString(MyString&& str) noexcept : _data(str._data), _len(str._len) {
        ++MCtor;
        str._len = 0;
        str._data = NULL;  	//避免 delete (in dtor) 
    }

    //copy assignment
    MyString& operator=(const MyString& str) {
        ++CAsgn;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _init_data(str._data); 	//COPY! 
        }
        else {
            // Self Assignment, Nothing to do.   
        }
        return *this;
    }

    //move assignment
    MyString& operator=(MyString&& str) noexcept {
        ++MAsgn;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _data = str._data;	//MOVE!
            str._len = 0;
            str._data = NULL; 	//避免 deleted in dtor 
        }
        return *this;
    }

    //dtor
    virtual ~MyString() {
        ++Dtor;
        if (_data) {
            delete _data;
        }
    }

    bool
        operator<(const MyString& rhs) const	//為了讓 set 比較大小  
    {
        return std::string(this->_data) < std::string(rhs._data); 	//借用事實：string 已能比較大小. 
    }
    bool
        operator==(const MyString& rhs) const	//為了讓 set 判斷相等. 
    {
        return std::string(this->_data) == std::string(rhs._data); 	//借用事實：string 已能判斷相等. 
    }

    char* get() const { return _data; }
};
size_t MyString::DCtor = 0;
size_t MyString::Ctor = 0;
size_t MyString::CCtor = 0;
size_t MyString::CAsgn = 0;
size_t MyString::MCtor = 0;
size_t MyString::MAsgn = 0;
size_t MyString::Dtor = 0;

//以下 MyStrNoMove 是為了測試 containers with no-moveable elements 效果.  
class MyStrNoMove {
public:
    static size_t DCtor;  	//累計 default-ctor 的呼叫次數 
    static size_t Ctor;  	//累計 ctor      的呼叫次數 
    static size_t CCtor;  	//累計 copy-ctor 的呼叫次數 
    static size_t CAsgn;  	//累計 copy-asgn 的呼叫次數 
    static size_t MCtor;  	//累計 move-ctor 的呼叫次數 
    static size_t MAsgn;  	//累計 move-asgn 的呼叫次數 		    
    static size_t Dtor;	    //累計 dtor 的呼叫次數 
private:
    char* _data;
    size_t _len;
    void _init_data(const char* s) {
        _data = new char[_len + 1];
        memcpy(_data, s, _len);
        _data[_len] = '\0';
    }
public:
    //default ctor
    MyStrNoMove() : _data(NULL), _len(0) { ++DCtor; _init_data("jjhou"); }

    //ctor
    MyStrNoMove(const char* p) : _len(strlen(p)) {
        ++Ctor;  _init_data(p);
    }

    // copy ctor
    MyStrNoMove(const MyStrNoMove& str) : _len(str._len) {
        ++CCtor;
        _init_data(str._data); 	//COPY
    }

    //copy assignment
    MyStrNoMove& operator=(const MyStrNoMove& str) {
        ++CAsgn;

        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _init_data(str._data); 	//COPY! 
        }
        else {
            // Self Assignment, Nothing to do.   
        }
        return *this;
    }

    //dtor
    virtual ~MyStrNoMove() {
        ++Dtor;
        if (_data) {
            delete _data;
        }
    }

    bool
        operator<(const MyStrNoMove& rhs) const		//為了讓 set 比較大小 
    {
        return string(this->_data) < string(rhs._data);  //借用事實：string 已能比較大小. 
    }

    bool
        operator==(const MyStrNoMove& rhs) const	//為了讓 set 判斷相等. 
    {
        return string(this->_data) == string(rhs._data);  //借用事實：string 已能判斷相等. 
    }

    char* get() const { return _data; }
};
size_t MyStrNoMove::DCtor = 0;
size_t MyStrNoMove::Ctor = 0;
size_t MyStrNoMove::CCtor = 0;
size_t MyStrNoMove::CAsgn = 0;
size_t MyStrNoMove::MCtor = 0;
size_t MyStrNoMove::MAsgn = 0;
size_t MyStrNoMove::Dtor = 0;

namespace std {

    template <>
    struct hash<MyStrNoMove> {
        size_t operator()(const MyStrNoMove& s) const noexcept {
            return hash<string>()(string(s.get()));
        }
    };

    template <>
    struct hash<MyString> {
        size_t operator()(const MyString& s) const noexcept {
            return hash<string>()(string(s.get()));
        }
    };

}

#endif  //UTILCLASS_H
