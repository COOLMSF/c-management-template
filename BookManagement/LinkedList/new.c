#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXNAME 254
#define MAXBUF 1024
#define MAXBOOK 100

enum BookType {
    Novel,
};

typedef void *SListValue;
typedef struct _SListEntry SListEntry;
struct _SListEntry {
	SListValue data;
	SListEntry *next;
};

typedef struct _SListIterator SListIterator;

struct _SListIterator {
	SListEntry **prev_next;
	SListEntry *current;
};

#define SLIST_NULL ((void *) 0)

struct _BookInfo {
    int id;
    int num_borrowed;
    int num_left;
    enum BookType type;
    char name[MAXNAME];
    char author_name[MAXNAME];
};
typedef struct _BookInfo BookInfo;

struct _User {
    char id[4];
    char name[MAXNAME];
    int num_borrow;
    // time_t time_borrow;
    // time_t time_return;
    int time_borrow;
    int time_return;
    int max_num_borrow;
    int max_time_borrow;
    SListEntry *borrowed_bookid;
    SListEntry *borrowed_bookname;
    enum { TEACHER, STUDENT } type;
};

typedef int (*SListCompareFunc)(SListValue value1, SListValue value2);
typedef int (*SListEqualFunc)(SListValue value1, SListValue value2);


void slist_free(SListEntry *list)
{
	SListEntry *entry;

	/* Iterate over each entry, freeing each list entry, until the
	 * end is reached */

	entry = list;

	while (entry != NULL) {
		SListEntry *next;

		next = entry->next;

		free(entry);

		entry = next;
	}
}

SListEntry *slist_prepend(SListEntry **list, SListValue data)
{
	SListEntry *newentry;

	/* Create new entry */

	newentry = malloc(sizeof(SListEntry));

	if (newentry == NULL) {
		return NULL;
	}

	newentry->data = data;

	/* Hook into the list start */

	newentry->next = *list;
	*list = newentry;

	return newentry;
}

SListEntry *slist_append(SListEntry **list, SListValue data)
{
	SListEntry *rover;
	SListEntry *newentry;

	/* Create new list entry */

	newentry = malloc(sizeof(SListEntry));

	if (newentry == NULL) {
		return NULL;
	}

	newentry->data = data;
	newentry->next = NULL;

	/* Hooking into the list is different if the list is empty */

	if (*list == NULL) {

		/* Create the start of the list */

		*list = newentry;

	} else {

		/* Find the end of list */

		for (rover=*list; rover->next != NULL; rover = rover->next);

		/* Add to the end of list */

		rover->next = newentry;
	}

	return newentry;
}

SListValue slist_data(SListEntry *listentry)
{
	return listentry->data;
}

void slist_set_data(SListEntry *listentry, SListValue data)
{
	if (listentry != NULL) {
		listentry->data = data;
	}
}

SListEntry *slist_next(SListEntry *listentry)
{
	return listentry->next;
}

SListEntry *slist_nth_entry(SListEntry *list, unsigned int n)
{
	SListEntry *entry;
	unsigned int i;

	/* Iterate through n list entries to reach the desired entry.
	 * Make sure we do not reach the end of the list. */

	entry = list;

	for (i=0; i<n; ++i) {

		if (entry == NULL) {
			return NULL;
		}
		entry = entry->next;
	}

	return entry;
}

SListValue slist_nth_data(SListEntry *list, unsigned int n)
{
	SListEntry *entry;

	/* Find the specified entry */

	entry = slist_nth_entry(list, n);

	/* If out of range, return NULL, otherwise return the data */

	if (entry == NULL) {
		return SLIST_NULL;
	} else {
		return entry->data;
	}
}

unsigned int slist_length(SListEntry *list)
{
	SListEntry *entry;
	unsigned int length;

	length = 0;
	entry = list;

	while (entry != NULL) {

		/* Count the number of entries */

		++length;

		entry = entry->next;
	}

	return length;
}

SListValue *slist_to_array(SListEntry *list)
{
	SListEntry *rover;
	SListValue *array;
	unsigned int length;
	unsigned int i;

	/* Allocate an array equal in size to the list length */

	length = slist_length(list);

	array = malloc(sizeof(SListValue) * length);

	if (array == NULL) {
		return NULL;
	}

	/* Add all entries to the array */

	rover = list;

	for (i=0; i<length; ++i) {

		/* Add this node's data */

		array[i] = rover->data;

		/* Jump to the next list node */

		rover = rover->next;
	}

	return array;
}

int slist_remove_entry(SListEntry **list, SListEntry *entry)
{
	SListEntry *rover;

	/* If the list is empty, or entry is NULL, always fail */

	if (*list == NULL || entry == NULL) {
		return 0;
	}

	/* Action to take is different if the entry is the first in the list */

	if (*list == entry) {

		/* Unlink the first entry and update the starting pointer */

		*list = entry->next;

	} else {

		/* Search through the list to find the preceding entry */

		rover = *list;

		while (rover != NULL && rover->next != entry) {
			rover = rover->next;
		}

		if (rover == NULL) {

			/* Not found in list */

			return 0;

		} else {

			/* rover->next now points at entry, so rover is the
			 * preceding entry. Unlink the entry from the list. */

			rover->next = entry->next;
		}
	}

	/* Free the list entry */

	free(entry);

	/* Operation successful */

	return 1;
}

unsigned int slist_remove_data(SListEntry **list, SListEqualFunc callback,
                               SListValue data)
{
	SListEntry **rover;
	SListEntry *next;
	unsigned int entries_removed;

	entries_removed = 0;

	/* Iterate over the list.  'rover' points at the entrypoint into the
	 * current entry, ie. the list variable for the first entry in the
	 * list, or the "next" field of the preceding entry. */

	rover = list;

	while (*rover != NULL) {

		/* Should this entry be removed? */

		if (callback((*rover)->data, data) != 0) {

			/* Data found, so remove this entry and free */

			next = (*rover)->next;
			free(*rover);
			*rover = next;

			/* Count the number of entries removed */

			++entries_removed;
		} else {

			/* Advance to the next entry */

			rover = &((*rover)->next);
		}
	}

	return entries_removed;
}

/* Function used internally for sorting.  Returns the last entry in the
 * new sorted list */

static SListEntry *slist_sort_internal(SListEntry **list,
                                       SListCompareFunc compare_func)
{
	SListEntry *pivot;
	SListEntry *rover;
	SListEntry *less_list, *more_list;
	SListEntry *less_list_end, *more_list_end;

	/* If there are less than two entries in this list, it is
	 * already sorted */

	if (*list == NULL || (*list)->next == NULL) {
		return *list;
	}

	/* The first entry is the pivot */

	pivot = *list;

	/* Iterate over the list, starting from the second entry.  Sort
	 * all entries into the less and more lists based on comparisons
	 * with the pivot */

	less_list = NULL;
	more_list = NULL;
	rover = (*list)->next;

	while (rover != NULL) {
		SListEntry *next = rover->next;

		if (compare_func(rover->data, pivot->data) < 0) {

			/* Place this in the less list */

			rover->next = less_list;
			less_list = rover;

		} else {

			/* Place this in the more list */

			rover->next = more_list;
			more_list = rover;

		}

		rover = next;
	}

	/* Sort the sublists recursively */

	less_list_end = slist_sort_internal(&less_list, compare_func);
	more_list_end = slist_sort_internal(&more_list, compare_func);

	/* Create the new list starting from the less list */

	*list = less_list;

	/* Append the pivot to the end of the less list.  If the less list
	 * was empty, start from the pivot */

	if (less_list == NULL) {
		*list = pivot;
	} else {
		less_list_end->next = pivot;
	}

	/* Append the more list after the pivot */

	pivot->next = more_list;

	/* Work out what the last entry in the list is.  If the more list was
	 * empty, the pivot was the last entry.  Otherwise, the end of the
	 * more list is the end of the total list. */

	if (more_list == NULL) {
		return pivot;
	} else {
		return more_list_end;
	}
}

void slist_sort(SListEntry **list, SListCompareFunc compare_func)
{
	slist_sort_internal(list, compare_func);
}

SListEntry *slist_find_data(SListEntry *list,
                            SListEqualFunc callback,
                            SListValue data)
{
	SListEntry *rover;

	/* Iterate over entries in the list until the data is found */

	for (rover=list; rover != NULL; rover=rover->next) {
		if (callback(rover->data, data) != 0) {
			return rover;
		}
	}

	/* Not found */

	return NULL;
}

void slist_iterate(SListEntry **list, SListIterator *iter)
{
	/* Start iterating from the beginning of the list. */

	iter->prev_next = list;

	/* We have not yet read the first item. */

	iter->current = NULL;
}

int slist_iter_has_more(SListIterator *iter)
{
	if (iter->current == NULL || iter->current != *iter->prev_next) {

		/* Either we have not read the first entry, the current
		 * item was removed or we have reached the end of the
		 * list.  Use prev_next to determine if we have a next
		 * value to iterate over. */

		return *iter->prev_next != NULL;

	} else {

		/* The current entry has not been deleted.  There
		 * is a next entry if current->next is not NULL. */

		return iter->current->next != NULL;
	}
}

SListValue slist_iter_next(SListIterator *iter)
{
	if (iter->current == NULL || iter->current != *iter->prev_next) {

		/* Either we are reading the first entry, we have reached
		 * the end of the list, or the previous entry was removed.
		 * Get the next entry with iter->prev_next. */

		iter->current = *iter->prev_next;

	} else {

		/* Last value returned from slist_iter_next was not
		 * deleted. Advance to the next entry. */

		iter->prev_next = &iter->current->next;
		iter->current = iter->current->next;
	}

	/* Have we reached the end of the list? */

	if (iter->current == NULL) {
		return SLIST_NULL;
	} else {
		return iter->current->data;
	}
}

void slist_iter_remove(SListIterator *iter)
{
	if (iter->current == NULL || iter->current != *iter->prev_next) {

		/* Either we have not yet read the first item, we have
		 * reached the end of the list, or we have already removed
		 * the current value.  Either way, do nothing. */

	} else {

		/* Remove the current entry */

		*iter->prev_next = iter->current->next;
		free(iter->current);
		iter->current = NULL;
	}
}

int callback_check_id(SListValue a, SListValue b)
{
    if (((struct _BookInfo *)a)->id == *(int *)b)
        return 1;
    else
        return 0;
}

void welcome()
{
    puts("");
    puts("-------------------");
    puts("??????????????????????????????");
    puts("1:) ??????????????????");
    puts("2:) ??????????????????");
    puts("3:) ??????????????????");
    puts("4:) ??????????????????");
    puts("5:) ??????");
    puts("6:) ??????");
    puts("7:) ????????????????????????");
    puts("8:) ??????");
    puts("9:) ??????????????????");
    puts("10:) ??????");
    puts("-------------------");
    puts("");
}

int insert_book(SListEntry **list)
{
    BookInfo *book_info;
    SListEntry *p;

    book_info = malloc(sizeof (struct _BookInfo));
    if (book_info == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    puts("????????????id:");
    scanf("%d", &book_info->id);
    puts("??????????????????:");
    scanf("%s", book_info->name);
    puts("??????????????????:");
    scanf("%s", book_info->author_name);
    puts("");

    // ??????????????????????????? ?????????num_borrowed++, ????????????????????? ????????????????????????
    p = slist_find_data(*list, callback_check_id, (SListValue *)&book_info->id);
    if (p == NULL) {
        if (slist_append(list, (SListValue *)book_info) != NULL) {
            book_info->num_left++;
            puts("-------------------");
            puts("????????????");
            puts("-------------------");
            return 1;
        } else {
            puts("-------------------");
            puts("????????????");
            puts("-------------------");
            return 0;
        }
    } else {
        SListValue *value;

        value = p->data;
        // ??????????????????
        ((struct _BookInfo *)value)->num_left++;
    }
}

int delete_book_by_id(SListEntry **entry, int id)
{
    SListEntry *p;
    SListValue *value;
    struct _BookInfo *book_info;

    // ?????????????????????, ?????? ?????????????????????, ?????? ????????????????????????(??????num_borrowed--)???
    // ?????? ????????????????????????(??????????????????)
    p = slist_find_data(*entry, callback_check_id, (SListValue *)&id);
    if (p == NULL) {
        puts("-------------------");
        puts("???????????????");
        puts("-------------------");
        return -1;
    } else {
        value = slist_data(p);
        book_info = (struct _BookInfo *) value;

        if (book_info->num_left == 1) {
            if (slist_remove_entry(entry, p) == 0) {
                puts("-------------------");
                puts("??????????????????");
                puts("-------------------");
                return -1;
            } else {
                puts("-------------------");
                puts("????????????");
                puts("-------------------");
                return 1;
            }
        } else {
            value = slist_data(p);
            book_info = (struct _BookInfo *) value;
            // ??????????????????
            book_info->num_left--;
        }
    }
    puts("");
}

int delete_book(SListEntry **entry)
{
    int id;

    puts("????????????????????????id:");
    scanf("%d", &id);
    delete_book_by_id(entry, id);

    return 1;
}

int query_book_by_id(SListEntry **entry, int id)
{
    SListEntry *p;
	SListEntry *rover;
    SListValue *data;

    p = slist_find_data(*entry, callback_check_id, (SListValue *)&id);
    if (p == NULL) {
        puts("-------------------");
        puts("??????????????????");
        puts("-------------------");
        return -1;
    } else {
        data = slist_data(p);

        puts("----------------------");
        printf("??????????????????\n");
        // ????????????????????????
        printf("id:%d\n", ((struct _BookInfo *)data)->id);
        printf("??????:%s\n", ((struct _BookInfo *)data)->name);
        printf("??????:%s\n", ((struct _BookInfo *)data)->author_name);
        printf("????????????:%d\n", ((struct _BookInfo *)data)->num_borrowed);
        printf("????????????:%d\n", ((struct _BookInfo *)data)->num_left);
        puts("----------------------");
    }
    return 1;
}

int query_book(SListEntry **entry)
{
    int id;

    puts("?????????????????????id:");
    scanf("%d", &id);
    query_book_by_id(entry, id);

    return 1;
}


int modify_book(SListEntry **entry)
{
    return 0;
}

int borrow_book(struct _User *p, SListEntry *book_list)
{
    int id;
    SListEntry *entry;

    puts("????????????????????????id:");
    scanf("%d", &id);

    p->num_borrow++;
    entry = slist_find_data(book_list, callback_check_id, (SListValue *)&id);
    if (entry == NULL) {
        puts("----------------------");
        puts("???????????????");
        puts("----------------------");
        return -1;
    }

    // ??????id?????????????????????
    slist_append(&p->borrowed_bookid, (struct _BookInfo *) &id);

    // ????????????1, ????????????1
    ((struct _BookInfo *) entry->data)->num_left--;
    ((struct _BookInfo *) entry->data)->num_borrowed++;
}

int return_book(struct _User *p, SListEntry *book_list)
{
    int id;
    SListEntry *entry;

    puts("????????????????????????id:");
    scanf("%d", &id);

    entry = slist_find_data(book_list, callback_check_id, (SListValue *)&id);
    if (entry == NULL) {
        puts("----------------------");
        puts("???????????????");
        puts("----------------------");
        return -1;
    }
    // ???????????????????????????????????????????????????
    slist_remove_entry(&p->borrowed_bookid, entry);
    // ?????????????????????1
    p->num_borrow--;
    // ?????????????????????1, ????????????1
    ((struct _BookInfo *) entry->data)->num_left++;
    ((struct _BookInfo *) entry->data)->num_borrowed--;
}

void init_teacher(struct _User *p)
{
    p->max_num_borrow = 15;
    p->max_time_borrow = 180;
}

void init_student(struct _User *p)
{
    p->max_time_borrow = 10;
    p->max_time_borrow = 120;
}

struct _User *ask_user_info()
{
    struct _User *user;

    user = malloc(sizeof(struct _User));
    if (user == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    puts("???????????????:");
    scanf("%s", user->name);
    puts("???????????????:");
    scanf("%s", user->id);
    puts("??????????????? 0:)??????, 1:)??????:");
    scanf("%d", &user->type);

    return user;
}

int insert_user(SListEntry **list, SListValue *user)
{
    SListEntry *p;

    if (slist_append(list, (SListValue *)user) != NULL) {
        ((struct _User *) user)->num_borrow++;
        puts("----------------------");
        puts("??????????????????");
        puts("----------------------");
        return 1;
    } else {
        puts("----------------------");
        puts("??????????????????");
        puts("----------------------");
        return 0;
    }
}

int update_user(SListEntry **list, SListValue *value)
{
    // ???????????????????????? ?????? ??????????????????????????? ???????????????????????????????????? ?????? ?????????????????? ??????????????????????????????
    // ?????????????????????
    SListEntry *p;
    int id;

    puts("?????????????????????id:");
    scanf("%d", &id);
    // ?????????????????????????????????
    p = slist_find_data(*list, callback_check_id, ((struct _User *)value)->id);
    if (p == NULL) {
        insert_user(list, value);
    } else {
        // (struct _User *)value.
        struct _User *p;
        p = (struct _User *) value;
        p->num_borrow++;
        // ??????id???????????????
        // p->id.append(id);
        // p->time_borrow = gettime_now();
    }
}

void show_userinfo(SListEntry *user_list, struct _User *p)
{
    SListEntry *entry;

    entry = slist_find_data(user_list, callback_check_id, p->id);
    if (entry != NULL) {
        struct _User *t;

        t = entry->data;
        puts("-------------------");
        printf("?????????:%s\n", t->name);
        printf("????????????:%d\n", t->num_borrow);

        // ???????????????
        while (p->borrowed_bookid != NULL) {
            printf("%s\n", (char *)(p->borrowed_bookname->data));
        }
        puts("-------------------");
    } else {
        puts("-------------------");
        puts("??????????????????");
        puts("-------------------");
    }
}

void show_books(SListEntry *book_list)
{
    SListValue *value;
    SListEntry *p = book_list;

    int n = 0;
    puts("-------------------");
    while (p != NULL) {
        value = p->data;

        printf("ID:%d\t", ((struct _BookInfo *) value)->id);
        printf("??????:%s\t", ((struct _BookInfo *) value)->name);
        printf("??????:%s\t", ((struct _BookInfo *) value)->author_name);
        printf("????????????:%d\n", ((struct _BookInfo *) value)->num_left);

        p = p->next;
        n++;
    }
    puts("-------------------");

    if (n == 0) {
        puts("-------------------");
        puts("????????????????????????");
        puts("-------------------");
    }
}

int statistic(SListEntry *book_list)
{
    int id;
    int n = 0;
    SListEntry *p;
    int num_borrowed, num_left;

    puts("-------------------");
    puts("??????????????????:");

    p = book_list;
    while (p != NULL) {
        id = ((struct _BookInfo *)p->data)->id;
        num_left = ((struct _BookInfo *)p->data)->num_left;
        num_borrowed = ((struct _BookInfo *)p->data)->num_borrowed;

        printf("??????id:%d ????????????:%d ????????????:%d\n", id, num_left, num_borrowed);
        // printf("??????id:%d ?????????:%lf\n", id, ((double) num_left) / num_borrowed);
        n++;
        p = p->next;
    }
    puts("-------------------");

    if (n == 0) {
        puts("??????????????????");
    }
}

int main(int argc, char const* argv[])
{
    int input;
    // ?????????????????????????????????
    SListEntry *book_list = NULL;
    // ?????????????????????????????????
    SListEntry *user_list = NULL;
    struct _User *user;

    while (1) {
        welcome();
        puts("");
        puts("??????????????????:");
        scanf("%d", &input);

        switch (input) {
            case 1:
                user = ask_user_info();
                if (user->type != TEACHER) {
                    puts("-------------------");
                    printf("????????????????????????\n");
                    puts("-------------------");
                    break;
                }
                // ????????????
                insert_book(&book_list);
                break;
            case 2:
                user = ask_user_info();
                if (user->type != TEACHER) {
                    puts("-------------------");
                    printf("????????????????????????\n");
                    puts("-------------------");
                    break;
                }
                // ????????????
                delete_book(&book_list);
                break;
            case 3:
                user = ask_user_info();
                if (user->type != TEACHER) {
                    puts("-------------------");
                    printf("????????????????????????\n");
                    puts("-------------------");
                    break;
                }
                // ????????????
                modify_book(&book_list);
                break;
            case 4:
                // ??????????????????
                query_book(&book_list);
                break;
            case 5:
                user = ask_user_info();
                if (user->type == TEACHER)
                    init_teacher(user);
                else
                    init_student(user);
                // ??????
                borrow_book(user, book_list);
                // ????????????
                update_user(&user_list, (SListValue *) user);
                break;
            case 6:
                user = ask_user_info();
                if (user->type == TEACHER)
                    init_teacher(user);
                else
                    init_student(user);
                // ??????
                return_book(user, book_list);
                update_user(&user_list, (SListValue *) user);
                break;
            case 7:
                user = ask_user_info();
                show_userinfo(user_list, user);
                break;
            case 8:
                statistic(book_list);
                break;
            case 9:
                show_books(book_list);
                break;
            case 10:
                exit(EXIT_SUCCESS);
            default:
                puts("-------------------");
                puts("????????????");
                puts("-------------------");
                break;
        }
    }

    slist_free(book_list);
    return 0;
}

