#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include"Syntax.h"


namespace Callback{
    template <typename FuncType>
    bool check_track(void (FuncType::* func)(void) ){

        vector<Token>::iterator aux = Syntax::getInstance()->getTb_Token();
        try{
            // (FuncType*func)();
            // (FuncType::func*)();
        }catch(runtime_error &e){
            Syntax::getInstance()->setTb_Token(aux);
            return false;
        }
        return true;
    }
}

#endif
