#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

int vector_init(Vector *v, int initial_size)
{
	v->data = malloc(initial_size * sizeof(void *));
	if (!v->data)
	{
		//error \o/
		printf("Vector failed to allocate.\n");
		return -1;
	}

	v->count = 0;
	v->bounds = initial_size;
	return 0;
}

void vector_fill(Vector *v, void *value)
{
	for (int i = 0; i < v->bounds; i++)
	{
		v->data[i] = value;
	}

	v->count = v->bounds;
}

int vector_get_size(Vector *v)
{
	return v->count;
}

int vector_get_index(Vector *v, void *value)
{
	for (int i = 0; i < v->count; ++i)
	{
		if (v->data[i] == value)
		{
			return i;
		}
	}
	return -1;
}

void vector_add(Vector *v, void *value)
{
	if (v->count == v->bounds)
	{
		vector_resize(v, v->bounds * 2); //todo: do we want to double all the time?
	}
	v->data[v->count++] = value;
}

void *vector_get(Vector *v, int index)
{
	if (index >= 0 && index < v->count)
	{
		return v->data[index];
	}
	else
	{
		printf("Array index %d out of bounds in get.\n", index);
		return NULL;
	}
}

void vector_set(Vector *v, int index, void *value)
{
	if (index >= 0 && index < v->count)
	{
		v->data[index] = value;
	}
	else
	{
		printf("Array index %d out of bounds in set.\n", index);
	}
}

void vector_remove(Vector *v, int index)
{
	if (index >= 0 && index < v->count)
	{
		v->data[index] = NULL;
		for (int i = index; i < v->count - 1; i++)
		{
			v->data[i] = v->data[i + 1];
			v->data[i + 1] = NULL;
		}

		v->count --;
		if (v->count > 0 && v->count == v->bounds / 4)
		{
			vector_resize(v, v->bounds / 2);
		}
	}
	else
	{
		printf("Array index %d out of bounds in remove.\n", index);
	}
}

void vector_resize(Vector *v, int new_size)
{

	//printf("Adjusting vector size from %d to %d.\n", v->bounds, new_size);
	void **new_data = realloc(v->data, sizeof(void *) * new_size);
	if (new_data)
	{
		v->data = new_data;
		v->bounds = new_size;
	}
	else
	{
		printf("Vector failed to reallocate.\n");
	}
}

void vector_shuffle(Vector *v)
{
	for (int i = 0; i < v->count; i++)
	{
		void * temp = v->data[i];
		int j = i + rand() / (RAND_MAX / (v->count - i) + 1);
		v->data[i] = v->data[j];
		v->data[j] = temp;
	}
}

void vector_free(Vector *v)
{
	if (v->data == NULL)
	{
		return;
	}

	free(v->data);
	v->data = NULL;
	v->count = 0;
	v->bounds = 0;
}

void vector_free_and_free(Vector *v)
{
	if (v->data == NULL)
	{
		return;
	}

	if (v->bounds < v->count)
	{
		printf("Error while attempting to free vector contents. Contents not freed.\n");
	}
	else
	{
		for (int i = 0; i < v->count; i++)
		{
			if (v->data[i] != NULL)
			{
				free(v->data[i]);
			}
		}
	}
	
	vector_free(v);
}
