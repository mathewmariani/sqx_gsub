/**
 * Copyright (c) 2017 Mathew Mariani
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `sqx_require.cc` for details.
 */

#include <squirrel.h>
#include <sqxstring.h>
#include <string.h>

// NOTE: I couldn't find a "default" size for scratchpads used in squirrel so I just made it whatever
#define SQX_DEFAULT_BUFFER_SIZE (1024 * sizeof(SQChar))

struct SQStringBuffer {
	SQChar *b;
	SQUnsignedInteger size;
	SQUnsignedInteger n;
};

static void sqx_newstringbuffer(SQVM *v, SQStringBuffer* buffer) {
	buffer->b = sq_getscratchpad(v, SQX_DEFAULT_BUFFER_SIZE);
	buffer->size = SQX_DEFAULT_BUFFER_SIZE;
	buffer->n = 0;
}

static void* resize(SQVM *v, SQStringBuffer *buffer, SQUnsignedInteger newsize) {
	void* ud;
	sq_getuserpointer(v, -1, (SQUserPointer*) &ud);
	void *temp = sq_realloc(ud, buffer->size, newsize);
	if (temp == NULL && newsize > 0) {
		sq_throwerror(v, _SC("buffer reallocation failed"));
	}
	return temp;
}

static char* bufferoffset(SQVM *v, SQStringBuffer *buffer, SQUnsignedInteger size) {
	if (buffer->size - buffer->n < size) {
		char* newbuffer;
		size_t newsize = buffer->n + size;
		if (sq_gettype(v, -1) != OT_USERPOINTER) {
			newbuffer = (char*) resize(v, buffer, newsize);
		} else {
			sq_pushuserpointer(v, buffer->b);
			newbuffer = (char*) resize(v, buffer, newsize);
		}
		buffer->b = newbuffer;
		buffer->size = newsize;
	}
	// return offset
	return &buffer->b[buffer->n];
}

static void concat(SQVM *v, SQStringBuffer *buffer, const SQChar *s, SQUnsignedInteger l) {
	if (l > 0) {
		SQChar *offset = bufferoffset(v, buffer, l);
		memcpy(offset, s, l * sizeof(SQChar));
		buffer->n += l;
	}
}

static int sqx_gsub(SQVM *v) {
	// get arguments
	const char *s, *p, *r;
	sq_getstring(v, -3, &s);
	sq_getstring(v, -2, &p);
	sq_getstring(v, -1, &r);
	// get size of pattern
	size_t l = strlen(p);
	// create string buffer
	SQStringBuffer buffer;
	sqx_newstringbuffer(v, &buffer);
	const char* pch = strstr(s, p);
	while (pch != NULL) {
		concat(v, &buffer, s, pch - s);
		concat(v, &buffer, r, strlen(r));
		s = pch + l;
		pch = strstr(s, p);
	}
	sq_pushstring(v, buffer.b, buffer.n);
	return 1;
}

int sqx_register_gsub(SQVM *v) {
	sq_pushstring(v, "gsub", -1);
	sq_newclosure(v, sqx_gsub, 0);
	sq_setparamscheck(v, 4, _SC(".sss"));
	sq_setnativeclosurename(v, -1, "gsub");
	sq_newslot(v, -3, SQFalse);
	return 1;
}
