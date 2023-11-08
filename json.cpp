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

json const& json::operator[](std::string const& key) const {
    if (!is_dictionary()) {
        throw json_exception("L'oggetto non è un dizionario.");
    }

    // Cerca la chiave nell'oggetto JSON.
    for (const auto& kv : dictionaryValue) {
        if (kv.first == key) {
            return kv.second;
        }
    }
    // Se la chiave non esiste, restituisci un JSON nullo.
    return json();
}

json& json::operator[](std::string const& key) {
    if (!is_dictionary()) {
        throw json_exception("L'oggetto non è un dizionario.");
    }
    // Cerca la chiave nell'oggetto JSON.
    Dict current = pimpl->dictionaryValue;
    while (current != nullptr) {
        if (current->info.first == key) {
            return current->info.second;
        }
        current = current->next;
    }
    // Se la chiave non esiste, crea una nuova coppia chiave-valore e restituisci il valore.
    Dict newEntry = new Dictionary();
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

// Funzioni begin e end per gli iteratori IN QUESTE CI SONO FUNZIONI BEGIN() E END() CHE HO IMPLEMENTATO NELLE STRUCT LIST E DICT

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

// CI VANNO GETTER E SETTER
void json::push_front(const json &) {
    if (!is_list()) {
        throw json_exception("Impossibile aggiungere elemento in testa a un JSON non di tipo lista.");
    }
    // Crea un nuovo nodo List per il valore da aggiungere.
    list newNode = new List();
    newNode->info = x;
    // Aggiungi il nuovo nodo in testa alla lista.
    newNode->next = pimpl->listValue;
    pimpl->listValue = newNode;
}

void json::push_back(json const& x) {
    // Verifica se il tipo dell'oggetto è una lista.
    if (!is_list()) {
        throw json_exception("Impossibile aggiungere elemento in fondo a un JSON non di tipo lista.");
    }
    // Crea un nuovo nodo List per il valore da aggiungere.
    list newNode = new List();
    newNode->info = x;
    newNode->next = nullptr;
    // Se la lista è vuota, il nuovo nodo diventa l'unico elemento della lista.
    if (pimpl->listValue == nullptr) {
        pimpl->listValue = newNode;
    } else {
        // Trova l'ultimo elemento attuale e collega il nuovo nodo.
        list current = pimpl->listValue;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void json::insert(const std::pair<std::string, json>& x) {
    // Verifica se il tipo dell'oggetto è un dizionario.
    if (!is_dictionary()) {
        throw json_exception("Impossibile aggiungere una coppia chiave-valore in un JSON non di tipo dizionario.");
    }
    // Crea un nuovo nodo Dictionary per la coppia chiave-valore da aggiungere.
    Dict newNode = new Dictionary();
    newNode->info = x;
    newNode->next = nullptr;
    // Se il dizionario è vuoto, il nuovo nodo diventa l'unico elemento del dizionario.
    if (pimpl->dictionaryValue == nullptr) {
        pimpl->dictionaryValue = newNode;
    } else {
        // Trova l'ultimo elemento attuale e collega il nuovo nodo.
        Dict current = pimpl->dictionaryValue;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

//----------PARSING------------

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
                value += '"';
            } else if (next == '\\') {
                value += '\\';
            } else {
                throw json_exception("Errore nella stringa JSON.");
            }
        } else {
            value += ch;
        }
    }
    return value;
}

json parseNumber(std::istream& is) {
    double number;
    is >> number;
    return json(number);
}

json parseBoolean(std::istream& is) {
    string word;
    is >> word;
    if (word == "true") {
        return json(true);
    } else if (word == "false") {
        return json(false);
    } else {
        throw json_exception("Errore nel valore booleano JSON.");
    }
}

json parseNull(istream& is) {
    string word;
    is >> word;
    if (word != "null") {
        throw json_exception("Errore nel valore null JSON.");
    }
    return json();
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
            throw json_exception("Errore nella lista JSON.");
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
            throw json_exception("Errore nell'oggetto JSON.");
        }
    }
    return object;
}

json parseValue(std::istream& is) {
    char ch;
    is >> ch;

    if (ch == '"') {
        return new json()->set_string(parseString(is)); // ritorna una string non un json DA SISTEMARE
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
        throw json_exception("Errore durante il parsing JSON.");
    }
}

ostream& operator<<(ostream& lhs,  json const& rhs) {
    switch (rhs.type()) { //avrebbe più senso usare gli if is_null is_bool etc etc
        case Type::Null:
            lhs << "null";
            break;
        case Type::Number:
            lhs << rhs.get_number();
            break;
        case Type::Boolean:
            lhs << (rhs.get_bool() ? "true" : "false");
            break;
        case Type::String:
            lhs << '"' << escapeString(rhs.get_string()) << '"';
            break;
        case Type::List:
            lhs << "[";
            for (size_t i = 0; i < rhs.size(); ++i) {
                if (i > 0) lhs << ",";
                lhs << rhs[i];
            }
            lhs << "]";
            break;
        case Type::Dictionary:
            lhs << "{";
            bool first = true;
            for (const auto& kv : rhs.get_dictionary()) {
                if (!first) lhs << ",";
                lhs << '"' << escapeString(kv.first) << "\":" << kv.second;
                first = false;
            }
            lhs << "}";
            break;
    }
    return lhs;
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

istream& operator>>(istream& lhs, json& rhs) {
    json parsedJson;

    try {
        char ch;
        lhs >> ch;  // Leggi il primo carattere.

        if (ch == '[') {
            parsedJson = parseList(lhs);
        } else if (ch == '{') {
            parsedJson = parseObject(lhs);
        } else {
            throw json_exception("Errore nel parsing JSON: Il documento JSON deve iniziare con '[' o '{'.");
        }
        // Sovrascrivi l'oggetto rhs con il JSON analizzato.
        rhs = parsedJson;
    } catch (const json_exception& ex) {
        // Se si verifica un errore di parsing, lancia un'eccezione json_exception con un messaggio.
        throw json_exception("Errore durante il parsing JSON: " );
    }

    return lhs;
}

