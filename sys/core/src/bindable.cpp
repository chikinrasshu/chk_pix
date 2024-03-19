//
// Created by Jorge Botarro on 19-03-24.
//

#include <core/bindable.hpp>

b32 IBindable::bind() {
    if (_bound) { return false; }
    _bound = true;
    return true;
}

b32 IBindable::unbind() {
    if (!_bound) { return false; }
    _bound = false;
    return true;
}
