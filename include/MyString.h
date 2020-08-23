#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>
#include <string>
#include <vector>
#include "MyCommon.h"

using std::vector;
using std::string;

namespace my_string {

class MyString {
public:
    static int BruteForceV1(const string& text,
                            const string& pattern) {
        int tlen = text.length();
        int plen = pattern.length();
        if (tlen == 0 || plen == 0 || plen > tlen) return -1;
        int pi = 0, ti = 0;
        while (pi < plen && ti < tlen) {
            if (text[ti] == pattern[pi]) {
                pi++;
                ti++;
            } else {
                pi = 0;
                ti -= pi - 1;
            }
        }

        return pi == plen ? ti - pi : -1;
    }

    //返回所有的位置
    static void BruteForceV2(const string& text,
                             const string& pattern,
                             std::vector<int> &results) {
        int tlen = text.length();
        int plen = pattern.length();
        if (tlen == 0 || plen == 0 || plen > tlen) return;
        int pi = 0, ti = 0;

        while (ti < tlen) {
            while (pi < plen && ti < tlen) {
                if (text[ti] == pattern[pi]) {
                    pi++;
                    ti++;
                } else {
                    pi = 0;
                    ti -= pi - 1;
                }
            }
            if (pi == plen) {
                results.push_back(ti - pi);
                pi = 0;
            }
            if (ti > pi + (tlen - plen)) {
                break;
            }
        }

    }

    static int BruteForceV3(const string& text,
                            const string& pattern) {
        int tlen = text.length();
        int plen = pattern.length();
        if (tlen == 0 || plen == 0 || plen > tlen) return -1;
        int pi = 0, ti = 0;
        while (pi < plen && ti - pi <= (tlen - plen)) {
            if (text[ti] == pattern[pi]) {
                pi++;
                ti++;
            } else {
                pi = 0;
                ti -= pi - 1;
            }
        }

        return pi == plen ? ti - pi : -1;
    }

    static int BruteForceV4(const string& text,
                            const string& pattern) {
        int tlen = text.length();
        int plen = pattern.length();
        if (tlen == 0 || plen == 0 || plen > tlen) return -1;

        int tMax = tlen - plen;
        int pi = 0, ti = 0;
        for(ti = 0; ti <= tMax; ti++) {
            for (pi = 0; pi < plen; pi++) {
                if (text[ti + pi] != pattern[pi]) break;
            }
            if (pi == plen) {
                return ti;
            }
        }
        return -1; //没找到匹配子串
    }


    static int KMP(const string& text,
                   const string& pattern) {
        int tlen = text.length();
        int plen = pattern.length();
        if (tlen == 0 || plen == 0 || plen > tlen) return -1;
        int pi = 0, ti = 0;
        int* next = NextArray(pattern);
        while (pi < plen && ti - pi <= (tlen - plen)) {
            if (pi < 0 || text[ti] == pattern[pi]) {
                pi++;
                ti++;
            } else {
                pi = next[pi];
            }
        }

        delete[] next;

        return pi == plen ? ti - pi : -1;
    }

    static int* NextArray_bak(const string& pattern) {
        int len = pattern.length();
        int* next = new int[len]{-1};
        int i = 0;
        int n = next[i] = -1;
        int iMax = len - 1;
        while (i < iMax) {
            if (n < 0 || pattern[i] == pattern[n]) {
                next[++i] = ++n;
            } else {
                n = next[n];
            }
        }
        return next;
    }

    static int* NextArray(const string& pattern) {
        int len = pattern.length();
        int* next = new int[len]{-1};
        int i = 0;
        int n = next[i] = -1;
        int iMax = len - 1;
        while (i < iMax) {
            if (n < 0 || pattern[i] == pattern[n]) {
                i++;
                n++;
                if (pattern[i] == pattern[n]) {
                    next[i] = next[n];
                } else {
                    next[i] = n;
                }
            } else {
                n = next[n];
            }
        }
        return next;
    }

};

}


#endif // MYSTRING_H
