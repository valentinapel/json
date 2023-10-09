//
// Created by valentina vardanega on 09/10/23.
//
#include "json.hpp"

// i tipi del json
enum class Type {
    Null,
    Number,
    Boolean,
    String,
    List,
    Dictionary
};

//DEFINISCO LISTA
struct List {
    json info;
    List* next;
};
typedef List* list;

//DEFINISCO DIZIONARIO
struct Dictionary {
    pair<string, json> info;
    Dictionary* next;
};

typedef Dictionary* Dict;

//IMPLEMENTAZIONE
struct json::impl {

    Type type;
    int numberValue;
    bool booleanValue;
    std::string stringValue;
    list listValue;
    Dict dictionaryValue;

    impl() : type(Type::Null) {}
};

//DEFAULT CONSTRUCTOR
json::json() : pimpl(new impl()) {}

//COPY CONSTRUCTOR
json::json(const json& other) : pimpl(new impl(*(other.pimpl))) {}

//MOVE CONSTRUCTOR NON SICURA
json::json(json&& other) noexcept : pimpl(other.pimpl) {
other.pimpl = nullptr;
}
// pimpl(new Impl) { *this=std::move(other);

//DISTRUTTORE
json::~json() {
    delete pimpl;
}

//COPY ASSIGNMENT
json& json::operator=(const json& other) {
    if (this != &other) {
        delete pimpl;
        pimpl = new impl(*(other.pimpl));
    }
    return *this;
}

//MOVE ASSIGNMENT
json& json::operator=(json&& other) noexcept {
if (this != &other) {
delete pimpl;
pimpl = other.pimpl;
other.pimpl = nullptr;
}
return *this;
}

bool json::is_list() const {
    return (pimpl->type == Type::List);
}

bool json::is_dictionary() const {
    return (pimpl->type == Type::Dictionary);
}

bool json::is_string() const {
    return (pimpl->type == Type::String);
}

bool json::is_number() const {
    return {pimpl->type == Type::Number};
}

bool json::is_bool() const {
    return {pimpl->type==Type::Boolean};
}

bool json::is_null() const {
    return {pimpl->type==Type::Null};
}

// Funzioni begin e end per gli iteratori IN QUESTE CI SONO FUNZIONI BEGIN() E END() CHE NON SONO IMPLEMENTATE

list_iterator json::begin_list() {
    if (pimpl && pimpl->type == Type::List) {
        return list_iterator(pimpl->listValue->begin()); //NON HO BEGIN
    }
    return list_iterator();
}

const_list_iterator json::begin_list() const {
    if (pimpl && pimpl->type == Type::List) {
        return const_list_iterator(pimpl->listValue->begin()); // NON HO BEGIN
    }
    return const_list_iterator();
}

list_iterator json::end_list() {
    if (pimpl && pimpl->type == Type::List) {
        return list_iterator(pimpl->listValue->end()); //NON HO END
    }
    return list_iterator();
}

const_list_iterator json::end_list() const {
    if (pimpl && pimpl->type == Type::List) {
        return const_list_iterator(pimpl->listValue->end()); // NON HO END
    }
    return const_list_iterator();
}

dictionary_iterator json::begin_dictionary() {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return dictionary_iterator(pimpl->dictionaryValue->begin()); // non ho begin
    }
    return dictionary_iterator();
}

const_dictionary_iterator json::begin_dictionary() const {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return const_dictionary_iterator(pimpl->dictionaryValue->begin()); //non ho begin
    }
    return const_dictionary_iterator();
}

dictionary_iterator json::end_dictionary() {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return dictionary_iterator(pimpl->dictionaryValue->end()); //non ho end
    }
    return dictionary_iterator();
}

const_dictionary_iterator json::end_dictionary() const {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return const_dictionary_iterator(pimpl->dictionaryValue->end()); // non ho end
    }
    return const_dictionary_iterator();
}