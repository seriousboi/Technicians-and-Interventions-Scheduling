/*The structures are pretty self explanatory*/



typedef struct{
    char * name;
    int domains_amount;
    int skill_levels_amount;
    int technicians_amount;
    int interventions_amount;
    int subcontracting_budget;
    struct Intervention *interventions_list;
    struct Technician *technicians_list;
}TTSPData;



typedef struct{
  int number;
  int time_needed;
  int *previous_interventions_number;
  int priority;
  int subcontracting_cost;
  int *required_technicians_amount;
}Intervention;



typedef struct{
  int number;
  int *skill_levels;
  int *unavailability;
}Technician;
