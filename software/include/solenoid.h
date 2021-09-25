#pragma once

#include "general.h"

void solenoid_init(void);
void solenoid_task(void* args);

void solenoid_dir1_enable(bool enable);
void solenoid_dir2_enable(bool enable);
void solenoid_en1_enable(bool enable);
void solenoid_en2_enable(bool enable);