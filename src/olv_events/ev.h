
#ifndef OLV_EVENTS
#define OLV_EVENTS

typedef void (*olv_event_func)(olv_event_pool* evp);

typedef struct {
  unsigned char id;
  unsigned char enabled;
  olv_event_func func;
  unsigned long ttn;
} olv_event_pool;

#endif

