#include "data.h"

static void _data_free_(Data * data) {
	free(data->data);
	data->data = NULL;
}

static size_t _data_real_size_(Data * data) {
	size_t size = sizeof(data);
	if (data)
		size += data->size;
	return size;
}

static void _data_print_(Data * data) {
	printf("data:\nAt:\t%p\nSize:\t%lli\n", data->data, data->size);
}
#if 0
/**
	Forward declarations
*/
#endif
static Data *	_data_copy_(Data * _data);
static Data *	_data_copy_stack_(Data * _data);
static bool		_data_equals_(Data * _data, Data * _data2);

void data_create_vtable(  data_vtbl_t* vt,
			DATA_DELETE_FUNC df,
			DATA_COPY_FUNC cf,
			DATA_EQUALS_FUNC ef,
			DATA_REAL_SIZE_FUNC rsf,
			DATA_PRINT_FUNC pf
) {
	vt->delete = ( df == NULL ? _data_free_ : df );
	vt->copy = ( cf == NULL ? _data_copy_ : cf );
	vt->equals = ( ef == NULL ? _data_equals_ : ef );
	vt->real_size = ( rsf == NULL ? _data_real_size_ : rsf );
	vt->print = ( pf == NULL ? _data_print_ : pf );
}

static void __data_ctor_raw(Data * _data, data_vtbl_t * const vtable) {
	Data * data = _data; 
	data->vptr = vtable;
	data->data = NULL;
	data->size = 0;
}

void data_ctor(Data * _data) {
	Data * data = _data; 
	static struct data_vtbl const vtbl = {
		&_data_free_,
		&_data_copy_,
		&_data_equals_,
		&_data_real_size_,
		&_data_print_
	};
	__data_ctor_raw(data, (data_vtbl_t * const)&vtbl);
}

void data_ctor_vtable(Data * _data, data_vtbl_t * const vtable) {
	__data_ctor_raw(_data, vtable);
}

Data * data_new_empty() {
	Data * newdata = malloc(sizeof(Data));
	data_ctor(newdata);
	return newdata;
}

Data * data_new_empty_vtable(data_vtbl_t * const vtable) {
	Data * newdata = malloc(sizeof(Data));
	data_ctor_vtable(newdata, vtable);
	return newdata;
}

Data * data_new(void ** data, size_t size) {
	Data * newdata = data_new_empty();
	newdata->data = *data;
	*data = NULL;
	newdata->size = size;
	return newdata;
}

Data * data_new_vtable(void ** data, size_t size, data_vtbl_t * const vtable) {
	Data * newdata = data_new_empty_vtable(vtable);
	newdata->data = *data;
	*data = NULL;
	newdata->size = size;
	return newdata;
}

void data_ctor_stack(Data * _data) {
	Data * data = _data; 
	static struct data_vtbl const vtbl_stack = {
		NULL,
		&_data_copy_stack_,
		&_data_equals_,
		&_data_real_size_,
		&_data_print_
	};
	__data_ctor_raw(data, (data_vtbl_t * const)&vtbl_stack);
}

void data_ctor_stack_vtable(Data * _data, data_vtbl_t * const vtable) {
	__data_ctor_raw(_data, (data_vtbl_t * const)&vtable);
}

Data * data_new_stack_empty() {
	Data * newdata = malloc(sizeof(Data));
	data_ctor_stack(newdata);
	return newdata;
}

Data * data_new_stack_empty_vtable(data_vtbl_t * const vtable) {
	Data * newdata = malloc(sizeof(Data));
	data_ctor_stack_vtable(newdata, vtable);
	return newdata;
}

Data * data_new_stack(void * data, size_t size) {
	Data * newdata = data_new_stack_empty();
	newdata->data = data;
	newdata->size = size;
	return newdata;
}

Data * data_new_stack_vtable(void *data, size_t size, data_vtbl_t * const vtable) {
	Data * newdata = data_new_stack_empty_vtable(vtable);
	newdata->data = data;
	newdata->size = size;
	return newdata;
}

static void * copy_data_raw(Data * _data) {
	Data * data = _data;
	void * cpydata = malloc(data->size);
	memcpy(cpydata, data->data, data->size);
	return cpydata;
}

static Data * _data_copy_(Data * _data) {
	Data * data = _data;
	void * cpydata = copy_data_raw(data);
	Data * copy = data_new(&cpydata, data->size);
	return copy;
}

static Data * _data_copy_stack_(Data * _data) {
	Data * data = _data;
	Data * copy = data_new_stack(data->data, data->size);
	return copy;
}

Data * data_copy(Data * const _data) {
	Data * data = _data;
	Data * copy = NULL;
	if (data && data->data && data->vptr->copy)
		copy = data->vptr->copy(data);
	else
		copy = data;
	return copy;
}

void data_copy_dest(Data * dest, Data * src) {
	if (dest != NULL && src != NULL) {
		dest->size	= src->size;
		dest->data = NULL;
		if ( dest->data != NULL ) {
			dest->data 	= copy_data_raw(src);
		}
	}
}

static bool _data_equals_(Data * const _data, Data * const _data2) {
	Data * const data  = _data;
	Data * const data2 = _data2;
	bool equals = ((data != NULL) && (data2 != NULL)) && 
				  ((data->data == data2->data) && (data->size == data2->size));
	return equals;
}

bool data_equals(Data * const _data, Data * const _data2) {
	bool equals = false;
	if (_data && _data->vptr->equals)
		equals = _data->vptr->equals(_data, _data2);
	return equals;
}

bool data_is_empty(Data * const _data) {
	Data * const data  = _data;
	return (data == NULL) || ( data->data == NULL && data->size == 0 );
}

void data_clear(Data * data) {
	if (data && data->vptr->delete) {
		data->vptr->delete(data);
	}
	data->data = NULL;
	data->size = 0;
}

void data_move(Data * dest, Data * src) {
	if (dest) {
		dest->data = src->data;
		dest->size = src->size;
		dest->vptr = src->vptr;
		src->data = NULL;
		src->size = 0;
	}
}

bool data_set(Data * container, void ** data, size_t size) {
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
bool data_set_stack(Data * container, void * data, size_t size) {
	bool was_set = false;
	if ( container && data_is_empty(container)) {
		data_ctor_stack(container);
		container->data = data;
		container->size = size;
		was_set = true;
	}
	return was_set;
}

bool data_override(Data * container, void ** data, size_t size) {
	bool was_over = false;
	if (container && container->vptr->delete) {
		data_clear(container);
		was_over = data_set(container, data, size);
	}
	return was_over;
}

bool data_override_stack(Data * container, void * data, size_t size) {
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

void data_free(Data * data) {
	if(!data_is_empty(data))
		data_clear(data);
	free(data);
}

void data_print(Data * _data) {
	if (_data && _data->vptr->print)
		_data->vptr->print(_data);
}

