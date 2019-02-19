#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "data.h"

static void _test_data_alloc_() {
	#ifdef debug 
		printf(">>> _test_data_alloc_:\n");
	#endif
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * data = data_new((void**)&i, size_int);
		
		assert(i == NULL);
		#ifdef debug 
			data_print(data);
			printf("should %i is %i\n",666,*(unsigned int *)data->data);
		#endif
		data_free(data);
	}
	
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int i = 667;
		data_t * data = data_new_stack(&i, size_int);
		i = 669;
		#ifdef debug 
			data_print(data);
			printf("should %i is %i\n",i,*(unsigned int *)data->data);
		#endif
		data_free(data);
	}
	
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * data = data_new_stack(i, size_int);
		
		assert(*i == 666);
		#ifdef debug 
			data_print(data);
			printf("should %i is %i\n",*i,*(unsigned int *)data->data);
		#endif
		data_free(data);
		
		assert(*i == 666);
		free(i);
	}
	
	{
		data_t * data = data_new_empty();
		data_free(data);
	}
	#ifdef debug 
		printf("<<< _test_data_alloc_:\n");
	#endif
}

static void _test_data_copy_() {
	#ifdef debug 
		printf(">>> _test_data_copy_:\n");
	#endif
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * data = data_new((void**)&i, size_int);
		assert(i == NULL);
		
		data_t * copy = data_copy(data);
		
		#ifdef debug	
			printf("src:\n");
			data_print(data);
			printf("copy:\n");
			data_print(copy);
		#endif

		assert(!data_equals(data, copy));
		assert(!data_equals(data, NULL));
		assert(!data_equals(copy, NULL));
		assert(!data_equals(NULL, data));
		assert(!data_equals(NULL, copy));
		assert(!data_equals(NULL, NULL));
		assert(data_equals(data, data));
		assert(data_equals(copy, copy));
		
		data_free(data);
		data_free(copy);
	}
	
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * data = data_new_stack(i, size_int);
		assert(*i == 666);
		
		data_t * copy = data_copy(data);
		
		#ifdef debug	
			printf("src:\n");
			data_print(data);
			printf("src data: %i\n", *(unsigned int *)copy->data);
			printf("copy:\n");
			printf("copy data: %i\n", *(unsigned int *)copy->data);
			data_print(copy);
		#endif

		assert(data_equals(data, copy));
		assert(!data_equals(data, NULL));
		assert(!data_equals(copy, NULL));
		assert(!data_equals(NULL, data));
		assert(!data_equals(NULL, copy));
		assert(!data_equals(NULL, NULL));
		assert(data_equals(data, data));
		assert(data_equals(copy, copy));
		
		void * cpydata = copy->data;
		
		data_free(data);
		data_free(copy);
		assert(*i == 666);
		
		//Here we must free the deep copy value of data
		#ifdef debug
			printf("copy data: %i\n", *((unsigned int *)cpydata));
		#endif
		assert(*((unsigned int *)cpydata) == 666);
		free(cpydata);
	}
	
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int i = 666;
		data_t * data = data_new_stack(&i, size_int);
		data_t * copy = data_copy(data);
		
		#ifdef debug	
			printf("src:\n");
			data_print(data);
			printf("copy:\n");
			data_print(copy);
		#endif

		assert(data_equals(data, copy));
		assert(!data_equals(data, NULL));
		assert(!data_equals(copy, NULL));
		assert(!data_equals(NULL, data));
		assert(!data_equals(NULL, copy));
		assert(!data_equals(NULL, NULL));
		assert(data_equals(data, data));
		assert(data_equals(copy, copy));
		
		data_free(data);
		data_free(copy);
	}
	
	#ifdef debug 
		printf("<<< _test_data_copy_:\n");
	#endif
}

static void _test_data_copy_dest_() {
	#ifdef debug 
		printf(">>> _test_data_copy_dest_:\n");
	#endif
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * data = data_new((void**)&i, size_int);
		assert(i == NULL);
		data_t * copy = data_new_empty();
		data_copy_dest(copy, data);
		
		#ifdef debug	
			printf("src:\n");
			data_print(data);
			printf("copy:\n");
			data_print(copy);
		#endif

		assert(!data_equals(data, copy));
		assert(!data_equals(data, NULL));
		assert(!data_equals(copy, NULL));
		assert(!data_equals(NULL, data));
		assert(!data_equals(NULL, copy));
		assert(!data_equals(NULL, NULL));
		assert(data_equals(data, data));
		assert(data_equals(copy, copy));
		
		data_free(data);
		data_free(copy);
	}
	
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * data = data_new_stack(i, size_int);
		data_t * copy = data_new_stack_empty();
		data_copy_dest(copy, data);
		assert(*i = 666);
		#ifdef debug	
			printf("src:\n");
			data_print(data);
			printf("copy:\n");
			data_print(copy);
		#endif

		assert(!data_equals(data, copy));
		assert(!data_equals(data, NULL));
		assert(!data_equals(copy, NULL));
		assert(!data_equals(NULL, data));
		assert(!data_equals(NULL, copy));
		assert(!data_equals(NULL, NULL));
		assert(data_equals(data, data));
		assert(data_equals(copy, copy));
		
		data_free(data);
		data_free(copy);
		assert(*i = 666);
		free(i);
	}
	
	
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int i = 666;
		data_t * data = data_new_stack(&i, size_int);
		data_t * copy = data_new_empty();
		data_copy_dest(copy, data);
		
		#ifdef debug	
			printf("src:\n");
			data_print(data);
			printf("copy:\n");
			data_print(copy);
		#endif
		
		assert(!data_equals(data, copy));
		assert(!data_equals(data, NULL));
		assert(!data_equals(copy, NULL));
		assert(!data_equals(NULL, data));
		assert(!data_equals(NULL, copy));
		assert(!data_equals(NULL, NULL));
		assert(data_equals(data, data));
		assert(data_equals(copy, copy));
		
		data_free(data);
		data_free(copy);
	}
	#ifdef debug 
		printf("<<< _test_data_copy_dest_:\n");
	#endif
}

static void _test_data_empty_() {
	#ifdef debug 
		printf(">>> _test_data_empty_:\n");
	#endif
	{
		data_t * data = data_new_empty();
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * full = data_new((void**)&i, size_int);
		assert(i == NULL);
		assert(data_is_empty(data));
		assert(!data_is_empty(full));
		#ifdef debug 
			data_print(data);
			data_print(full);
		#endif
		data_free(data);
		data_free(full);
	}
	
	{
		data_t * data = data_new_stack_empty();
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * full = data_new_stack(i, size_int);
		assert(*i == 666);
		assert(data_is_empty(data));
		assert(!data_is_empty(full));
		#ifdef debug 
			data_print(data);
			data_print(full);
		#endif
		data_free(data);
		data_free(full);
		assert(*i == 666);
		free(i);
	}
	
	#ifdef debug 
		printf("<<< _test_data_empty_:\n");
	#endif
}

static void _test_data_clear_() {
	#ifdef debug 
		printf(">>> _test_data_clear_:\n");
	#endif
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * full = data_new((void**)&i, size_int);
		assert(i == NULL);
		assert(!data_is_empty(full));
		#ifdef debug 
			data_print(full);
		#endif
		
		data_clear(full);
		assert(data_is_empty(full));
		
		#ifdef debug 
			data_print(full);
		#endif
		
		data_free(full);
	}

	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * full = data_new_stack(i, size_int);
		assert(*i == 666);
		assert(!data_is_empty(full));
		#ifdef debug 
			data_print(full);
		#endif
		
		data_clear(full);
		assert(data_is_empty(full));
		
		#ifdef debug 
			data_print(full);
		#endif
		
		data_free(full);
		assert(*i == 666);
		free(i);
	}
	
	#ifdef debug 
		printf("<<< _test_data_clear_:\n");
	#endif
}

static void _test_data_move_() {
	#ifdef debug 
		printf(">>> _test_data_move_:\n");
	#endif
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		*i = 666;
		data_t * full = data_new((void**)&i, size_int);
		assert(i == NULL);
		data_t * empty = data_new_empty();
		
		assert(data_is_empty(empty));
		assert(!data_is_empty(full));
		#ifdef debug 
			data_print(full);
			printf("full should %i is %i\n",666,*(unsigned int *)full->data);
			data_print(empty);
			printf("empty should 0 is %i\n",( empty->data ? (*(unsigned int *)empty->data) : 0));
		#endif
		
		data_move(empty, full);
		assert(data_is_empty(full));
		assert(!data_is_empty(empty));
		
		#ifdef debug 
			data_print(full);
			printf("full should 0 is %i\n",( full->data ? (*(unsigned int *)full->data) : 0));
			data_print(empty);
			printf("empty should %i is %i\n",666,*(unsigned int *)empty->data);
		#endif
		
		data_free(full);
		data_free(empty);
	}

	{
		size_t size_int = sizeof(unsigned int);
		unsigned int i = 666;
		data_t * full = data_new_stack(&i, size_int);
		data_t * empty = data_new_empty();
		
		assert(data_is_empty(empty));
		assert(!data_is_empty(full));
		#ifdef debug 
			data_print(full);
			printf("full should %i is %i\n",i,*(unsigned int *)full->data);
			data_print(empty);
			printf("empty should 0 is %i\n",( empty->data ? (*(unsigned int *)empty->data) : 0));
		#endif
		
		data_move(empty, full);
		assert(data_is_empty(full));
		assert(!data_is_empty(empty));
		
		#ifdef debug 
			data_print(full);
			printf("full should 0 is %i\n",( full->data ? (*(unsigned int *)full->data) : 0));
			data_print(empty);
			printf("empty should %i is %i\n",i,*(unsigned int *)empty->data);
		#endif
		
		data_free(full);
		data_free(empty);
	}
	
	#ifdef debug 
		printf("<<< _test_data_move_:\n");
	#endif
}

static void _test_data_set_() {
	#ifdef debug 
		printf(">>> _test_data_set_:\n");
	#endif
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		unsigned int * i2 = malloc(size_int);
		*i = 666;
		*i2 = 667;
		
		data_t * full = data_new((void**)&i, size_int);
		assert(i == NULL);
		data_t * empty = data_new_empty();
		
		assert(data_is_empty(empty));
		assert(!data_is_empty(full));
		
		assert(!data_set(full, (void**)&i2, size_int));
		assert(!data_is_empty(full));
		assert(!(*(unsigned int *)full->data == *i2));
		assert(*(unsigned int *)full->data == 666);
		
		assert(data_is_empty(empty));
		assert(data_set(empty, (void**)&i2, size_int));
		assert(!data_is_empty(empty));
		
		assert(!(*(unsigned int *)empty->data == 666));
		assert(*(unsigned int *)empty->data == 667);
		
		data_free(full);
		data_free(empty);
	}
	
	#ifdef debug 
		printf("<<< _test_data_set_:\n");
	#endif
}
static void _test_data_set_stack_(){
	#ifdef debug 
		printf(">>> _test_data_set_stack_:\n");
	#endif
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int i = 666;
		unsigned int i2 = 667;
		
		data_t * full = data_new_stack(&i, size_int);
		data_t * empty = data_new_empty();
		
		assert(data_is_empty(empty));
		assert(!data_is_empty(full));
		
		assert(!data_set_stack(full, &i2, size_int));
		assert(!data_is_empty(full));
		assert(!(*(unsigned int *)full->data == i2));
		assert(*(unsigned int *)full->data == i);
		
		assert(data_is_empty(empty));
		assert(data_set_stack(empty, &i2, size_int));
		assert(!data_is_empty(empty));
		
		assert(!(*(unsigned int *)empty->data == i));
		assert(*(unsigned int *)empty->data == i2);
		
		data_free(full);
		data_free(empty);
	}
	#ifdef debug 
		printf("<<< _test_data_set_stack_:\n");
	#endif
}
static void _test_data_override_(){
	#ifdef debug 
		printf(">>> _test_data_override_:\n");
	#endif
		{
		size_t size_int = sizeof(unsigned int);
		unsigned int * i = malloc(size_int);
		unsigned int * i2 = malloc(size_int);
		unsigned int * i3 = malloc(size_int);
		unsigned int * i4 = malloc(size_int);
		*i = 666;
		*i2 = 667;
		*i3 = 668;
		*i4 = 669;
		
		data_t * full = data_new((void**)&i, size_int);
		data_t * full2 = data_new((void**)&i2, size_int);
		
		assert(i == NULL);
		assert(i2 == NULL);
		assert(!data_is_empty(full2));
		assert(!data_is_empty(full));
		
		//after this method i is not longer available
		assert(data_override(full, (void**)&i3, size_int));
		assert(i3 == NULL);
		assert(!data_is_empty(full));
		assert(*(unsigned int *)full->data == 668);
		assert(!(*(unsigned int *)full->data == 666));
		
		//after this i2 is not longer available
		assert(data_override(full2, (void**)&i4, size_int));
		assert(i4 == NULL);
		assert(!data_is_empty(full2));
		
		assert(*(unsigned int *)full2->data == 669);
		assert(!(*(unsigned int *)full2->data == 667));
		
		data_free(full);
		data_free(full2);
	}
	#ifdef debug 
		printf("<<< _test_data_override_:\n");
	#endif
}
static void _test_data_override_stack_(){
	#ifdef debug 
		printf(">>> _test_data_override_stack_:\n");
	#endif
	{
		size_t size_int = sizeof(unsigned int);
		unsigned int i = 666;
		unsigned int i2 = 667;
		unsigned int i3 = 668;
		unsigned int i4 = 669;
		
		data_t * full = data_new_stack(&i, size_int);
		data_t * full2 = data_new_stack(&i2, size_int);
		
		assert(!data_is_empty(full2));
		assert(!data_is_empty(full));
		
		assert(data_override_stack(full, &i3, size_int));
		assert(!data_is_empty(full));
		assert(*(unsigned int *)full->data == i3);
		assert(!(*(unsigned int *)full->data == 666));
		
		assert(data_override_stack(full2, &i4, size_int));
		assert(!data_is_empty(full2));
		
		assert(*(unsigned int *)full2->data == i4);
		assert(!(*(unsigned int *)full2->data == 667));
		
		data_free(full);
		data_free(full2);
	}
	#ifdef debug 
		printf("<<< _test_data_override_stack_:\n");
	#endif
}

int 
main() 
{
	#ifdef debug 
		printf(">>> test data:\n");
	#endif
	_test_data_alloc_();
	_test_data_copy_();
	_test_data_copy_dest_();
	_test_data_empty_();
	_test_data_clear_();
	_test_data_move_();
	_test_data_set_();
	_test_data_set_stack_();
	_test_data_override_();
	_test_data_override_stack_();
	#ifdef debug 
		printf("<<< test data:\n");
	#endif
	return 0;
}
