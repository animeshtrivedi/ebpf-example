#define _GNU_SOURCE         /* See feature_test_macros(7) */

#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <linux/bpf.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

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
int bpf(enum bpf_cmd cmd, union bpf_attr *attr, unsigned int size)
{
    return syscall(__NR_bpf, cmd, attr, size);
}

int bpf_create_map(enum bpf_map_type map_type,
                   unsigned int key_size,
                   unsigned int value_size,
                   unsigned int max_entries)
{
  
  union bpf_attr *attr = NULL;
  attr =  (union bpf_attr *) calloc(sizeof(*attr), 1);
  printf("Size is %lu all zero\n", sizeof(*attr));
  attr->map_type    = map_type;
  attr->key_size    = key_size;
  attr->value_size  = value_size;
  attr->max_entries = max_entries;
  attr->map_name[0] = '\0';
//  CHECK_ATTR(BPF_MAP_CREATE);
  return bpf(BPF_MAP_CREATE, attr, sizeof(*attr));
}

int main(){

	printf("Hello world!\n");
        //The key size is fixed for array to be <=4 bytes
	int ret = bpf_create_map(BPF_MAP_TYPE_ARRAY, 4, 64, 1024);
//	int ret = bpf_create_map(BPF_MAP_TYPE_HASH, 64, 64, 1024);
	int _err = errno;
	printf("ret code: %d erno %d \n", ret, _err);
	return 0;    
}
