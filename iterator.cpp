//
// Created by valentina vardanega on 09/10/23.
//

#include "json.cpp"

struct json::list_iterator {
public:

    using iterator_category = std::forward_iterator_tag;

    list_iterator(list p) : ptr(p) {}

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
        auto old_it {ptr};
        ++(*this);
        return old_it;
    }

    bool operator==(list_iterator const& rhs) const{ return ptr == rhs.ptr; }

    bool operator!=(list_iterator const& rhs) const{ return !(*this == rhs);}

    operator bool() const{	return ptr != nullptr;}

private:
    list ptr;
};

struct json::const_list_iterator{
public:

    using iterator_category = std::forward_iterator_tag;

    const_list_iterator(list p) : ptr(p) {}
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

    bool operator==(const_list_iterator const&) const{ return ptr == rhs.ptr; }
    bool operator!=(const_list_iterator const&) const {	return !(*this == rhs);}
    operator bool() const {	return ptr != nullptr;}

private:
    list ptr;
};

struct json::dictionary_iterator{
public:

    using iterator_category = std::forward_iterator_tag;

    dictionary_iterator(Dict p ): ptr(p) {}
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

    bool operator==(dictionary_iterator const&) const { return ptr == rhs.ptr; }
    bool operator!=(dictionary_iterator const&) const{	return !(*this == rhs);}
    operator bool() const; {	return ptr != nullptr;}
private:

    Dict ptr;
};

struct json::const_dictionary_iterator{
public:

    using iterator_category = std::forward_iterator_tag;

    const_dictionary_iterator(Dict p ): ptr(p) {}

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

    bool operator==(const_dictionary_iterator const&) const { return ptr == rhs.ptr; }
    bool operator!=(const_dictionary_iterator const&) const{	return !(*this == rhs);}
    operator bool() const; {	return ptr != nullptr;}
private:

    Dict ptr;
};