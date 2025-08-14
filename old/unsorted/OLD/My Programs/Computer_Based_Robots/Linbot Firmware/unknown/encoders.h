#include <stdint.h>

int64_t cntL , cntR;
int8_t dirL , dirR;

void initENC(void);
void setDirL(int8_t mode);
void setDirR(int8_t mode);
int64_t getDisL(void);
int64_t getDisR(void);
