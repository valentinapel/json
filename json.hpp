#include <iostream>
#include <string>
#include <limits>
#include <assert.h>
#include <fstream>

struct json_exception {
    std::string msg;
};

class json {
public:
    struct list_iterator; //OK
    struct dictionary_iterator; //OK
    struct const_list_iterator; //OK
    struct const_dictionary_iterator;//OK

    json(); //OK
    json(json const&);//OK
    json(json&&);//OK
    ~json();//OK

    json& operator=(json const&);//OK
    json& operator=(json&&);//OK

    bool is_list() const; //OK
    bool is_dictionary() const; //OK
    bool is_string() const; //OK
    bool is_number() const;// OK
    bool is_bool() const;// OK
    bool is_null() const;// OK

    json const& operator[](std::string const&) const;
    json& operator[](std::string const&);

    list_iterator begin_list(); // da qui
    const_list_iterator begin_list() const;
    list_iterator end_list();
    const_list_iterator end_list() const;

    dictionary_iterator begin_dictionary();
    const_dictionary_iterator begin_dictionary() const;
    dictionary_iterator end_dictionary();
    const_dictionary_iterator end_dictionary() const; // a qui non sono completate

    double& get_number(); //OK
    double const& get_number() const; //OK

    bool& get_bool(); //OK
    bool const& get_bool() const; //OK

    std::string& get_string(); //OK
    std::string const& get_string() const; //OK

    void set_string(std::string const&); //OK
    void set_bool(bool); //OK
    void set_number(double); //OK
    void set_null(); //OK
    void set_list(); //OK
    void set_dictionary(); //OK
    void push_front(json const&);//OK
    void push_back(json const&);//OK
    void insert(std::pair<std::string, json> const&);//OK

private:
    struct impl; // OK PROBABILMENTE MANCANO METODI PER GESTIONE
    impl* pimpl;
};

std::ostream& operator<<(std::ostream& lhs, json const& rhs);
std::istream& operator>>(std::istream& lhs, json& rhs);
