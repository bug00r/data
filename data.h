#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>


struct data_vtbl;
typedef struct {
	struct data_vtbl const 	*vptr;
	void 					*data;
	size_t 					size; 
} data_t;

#if 0
/**
	Data structure:
	- delete: Delete function for data Object. Need to delete complex data structure with pointer so other data. If NULL 
			  a dummy method will call onyl free(data).
	- copy: Copy function for data Object. Need to copy complex data structure with pointer so other data. If NULL 
			a dummy method will return use memcpy.
	- equals: Equals Function to check if data are the same. As default it only checks against NULL and data adress.
	- real_size: Function for calculating complete Size of complex data structures. If NULL a dummy method will
				 return the value of sizeof.
	- data: Pointer to working data
	- size: Only the size for the root object. Not including size of other pointed memory inside.
*/
#endif

struct data_vtbl {
	void 	(*delete)	(data_t *data);
	data_t *(*copy)		(data_t *data);
	bool	(*equals)	(data_t *data, data_t *data2);
	size_t 	(*real_size)(data_t *data);
	void 	(*print)	(data_t *data);
};

#if 0
/**
	Data ctor only sets data.
*/
#endif
void data_ctor(data_t * _data);
void data_ctor_stack(data_t * _data);
#if 0
/**
	Data ctor used for Heap allocation. Does not works with stack variables. Because of the use 
	standard free method. This method takes ownership of allocated memory and sets calling pointer to 
	NULL.
*/
#endif 
data_t * data_new(void ** data, size_t size);

#if 0
/**
	Data ctor used for Stack allocation. internally skipping free method. Hint: After changing
	local variable the value of data segment was changed too.
*/
#endif 
data_t * data_new_stack(void * data, size_t size);
#if 0
/**
	Data ctor for completly empty data segment. data will be NULL and size 0.
*/
#endif 
data_t * data_new_empty();
data_t * data_new_stack_empty();

void data_free(data_t * data);

#if 0
/**
	Prints data informations
*/
#endif
void data_print(data_t *data);

#if 0
/**
	Makes deep copy of data with new Data Object. Heap allocated memory becomes new heap object.
	A local object becomes a heap object.
*/
#endif
data_t * data_copy(data_t * const data);


#if 0
/**
	Makes deep copy of src into target. Both will have the same data
*/
#endif
void data_copy_dest(data_t * dest, data_t * src);

#if 0
/**
	Compares two data objects.
*/
#endif
bool data_equals(data_t * const _data, data_t * const _data2);

#if 0
/**
*	Checks if Data is empty
*/
#endif
bool data_is_empty(data_t * const data);

#if 0
/**
	Only removes working data. called delete function and set data ptr to NULL and size to 0.
*/
#endif
void data_clear(data_t * data);


#if 0
/**
	Makes copies the data references from src to target. src will then be empty. There is no call for
	delete data. Only adresses will be copied.
*/
#endif
void data_move(data_t * dest, data_t * src);

#if 0
/**
	Sets new Data to an empty data object. Does nothing if data was not empty. Will transform empty
	objects into heap or stack if it was opposite one.
*/
#endif
bool data_set(data_t * container, void ** data, size_t size);
bool data_set_stack(data_t * container, void * data, size_t size);

#if 0
/**
	Overrides data. Simplifying the call of delete and set. Removes all data in container and set new one.
	without stack the data takes ownership of data *_stack does nothing.
	
	Hint: you must get data before free data container. Otherwise the data point get lost.
*/
#endif
bool data_override(data_t * container, void ** data, size_t size);
bool data_override_stack(data_t * container, void * data, size_t size);

#endif
