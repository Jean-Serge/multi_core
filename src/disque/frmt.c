#include "../include/hardware.h"
#include "drive.h"
#include "volume.h"
#include "context.h"

void write_mbr(){
	struct mbr_s *mbr;
	mbr = get_mbr();
	mbr->magic = MBR_MAGIC;
	mbr->nvol = 0;
	save_mbr();
}

int frmt(char **argc, int x){
	int i;
	int j;
	struct disk_info_s *dinfo = get_disk_info();

	printf("core %d\n", _in(CORE_ID));
	for(i = 0; i < dinfo->nb_cyl; i++){
		for(j = 0 ; j < dinfo->nb_sec; j++){
		  format_sector(i, j, 1, 0);
		}
	}
	write_mbr();
	printf("Fini de formater\n");
	return 0;
}

int main(){
  mkhd();
  create_ctx(2000, frmt, NULL, 0, "frmt");
  /* frmt(NULL, 0); */
  start_sched();
  return 0;
}
