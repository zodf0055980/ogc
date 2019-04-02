#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "gc.h"

#define IS_DEL(c, d) (c == d)

static int count_words(char *s, char delimiter)
{
    int i = -1, words = 0;
    while (s[++i])
        if (((i != 0 && IS_DEL(s[i], delimiter) &&
              !IS_DEL(s[i - 1], delimiter)) ||
             (s[i + 1] == '\0' && !IS_DEL(s[i], delimiter))))
            words++;
    return words;
}

static char *get_word(char *s, char delimiter)
{
    int i = -1, size = 0;
    while (s[++i] && !IS_DEL(s[i], delimiter))
        size++;
    char *word = gc_alloc(sizeof(char) * (size + 1));
    word[size] = '\0';
    while (*s && !IS_DEL(*s, delimiter)) {
        *word++ = *s;
        s++;
    }
    return word - size;
}

char **my_strsep(char *s, char delimiter)
{
    if (!s)
        return NULL;

    char **tab;
    int size = count_words(s, delimiter);
    if (!(tab = gc_alloc((size + 1) * sizeof(char *))))
        return NULL;

    tab[size] = 0;
    char *str = s;
    while (*str) {
        if (!IS_DEL(*str, delimiter)) {
            *tab++ = get_word(str, delimiter);
            str += strlen(get_word(str, delimiter)) - 1;
        }
        str++;
    }
    return tab - size;
}


char *my_strdup(char *s)
{
    if(!s)
        return NULL;
    char *c =s;
    int size = 0;
    while(*c) {
        size++;
        c++;
    }
    char *word = gc_alloc(sizeof(char)*(size+1));
    int i;
    for(i=0 ;i < size ;i++){
        *(word+i) = *(s+i);
    }
    *(word+size) = '\0';
    
    return word;
}

int main(int argc, char *argv[])
{
    gc_init(&argc, 1);

    char **sep = my_strsep("hello,world", ',');
    assert(sep);

    char *dup1 = my_strdup(sep[0]);
    char *dup2 = my_strdup(sep[1]);
    assert(dup1);
    assert(dup2);
    printf("%s\n", sep[0]);
    printf("%s\n", sep[1]);

    printf("d1 =%s\n", dup1);
    printf("d2 =%s\n", dup2);
    gc_run();

    gc_dump_internals();
    gc_destroy();

    return 0;
}
