#ifndef __TARGET_FILE_H__
#define __TARGET_FILE_H__

void TargetFileRead(void *buf, size_t size, int count, FILE *fp);
void TargetFileWrite(void *buf, size_t size, int count, FILE *fp);

#endif
