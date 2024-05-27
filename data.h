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
} Data;

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

typedef void 	(*DATA_DELETE_FUNC)		(Data *data);
typedef Data *(*DATA_COPY_FUNC)		(Data *data);
typedef bool	(*DATA_EQUALS_FUNC)		(Data *data, Data *data2);
typedef size_t 	(*DATA_REAL_SIZE_FUNC)	(Data *data);
typedef void 	(*DATA_PRINT_FUNC)		(Data *data);

struct data_vtbl {
	void 	(*delete)	(Data *data);
	Data *(*copy)		(Data *data);
	bool	(*equals)	(Data *data, Data *data2);
	size_t 	(*real_size)(Data *data);
	void 	(*print)	(Data *data);
};

typedef struct data_vtbl data_vtbl_t;


#if 0
/**
 * creation func for vtable
*/
#endif
void data_create_vtable( data_vtbl_t* vt,
			DATA_DELETE_FUNC df,
			DATA_COPY_FUNC cf,
			DATA_EQUALS_FUNC ef,
			DATA_REAL_SIZE_FUNC rsf,
			DATA_PRINT_FUNC pf
);

#if 0
/**
	Data ctor only sets data.
*/
#endif
void data_ctor(Data * _data);
void data_ctor_stack(Data * _data);
void data_ctor_vtable(Data * _data, data_vtbl_t * const vtable);
void data_ctor_stack_vtable(Data * _data, data_vtbl_t * const vtable);


#if 0
/**
	Data ctor used for Heap allocation. Does not works with stack variables. Because of the use 
	standard free method. This method takes ownership of allocated memory and sets calling pointer to 
	NULL.
*/
#endif 
Data * data_new(void ** data, size_t size);
Data * data_new_vtable(void ** data, size_t size, data_vtbl_t * const vtable);

#if 0
/**
	Data ctor used for Stack allocation. internally skipping free method. Hint: After changing
	local variable the value of data segment was changed too.
*/
#endif 
Data * data_new_stack(void * data, size_t size);
Data * data_new_stack_vtable(void *data, size_t size, data_vtbl_t * const vtable);
#if 0
/**
	Data ctor for completly empty data segment. data will be NULL and size 0.
*/
#endif 
Data * data_new_empty();
Data * data_new_stack_empty();
Data * data_new_empty_vtable(data_vtbl_t * const vtable);
Data * data_new_stack_empty_vtable(data_vtbl_t * const vtable);

void data_free(Data * data);

#if 0
/**
	Prints data informations
*/
#endif
void data_print(Data *data);

#if 0
/**
	Makes deep copy of data with new Data Object. Heap allocated memory becomes new heap object.
	A local object becomes a heap object.
*/
#endif
Data * data_copy(Data * const data);


#if 0
/**
	Makes deep copy of src into target. Both will have the same data
*/
#endif
void data_copy_dest(Data * dest, Data * src);

#if 0
/**
	Compares two data objects.
*/
#endif
bool data_equals(Data * const _data, Data * const _data2);

#if 0
/**
*	Checks if Data is empty
*/
#endif
bool data_is_empty(Data * const data);

#if 0
/**
	Only removes working data. called delete function and set data ptr to NULL and size to 0.
*/
#endif
void data_clear(Data * data);


#if 0
/**
	Makes copies the data references from src to target. src will then be empty. There is no call for
	delete data. Only adresses will be copied.
*/
#endif
void data_move(Data * dest, Data * src);

#if 0
/**
	Sets new Data to an empty data object. Does nothing if data was not empty. Will transform empty
	objects into heap or stack if it was opposite one.
*/
#endif
bool data_set(Data * container, void ** data, size_t size);
bool data_set_stack(Data * container, void * data, size_t size);

#if 0
/**
	Overrides data. Simplifying the call of delete and set. Removes all data in container and set new one.
	without stack the data takes ownership of data *_stack does nothing.
	
	Hint: you must get data before free data container. Otherwise the data point get lost.
*/
#endif
bool data_override(Data * container, void ** data, size_t size);
bool data_override_stack(Data * container, void * data, size_t size);

#endif
