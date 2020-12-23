#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



FILE ** open_instance_files(char *foldername){
  char instance_filename[100] = "";
  char interventions_filename[100] = "";
  char technicians_filename[100] = "";

  strcat(instance_filename,foldername);
  strcat(interventions_filename,foldername);
  strcat(technicians_filename,foldername);

  strcat(instance_filename,"/instance");
  strcat(interventions_filename,"/interv_list");
  strcat(technicians_filename,"/tech_list");

  FILE * instance_file = fopen(instance_filename,"r");
  FILE * interventions_file = fopen(interventions_filename,"r");
  FILE * technicians_file = fopen(technicians_filename,"r");

  FILE ** files = malloc(sizeof(void*)*3);
  files[0] = instance_file;
  files[1] = interventions_file;
  files[2] = technicians_file;
  return files;
};



void close_instance_files(FILE ** files){
  fclose(files[0]);
  fclose(files[1]);
  fclose(files[2]);
};



void assign_instance_data_from_file(TTSPData * instance,FILE * instance_file){
  char first_line[100];
  char * name= malloc(sizeof(char)*100);
  int domains_amount;
  int skill_levels_amount;
  int technicians_amount;
  int interventions_amount;
  int subcontracting_budget;

  fgets(first_line,100,instance_file);
  fscanf(instance_file,"%s",name);
  fscanf(instance_file,"%d",&domains_amount);
  fscanf(instance_file,"%d",&skill_levels_amount);
  fscanf(instance_file,"%d",&technicians_amount);
  fscanf(instance_file,"%d",&interventions_amount);
  fscanf(instance_file,"%d",&subcontracting_budget);

  (*instance).name = name;
  (*instance).domains_amount = domains_amount;
  (*instance).skill_levels_amount = skill_levels_amount;
  (*instance).technicians_amount = technicians_amount;
  (*instance).interventions_amount = interventions_amount;
  (*instance).subcontracting_budget = subcontracting_budget;
};



void assign_interventions_list_from_file(Intervention * interventions_list,FILE * interventions_file,int interventions_amount,int domains_amount,int skill_levels_amount){
  Intervention * intervention;
  char first_line[1000];
  char preds_buffer[10];
  int number;
  int time_needed;
  int previous_interventions_amount;
  int * previous_interventions_numbers;
  int priority;
  int subcontracting_cost;
  int * required_technicians_amounts;

  fgets(first_line,1000,interventions_file);

  for(int intervention_index = 1 ; intervention_index < interventions_amount + 1 ; intervention_index++){
    intervention = &interventions_list[intervention_index-1];

    fscanf(interventions_file,"%d",&number);
    fscanf(interventions_file,"%d",&time_needed);

    previous_interventions_amount = 0;
    previous_interventions_numbers = malloc(sizeof(int)*interventions_amount);

    fscanf(interventions_file,"%s",preds_buffer);
    fscanf(interventions_file,"%s",preds_buffer);

    while(isdigit(preds_buffer[0])){
      previous_interventions_numbers[previous_interventions_amount] = atoi(preds_buffer);
      previous_interventions_amount += 1;

      fscanf(interventions_file,"%s",preds_buffer);
    }

    fscanf(interventions_file,"%d",&priority);
    fscanf(interventions_file,"%d",&subcontracting_cost);

    required_technicians_amounts = malloc(sizeof(int)*domains_amount*skill_levels_amount);

    for(int required_technicians_index = 0 ; required_technicians_index < domains_amount*skill_levels_amount ; required_technicians_index++){
      fscanf(interventions_file,"%d",&required_technicians_amounts[required_technicians_index]);
    }

    (*intervention).number = number;
    (*intervention).time_needed = time_needed;
    (*intervention).previous_interventions_amount = previous_interventions_amount;
    (*intervention).previous_interventions_numbers = previous_interventions_numbers;
    (*intervention).priority = priority;
    (*intervention).subcontracting_cost = subcontracting_cost;
    (*intervention).required_technicians_amounts = required_technicians_amounts;
  }
};



void assign_technicians_list_from_file(Technician * technicians_list,FILE * technicians_file,int technicians_amount,int domains_amount){
  Technician * technician;
  char first_line[1000];
  char unav_buffer[10];
  int number;
  int * skill_levels;
  int unavailable_days_amount;
  int * unavailability;

  fgets(first_line,1000,technicians_file);

  for(int technician_index = 1 ; technician_index < technicians_amount + 1 ; technician_index++){
    technician = &technicians_list[technician_index-1];

    fscanf(technicians_file,"%d",&number);

    skill_levels = malloc(sizeof(int)*domains_amount);

    for(int domain_index = 0 ; domain_index < domains_amount ; domain_index++){
      fscanf(technicians_file,"%d",&skill_levels[domain_index]);
    }

    unavailable_days_amount = 0;
    unavailability = malloc(sizeof(int)*30);

    fscanf(technicians_file,"%s",unav_buffer);
    fscanf(technicians_file,"%s",unav_buffer);

    while(isdigit(unav_buffer[0])){
      unavailability[unavailable_days_amount] = atoi(unav_buffer);
      unavailable_days_amount += 1;

      fscanf(technicians_file,"%s",unav_buffer);
    }

    (*technician).number = number;
    (*technician).skill_levels = skill_levels;
    (*technician).unavailable_days_amount = unavailable_days_amount;
    (*technician).unavailability = unavailability;
  }
};



TTSPData * create_instance_from_folder(char * foldername){
  FILE ** files = open_instance_files(foldername);
  FILE * instance_file = files[0];
  FILE * interventions_file = files[1];
  FILE * technicians_file = files[2];

  TTSPData * instance = malloc(sizeof(TTSPData));

  assign_instance_data_from_file(instance,instance_file);

  int interventions_amount = (*instance).interventions_amount;
  int technicians_amount = (*instance).technicians_amount;
  int domains_amount = (*instance).domains_amount;
  int skill_levels_amount = (*instance).skill_levels_amount;

  Intervention * interventions_list = malloc(sizeof(Intervention)*interventions_amount);
  Technician * technicians_list = malloc(sizeof(Technician)*technicians_amount);

  (*instance).interventions_list = interventions_list;
  (*instance).technicians_list = technicians_list;

  assign_interventions_list_from_file(interventions_list,interventions_file,interventions_amount,domains_amount,skill_levels_amount);
  assign_technicians_list_from_file(technicians_list,technicians_file,technicians_amount,domains_amount);

  close_instance_files(files);
  return instance;
};



void print_intervention(Intervention * intervention,int domains_amount,int skill_levels_amount){
  printf("-> Interv #%d\n",(*intervention).number);
  printf("Time = %d Priority = %d Cost = %d\n",(*intervention).time_needed,(*intervention).priority,(*intervention).subcontracting_cost);

  for(int domain_index = 0 ; domain_index < domains_amount ; domain_index++){
    printf("Number of technicians required for domain %d ->",domain_index+1);

    for(int level_index = 0 ; level_index < skill_levels_amount ; level_index++){
      printf(" %d",(*intervention).required_technicians_amounts[ domain_index*skill_levels_amount + level_index ]);
    }
    printf("\n");
  }
  if((*intervention).previous_interventions_amount > 0){
    printf("Predecessors =");
    for(int predecessor_index = 0 ; predecessor_index < (*intervention).previous_interventions_amount ; predecessor_index++){
      printf(" %d",(*intervention).previous_interventions_numbers[predecessor_index]);
    }
    printf("\n");
  }
  else{
    printf("No Predecessors\n");
  }
};



void print_technician(Technician * technician,int domains_amount){
  printf("-> Tech #%d\n",(*technician).number);
  printf("Skills (mastered level per domain) ->");

  for(int domain_index = 0 ; domain_index < domains_amount ; domain_index++){
    printf(" %d",(*technician).skill_levels[domain_index]);
  }
  printf("\n");

  if((*technician).unavailable_days_amount > 0){
    printf("Not available on day(s) ->");
    for(int unav_index = 0 ; unav_index < (*technician).unavailable_days_amount ; unav_index++){
      printf(" %d",(*technician).unavailability[unav_index]);
    }
    printf("\n");
  }
  else{
    printf("Always available\n");
  }
};



void print_instance(TTSPData * instance){
  printf("///////////// Instance %s /////////////\n",(*instance).name);
  printf("#Interventions = %d\n",(*instance).interventions_amount);
  printf("#Technicians = %d\n",(*instance).technicians_amount);
  printf("#Domains / #Levels = %d / %d\n",(*instance).domains_amount,(*instance).skill_levels_amount);
  printf("Outsourcing budget = %d\n",(*instance).subcontracting_budget);
  printf("\n");

  printf("----------------------------------\n");
  printf("--------- INTERVENTIONS ----------\n");
  printf("----------------------------------\n");

  Intervention * intervention;
  for(int intervention_index = 0 ; intervention_index < (*instance).interventions_amount ; intervention_index++){
    intervention = &(*instance).interventions_list[intervention_index];
    print_intervention(intervention,(*instance).domains_amount,(*instance).skill_levels_amount);
  }
  printf("\n");

  printf("----------------------------------\n");
  printf("---------- TECHNICIANS -----------\n");
  printf("----------------------------------\n");

  Technician * technician;
  for(int technician_index = 0 ; technician_index < (*instance).technicians_amount ; technician_index++){
    technician = &(*instance).technicians_list[technician_index];
    print_technician(technician,(*instance).domains_amount);
  }

  printf("/////////////////////////////////////////\n");
};
