/*vex-vision-config:begin*/
#include "v5.h"
#include "v5_vcs.h"
vex::vision::signature SIG_1 = vex::vision::signature (1, -327, 203, -62, -3709, -3389, -3549, 3.8, 1);
vex::vision::signature SIG_2 = vex::vision::signature (2, 7229, 8807, 8018, -1153, -729, -941, 3.2, 1);
vex::vision::signature SIG_3 = vex::vision::signature (3, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::code ONTHING = vex::vision::code (SIG_1, SIG_2 );
vex::vision Vision1 = vex::vision (vex::PORT20, 58, SIG_1, SIG_2, SIG_3, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/