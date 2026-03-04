#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct StringView {
    const char *items;
    size_t count;
} StringView;
#define SV_FMT "%.*s"
#define SV_ARG(SV) (int)(SV).count, (SV).items
#define SV(CSTR) (StringView) { .items = (CSTR), .count = sizeof(CSTR) - 1 }

static inline StringView sv_from_cstr(const char *cstr) {
    size_t n = 0;
    while(cstr[n++] != 0);
    return (StringView) {
        .items = cstr,
        .count = n - 1,
    };
}

static inline StringView sv_lstrip(StringView sv) {
    for(size_t i = 0; i < sv.count; ++i) {
        if(!isspace(sv.items[i])) {
            sv.items = sv.items + i;
            sv.count = sv.count - i;
            return sv;
        }
    }
    return sv;
}

static inline StringView sv_rstrip(StringView sv) {
    for(int i = (int)sv.count - 1; i >= 0; --i) {
        if(!isspace(sv.items[i])) {
            sv.count = i + 1;
            return sv;
        }
    }
    return sv;
}
static inline StringView sv_strip(StringView sv) {
    return sv_lstrip(sv_rstrip(sv));
}

static inline bool sv_eq(StringView sv_a, StringView sv_b) {
    if(sv_a.count != sv_b.count) return false;
    for(size_t i = 0; i < sv_a.count; ++i) {
        if(sv_a.items[i] != sv_b.items[i])
            return false;
    }
    return true;
}

static inline StringView sv_chop_until_space(StringView *sv) {
    *sv = sv_strip(*sv);
    size_t i = 0;
    for(; i < sv->count; ++i) {
        if(isspace(sv->items[i])) break;
    }
    StringView result = { .items = sv->items, .count = i };
    sv->items += i;
    sv->count -= i;
    return result;
}

#endif // UTILS_H_
