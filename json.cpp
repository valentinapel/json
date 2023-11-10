//
// Created by valentina vardanega on 09/10/23.
//
#include "json.hpp"
#include <sstream>
#include <fstream>


using std::cout;
using std::cin;
using std::string;
using std::pair;
using std::endl;

// i tipi del json
enum class Type {
    Null,
    Number,
    Boolean,
    String,
    List,
    Dictionary
};
//IMPLEMENTAZIONE
struct json::impl {

//DEFINISCO LISTA
struct List {
    json info;
    List* next;

    List* begin() {
        return this;
    }

    List* end() {
        List* current = this;
        while (current->next != nullptr) {
            current = current->next;
        }
        return current;
    }

};
typedef List* list;

//DEFINISCO DIZIONARIO
struct Dictionary {
    pair<string, json> info;
    Dictionary* next;

    Dictionary* begin() {
        return this;
    }

    Dictionary* end() {
        Dictionary* current = this;
        while (current->next != nullptr) {
            current = current->next;
        }
        return current;
    }

    json find(const string& key) {
        Dictionary* current = this;
        while (current != nullptr) {
            if (current->info.first == key) {
                return current->info.second;
            }
            current = current->next;
        }
        // Restituisci un valore predefinito (ad esempio un json vuoto) se la chiave non viene trovata.
        return json();
    }

};

typedef Dictionary* Dict;


    Type type;
    double numberValue;
    bool booleanValue;
    std::string stringValue;
    list listValue;
    Dict dictionaryValue;

    impl() {
        type=Type::Null;
        numberValue=0.0;
        booleanValue=false;
        stringValue= "";
        listValue= nullptr;
        dictionaryValue= nullptr;
    }
};

//DEFAULT CONSTRUCTOR
json::json() { pimpl=new impl;}

//COPY CONSTRUCTOR
json::json(const json& other) : pimpl(new impl(*(other.pimpl))) {}

//MOVE CONSTRUCTOR NON SICURA
json::json(json&& other) : pimpl(other.pimpl) {
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
json& json::operator=(json&& other)  {
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
    return pimpl->type == Type::Number;
}

bool json::is_bool() const {
    return pimpl->type==Type::Boolean;
}

bool json::is_null() const {
    return pimpl->type==Type::Null;
}

json const& json::operator[](std::string const& key) const {
    if (!is_dictionary()) {
        throw json_exception{"L'oggetto non è un dizionario."};
    }
    else {
        impl::Dict x=pimpl->dictionaryValue->begin();
        bool found=false;
        while(x!= nullptr && !found){
            if(x->info.first==key){
                found=true;
            }
            else x=x->next;
        }
        if(found) return x->info.second;
        else throw json_exception{"impossibile aggiungere elemento"};
    }
}

json& json::operator[](std::string const& key) {
    if (!is_dictionary()) {
        throw json_exception{"L'oggetto non è un dizionario."};
    }
    // Cerca la chiave nell'oggetto JSON.
    impl::Dict current = pimpl->dictionaryValue;
    while (current != nullptr) {
        if (current->info.first == key) {
            return current->info.second;
        }
        current = current->next;
    }
    // Se la chiave non esiste, crea una nuova coppia chiave-valore e restituisci il valore.
    impl::Dict newEntry = new impl::Dictionary();
    newEntry->info.first = key;
    newEntry->info.second = json();  // JSON nullo iniziale
    newEntry->next = nullptr;

    // Aggiungi la nuova coppia alla fine del dizionario.
    if (current == nullptr) {
        // Il dizionario è vuoto, impostiamo il nuovo elemento come primo.
        pimpl->dictionaryValue = newEntry;
    } else {
        // Trova l'ultimo elemento e collega il nuovo elemento.
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newEntry;
    }
    return newEntry->info.second;
}

//ITERATORI---------------------------------------------
struct json::list_iterator{
public:

    using iterator_category = std::forward_iterator_tag;

    list_iterator(impl::list p) : ptr(p) {};

    json& operator*() const {
        if (ptr != nullptr) return ptr->info;
        else {
            throw json_exception{"operator*  itera su null"};
        }
    }

    json* operator->() const {
        if (ptr != nullptr) return &(ptr->info);
        else {
            throw json_exception{"operator-> itera su null"};
        }
    }

    //prefix increment
    //++it
    list_iterator& operator++(){
        ptr = ptr->next;
        return *this;}

    //postfix increment
    //it++
    list_iterator operator++(int){
        list_iterator old_it {ptr};
        ++(*this);
        return old_it;
    }

    bool operator==(list_iterator const& rhs) const{ return ptr == rhs.ptr; }

    bool operator!=(list_iterator const& rhs) const{ return !(*this == rhs);}

    operator bool() const{	return ptr != nullptr;}

private:
    impl::list ptr;
};

struct json::const_list_iterator{
public:

    using iterator_category = std::forward_iterator_tag;

    const_list_iterator(impl::list p) : ptr(p) {}
    json& operator*() const{
        if (ptr != nullptr) return ptr->info;
        else {
            throw json_exception{"operator*  itera su null"};
        }
    }
    json* operator->() const{
        if (ptr != nullptr) return &(ptr->info);
        else {
            throw json_exception{"operator-> itera su null"};
        }
    }
    //prefix ++it
    const_list_iterator& operator++(){
        ptr = ptr->next;
        return *this;}

    //potfix it++
    const_list_iterator operator++(int){
        auto old_it {ptr};
        ++(*this);
        return old_it;
    }

    bool operator==(const_list_iterator const& rhs) const{ return ptr == rhs.ptr; }
    bool operator!=(const_list_iterator const& rhs) const {	return !(*this == rhs);}
    operator bool() const {	return ptr != nullptr;}

private:
    impl::list ptr;
};

struct json::dictionary_iterator{
private:
    impl::Dict ptr;
public:
    using iterator_category = std::forward_iterator_tag;

    dictionary_iterator(impl::Dict p ): ptr(p) {}
    pair<string, json>& operator*() const{
        if (ptr != nullptr) return ptr->info;
        else {
            throw json_exception{"operator*  itera su null"};
        }
    }
    pair<string, json>* operator->() const{
        if (ptr != nullptr) return &(ptr->info);
        else {
            throw json_exception{"operator-> itera su null"};
        }
    }

    //prefix ++it
    dictionary_iterator& operator++(){
        ptr = ptr->next;
        return *this;
    }

    //postfix it++
    dictionary_iterator operator++(int) {
        auto old_it {ptr};
        ++(*this);
        return old_it;
    }

    bool operator==(dictionary_iterator const& rhs) const { return ptr == rhs.ptr; }
    bool operator!=(dictionary_iterator const& rhs) const{	return !(*this == rhs);}
    operator bool() const {	return ptr != nullptr;}

};

struct json::const_dictionary_iterator{
private:

    impl::Dict ptr;
public:

    using iterator_category = std::forward_iterator_tag;

    const_dictionary_iterator(impl::Dict p ): ptr(p) {}

    pair<string, json>& operator*() const{
        if (ptr != nullptr) return ptr->info;
        else {
            throw json_exception{"operator*  itera su null"};
        }
    }

    pair<string, json>* operator->() const{
        if (ptr != nullptr) return &(ptr->info);
        else {
            throw json_exception{"operator-> itera su null"};
        }
    }

    //prefix ++it
    const_dictionary_iterator& operator++(){
        ptr = ptr->next;
        return *this;
    }

    //postfix it++
    const_dictionary_iterator operator++(int) {
        auto old_it {ptr};
        ++(*this);
        return old_it;
    }

    bool operator==(const_dictionary_iterator const& rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const_dictionary_iterator const& rhs) const{	return !(*this == rhs);}
    operator bool() const {	return ptr != nullptr;}

};

// Funzioni begin & end per gli iteratori

json::list_iterator json::begin_list() {
    if (pimpl && pimpl->type == Type::List) {
        return list_iterator(pimpl->listValue->begin());
    }
    else throw json_exception{"begin su json di tipo non lista"};
}

json::const_list_iterator json::begin_list() const {
    if (pimpl && pimpl->type == Type::List) {
        return const_list_iterator(pimpl->listValue->begin());
    }
    else throw json_exception{"begin su json di tipo non lista"};
}

json::list_iterator json::end_list() {
    if (pimpl && pimpl->type == Type::List) {
        return list_iterator(pimpl->listValue->end()); //NON HO END
    }
    else throw json_exception{"end su json di tipo non lista"};
}

json::const_list_iterator json::end_list() const {
    if (pimpl && pimpl->type == Type::List) {
        return const_list_iterator(pimpl->listValue->end()); // NON HO END
    }
    else throw json_exception{"end su json di tipo non lista"};
}

json::dictionary_iterator json::begin_dictionary() {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return dictionary_iterator(pimpl->dictionaryValue->begin()); // non ho begin
    }
    else throw json_exception{"begin su json di tipo non dict"};
}

json::const_dictionary_iterator json::begin_dictionary() const {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return const_dictionary_iterator(pimpl->dictionaryValue->begin()); //non ho begin
    }
    else throw json_exception{"begin su json di tipo non dict"};
}

json::dictionary_iterator json::end_dictionary() {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return dictionary_iterator(pimpl->dictionaryValue->end()); //non ho end
    }
    else throw json_exception{"end su json di tipo non dict"};
}

json::const_dictionary_iterator json::end_dictionary() const {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return const_dictionary_iterator(pimpl->dictionaryValue->end()); // non ho end
    }
    else throw json_exception{"end su json di tipo non dict"};
}

// CI VANNO GETTER E SETTER---------------------------------
double& json::get_number() {
    if(! is_number())
        throw json_exception{ " get_number ma json non è tipo number "};
    return pimpl->numberValue;
}

const double& json::get_number() const {
    if(! is_number())
        throw json_exception{ " get_number ma json non è tipo number "};
    return pimpl->numberValue;
}

bool &json::get_bool() {
    if(! is_bool())
        throw json_exception{ " get_bool ma json non è tipo bool "};
    return pimpl->booleanValue;
}

const bool &json::get_bool() const {
    if(! is_bool())
        throw json_exception{ " get_bool ma json non è tipo bool "};
    return pimpl->booleanValue;
}

std::string &json::get_string() {
    if(! is_string())
        throw json_exception{ "  get_string ma json non è tipo string "};
    return pimpl->stringValue;
}

const std::string &json::get_string() const {
    if(! is_string())
        throw json_exception{ "  get_string ma json non è tipo string "};
    return pimpl->stringValue;
}

void json::set_string(const std::string& x) {
    delete pimpl;
    pimpl = new impl();
    pimpl->type = Type::String;
    pimpl->stringValue = x;
}

void json::set_bool(bool x) {
    delete pimpl;
    pimpl = new impl();
    pimpl->type = Type::Boolean;
    pimpl->booleanValue = x;
}

void json::set_number(double x) {
    delete pimpl;
    pimpl = new impl();
    pimpl->type = Type::Number;
    pimpl->numberValue = x;
}

void json::set_null() {
    delete pimpl;
    pimpl = new impl();
    pimpl->type = Type::Null;
}

void json::set_list() {
    delete pimpl;
    pimpl = new impl();
    pimpl->type = Type::List;
}

void json::set_dictionary() {
    delete pimpl;
    pimpl = new impl();
    pimpl->type = Type::Dictionary;
}


void json::push_front(const json & x) {
    if (!is_list()) {
        throw json_exception{"Impossibile aggiungere elemento in testa a un JSON non di tipo lista."};
    }
    // Crea un nuovo nodo List per il valore da aggiungere.
    impl::list newNode = new impl::List();
    newNode->info = x;
    // Aggiungi il nuovo nodo in testa alla lista.
    newNode->next = pimpl->listValue;
    pimpl->listValue = newNode;
}

void json::push_back(json const& x) {
    // Verifica se il tipo dell'oggetto è una lista.
    if (!is_list()) {
        throw json_exception{"Impossibile aggiungere elemento in fondo a un JSON non di tipo lista."};
    }
    // Crea un nuovo nodo List per il valore da aggiungere.
    impl::list newNode = new impl::List();
    newNode->info = x;
    newNode->next = nullptr;
    // Se la lista è vuota, il nuovo nodo diventa l'unico elemento della lista.
    if (pimpl->listValue == nullptr) {
        pimpl->listValue = newNode;
    } else {
        // Trova l'ultimo elemento attuale e collega il nuovo nodo.
        impl::list current = pimpl->listValue;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void json::insert(const std::pair<std::string, json>& x) {
    // Verifica se il tipo dell'oggetto è un dizionario.
    if (!is_dictionary()) {
        throw json_exception{"Impossibile aggiungere una coppia chiave-valore in un JSON non di tipo dizionario."};
    }
    // Crea un nuovo nodo Dictionary per la coppia chiave-valore da aggiungere.
    impl::Dict newNode = new impl::Dictionary();
    newNode->info = x;
    newNode->next = nullptr;
    // Se il dizionario è vuoto, il nuovo nodo diventa l'unico elemento del dizionario.
    if (pimpl->dictionaryValue == nullptr) {
        pimpl->dictionaryValue = newNode;
    } else {
        // Trova l'ultimo elemento attuale e collega il nuovo nodo.
        impl::Dict current = pimpl->dictionaryValue;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

//----------PARSING------------
string parseString(std::istream& is);// parser di una stringa
json parseNumber(std::istream& is);// parser di un double
json parseBoolean(std::istream& is);// parser di un bool
json parseNull(std::istream& is);// parser di null
json parseList(std::istream& is);// parser di una lista
json parseDict(std::istream& is); // parser di un dizionario
json parseValue(std::istream& is); //parser di tutti i valori


string parseString(std::istream& is) {
    string value;
    char ch;
    while (is.get(ch)) {
        if (ch == '"') {
            break;  // Fine della stringa.
        } else if (ch == '\\') {
            // Gestione delle escape sequences, es. \"
            char next;
            is.get(next);
            if (next == '"') {
                value += "\\\"";
            } else if (next == '\\') {
                value += '\\';
            } else {
                throw json_exception{"Errore nella stringa JSON."};
            }
        } else {
            value += ch;
        }
    }
    if (!is.eof()) {
        return value;
    } else {
        throw json_exception{"Stringa JSON non terminata correttamente."};
    }}

json parseNumber(std::istream& is) {
    json j;
    double number;
    is >> number;
    j.set_number(number);
    return j;
}

json parseBoolean(std::istream& is) {
    json j;
    string word;
    is >> word;
    if (word == "true") {
        j.set_bool(true);
        return j;
    } else if (word == "false") {
        j.set_bool(false);
        return j;
    } else {
        throw json_exception{"Errore nel valore booleano JSON."};
    }
}

json parseNull(std::istream& is) {
    json j;
    string word;
    is >> word;
    if (word != "null") {
        throw json_exception{"Errore nel valore null JSON."};
    }
    j.set_null();
    return j;
}

json parseList(std::istream& is) {
    json list;
    char ch;
    // Ignora i separatori iniziali (newline, tab, spazi).
    while (is.get(ch)) {
        if (ch != ' ' && ch != '\t' && ch != '\n') {
            is.unget();
            break;
        }
    }

    is >> ch;  // Leggi '['

    while (is.peek() != ']') {
        // Ignora i separatori tra gli elementi, ma non tra doppi apici.
        while (ch == ' ' || ch == '\t' || ch == '\n') {
            is >> ch;
        }

        json value = parseValue(is);
        list.push_back(value);

        is >> ch;

        if (ch == ',') {
            // Continua a leggere altri elementi della lista.
        } else if (ch == ']') {
            break;  // Fine della lista.
        } else {
            throw json_exception{"Errore nella lista JSON."};
        }
    }

    return list;
}

json parseDict(std::istream& is) {
    json object;
    char ch;
    //non so se serve

    // Ignora i separatori iniziali (newline, tab, spazi).
    while (is.get(ch)) {
        if (ch != ' ' && ch != '\t' && ch != '\n') {
            is.unget();
            break;
        }
    }

    is >> ch;  // Leggi '{'

    while (is.peek() != '}') {
        //non so se serve
        // Ignora i separatori tra le coppie chiave-valore, ma non tra doppi apici.
        while (ch == ' ' || ch == '\t' || ch == '\n') {
            is >> ch;
        }
        string key = parseString(is);
        is >> ch;  // Leggi ':'
        json value = parseValue(is);
        object[key] = value;

        is >> ch;
        if (ch == ',') {
            // Continua a leggere altre coppie chiave-valore.
        } else if (ch == '}') {
            break;  // Fine dell'oggetto.
        } else {
            throw json_exception{"Errore nell'oggetto JSON."};
        }
    }
    return object;
}

json parseValue(std::istream& is) {
    char ch;
    is >> ch;

    if (ch == '"') {
        json j;
        j.set_string(parseString(is));
        return j;
        //json()->set_string(parseString(is)); // ritorna una string non un json DA SISTEMARE
    } else if (isdigit(ch) || ch == '-') {
        is.unget();
        return parseNumber(is);
    } else if (ch == 't' || ch == 'f') {
        is.unget();
        return parseBoolean(is);
    } else if (ch == 'n') {
        is.unget();
        return parseNull(is);
    } else if (ch == '[') {
        return parseList(is);
    } else if (ch == '{') {
        return parseDict(is);
    } else {
        throw json_exception{"Errore durante il parsing JSON."};
    }
}

string escapeString(const string& str) {
    string result;
    for (char c : str) {
        if (c == '"' || c == '\\') {
            result += '\\';
        }
        result += c;
    }
    return result;
}

std::ostream& operator<<(std::ostream& lhs,  json const& rhs) {
            if(rhs.is_null()){ lhs << "null";}
            else if (rhs.is_number()){ lhs << rhs.get_number();}
            else if (rhs.is_bool()) {lhs << (rhs.get_bool() ? "true" : "false");}
            else if (rhs.is_string()){lhs << '"' << escapeString(rhs.get_string()) << '"';}
            else if (rhs.is_list()){
                lhs << "[";
                for (auto i = rhs.begin_list(); i < rhs.end_list(); ++i) {
                    if (i ==rhs.end_list()) lhs << *i;
                    else lhs << *i<<",";
                }
                lhs << "]";
            }
            else if (rhs.is_dictionary()){
                lhs << "{";
                bool first = true;
                for ( auto i= rhs.begin_dictionary(); i != rhs.end_dictionary();i++) {
                    if (!first) lhs << ",";
                    lhs << '"' << escapeString(i->first) << "\":" << i->second;
                    first = false;
                }
                lhs << "}";
            }
            else throw json_exception{"<< non funziona"};
    return lhs;
}


std::istream& operator>>(std::istream& lhs, json& rhs) {
    json parsedJson;

    try {
        char ch;
        lhs >> ch;  // Leggi il primo carattere.

        if (ch == '[') {
            parsedJson = parseList(lhs);
        } else if (ch == '{') {
            parsedJson = parseDict(lhs);
        } else {
            throw json_exception{"Errore nel parsing JSON: Il documento JSON deve iniziare con '[' o '{'."};
        }
        // Sovrascrivi l'oggetto rhs con il JSON analizzato.
        rhs = parsedJson;
    } catch (const json_exception& ex) {
        // Se si verifica un errore di parsing, lancia un'eccezione json_exception con un messaggio.
        throw json_exception{"Errore durante il parsing JSON: " };
    }

    return lhs;
}

