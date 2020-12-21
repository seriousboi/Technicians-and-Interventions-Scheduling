#include <stdio.h>
#include "structures.h"
#include "parser.h"



void main(int arguments_amount, char ** arguments_vector) {
  TTSPData * instance = create_instance_from_folder(arguments_vector[1]);
  print_instance(instance);
}
