#define PRAVY  1 //v pripade leveho motoru zakomentujte
#define AKTIVNI_ENDSTOP 1 //lze jednoduse invertovat v pripade invertovaneho koncaku
#define CANBUS
#define CLOSEDLOOP




#ifdef PRAVY
#define SMER_OTACENI 1 //smer homovani je opacny
#else
#define SMER_OTACENI -1
#endif
#define ENDSTOP PA5
#define POMER_PREVODOVKA 300 //kolik stupnu na motoru je treba na jeden stupen na vystupu prevodovky, nutno nakonfigurovat dle pouzite prevodovky
#define FEEDRATE 40
#define FEEDRATE_HOMOVANI 30
#define ODJETI_Z_KONCAKU 20
#define TIMEOUT_HOMOVANI 5000 //hodnota v ms
#define LIMIT_POHYBU 120 //limit ve stupnich