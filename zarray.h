#ifndef ZARRAY_H
#define ZARRAY_H

#define ZARRAY(T)                                                              \
  struct {                                                                     \
    T *data;                                                                   \
    size_t size;                                                               \
    size_t capacity;                                                           \
    Zone *z;                                                                   \
    int tag;                                                                   \
  }

#define zarray_init(arr, zone_ptr, t_tag)                                      \
  do {                                                                         \
    (arr).data = NULL;                                                         \
    (arr).size = 0;                                                            \
    (arr).capacity = 0;                                                        \
    (arr).z = (zone_ptr);                                                      \
    (arr).tag = (t_tag);                                                       \
  } while (0)

#define zarray_push(arr, val)                                                  \
  do {                                                                         \
    if ((arr).size >= (arr).capacity) {                                        \
      size_t new_cap = (arr).capacity == 0 ? 8 : (arr).capacity * 2;           \
      size_t elem_size = sizeof(*(arr).data);                                  \
      void *new_data;                                                          \
      if ((arr).data == NULL) {                                                \
        new_data = zone_alloc_array((arr).z, new_cap, elem_size, (arr).tag,    \
                                    (void **)&(arr).data);                     \
      } else {                                                                 \
        new_data = zone_realloc((arr).z, (arr).data, new_cap * elem_size);     \
      }                                                                        \
      if (new_data) {                                                          \
        (arr).data = new_data;                                                 \
        (arr).capacity = new_cap;                                              \
      }                                                                        \
    }                                                                          \
    if ((arr).data) {                                                          \
      (arr).data[(arr).size++] = (val);                                        \
    }                                                                          \
  } while (0)

#define zarray_set(arr, index, val)                                            \
  do {                                                                         \
    if ((index) < (arr).size) {                                                \
      (arr).data[index] = (val);                                               \
    }                                                                          \
  } while (0)

#define zarray_remove_at(arr, index)                                           \
  do {                                                                         \
    size_t idx = (index);                                                      \
    if (idx < (arr).size) {                                                    \
      for (size_t i = idx; i < (arr).size - 1; i++) {                          \
        (arr).data[i] = (arr).data[i + 1];                                     \
      }                                                                        \
      (arr).size--;                                                            \
    }                                                                          \
  } while (0)

#define zarray_remove_swap(arr, index)                                         \
  do {                                                                         \
    size_t idx = (index);                                                      \
    if (idx < (arr).size) {                                                    \
      (arr).data[idx] = (arr).data[(arr).size - 1];                            \
      (arr).size--;                                                            \
    }                                                                          \
  } while (0)

#define zarray_free(arr)                                                       \
  do {                                                                         \
    if ((arr).data) {                                                          \
      zone_free((arr).z, (arr).data);                                          \
      (arr).data = NULL;                                                       \
      (arr).size = 0;                                                          \
      (arr).capacity = 0;                                                      \
    }                                                                          \
  } while (0)

#endif // ZARRAY_H
