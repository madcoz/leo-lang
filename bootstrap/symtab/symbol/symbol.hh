/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for symbol */

#ifndef SYMBOL_HH
#define SYMBOL_HH

#include <cstdint>
#include <string>
#include <memory>

#include "../../classtypetab/classtype/classtype.hh"

#define SYMBOL_LITERAL_STRING "char*"
#define SYMBOL_LITERAL_CHAR "char"
#define SYMBOL_LITERAL_BOOL "bool"
#define SYMBOL_LITERAL_UINT32 "uint32"
#define SYMBOL_LITERAL_UINT64 "uint64"

#define SYMBOL_TYPE_IDENT "id"

class symbol {
    
    private:
        std::string uid;
    
    public:
        symbol() {}
            
        virtual ~symbol() {}
        
        virtual std::string get_type() = 0;
        
        virtual void set_uid(const std::string& uid) { this->uid = uid; }
        
        virtual std::string get_uid() { return uid; }
};

class ident_symbol : public symbol {
        
    private:
        std::unique_ptr<class_type> cls_t;
        
    public:
        ident_symbol(class_type* cls_t) : cls_t(cls_t) {}
    
        virtual ~ident_symbol() { cls_t.reset(); }
        
        std::string get_type() {  return SYMBOL_TYPE_IDENT; }
        
        class_type* get_class_type() { return cls_t.get(); }   

        void set_class_type(class_type* cls_t) { this->cls_t.reset(cls_t); }
};

class literal_string_symbol : public symbol {
        
    private:
        std::string value;
        
    public:
        literal_string_symbol(const std::string& value) : value(value) {}
        
        virtual ~literal_string_symbol() {}
        
        std::string get_type() { return SYMBOL_LITERAL_STRING; }
};

class literal_char_symbol : public symbol {

    private:
        std::string value;
        
    public:
        literal_char_symbol(const std::string& value) : value(value) {}
        
        virtual ~literal_char_symbol() {}
        
        std::string get_type() { return SYMBOL_LITERAL_CHAR; }
};

class literal_bool_symbol : public symbol {

    private:
        bool value;
        
    public:
        literal_bool_symbol(const bool value) : value(value) {}
        
        virtual ~literal_bool_symbol() {}
        
        std::string get_type() { return SYMBOL_LITERAL_BOOL; }
};

class literal_uint32_symbol : public symbol {
    
    private:
        uint32_t value;
    
    public:
        literal_uint32_symbol(const uint32_t value) : value(value) {}
        
        virtual ~literal_uint32_symbol() {}
        
        std::string get_type() { return SYMBOL_LITERAL_UINT32; }
        
        uint32_t get_value() { return value; }
};

class literal_uint64_symbol : public symbol {
        
    private:
        uint64_t value;
    
    public:
        literal_uint64_symbol(const uint64_t value) : value(value) {}
        
        virtual ~literal_uint64_symbol() {}
        
        std::string get_type() { return SYMBOL_LITERAL_UINT64; }
        
        uint64_t get_value() { return value; }
};

#endif