#ifndef VERTEX_PARSER_H
#define VERTEX_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser_types.h"

int allocate_vertex_memory(Vertex* vertex);
int obj_processing_by_vertex(Vertex* vertex, FILE* fptr);

#endif  //  VERTEX_PARSER_H