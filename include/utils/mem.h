#pragma once

#define sFree(ptr) safeFree((void**)ptr)

void safeFree (void** ptr);
