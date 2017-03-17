//
// Created by mao on 2016/12/2.
//

#ifndef MAO_SERVER_WIN_MAOLOG_H
#define MAO_SERVER_WIN_MAOLOG_H

#include <string>

using namespace std;

class MaoLog {
private:
    const string *m_tag;

    MaoLog(const char *tag);

public:

    ~MaoLog();

    void d(const char *format, ...);

    void i(const char *format, ...);

    void w(const char *format, ...);

    void e(const char *format, ...);

    static MaoLog *getLogger(const char *tag);
};


#endif //MAO_SERVER_WIN_MAOLOG_H
