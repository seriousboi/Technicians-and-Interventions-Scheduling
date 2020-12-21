/*The structures are pretty self explanatory*/



typedef struct Intervention{
  int number;
  int time_needed;
  int previous_interventions_amount;
  int * previous_interventions_numbers;
  int priority;
  int subcontracting_cost;
  int * required_technicians_amounts;
}Intervention;



typedef struct Technician{
  int number;
  int * skill_levels;
  int unavailable_days_amount;
  int * unavailability;
}Technician;



typedef struct TTSPData{
  char * name;
  int domains_amount;
  int skill_levels_amount;
  int technicians_amount;
  int interventions_amount;
  int subcontracting_budget;
  Intervention * interventions_list;
  Technician * technicians_list;
}TTSPData;
