/* 
Copyright (C) 2014 Jonathon Ogden   < jeog.dev@gmail.com >

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses.
*/

#ifndef JO_TOSDB_EXCEPTIONS
#define JO_TOSDB_EXCEPTIONS

#include <string>
#include <exception>
#include <Windows.h>

class TOSDB_Error 
        : public std::exception{
    DWORD _threadID;
    DWORD _processID;
    std::string _tag;
    std::string _info;

public:
    TOSDB_Error(std::string info, std::string tag)
        : 
            std::exception(info.c_str()),
            _tag(tag),
            _info(info),
            _threadID(GetCurrentThreadId()),
            _processID(GetCurrentProcessId())
        {         
        }

    TOSDB_Error(const std::exception& e, std::string tag)
        : 
            std::exception(e),
            _tag(tag),    
            _threadID(GetCurrentThreadId()),
            _processID(GetCurrentProcessId())
        {         
        }

    TOSDB_Error(const std::exception& e, std::string info, std::string tag)
        : 
            std::exception(e),
            _tag(tag),
            _info(info),
            _threadID(GetCurrentThreadId()),
            _processID(GetCurrentProcessId())
        {        
        }

    virtual 
    ~TOSDB_Error() 
        {
        }

    inline DWORD 
    threadID() const 
    { 
        return _threadID; 
    }

    inline DWORD 
    processID() const 
    { 
        return _processID; 
    }

    inline std::string 
    tag() const 
    { 
        return _tag; 
    }

    inline std::string 
    info() const 
    { 
        return _info; 
    }

    /* do better than this */
    inline std::string 
    info_and_what() const 
    { 
        return "info: " + _info + ", what: " + std::string(what());
    }
};


class TOSDB_BufferError 
        : public TOSDB_Error{
public:
    TOSDB_BufferError(std::string info, std::string tag = "DataBuffer")
        : 
            TOSDB_Error(info, tag) 
        {
        }
};


class TOSDB_DDE_Error 
        : public TOSDB_Error{
public:
    TOSDB_DDE_Error(std::string info, std::string tag = "DDE")
        : 
            TOSDB_Error(info, tag) 
        {
        }

    TOSDB_DDE_Error(const std::exception& e, std::string info, std::string tag = "DDE")
        : 
            TOSDB_Error(e, info, tag) 
        {
        }
};    


class TOSDB_DataBlockError 
        : public TOSDB_Error{
public:
    TOSDB_DataBlockError(std::string info, std::string tag = "DataBlock")
        : 
            TOSDB_Error(info, tag) 
        {
        }

    TOSDB_DataBlockError(const std::exception& e, std::string info, std::string tag = "DataBlock") 
        :  
            TOSDB_Error(e, info, tag) 
        {
        }
};  


class TOSDB_DataBlockDoesntExist 
        : public TOSDB_DataBlockError{
public:
    TOSDB_DataBlockDoesntExist(std::string name)
        : 
            TOSDB_DataBlockError("block (" + std::string(name) + ") doesn't exist") 
        {
        }
};  


class TOSDB_DataBlockLimitError 
        : public TOSDB_DataBlockError{
public:
    const size_t limit;  

    TOSDB_DataBlockLimitError(const size_t limit)
        : 
            TOSDB_DataBlockError("would exceed RawDataBlock limit"),           
            limit(limit)
        {
        }
};            


/* TOSDB_DataStreamError is the 'external' version of DataStreamError (data_stream.hpp/tpp)
   The latter is caught 'internally' and passed to TOSDB_DataStreamError cstr */
class TOSDB_DataStreamError 
        : public TOSDB_DataBlockError{
public:
    TOSDB_DataStreamError(std::string info, std::string tag = "DataStream")
        : 
            TOSDB_DataBlockError(info, tag) 
        {
        }

    TOSDB_DataStreamError(const std::exception& e, std::string info, std::string tag = "DataStream")
        : 
            TOSDB_DataBlockError(e, info, tag) 
        {
        }
};


#endif
