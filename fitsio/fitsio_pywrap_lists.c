#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Python.h>
#include <numpy/arrayobject.h> 
#include "fitsio_pywrap_lists.h"

/*
 * Python 2.x and Python 3.x have different ideas of what a basic string
 * and int types are. These blocks help us sort things out if we just want a
 * "plain" integer or string
 */
#if PY_MAJOR_VERSION >= 3
/*
 * PYTHON 3 DEFINITIONS
 */

/* 
 * The PyInt_From* macros get us a 'default' integer type from a long, etc.
 * Implemented (if not a simple macro) in numutil.c
 */
#define PyInt_FromLong PyLong_FromLong
#define PyInt_FromUnsignedLong PyLong_FromUnsignedLong
#define PyInt_FromUnsignedLongLlong PyLong_FromUnsignedLongLong

/* 
 *  The PyInt_As* convert the integer type (long, int) into something we want
 */
#define PyInt_AsUnsignedLongLong PyLong_AsUnsignedLongLong
#define PyInt_AsLongLong PyLong_AsLongLong
#define PyInt_AsUnsignedLong PyLong_AsUnsignedLong
#define PyInt_AsLong PyLong_AsLong

/*
 * The PyString macros generate strings for us. The 'Z' variant takes a
 *    NUL-terminated string, while the 'N' variant accepts a length specifier
 */
#define PyStringZ(c) PyUnicode_FromString(c)
#define PyStringN(c, n) PyUnicode_FromStringAndSize(c, n)
#define PyString PyUnicode
#define PyString_AsString PyUnicode_AsUnicode
#define PyString_Check PyUnicode_Check
#define PyString_FromString PyUnicode_FromString
#define PyString_FromFormat PyUnicode_FromFormat
#define PyString_Size PyUnicode_GET_SIZE
#define PyString_Format PyUnicode_Format
/*
 * Python 3 has a revamped extension module initialization system. (See PEP
 * 3121.) Instead of storing module state in globals, they should be stored
 * in an interpreter specific structure. Creating modules that act correctly
 * in both Python 2 and Python 3 is tricky.
 * Note: the init function is also updated
 */
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))

#else
/*
 * PYTHON 2 DEFINITIONS
 */
#define GETSTATE(m) (&_state)

#endif

struct i64list* i64list_new(void) {
    struct i64list* slist=NULL;

    slist = malloc(sizeof(struct i64list));
    slist->size = 0;
    slist->data=NULL;
    return slist;
}

struct i64list* i64list_delete(struct i64list* list) {
    if (list != NULL) {
        free(list->data);
        free(list);
    }
    return NULL;
}

// push a copy of the string onto the string list
void i64list_push(struct i64list* list, int64_t val) {
    size_t newsize=0;
    size_t i=0;

    newsize = list->size+1;
    list->data = realloc(list->data, sizeof(int64_t)*newsize);
    list->size += 1;

    i = list->size-1;

    list->data[i] = val;
}

void i64list_print(struct i64list* list) {
    size_t i=0;
    if (list == NULL) {
        return;
    }
    for (i=0; i<list->size; i++) {
        printf("  list[%ld]: %ld\n", i, list->data[i]);
    }
}



struct stringlist* stringlist_new(void) {
    struct stringlist* slist=NULL;

    slist = malloc(sizeof(struct stringlist));
    slist->size = 0;
    slist->data=NULL;
    return slist;
}
// push a copy of the string onto the string list
void stringlist_push(struct stringlist* slist, const char* str) {
    size_t newsize=0;
    size_t i=0;

    newsize = slist->size+1;
    slist->data = realloc(slist->data, sizeof(char*)*newsize);
    slist->size += 1;

    i = slist->size-1;

    slist->data[i] = strdup(str);
}

void stringlist_push_size(struct stringlist* slist, size_t slen) {
    size_t newsize=0;
    size_t i=0;

    newsize = slist->size+1;
    slist->data = realloc(slist->data, sizeof(char*)*newsize);
    slist->size += 1;

    i = slist->size-1;

    slist->data[i] = calloc(slen+1,sizeof(char));
    //slist->data[i] = malloc(sizeof(char)*(slen+1));
    //memset(slist->data[i], 0, slen+1);
}
struct stringlist* stringlist_delete(struct stringlist* slist) {
    if (slist != NULL) {
        size_t i=0;
        if (slist->data != NULL) {
            for (i=0; i < slist->size; i++) {
                free(slist->data[i]);
            }
        }
        free(slist->data);
        free(slist);
    }
    return NULL;
}


void stringlist_print(struct stringlist* slist) {
    size_t i=0;
    if (slist == NULL) {
        return;
    }
    for (i=0; i<slist->size; i++) {
        printf("  slist[%ld]: %s\n", i, slist->data[i]);
    }
}


int stringlist_addfrom_listobj(struct stringlist* slist, 
                               PyObject* listObj, 
                               const char* listname) {
    size_t size=0, i=0;

    if (!PyList_Check(listObj)) {
        PyErr_Format(PyExc_ValueError, "Expected a list for %s.", listname);
        return 1;
    }
    size = PyList_Size(listObj);

    for (i=0; i<size; i++) {
        PyObject* tmp = PyList_GetItem(listObj, i);
        const char* tmpstr;
        if (!PyBytes_Check(tmp)) {
            PyErr_Format(PyExc_ValueError, 
                         "Expected only strings in %s list.", listname);
            return 1;
        }
        tmpstr = (const char*) PyBytes_AsString(tmp);
        stringlist_push(slist, tmpstr);
    }
    return 0;
}

