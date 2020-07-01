#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PIN  11 /* P1-18 */
#define POUT 10 /* P1-07 */

//@pin --> you have to pass here gpio pin number -- which pin you want export
//function is success return value is 0 function is faile return value -1
int KM_GPIO_Export(int pin);

//@pin --> you have to pass here gpio pin number -- which pin you want export
//@dir --> if you want chose input direction you have to pass 0  output direction you have to pass 1
//function is success return value is 0 function is faile return value -1
int KM_GPIO_Direction(int pin, int dir);


//@pin --> you have to pass here gpio pin number -- which pin you want unexport
//function is success return value is 0 function is faile return value -1
int KM_GPIO_Unexport(int pin);


//@pin --> you have to pass here gpio pin number -- which pin data  you want read
//function is success return value is 0 function is faile return value -1
int KM_GPIO_Read(int pin);


//@pin --> you have to pass here gpio pin number -- which pin data  you want write
//@value -->value 1 or 0 
//function is success return value is 0 function is faile return value -1
int KM_GPIO_Write(int pin, int value);
