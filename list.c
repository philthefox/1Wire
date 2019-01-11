/** ****************************************************************
 * @brief   Implementation for dynamic List module
 ******************************************************************
 */

 #include "list.h"
 #include "ErrorHandler.h"

/*
 * Initializes a new list
 */
int list_init(List *list) {
    if (list == NULL) {
			errorHandler(55);
    }

    list->first = NULL;
    list->size = 0;
    return 0;
}


/*
 * Adds a new Element to the List
 */
int list_append(List *list, TYPE val) {
    if (list == NULL) {
				errorHandler(55);
    }
    if (list->first == NULL) {
        Element *element = (Element*) malloc(sizeof(Element));
        if (element == NULL) {
								errorHandler(55);
        }
        element->next = NULL;
        element->value = val;
        list->first = element;
    } else {
        Element *element = list->first;
        // go through list
        while (element->next != NULL) {
            element = element->next;
        }
        Element *newElement = (Element*) malloc(sizeof(Element));
        if (newElement == NULL) {
					errorHandler(55);
        }
        newElement->next = NULL;
        newElement->value = val;
        element->next = newElement;
    }
    list->size++;
    return 0;

}


/*
 * Returns an Iterator for the List
 */
Iterator list_get_iterator(List *list) {
    Iterator it;
    if (list == NULL) {
        it.list = NULL;
        it.next = NULL;
        it.current_index = 0;
    } else {
        it.list = list;
        it.next = list->first;
        it.current_index = 0;
    }
    return it;
}

/*
 * Checks whether there are more elements to process
 */
int iterator_has_next(Iterator *it) {
    if (it == NULL) {
        return 0;
    }
    return it->next != NULL;
}

/*
 * Gets next element to process
 */
int iterator_next(Iterator *it, TYPE *val) {
    if (it == NULL || !iterator_has_next(it)) {
			errorHandler(55);
    }
    *val = it->next->value;
    it->next = it->next->next;
    it->current_index++;
    return 0;
}





/*
 * Removes (deletes) all elements
 *
 * This method MUST be called before list goes out of scope, otherwise there
 * might be unfreed memory left on the heap

int list_clear(List *list) {
    if (list == NULL) {
        return E_LIST_NULL;
    }
    Element *element = list->first;
    // go through list
    while (element != NULL) {
        Element *next = node->next;
        free(element);
        node = next;
    }
    list->first = NULL;
    list->size = 0;
    return 0;
}
 */
