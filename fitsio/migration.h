/*
 * =====================================================================================
 *
 *       Filename:  migration.h
 *
 *    Description:  Easy migration of c-extensions from python2 to python3.
 *    Should also works from 3 to 2.
 *
 *        Version:  1.0
 *       Compiler:  gcc
 *
 *         Author:  Morgan Fouesneau, 
 * =====================================================================================
 */
#include <Python.h>
#include <math.h>

#include <stddef.h> /* For offsetof */
#ifndef offsetof
  #define offsetof(type, member) ( (size_t) & ((type*)0) -> member )
#endif

#ifndef Py_MIGRATION

  #if PY_VERSION_HEX < 0x02050000
    typedef int Py_ssize_t;
    #define PY_SSIZE_T_MAX INT_MAX
    #define PY_SSIZE_T_MIN INT_MIN
    #define PY_FORMAT_SIZE_T ""
    #define PyInt_FromSsize_t(z) PyInt_FromLong(z)
    #define PyInt_AsSsize_t(o)   __Pyx_PyInt_AsInt(o)
    #define PyNumber_Index(o)    PyNumber_Int(o)
    #define PyIndex_Check(o)     PyNumber_Check(o)
    #define PyErr_WarnEx(category, message, stacklevel) PyErr_Warn(category, message)
  #endif
  
  #if PY_VERSION_HEX < 0x02060000
    #define Py_REFCNT(ob) (((PyObject*)(ob))->ob_refcnt)
    #define Py_TYPE(ob)   (((PyObject*)(ob))->ob_type)
    #define Py_SIZE(ob)   (((PyVarObject*)(ob))->ob_size)
    #define PyVarObject_HEAD_INIT(type, size) \
            PyObject_HEAD_INIT(type) size,
    #define PyType_Modified(t)
  
    typedef struct {
       void *buf;
       PyObject *obj;
       Py_ssize_t len;
       Py_ssize_t itemsize;
       int readonly;
       int ndim;
       char *format;
       Py_ssize_t *shape;
       Py_ssize_t *strides;
       Py_ssize_t *suboffsets;
       void *internal;
    } Py_buffer;
  
    #define PyBUF_SIMPLE 0
    #define PyBUF_WRITABLE 0x0001
    #define PyBUF_FORMAT 0x0004
    #define PyBUF_ND 0x0008
    #define PyBUF_STRIDES (0x0010 | PyBUF_ND)
    #define PyBUF_C_CONTIGUOUS (0x0020 | PyBUF_STRIDES)
    #define PyBUF_F_CONTIGUOUS (0x0040 | PyBUF_STRIDES)
    #define PyBUF_ANY_CONTIGUOUS (0x0080 | PyBUF_STRIDES)
    #define PyBUF_INDIRECT (0x0100 | PyBUF_STRIDES)
    #define PyBUF_RECORDS (PyBUF_STRIDES | PyBUF_FORMAT | PyBUF_WRITABLE)
    #define PyBUF_FULL (PyBUF_INDIRECT | PyBUF_FORMAT | PyBUF_WRITABLE)
  
    typedef int (*getbufferproc)(PyObject *, Py_buffer *, int);
    typedef void (*releasebufferproc)(PyObject *, Py_buffer *);
  #endif
  
  #if PY_MAJOR_VERSION < 3 && PY_MINOR_VERSION < 6
    #define PyUnicode_FromString(s) PyUnicode_Decode(s, strlen(s), "UTF-8", "strict")
  #endif
  
  #if PY_VERSION_HEX < 0x03020000
    typedef long Py_hash_t;
  #endif
  
  #if PY_MAJOR_VERSION >= 3
    #define Py_TPFLAGS_CHECKTYPES 0
    #define Py_TPFLAGS_HAVE_INDEX 0
    #define Py_TPFLAGS_HAVE_NEWBUFFER 0
  
    #define PyIntObject                  PyLongObject
    #define PyInt_AS_LONG                PyLong_AS_LONG
    #define PyInt_AsLong                 PyLong_AsLong
    #define PyInt_AsLongLong             PyLong_AsLongLong
    #define PyInt_AsSsize_t              PyLong_AsSsize_t
    #define PyInt_AsUnsignedLong         PyLong_AsUnsignedLong
    #define PyInt_AsUnsignedLongLong     PyLong_AsUnsignedLongLong
    #define PyInt_AsUnsignedLongLongMask PyLong_AsUnsignedLongLongMask
    #define PyInt_AsUnsignedLongMask     PyLong_AsUnsignedLongMask
    #define PyInt_Check(op)              PyLong_Check(op)
    #define PyInt_CheckExact(op)         PyLong_CheckExact(op)
    #define PyInt_FromLong               PyLong_FromLong
    #define PyInt_FromSize_t             PyLong_FromSize_t
    #define PyInt_FromSsize_t            PyLong_FromSsize_t
    #define PyInt_FromString             PyLong_FromString
    #define PyInt_FromUnicode            PyLong_FromUnicode
    #define PyInt_FromUnsignedLong       PyLong_FromUnsignedLong
    #define PyInt_FromUnsignedLongLlong  PyLong_FromUnsignedLongLong
    #define PyInt_Type                   PyLong_Type
    
    #define PyBaseString_Type            PyUnicode_Type
    #define PyString                     PyUnicode
    #define PyStringN(c, n)              PyUnicode_FromStringAndSize(c, n)
    #define PyStringObject               PyUnicodeObject
    #define PyStringZ(c)                 PyUnicode_FromString(c)
    #define PyString_AsString            PyUnicode_AsUnicode
    #define PyString_Check               PyUnicode_Check
    #define PyString_CheckExact          PyUnicode_CheckExact
    #define PyString_Format              PyUnicode_Format
    #define PyString_FromFormat          PyUnicode_FromFormat
    #define PyString_FromString          PyUnicode_FromString
    #define PyString_Size                PyUnicode_GET_SIZE
    #define PyString_Type                PyUnicode_Type
    
    #define PyBoolObject                 PyLongObject
    #define PyMethod_New(func, self, klass) ((self) ? PyMethod_New(func, self) : PyInstanceMethod_New(func))
  #endif
  
  #if PY_VERSION_HEX < 0x02060000
    #define Py_TPFLAGS_HAVE_NEWBUFFER 0
  
    #define PyBytesObject                PyStringObject
    #define PyBytes_Type                 PyString_Type
    #define PyBytes_Check                PyString_Check
    #define PyBytes_CheckExact           PyString_CheckExact
    #define PyBytes_FromString           PyString_FromString
    #define PyBytes_FromStringAndSize    PyString_FromStringAndSize
    #define PyBytes_FromFormat           PyString_FromFormat
    #define PyBytes_DecodeEscape         PyString_DecodeEscape
    #define PyBytes_AsString             PyString_AsString
    #define PyBytes_AsStringAndSize      PyString_AsStringAndSize
    #define PyBytes_Size                 PyString_Size
    #define PyBytes_AS_STRING            PyString_AS_STRING
    #define PyBytes_GET_SIZE             PyString_GET_SIZE
    #define PyBytes_Repr                 PyString_Repr
    #define PyBytes_Concat               PyString_Concat
    #define PyBytes_ConcatAndDel         PyString_ConcatAndDel
  
    #define PySet_Check(obj)             PyObject_TypeCheck(obj, &PySet_Type)
    #define PyFrozenSet_Check(obj)       PyObject_TypeCheck(obj, &PyFrozenSet_Type)
  #endif
  
  #ifndef PySet_CheckExact
    #define PySet_CheckExact(obj)        (Py_TYPE(obj) == &PySet_Type)
  #endif

  #define Py_MIGRATION
#endif /*  Py_MIGRATION */
