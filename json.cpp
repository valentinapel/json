//
// Created by valentina vardanega
//
#include "json.hpp"
#include <sstream>

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

        // Funzione per eseguire una deep copy di una lista
        //Se ho due oggetti JSON, uno originale e uno copiato,
        //posso modificarli separatamente senza che le modifiche su uno influenzino l'altro.
        static List* deepCopy(const List* source) {
            if (!source ) {return nullptr; } // Lista vuota

            // Copio il primo elemento
            List* newList = new List;
            newList->info = source->info;
            newList->next = nullptr;

            // Copio gli elementi successivi
            List* currentNew = newList;
            const List* currentSource = source->next;

            while (currentSource) {
                List* newElement = new List;
                newElement->info = currentSource->info;
                newElement->next = nullptr;

                // Aggiungo il nuovo elemento in coda
                currentNew->next = newElement;
                currentNew = newElement;

                currentSource = currentSource->next;
            }
            return newList;
        }


    };
    typedef List* list;

    //DEFINISCO DIZIONARIO
    struct Dictionary {
        std::pair<std::string, json> info;
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

        //Se ho due oggetti JSON, uno originale e uno copiato,
        //posso modificarli separatamente senza che le modifiche su uno influenzino l'altro.
        static Dictionary* deepCopy(const Dictionary* source) {
            if (!source) {return nullptr;}

            // Copio il primo elemento
            Dictionary* newDict = new Dictionary;
            newDict->info = source->info;
            newDict->next = nullptr;

            // Copio gli elementi successivi
            Dictionary* currentNew = newDict;
            const Dictionary* currentSource = source->next;

            while (currentSource) {
                Dictionary* newElement = new Dictionary;
                newElement->info = currentSource->info;
                newElement->next = nullptr;

                // Aggiungo il nuovo elemento in coda
                currentNew->next = newElement;
                currentNew = newElement;

                currentSource = currentSource->next;
            }
            return newDict;
        }
    };
    typedef Dictionary* Dict;

    //CAMPI
    Type type;
    double numberValue;
    bool booleanValue;
    std::string stringValue;
    list listValue;
    Dict dictionaryValue;

    impl(){
        type = Type::Null;
        numberValue = 0.0;
        booleanValue = false;
        stringValue = "";
        listValue  = nullptr;
        dictionaryValue  = nullptr;
    }

    ~impl() {
        reset();
    }

    impl(const impl &source){
        if (&source != this){
            copy(source);
        }
    }

    impl(impl&& source)
            : type(source.type),
              numberValue(source.numberValue),
              booleanValue(source.booleanValue),
              stringValue(std::move(source.stringValue)),
              listValue(std::move(source.listValue)),
              dictionaryValue(std::move(source.dictionaryValue)) {
        source.type = Type::Null;
    }

    impl& operator=(const impl &source) {
        if (this != &source) {
            copy(source);
        }
        return *this;
    }

    void resetList(){
        while(listValue){
            List* del=listValue;
            listValue=listValue->next;
            delete del;
        }
    }

    void resetDict() {
        while (dictionaryValue) {
            Dictionary* del = dictionaryValue;
            dictionaryValue = dictionaryValue->next;
            delete del;
        }
    }

    void reset(){

        switch (type) {
            case Type::String:
                stringValue.clear();
                break;
            case Type::Boolean:
                break;
            case Type::Number:
                break;
            case Type::List:
                resetList();
                break;
            case Type::Dictionary:
                resetDict();
                break;
            case Type::Null:
                break;
        }
    }

    void copy(impl const& source){
        type = source.type;

        switch(type){
            case Type::String:
                stringValue=source.stringValue;
                break;
            case Type::Boolean:
                booleanValue=source.booleanValue;
                break;
            case Type::Number:
                numberValue=source.numberValue;
                break;
            case Type::Dictionary:
                dictionaryValue=Dictionary::deepCopy(source.dictionaryValue);
                break;
            case Type::List:
                listValue=List::deepCopy(source.listValue);
                break;
        }
    }

};


//DEFAULT CONSTRUCTOR
json::json(): pimpl(new impl()) { }

//COPY CONSTRUCTOR
json::json(const json& source) : pimpl(new impl(*source.pimpl)) {}

//MOVE CONSTRUCTOR
json::json(json&& source) : pimpl(new impl) { *this=std::move(source);}

//DISTRUTTORE
json::~json() {
    if(pimpl) delete pimpl;
}

//COPY ASSIGNMENT
json& json::operator=(json const& source){

    if (this != &source) {
        *(this->pimpl) = *(source.pimpl);

    }
    return *this;

}

//MOVE ASSIGNMENT
json& json::operator=(json&& source)  {
    if (this != &source) {
        delete pimpl;
        pimpl = source.pimpl;
        source.pimpl = nullptr;
    }
    return *this;

}

//BOOLEAN FUNCTION
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

//ACCESS FUNCTION
json const& json::operator[](std::string const& key) const {
    if (!is_dictionary()) {
        throw json_exception{"L'oggetto non è un dizionario."};
    }
    else {
        impl::Dict current=pimpl->dictionaryValue->begin();
        bool found=false;
        while(current!= nullptr && !found){
            if(current->info.first==key){
                found=true;
            }
            else current=current->next;
        }
        if(found) return current->info.second;
        else throw json_exception{"impossibile aggiungere elemento"};
    }
}

json& json::operator[](std::string const& key) {
    if (!is_dictionary()) {
        throw json_exception{"L'oggetto non è un dizionario."};
    } else {
        impl::Dict current = pimpl->dictionaryValue->begin();
        bool found = false;
        while (current != nullptr && !found) {
            if (current->info.first == key) {
                found = true;
            } else current = current->next;
        }
        if (found) return current->info.second;
        else {
            insert(std::pair<std::string, json>{key, json()});
            return pimpl->dictionaryValue->end()->info.second;

        }
    }
}

//ITERATOR-----------------------------------------------
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
    std::pair<std::string, json>& operator*() const{
        if (ptr != nullptr) return ptr->info;
        else {
            throw json_exception{"operator*  itera su null"};
        }
    }
    std::pair<std::string, json>* operator->() const{
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

    std::pair<std::string, json>& operator*() const{
        if (ptr != nullptr) return ptr->info;
        else {
            throw json_exception{"operator*  itera su null"};
        }
    }

    std::pair<std::string, json>* operator->() const{
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

//BEGIN AND END FUNCTION FOR ITERATORS
json::list_iterator json::begin_list() {
    if (pimpl && pimpl->type == Type::List) {
        return list_iterator(pimpl->listValue);
    }
    else throw json_exception{"begin su json di tipo non lista"};
}

json::const_list_iterator json::begin_list() const {
    if (pimpl && pimpl->type == Type::List) {
        return const_list_iterator(pimpl->listValue);
    }
    else throw json_exception{"begin su json di tipo non lista"};
}

json::list_iterator json::end_list() {
    return list_iterator(nullptr);
}

json::const_list_iterator json::end_list() const {
    return const_list_iterator(nullptr);

}

json::dictionary_iterator json::begin_dictionary() {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return dictionary_iterator(pimpl->dictionaryValue);
    }
    else throw json_exception{"begin su json di tipo non dict"};
}

json::const_dictionary_iterator json::begin_dictionary() const {
    if (pimpl && pimpl->type == Type::Dictionary) {
        return const_dictionary_iterator(pimpl->dictionaryValue);
    }
    else throw json_exception{"begin su json di tipo non dict"};
}

json::dictionary_iterator json::end_dictionary() {
    return dictionary_iterator(nullptr);
}

json::const_dictionary_iterator json::end_dictionary() const {
    return const_dictionary_iterator(nullptr);
}

// GETTERS AND SETTERS
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
    pimpl->reset();
    pimpl->type = Type::String;
    pimpl->stringValue = x;

    if (!is_string() || is_null() ||is_bool()  ||  is_number()  || is_list() || is_dictionary()){
        throw json_exception{"is_string() deve restituire true (nessun altro metodo booleano deve restituire true)"};
    }
}

void json::set_bool(bool x) {
    pimpl->reset();
    pimpl->type = Type::Boolean;
    pimpl->booleanValue = x;

    if (!is_bool()){
        throw json_exception{"is_bool() deve restituire true "};
    }
}

void json::set_number(double x) {
    pimpl->reset();
    pimpl->type = Type::Number;
    pimpl->numberValue = x;

    if (!is_number()  ){
        throw json_exception{"is_number() deve restituire true "};
    }
}

void json::set_null() {
    pimpl->reset();
    pimpl->type = Type::Null;

    if (!is_null() ){
        throw json_exception{"is_null() deve restituire true"};
    }
}

void json::set_list() {
    pimpl->reset();
    pimpl->type = Type::List;

    if (!is_list() ){
        throw json_exception{"is_list() deve restituire true"};
    }
}

void json::set_dictionary() {
    pimpl->reset();
    pimpl->type = Type::Dictionary;

    if (!is_dictionary()){
        throw json_exception{"is_string() deve restituire true "};
    }
}

//aggiunge x in testa alla lista.
void json::push_front(json const& x) {
    if (!is_list()) {
        throw json_exception{"Impossibile aggiungere elemento in testa a un JSON non di tipo lista."};
    }
    // Creo un nuovo nodo List per il valore da aggiungere.
    impl::list newNode = new impl::List();
    newNode->info = x;
    // Aggiungo il nuovo nodo in testa alla lista.
    newNode->next = pimpl->listValue;
    pimpl->listValue = newNode;
}

//aggiunge x in fondo alla lista.
void json::push_back(json const& x) {
    if (!is_list()) {
        throw json_exception{"Impossibile aggiungere elemento in fondo a un JSON non di tipo lista."};
    }
    // Creo un nuovo nodo List per il valore da aggiungere.
    impl::list newNode = new impl::List();
    newNode->info = x;
    newNode->next = nullptr;
    // Se la lista è vuota, il nuovo nodo diventa l'unico elemento della lista.
    if (pimpl->listValue == nullptr) {
        pimpl->listValue = newNode;
    } else {
        // Trovo l'ultimo elemento attuale e collega il nuovo nodo.
        impl::list current = pimpl->listValue;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

//inserisce coppia in coda a dizionario
void json::insert(std::pair<std::string, json> const &x) {
    if (!is_dictionary()) {
        throw json_exception{"inserimento in un json che non è dizionario"};
    } else {
        // Inserisco in coda
        impl::Dict nuova = new impl::Dictionary;
        nuova->info = x;
        nuova->next = nullptr;

        if (pimpl->dictionaryValue == nullptr) {
            pimpl->dictionaryValue = nuova; // La lista è vuota, quindi l'elemento è sia in testa che in coda.
        } else {
            impl::Dict current = pimpl->dictionaryValue;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = nuova; // Inserisco il nuovo elemento alla fine della lista.
        }
    }
}

//rimuove gli spazi
void remove_ws(std::istream& is){
    while (std::isspace(is.peek()))
        is.ignore();
}

std::ostream& operator<<(std::ostream& lhs, json const& rhs) {
    if (rhs.is_null()) {
        lhs << "null";
    }else if (rhs.is_number()) {
        lhs << rhs.get_number();
    }else if (rhs.is_bool()) {
        lhs << (rhs.get_bool() ? "true" : "false");
    } else if (rhs.is_string()) {
        lhs << '"' << rhs.get_string() << '"';
    } else if (rhs.is_list()) {
        lhs << '[';
        bool flag = true;
        for (auto it = rhs.begin_list(); it != rhs.end_list(); ++it) {
            if (!flag) {
                lhs << ", ";
            }
            lhs << *it;
            flag = false;
        }
        lhs << ']';
    } else if (rhs.is_dictionary()) {
        lhs << '{';
        bool flag = true;
        for (auto it = rhs.begin_dictionary(); it != rhs.end_dictionary(); ++it) {
            if (!flag) {
                lhs << ", ";
            }
            lhs << "\"" << it->first << "\": " << it->second;
            flag = false;
        }
        lhs << '}';
    }
    return lhs;
}

//parsing stringa
std::string parse_string(std::istream& is) {
    std::string result;
    char ch;
    bool flag = false;

    if (is.peek() != '"') throw json_exception{"mi aspetto una stringa"};
    is.ignore(); // Ignora la prima virgoletta

    while (is.get(ch)) {
        if (flag) {
            if (ch == 'n') {
                result += "\\n"; //  il carattere di escape \n

            }
            else {
                result += '\\'; // Aggiungo backslash per altre sequenze di escape
                result += ch; // Aggiungo il carattere escaped
            }
            flag = false;
        } else {
            if (ch == '\\') {
                flag = true;
            } else if (ch == '"') {
                break; // Fine della stringa
            } else {
                result += ch;
            }
        }
    }
    return result;
}

std::istream& operator>>(std::istream& lhs, json& rhs) {

    try {
        remove_ws(lhs);
        if (!lhs.eof()) {
            if (lhs.eof()) throw json_exception{"fine inaspettata"};
            //FUNZIONI PARSING
            remove_ws(lhs);
            if (lhs.eof()) throw json_exception{"fine inaspettata"};

            char c = lhs.peek();
            if(c=='['){
                lhs.ignore();
                rhs.set_list();

                //PARSING LISTA
                remove_ws(lhs);
                if (lhs.peek() != ']') {
                    while ( lhs.peek() != ']') {
                        json item;
                        lhs >> item;
                        rhs.push_back(item);
                        remove_ws(lhs);

                        if ( lhs.peek() == ']') {
                            break;
                        } else if (lhs.peek() != ',') {
                            throw json_exception{"lista non valida"};
                        } else {
                            lhs.ignore();
                            remove_ws(lhs);

                            if (lhs.peek() == ']') {
                                throw json_exception{"virgola in eccesso dopo l'ultimo elemento"};
                            }
                        }
                    }
                }
                remove_ws(lhs);
                if (lhs.get() != ']') throw json_exception{" fine lista"};
            }else if(c=='{'){
                lhs.ignore();
                rhs.set_dictionary();

                //PARSING DICTIONARY
                remove_ws(lhs);
                if (lhs.peek() != '}') {
                    while (lhs.peek() != '}') {
                        remove_ws(lhs);

                        std::string key = parse_string(lhs);

                        remove_ws(lhs);

                        if (lhs.get() != ':')  throw json_exception{"dizionario non valido"};

                        remove_ws(lhs);

                        json value;
                        lhs >> value;
                        rhs[key] = value;
                        remove_ws(lhs);
                        if (lhs.peek() == ',') {
                            lhs.ignore();
                            remove_ws(lhs);

                            if (lhs.peek() == '}') throw json_exception{"ultima virgola prima di '}'"};
                        } else if (lhs.peek() == '}') {
                            break; // Fine del dizionario
                        } else throw json_exception{" formato non valido"};
                    }
                }
                remove_ws(lhs);
                if (lhs.get() != '}') throw json_exception{" fine dizionario"};
            }else if(c=='"'){ //PARSING STRINGA
                std::string str = parse_string(lhs);
                rhs.set_string(str);
            }
                // PARSING VALORI NULL, TRUE E FALSE
            else if (c == 'n' || c == 't' || c == 'f') {
                char buffer[6];
                std::string expected_value;

                if (c == 'n') {
                    expected_value = "null";
                } else if (c == 't') {
                    expected_value = "true";
                } else if (c == 'f') {
                    expected_value = "false";
                }

                if (lhs.read(buffer, expected_value.size()) &&
                    std::string(buffer, expected_value.size()) == expected_value) {
                    remove_ws(lhs);

                    if (c == 'n') {
                        rhs.set_null();
                    } else if (c == 't') {
                        rhs.set_bool(true);
                    } else if (c == 'f') {
                        rhs.set_bool(false);
                    }
                } else {
                    throw json_exception{"valore " + expected_value + " atteso"};
                }
            }
                //PARSING VALORI DOUBLE
            else {
                if(isdigit(c) || c == '-' || c == '+'){
                    double d;
                    if ((lhs >> d)) {
                        rhs.set_number(d);
                    } else {
                        throw json_exception{"deve esserci double"};
                    }
                } else {
                    throw json_exception{"carattere non valido"};
                }
            }
        }
    } catch (const json_exception& e) {
        std::cerr << "Error parsing JSON: " << e.msg << std::endl;
    }

    return lhs;
}


