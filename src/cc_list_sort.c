#include "cc_common.h"
#include "cc_dbg.h"
#include "cc_stdint.h"

#include "cc_array.h"
#include "cc_list.h"
#include "cc_list_sort.h"


/*


    only in same list
*/
static int cc_list_node_swap(cc_list_node_t *a, cc_list_node_t *b)
{
    if(a == NULL || b == NULL) return ERR_CC_COMMON_INVALID_ARG;
    void *data = a->data;
    a->data = b->data;
    b->data = data;
    return ERR_CC_COMMON_OK;
}

static int cc_list_cmp(cc_cmp_fn_t cmp, cc_list_node_t *i, cc_list_node_t *j)
{
    return cmp(i, j);
}

static int cc_list_sort_bubble_traditional(cc_list_t *self, cc_cmp_fn_t cmp)
{
    if(self == NULL || cmp == NULL) return ERR_CC_COMMON_INVALID_ARG;
    cc_list_node_t *slow, *fast, *end;
    end = &self->root;
    slow = self->root.next;
    int max_times = self->root.size * self->root.size;
    while(slow != end && slow->next != end){
        fast = self->root.next;
        while(fast != end && fast->next != end) {
            if(cc_list_cmp(cmp, fast->data, fast->next->data) > 0) {
                cc_list_node_swap(fast, fast->next);
            }
            fast = fast->next;
        }
        slow = slow->next;
    }

    return ERR_CC_COMMON_OK;
error:
    exit(1);
}

static int cc_list_sort_bubble_adaptive(cc_list_t *self, cc_cmp_fn_t cmp)
{
    if(self == NULL || cmp == NULL) return ERR_CC_COMMON_INVALID_ARG;
    cc_list_node_t *slow, *fast, *end;
    end = &self->root;
    slow = self->root.next;
    int max_times = self->root.size * self->root.size;

    int swapped = 0;
    while(slow != end && slow->next != end){
        fast = self->root.next;
        swapped = 0;
        while(fast != end && fast->next != end) {
            if(cc_list_cmp(cmp, fast->data, fast->next->data) > 0) {
                cc_list_node_swap(fast, fast->next);
                swapped = 1;
            }
            fast = fast->next;
        }
        if(swapped == 0) break;
        slow = slow->next;
    }

    return ERR_CC_COMMON_OK;
error:
    exit(1);
}

static int cc_list_sort_bubble_get_end(cc_list_t *self, cc_cmp_fn_t cmp)
{
    if(self == NULL || cmp == NULL) return ERR_CC_COMMON_INVALID_ARG;
    cc_list_node_t *slow, *fast, *end, *last_swap;
    end = &self->root;
    slow = self->root.next;
    int max_times = self->root.size * self->root.size;

    while(slow != &self->root && slow->next != &self->root){
        fast = self->root.next;
        last_swap = NULL;
        while(fast != end && fast->next != end) {
            if(cc_list_cmp(cmp, fast->data, fast->next->data) > 0) {
                cc_list_node_swap(fast, fast->next);
                last_swap = fast->next;
            }
            fast = fast->next;
        }

        if(last_swap == NULL) break;

        end = last_swap;
        slow = slow->next;
    }

    return ERR_CC_COMMON_OK;
error:
    exit(1);
}

int cc_list_sort_bubble(cc_list_t *self, cc_cmp_fn_t cc_list_cmp)
{
    return cc_list_sort_bubble_get_end(self, cc_list_cmp);
}

int _cc_list_sort_bubble(cc_list_t *self, cc_cmp_fn_t cmp, cc_size_t optimize)
{
    if(self == NULL || cmp == NULL) return ERR_CC_COMMON_INVALID_ARG;

    switch(optimize){
        case 1:
            return cc_list_sort_bubble_traditional(self, cmp);
        case 2:
            return cc_list_sort_bubble_adaptive(self, cmp);
        case 3:
            return cc_list_sort_bubble_get_end(self, cmp);
        default:
            return ERR_CC_COMMON_INVALID_ARG;
    }
    return ERR_CC_COMMON_OK;
}

int cc_list_sort_merge_traditional(cc_list_t *self, cc_cmp_fn_t cmp)
{
    if(self == NULL || cmp == NULL) return ERR_CC_COMMON_INVALID_ARG;
    int res = ERR_CC_COMMON_OK;
    if(self->root.size <= 1) return ERR_CC_COMMON_OK;

    cc_list_t *right_list = NULL;
    res = cc_list_split_middle(&right_list, self);
    if(res != ERR_CC_COMMON_OK) {
        printf("cc_list_split_middle res: %d\n",res);
        goto fail1;
    }
    res = cc_list_sort_merge_traditional(self, cmp);
    if(res != ERR_CC_COMMON_OK) {
        printf("cc_list_sort_merge_traditional self res: %d\n",res);
        goto fail2;
    }
    res = cc_list_sort_merge_traditional(right_list, cmp);
    if(res != ERR_CC_COMMON_OK) {
        printf("cc_list_sort_merge_traditional right_list res: %d\n",res);
        goto fail2;
    }
    res = cc_list_concat_by_cmp(self, right_list, cmp);
    if(res != ERR_CC_COMMON_OK) {
        printf("cc_list_concat_by_cmp self right_list res: %d\n",res);
        goto fail2;
    }
    cc_list_destroy(right_list);

    return ERR_CC_COMMON_OK;
fail2:
    cc_list_destroy(right_list);
fail1:
    return res;
}

int cc_list_sort_merge_bottom_up(cc_list_t *self, cc_cmp_fn_t cmp)
{
    int res = ERR_CC_COMMON_OK;
    cc_size_t block_size = 1;
    while(block_size < self->root.size) {
        cc_list_node_t *current = self->root.next;
        cc_list_t *temp_merge_list;
        res = cc_list_new(&temp_merge_list, self->remove_fn);
        if(res != ERR_CC_COMMON_OK) return res;

        while(current != &self->root) {
            cc_list_t *left,*right;
            res = cc_list_split_block(&left, self, block_size, &current);
            if(res != ERR_CC_COMMON_OK) return res;
            res = cc_list_split_block(&right, self, block_size, &current);
            if(res != ERR_CC_COMMON_OK) return res;

            // res = cc_list_concat_ab_by_cmp_to_c(temp_merge_list, left, right, cmp);
            // if(res != ERR_CC_COMMON_OK) return res;
            res = cc_list_concat_by_cmp(temp_merge_list, left, cmp);
            if(res != ERR_CC_COMMON_OK) return res;
            res = cc_list_concat_by_cmp(temp_merge_list, right, cmp);
            if(res != ERR_CC_COMMON_OK) return res;

            cc_list_destroy(left);
            cc_list_destroy(right);
        }
        res = cc_list_root_swap(self, temp_merge_list);
        if(res != ERR_CC_COMMON_OK) return res;
        cc_list_destroy(temp_merge_list);
        block_size <<= 1;
    }
    return ERR_CC_COMMON_OK;
}

int cc_list_sort_merge(cc_list_t *self, cc_cmp_fn_t cmp)
{
    return cc_list_sort_bubble_get_end(self, cmp);
}

int _cc_list_sort_merge(cc_list_t *self, cc_cmp_fn_t cmp, cc_size_t optimize)
{
    if(self == NULL || cmp == NULL) return ERR_CC_COMMON_INVALID_ARG;

    switch(optimize){
        case 1:
            return cc_list_sort_merge_traditional(self, cmp);
        case 2:
            return cc_list_sort_merge_bottom_up(self, cmp);
        // case 3:
        //     return cc_list_sort_merge_get_end(self, cmp);
        default:
            return ERR_CC_COMMON_INVALID_ARG;
    }
    return ERR_CC_COMMON_OK;
}





int cc_list_sort_check(cc_list_t *self, cc_cmp_fn_t cmp, cc_size_t *dir)
{
    if(self == NULL || cmp == NULL || dir == NULL) return ERR_CC_COMMON_INVALID_ARG;

    *dir = 0;

    if(cc_list_size(self) == 0) {
        *dir = 1;
        return ERR_CC_COMMON_OK;
    }

    int expect_dir = 0;
    int all_same = 1;

    cc_list_node_t *current = self->root.next;

    while(current->next != &self->root) {
        int cmp_result = cmp(current->data, current->next->data);

        if(cmp_result != 0) all_same = 0;

        if(expect_dir != 0) {
            if(cmp_result < 0 && expect_dir == 2) {
                *dir = 0;
                return ERR_CC_COMMON_OK;
            }
            if(cmp_result > 0 && expect_dir == 1) {
                *dir = 0;
                return ERR_CC_COMMON_OK;
            }
        } else {
            if(cmp_result < 0) expect_dir = 1; // a < b list 0 1 2 3 ~
            else if(cmp_result > 0) expect_dir = 2; // a > b list ~  3 2 1 0
        }
        current = current->next;
    }

    if(all_same) {
        *dir = 1;
    } else {
        *dir = expect_dir;
    }

    return ERR_CC_COMMON_OK;
}
