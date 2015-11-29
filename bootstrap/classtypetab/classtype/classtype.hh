/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for class type */

#ifndef CLASSTYPE_HH
#define CLASSTYPE_HH

#include <cstdint>
#include <string>

#define CLASS_TYPE_INT8 "int8"
#define CLASS_TYPE_INT16 "int16"
#define CLASS_TYPE_INT32 "int32"
#define CLASS_TYPE_INT64 "int64"
#define CLASS_TYPE_UINT8 "uint8"
#define CLASS_TYPE_UINT16 "uint16"
#define CLASS_TYPE_UINT32 "uint32"
#define CLASS_TYPE_UINT64 "uint64"
#define CLASS_TYPE_BOOL "bool"
#define CLASS_TYPE_CHAR "char"
#define CLASS_TYPE_FUNC "func"

class class_type {
    
    public:
        virtual std::string get_type() = 0;
};

class int8_class_type : public class_type {
    
    private:
        int8_t value;
    
    public:
        std::string get_type() { return CLASS_TYPE_INT8; }
};

class int16_class_type : public class_type {
    
    private:
        int16_t value;
    
    public:
        std::string get_type() { return CLASS_TYPE_INT16; }
};

class int32_class_type : public class_type {
    
    private:
        int32_t value;
    
    public:
        std::string get_type() { return CLASS_TYPE_INT32; }
};

class int64_class_type : public class_type {
    
    private:
        int64_t value;
    
    public:
        std::string get_type() { return CLASS_TYPE_INT64; }
};

class uint8_class_type : public class_type {
    
    private:
        uint8_t value;
    
    public:
        std::string get_type() { return CLASS_TYPE_UINT8; }
};

class uint16_class_type : public class_type {
    
    private:
        uint16_t value;
    
    public:
        std::string get_type() { return CLASS_TYPE_UINT16; }
};

class uint32_class_type : public class_type {
    
    private:
        uint32_t value;
    
    public:
        std::string get_type() { return CLASS_TYPE_UINT32; }
};

class uint64_class_type : public class_type {
    
    private:
        uint64_t value;
    
    public:
        std::string get_type() { return CLASS_TYPE_UINT64; }
};

class bool_class_type : public class_type {
    
    private:
        bool value;
    
    public:
        std::string get_type() { return CLASS_TYPE_BOOL; }
};

class char_class_type : public class_type {
    
    private:
        std::string value;
    
    public:
        std::string get_type() { return CLASS_TYPE_CHAR; }
};

#endif