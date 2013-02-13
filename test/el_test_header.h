#ifndef __ELIB_TEST_HEADER_H__
#define __ELIB_TEST_HEADER_H__

#if (defined(_WIN32) || defined(_WIN64))
  #define __func__  __FUNCTION__
#endif 

extern void el_test_time(void* arg);

#endif  /* __ELIB_TEST_HEADER_H__ */
