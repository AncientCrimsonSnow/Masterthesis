#pragma once

#define BIT(x) (1 << x)
#define GG_BIND_EVENT_FN(fn) Gengine::BindEvent(this, &fn)

#define var auto


namespace Gengine {
    template <typename ReturnType, typename ClassType, typename... ArgTypes>
    var BindEvent(ClassType* instance, ReturnType(ClassType::* func)(ArgTypes...)) {
        return [instance, func](ArgTypes... args) -> ReturnType {
            return (instance->*func)(args...);
            };
    }
}
