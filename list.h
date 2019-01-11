/******************************************************************
 * @brief   Header for a dynamic List
 ******************************************************************
 */


#ifndef LIST_H_
#define LIST_H_

#include <stdint.h>
#include <stdlib.h>


#define TYPE uint64_t

typedef struct Node_s Element;

struct Node_s{
    Element *next;
    TYPE value;
};

typedef struct{
    Element *first;
    int size;
}List;

typedef struct{
    List *list;
    Element *next;
    int current_index;
} Iterator;

/**
 * @brief Initializes a new list
 */
int list_init(List *list);

/**
 * @brief Adds a Elements to the List
 */
int list_append(List *list, TYPE val);

/**
 * @brief Returns an Iterator for the List
 *
 * @param[in] list the List to iterate over
 * @return an Iterator
 */
Iterator list_get_iterator(List *list);

/**
 * @brief Checks whether there are more elements to process
 *
 * @param[in, out] it the Iterator to check
 * @return whether or not there are more elements
 */
int iterator_has_next(Iterator *it);

/**
 * @brief Gets next element to process
 *
 * @param[in, out] it the Iterator to check
 * @param[out] val    will contain the next VALUE_TYPE if method returned 0
 * @return error code
 * @retval 0               no error
 * @retval E_ILLEGAL_STATE the Iterator has no next
 */
int iterator_next(Iterator *it, TYPE *val);



#endif /* LIST_H_ */






















/**
 * @brief Removes (deletes) all elements
 *
 * This method MUST be called before list goes out of scope, otherwise there
 * might be unfreed memory left on the heap
 *
 * @param[in, out] list the list to clear
 * @return error code
 * @retval 0           no error
 * @retval E_LIST_NULL the list is null, so it can't be accessed

int list_clear(List *list);
 */
 
 
 
 
 /**
 * @brief Sets the VALUE_TYPE at a specific index
 *
 * @param[in, out] list the list to add to
 * @param[in] index     the index at which to set
 * @param[in] val       the VALUE_TYPE to set
 * @return error code
 * @retval 0                     no error
 * @retval E_LIST_NULL           the list is null, so it can't be accessed
 * @retval E_INDEX_OUT_OF_BOUNDS the index is invalid
 
int list_set(List *list, int index, VALUE_TYPE val);
*/



/**
 * @brief Removes an element from the list
 *
 * This method silently ignores errors
 *
 * @param[in, out] list the list to delete from
 * @param[in] index     the index at which to delete
 
void list_remove(List *list, int index);
*/

