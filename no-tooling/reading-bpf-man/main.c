#define _GNU_SOURCE         /* See feature_test_macros(7) */

#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <linux/bpf.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static inline __u64 ptr_to_u64(const void *ptr)
{
    return (__u64) (unsigned long) ptr;
}


#if 0
#define BPF_MAP_CREATE_LAST_FIELD map_name

#define CHECK_ATTR(CMD) \
	memchr_inv((void *) &attr->CMD##_LAST_FIELD + \
		   sizeof(attr->CMD##_LAST_FIELD), 0, \
		   sizeof(*attr) - \
		   offsetof(union bpf_attr, CMD##_LAST_FIELD) - \
		   sizeof(attr->CMD##_LAST_FIELD)) != NULL
#endif

// there is no explicit system call called bpf(...)
int syscall_bpf(enum bpf_cmd cmd, union bpf_attr *attr, unsigned int size)
{
    return syscall(__NR_bpf, cmd, attr, size);
}

int bpf_create_map(union bpf_attr *attr, enum bpf_map_type map_type,
                   unsigned int key_size,
                   unsigned int value_size,
                   unsigned int max_entries)
{
    attr->map_type    = map_type;
    attr->key_size    = key_size;
    attr->value_size  = value_size;
    attr->max_entries = max_entries;
    attr->map_name[0] = '\0';
//  CHECK_ATTR(BPF_MAP_CREATE);
    return syscall_bpf(BPF_MAP_CREATE, attr, sizeof(*attr));
}

int bpf_update_elem(union bpf_attr *attr, int fd, const void *key, const void *value,
                    uint64_t flags)
{

    attr->map_fd = fd;
    attr->key    = ptr_to_u64(key);
    attr->value  = ptr_to_u64(value);
    attr->flags  = flags;
    return syscall_bpf(BPF_MAP_UPDATE_ELEM, attr, sizeof(*attr));
}

int bpf_lookup_elem(union bpf_attr *attr, int fd, const void *key, void *value)
{
    attr->map_fd = fd;
    attr->key    = ptr_to_u64(key);
    attr->value  = ptr_to_u64(value);
    return syscall_bpf(BPF_MAP_LOOKUP_ELEM, attr, sizeof(*attr));
}

int bpf_delete_elem(union bpf_attr *attr, int fd, const void *key)
{
    attr->map_fd = fd;
    attr->key = ptr_to_u64(key);
    return syscall_bpf(BPF_MAP_DELETE_ELEM, attr, sizeof(*attr));
}


int main(){

    printf("Hello world!\n");
    union bpf_attr *attr = NULL;
    attr =  (union bpf_attr *) calloc(sizeof(*attr), 1);
    printf("Size is %lu all zero\n", sizeof(*attr));

    //The key size is fixed for array to be <=4 bytes
    int ret = bpf_create_map(attr, BPF_MAP_TYPE_ARRAY, 4, 4, 1024);
    int _err = errno;
    printf("map create code: %d errno %d \n", ret, _err);
    if(ret < 0){
        printf("Aborting\n");
        return ret;
    }
    int fd = ret;
    // now insert a new element with the given fd
    int key = 2;
    int value = 1986;
    bzero(attr, sizeof(*attr));
    printf("inserting key: %d value %d \n", key, value);
    ret = bpf_update_elem(attr, fd, &key, &value, BPF_ANY);
    _err = errno;
    printf("map update code: %d errno %d \n", ret, _err);
    if(ret < 0){
        printf("Aborting\n");
        return ret;
    }

    // fetch the value now
    int new_value;
    bzero(attr, sizeof(*attr));
    printf("fetching the value for the key %d \n", key);
    ret = bpf_lookup_elem(attr, fd, &key, &new_value);
    _err = errno;
    printf("map lookup code: %d errno %d \n", ret, _err);
    if(ret < 0){
        printf("Aborting\n");
        return ret;
    }
    printf("The fetched value is : %d \n", new_value);
    assert(value == new_value);

#if 0
    // from the man page: map_delete_elem() fails with the error EINVAL, since elements cannot be deleted.

    //lets delete the value now
    bzero(attr, sizeof(*attr));
    printf("attempting to delete key %d \n", key);
    ret = bpf_delete_elem(attr, fd, &key);
    _err = errno;
    printf("map delete code: %d errno %d \n", ret, _err);
    if(ret < 0){
        printf("Aborting\n");
        return ret;
    }
#endif
    // with the hash I can use any key size, this below of 64 * 64 * 1024 works.
    //int ret = bpf_create_map(BPF_MAP_TYPE_HASH, 64, 64, 1024);
    //int _err = errno;
    //printf("ret code: %d errno %d \n", ret, _err);
    printf("End of the program\n");
    return 0;
}
