//
// Created by mao on 2016/12/2.
//

#include <iostream>
#include <cstdarg>
#include "MaoLog.h"

MaoLog::MaoLog(const char *tag) {
    m_tag = new string(tag);
}

MaoLog::~MaoLog() {
    if (m_tag != NULL) {
        delete (m_tag);
    }
}

void MaoLog::d(const char *format, ...) {
    printf("%s: ", m_tag->c_str());
    va_list vp;
    va_start(vp, format);
    vprintf(format, vp);
    printf("\n");
}

void MaoLog::i(const char *format, ...) {
    printf("%s: ", m_tag->c_str());
    va_list vp;
    va_start(vp, format);
    vprintf(format, vp);
    printf("\n");
}

void MaoLog::w(const char *format, ...) {
    printf("%s: ", m_tag->c_str());
    va_list vp;
    va_start(vp, format);
    vprintf(format, vp);
    printf("\n");
}

void MaoLog::e(const char *format, ...) {
    printf("%s: ", m_tag->c_str());
    va_list vp;
    va_start(vp, format);
    vprintf(format, vp);
    printf("\n");
}

MaoLog *MaoLog::getLogger(const char *tag) {
    return new MaoLog(tag);
}