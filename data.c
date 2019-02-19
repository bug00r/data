#include "data.h"

static void _data_free_(data_t * data) {
	free(data->data);
	data->data = NULL;
}

static size_t _data_real_size_(data_t * data) {
	size_t size = sizeof(data);
	if (data)
		size += data->size;
	return size;
}

static void _data_print_(data_t * data) {
	printf("data:\nAt:\t%p\nSize:\t%i\n", &data->data, data->size);
}
#if 0
/**
	Forward declarations
*/
#endif
static data_t *	_data_copy_(data_t * _data);
static data_t *	_data_copy_stack_(data_t * _data);
static bool		_data_equals_(data_t * _data, data_t * _data2);

void data_ctor(data_t * _data) {
	data_t * data = _data; 
	static struct data_vtbl const vtbl = {
		&_data_free_,
		&_data_copy_,
		&_data_equals_,
		&_data_real_size_,
		&_data_print_
	};
	data->vptr = &vtbl;
	data->data = NULL;
	data->size = 0;
}

data_t * data_new_empty() {
	data_t * newdata = malloc(sizeof(data_t));
	data_ctor(newdata);
	return newdata;
}

data_t * data_new(void ** data, size_t size) {
	data_t * newdata = data_new_empty();
	newdata->data = *data;
	*data = NULL;
	newdata->size = size;
	return newdata;
}

void data_ctor_stack(data_t * _data) {
	data_t * data = _data; 
	static struct data_vtbl const vtbl_stack = {
		NULL,
		&_data_copy_stack_,
		&_data_equals_,
		&_data_real_size_,
		&_data_print_
	};
	data->vptr = &vtbl_stack;
	data->data = NULL;
	data->size = 0;
}

data_t * data_new_stack_empty() {
	data_t * newdata = malloc(sizeof(data_t));
	data_ctor_stack(newdata);
	return newdata;
}

data_t * data_new_stack(void * data, size_t size) {
	data_t * newdata = data_new_stack_empty();
	newdata->data = data;
	newdata->size = size;
	return newdata;
}

static void * copy_data_raw(data_t * _data) {
	data_t * data = _data;
	void * cpydata = malloc(data->size);
	memcpy(cpydata, data->data, data->size);
	return cpydata;
}

static data_t * _data_copy_(data_t * _data) {
	data_t * data = _data;
	void * cpydata = copy_data_raw(data);
	data_t * copy = data_new(&cpydata, data->size);
	return copy;
}

static data_t * _data_copy_stack_(data_t * _data) {
	data_t * data = _data;
	data_t * copy = data_new_stack(data->data, data->size);
	return copy;
}

data_t * data_copy(data_t * const _data) {
	data_t * data = _data;
	data_t * copy = NULL;
	if (data && data->data && data->vptr->copy)
		copy = data->vptr->copy(data);
	else
		copy = data;
	return copy;
}

void data_copy_dest(data_t * dest, data_t * src) {
	if (dest != NULL && src != NULL) {
		dest->size	= src->size;
		dest->data = NULL;
		if ( dest->data != NULL ) {
			dest->data 	= copy_data_raw(src);
		}
	}
}

static bool _data_equals_(data_t * const _data, data_t * const _data2) {
	data_t * const data  = _data;
	data_t * const data2 = _data2;
	bool equals = ((data != NULL) && (data2 != NULL)) && 
				  ((data->data == data2->data) && (data->size == data2->size));
	return equals;
}

bool data_equals(data_t * const _data, data_t * const _data2) {
	bool equals = false;
	if (_data && _data->vptr->equals)
		equals = _data->vptr->equals(_data, _data2);
	return equals;
}

bool data_is_empty(data_t * const _data) {
	data_t * const data  = _data;
	return (data == NULL) || ( data->data == NULL && data->size == 0 );
}

void data_clear(data_t * data) {
	if (data && data->vptr->delete) {
		data->vptr->delete(data);
	}
	data->data = NULL;
	data->size = 0;
}

void data_move(data_t * dest, data_t * src) {
	if (dest) {
		dest->data = src->data;
		dest->size = src->size;
		dest->vptr = src->vptr;
		src->data = NULL;
		src->size = 0;
	}
}

bool data_set(data_t * container, void ** data, size_t size) {
	bool was_set = false;
	if ( container && data_is_empty(container) ) {
		data_ctor(container);
		container->data = *data;
		*data = NULL;
		container->size = size;
		was_set = true;
	}
	return was_set;
}
bool data_set_stack(data_t * container, void * data, size_t size) {
	bool was_set = false;
	if ( container && data_is_empty(container)) {
		data_ctor_stack(container);
		container->data = data;
		container->size = size;
		was_set = true;
	}
	return was_set;
}

bool data_override(data_t * container, void ** data, size_t size) {
	bool was_over = false;
	if (container && container->vptr->delete) {
		data_clear(container);
		was_over = data_set(container, data, size);
	}
	return was_over;
}

bool data_override_stack(data_t * container, void * data, size_t size) {
	bool was_over = false;
	if (container && !container->vptr->delete ) {
		data_clear(container);
		was_over = data_set_stack(container, data, size);
		//container->data = data;
		//container->size = size;
		//was_over = true;
	}
	return was_over;
}

void data_free(data_t * data) {
	if(!data_is_empty(data))
		data_clear(data);
	free(data);
}

void data_print(data_t * _data) {
	if (_data && _data->vptr->print)
		_data->vptr->print(_data);
}

