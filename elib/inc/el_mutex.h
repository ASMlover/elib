#ifndef __ELIB_MUTEX_HEADER_H__
#define __ELIB_MUTEX_HEADER_H__

typedef struct el_mutex_s el_mutex_t;

extern el_mutex_t* el_mutex_create(void);
extern void el_mutex_delete(el_mutex_t** mutex);
extern void el_mutex_lock(el_mutex_t* mutex);
extern void el_mutex_unlock(el_mutex_t* mutex);

#endif  /* __ELIB_MUTEX_HEADER_H__ */
