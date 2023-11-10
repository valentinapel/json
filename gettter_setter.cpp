//
// Created by valentina vardanega on 09/10/23.
//
/*
double &json::get_number() {
    if(! is_number())
        throw json_exception{ " get_number ma json non è tipo number "};
    return pimpl->numberValue;
}

const double &json::get_number() const {
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
}*/