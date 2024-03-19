//
// Created by Jorge Botarro on 19-03-24.
//

#ifndef CHK_PIX_BINDABLE_HPP
#define CHK_PIX_BINDABLE_HPP

#include "core.hpp"

class IBindable {
public:
    virtual b32 bind();
    virtual b32 unbind();

protected:
    b32 _bound{false};
};

#endif //CHK_PIX_BINDABLE_HPP
