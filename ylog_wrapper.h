#ifndef YLOG_WRAPPER_H_ 
#define YLOG_WRAPPER_H_
#ifdef __cplusplus
extern "C" {
#endif
struct YLogWrapper
{
    void *internal_;
    void (*W)(const struct YLogWrapper *log, const char *codefile, const int codeline, const int level, const char *info, const char *value);
};
extern struct YLogWrapper *NewYLog(const int level, const char *logfile, const int type);
extern void DeleteYLog(struct YLogWrapper *log);
#ifdef __cplusplus
};
#endif
#endif
