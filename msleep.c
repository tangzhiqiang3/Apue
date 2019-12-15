
static inline void msleep(unsigned long ms)
{
struct timeval tv;
tv.tv_sec = ms/1000;
tv.tv_usec = (ms%1000)*1000;
select(0, NULL, NULL, NULL, &tv);
}