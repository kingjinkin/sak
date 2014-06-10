#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    "stm_idle_event.h"
#include    "stm.h"
#include "common.h"

#include <pthread.h>

enum BombSignals {                          /* all signals for the Bomb FSM */
    UP_SIG = STM_USER_SIG,
    DOWN_SIG,
    ARM_SIG,
    TICK_SIG
};

typedef struct TickEvtTag {
    STATE_EVENT     super;                       /* derive from the QEvt structure */
    RAW_U8  fine_time;                           /* the fine 1/10 s counter */

} TickEvt;

typedef struct Bomb4Tag {

    ACTIVE_EVENT_STRUCT a1;                                       /* derive from QFsm */
    RAW_U32             timeout;                    /* number of seconds till explosion */
    RAW_U8          code;              /* currently entered code to disarm the bomb */
    RAW_U8          defuse;                /* secret defuse code to disarm the bomb */
    RAW_U8          internal_priority;                /* secret defuse code to disarm the bomb */

} Bomb4;

static void Bomb4_ctor(Bomb4 *me, RAW_U8 defuse);

static RAW_U16 Bomb4_initial(Bomb4 *me,         STATE_EVENT  *e);
static RAW_U16 Bomb4_setting(Bomb4 *me,     STATE_EVENT  *e);
static RAW_U16 Bomb4_timing (Bomb4 *me,     STATE_EVENT  *e);


static Bomb4 l_bomb;                                       /* time bomb FSM */
static Bomb4 l_bomb2;                                       /* time bomb FSM */


/*--------------------------------------------------------------------------*/
                                        /* the initial value of the timeout */
#define INIT_TIMEOUT   10

/*..........................................................................*/
static void Bomb4_ctor(Bomb4 *me, RAW_U8 defuse) {

    FSM_CONSTRUCTOR(&me->a1.super, (stm_state_handler)&Bomb4_initial);/* superclass ctor */
    me->defuse = defuse;    /* the defuse code is assigned at instantiation */
}
/*..........................................................................*/
static RAW_U16 Bomb4_initial(Bomb4 *me, STATE_EVENT  *e) {
    (void)e;
    me->timeout = INIT_TIMEOUT;
    return STM_TRAN(&Bomb4_setting);
}


/*..........................................................................*/
static RAW_U16 Bomb4_setting(Bomb4 *me, STATE_EVENT  *e) {
    switch (e->sig) {

        case STM_ENTRY_SIG: {
            me->code = 0;                          /* clear the defuse code */
            return STM_RET_HANDLED;
        }

        case UP_SIG: {
            
            ++me->timeout;
            //vc_port_printf("internal priority is %d\n", me->internal_priority);
            vc_port_printf("me->timeout is %d\n", me->timeout);
       
            return STM_RET_HANDLED;
        }
        case DOWN_SIG: {
            if (me->timeout > 1) {
                --me->timeout;
                vc_port_printf("me->timeout is %d\n", me->timeout);
            }
            return STM_RET_HANDLED;
        }
        case ARM_SIG: {
            return STM_TRAN(&Bomb4_timing);         /* transition to "timing" */
        }

        case STM_EXIT_SIG:
            vc_port_printf("setting state exit\n");
            return STM_RET_HANDLED;

    }
    return STM_RET_IGNORED;
}





/*..........................................................................*/
static RAW_U16 Bomb4_timing(Bomb4 *me, STATE_EVENT *e) {
    switch (e->sig) {
        case STM_ENTRY_SIG: {
            vc_port_printf("timing state enter\n");
            me->code = 0;                          /* clear the defuse code */
            return STM_RET_HANDLED;
        }
        case UP_SIG: {
            me->code <<= 1;
            me->code |= 1;
            return STM_RET_HANDLED;
        }
        case DOWN_SIG: {
            me->code <<= 1;
            return STM_RET_HANDLED;
        }
        case ARM_SIG: {
            if (me->code == me->defuse) {
                return STM_TRAN(&Bomb4_setting);
            }
            return STM_RET_HANDLED;
        }
        case TICK_SIG: {
            if (((TickEvt const *)(e->which_pool))->fine_time == 0) {
                --me->timeout;
               vc_port_printf("me->timeout is %d\n", me->timeout);

                if (me->timeout == 0) {
                    vc_port_printf("boom aaaaaa^^^^^^\n");
                    exit(0);
                }
            }
            return STM_RET_HANDLED;
        }
    }
    return STM_RET_IGNORED;
}


static IDLE_QUEUE_MSG q1[3];
static IDLE_QUEUE_MSG q2[3];


ACTIVE_EVENT_STRUCT_CB active_idle_task[] = {
    { 0, 0, 0},
    { &l_bomb.a1, q1, ARRAY_SIZE(q1)},
    { &l_bomb2.a1, q2, ARRAY_SIZE(q2)}
   
};


void test_event_task2(void * pParam)
{
    /*check whether MAX_IDLE_EVENT_TASK is valid*/
    RAW_ASSERT((ARRAY_SIZE(active_idle_task) - 1) == MAX_IDLE_EVENT_TASK);

    idle_event_init();

    Bomb4_ctor(&l_bomb, 0xd);       /* the secret defuse code, 1101 binary */
    Bomb4_ctor(&l_bomb2, 0xd);       /* the secret defuse code, 1101 binary */

    l_bomb.internal_priority = 1;
    l_bomb2.internal_priority = 2;


    fsm_init(&l_bomb.a1.super, 0);
    fsm_init(&l_bomb2.a1.super, 0);

     vc_port_printf("Time Bomb (raw os fsm case)\n"
           "Press 'u'   for UP   event\n"
           "Press 'd'   for DOWN event\n"
           "Press 'a'   for ARM  event\n"
           "Press <Esc> to quit.\n");

    while (1) {

        static TickEvt tick_evt = { TICK_SIG, 0, 0, 0 };

        raw_sleep(RAW_TICKS_PER_SECOND / 20);  

        if (++tick_evt.fine_time == 10) {
            tick_evt.fine_time = 0;
        }

        idle_event_post(&l_bomb.a1, TICK_SIG, (RAW_HDL)(&tick_evt));


//         if (_kbhit())
         {
            static STATE_EVENT  up_evt   = { UP_SIG,   0, 0 };
            static STATE_EVENT  down_evt = { DOWN_SIG, 0, 0 };
            static STATE_EVENT  arm_evt  = { ARM_SIG,  0, 0 };
            STATE_EVENT  *e = 0;

            switch (getchar()) {
                case 'u': {                                     /* UP event */
                    vc_port_printf("\nUP  : ");
                    e = &up_evt;                   /* generate the UP event */
                    break;
                }
                case 'd': {                                   /* DOWN event */
                    vc_port_printf("\nDOWN: ");
                    e = &down_evt;               /* generate the DOWN event */
                    break;
                }
                case 'a': {                                    /* ARM event */
                    vc_port_printf("\nARM : ");
                    e = &arm_evt;                 /* generate the ARM event */
                    break;
                }
                case '\33': {                                  /* <Esc> key */
                    vc_port_printf("\nESC : Bye! Bye!");
                    exit(0);
                    break;
                }
            }
            if (e != 0) {            /* keyboard event available? */
               
                idle_event_post(&l_bomb.a1, e->sig, 0);
            }

            }
    }


}

static void* idle_run_proc(void * arg)
{
    idle_run();
    return NULL;
}

void idle_event_test()
{
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, idle_run_proc, NULL);
    if (ret) {
        printf("create thread failed");
    }

    test_event_task2(NULL);

    ret = pthread_join(thread, NULL);
    if (ret) {
        printf("join thread failed");
    }

}



