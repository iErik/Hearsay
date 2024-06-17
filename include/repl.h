#pragma once

#include "lexer.h"
#include "common.h"

#define INPUT_MAX 1000

lexer* readLine (stream from);

void startRepl (stream in, stream out);
