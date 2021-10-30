#pragma once
#include "header.h"

void readsector(dword LDA_LOW, dword LDA_HIGH, word segment, word offset);
void initFsInfo(void);