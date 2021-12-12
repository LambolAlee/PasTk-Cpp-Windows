#ifndef SINGLETON_H
#define SINGLETON_H
#include <QMutex>
#include <QScopedPointer>

#define SINGLETON(CLS) \
    Q_DISABLE_COPY(CLS) \
    public: \
        static CLS &instance() \
        { \
            static QMutex mutex; \
            static QScopedPointer<CLS> inst; \
            if (Q_UNLIKELY(!inst)) { \
                mutex.lock(); \
                inst.reset(new CLS); \
                mutex.unlock(); \
            } \
            return *inst.data(); \
        } \

#endif // SINGLETON_H
