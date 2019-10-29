#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <native/sem.h>

#define TIME_UNIT 1000000000

RT_SEM sem_a;
RT_SEM sem_b;
RT_MUTEX mut_c;
RT_MUTEX mut_d_a;
RT_MUTEX mut_d_b;
RT_MUTEX mut_e_a;
RT_MUTEX mut_e_b;


void wait_for_semaphore()
{
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    rt_sem_p(sem_a, timeout);
    rt_printf("got semaphore\n");
    rt_sem_v(sem_a);
    rt_printf("released semaphore\n");
}

int task_A()
{
    rt_task_shadow(NULL, "main_task", 3, T_CPU(1));
    rt_sem_create (&sem_a, "sem", 1, S_PRIO)
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    rt_sem_p(sem_a, timeout);
    RT_TASK task_1, task_2;
    rt_task_create(&task_1, "task_1", 0, 1, T_CPU(1));
    rt_task_create(&task_2, "task_2", 0, 2, T_CPU(1));
    rt_printf("tasks created\n");
    rt_task_start(task_1, wait_for_semaphore, NULL);
    rt_task_start(task_2, wait_for_semaphore, NULL);
    rt_printf("tasks started\n");
    RTIME wait = 100*1000*1000;
    rt_printf("waiting\n");
    rt_timer_spin(wait);
    rt_printf("signaling semaphore\n");
    rt_sem_v(&sem_a);
    rt_printf("waiting\n");
    rt_timer_spin(wait);
    rt_printf("deleting semaphore\n");
    rt_sem_delete(&sem_a);
    rt_task_join(&task_1);
    rt_task_join(&task_2);
    return 0;
}


void B_low_pri_task()
{
    RTIME wait = 3*TIME_UNIT;
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    while (1)
    {
        rt_sem_p(&sem_b, timeout);
        rt_printf("low pri busy wait start\n");
        rt_timer_spin(wait);
        rt_printf("low pri busy wait stop\n");
        rt_sem_v(sem);
    }
}

void B_medium_pri_task()
{
    RTIME wait = 5*TIME_UNIT;
    RTIME sleep = 1*TIME_UNIT;
    while (1)
    {
        rt_task_sleep(sleep);
        rt_printf("medium pri busy wait start\n");
        rt_timer_spin(wait);
        rt_printf("medium pri busy wait stop\n");
    }
}

void B_high_pri_task()
{
    RTIME wait = 2*TIME_UNIT;
    RTIME sleep = 2*TIME_UNIT;
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    while (1)
    {
        rt_task_sleep(sleep);
        rt_sem_p(&sem_b, timeout);
        rt_printf("low pri busy wait start\n");
        rt_timer_spin(wait);
        rt_printf("low pri busy wait stop\n");
        rt_sem_v(&sem_b);
    }
}

void task_B()
{
    rt_sem_create (RT_SEM *sem_b, "sem", 1, S_PRIO)
    rt_task_shadow(NULL, "main_task", 4, T_CPU(1));
    RT_TASK low, medium, high;
    rt_task_create(&low, "low", 0, 1, T_CPU(1));
    rt_task_create(&medium, "medium", 0, 2, T_CPU(1));
    rt_task_create(&high, "high", 0, 3, T_CPU(1));
    rt_task_start(low, B_low_pri_task, NULL);
    rt_task_start(medium, B_medium_pri_task, NULL);
    rt_task_start(high, B_high_pri_task, NULL);
    rt_task_join(&low);
    rt_task_join(&medium);
    rt_task_join(&high);
    unsigned long duration = 10000000000; // 10 second timeout
    unsigned long endTime = rt_timer_read() + duration;
    while(1)
    {
        if(rt_timer_read() > endTime)
        {
            rt_printf("Time expired\n");
            rt_task_delete(NULL);
        }
        if(rt_task_yield())
        {
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
    }
    rt_sem_delete(&sem_b)
}


void C_low_pri_task()
{
    RTIME wait = 3*TIME_UNIT;
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    while (1)
    {
        rt_mutex_acquire(&mut_b, timeout);
        rt_printf("low pri busy wait start\n");
        rt_timer_spin(wait);
        struct rt_task_info temp;
        rt_task_inquire(NULL, &temp);
        rt_printf("low pri busy wait stop, Base prio: %i, Current prio: %i\n", temp.bprio, temp.cprio);
        rt_sem_v(sem);
    }
}

void C_medium_pri_task()
{
    RTIME wait = 5*TIME_UNIT;
    RTIME sleep = 1*TIME_UNIT;
    while (1)
    {
    RT_SEM sem;
        rt_task_sleep(sleep);
        rt_printf("medium pri busy wait start\n");
        rt_timer_spin(wait);
        struct rt_task_info temp;
        rt_task_inquire(NULL, &temp);
        rt_printf("medium pri busy wait stop, Base prio: %i, Current prio: %i\n", temp.bprio, temp.cprio);
    }
}

void C_high_pri_task()
{
    RTIME wait = 2*TIME_UNIT;
    RTIME sleep = 2*TIME_UNIT;
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    while (1)
    {
        rt_task_sleep(sleep);
        rt_mutex_acquire(&mut_c, timeout);
        rt_printf("high pri busy wait start\n");
        rt_timer_spin(wait);
        struct rt_task_info temp;
        rt_task_inquire(NULL, &temp);
        rt_printf("high pri busy wait stop, Base prio: %irt_sem_, Current prio: %i\n", temp.bprio, temp.cprio);
        rt_mutex_release(&mut_c);
    }
}


void task_C()
{
    rt_mutex_create(&mut_c, "mutex");
    rt_task_shadow(NULL, "main_task", 4, T_CPU(1));
    RT_TASK low, medium, high;
    rt_task_create(&low, "low", 0, 1, T_CPU(1));
    rt_task_create(&medium, "medium", 0, 2, T_CPU(1));
    rt_task_create(&high, "high", 0, 3, T_CPU(1));
    rt_task_start(low, C_low_pri_task, NULL);
    rt_task_start(medium, C_medium_pri_task, NULL);
    rt_task_start(high, C_high_pri_task, NULL);
    rt_task_join(&low);
    rt_task_join(&medium);
    rt_task_join(&high);
    unsigned long duration = 20 * 1000 * 1000 * 1000; // 20 second timeout
    unsigned long endTime = rt_timer_read() + duration;
    while(1)
    {
        if(rt_timer_read() > endTime)
        {
            rt_printf("Time expired\n");
            rt_task_delete(NULL);
        }
        if(rt_task_yield())
        {
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
    }

}


void D_low_pri_task()
{
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    while (1)
    {
        rt_mutex_acquire(&mut_d_a, timeout);
        rt_printf("low_pri got mutex A\n");
        rt_timer_spin(3*TIME_UNIT);
        rt_mutex_acquire(&mut_d_b, timeout);
        rt_printf("low_pri got mutex B\n")
        rt_timer_spin(3*TIME_UNIT);
        rt_mutex_release(&mut_d_b);
        rt_mutex_release(&mut_d_a);
        rt_printf("low_pri released both mutexes\n")
        rt_timer_spin(1*TIME_UNIT);
    }
}

void D_high_pri_task()
{
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    while (1)
    {
        rt_timer_sleep(1*TIME_UNIT);
        rt_mutex_acquire(&mut_d_b, timeout);
        rt_printf("high pri got mutex A\n");
        rt_timer_spin(1*TIME_UNIT);
        rt_mutex_acquire(&mut_d_a, timeout);
        rt_printf("high pri got mutex B\n");
        rt_timer_spin(2*TIME_UNIT);
        rt_mutex_release(&mut_d_a);
        rt_mutex_release(&mut_d_b);
        rt_printf("high pri released both mutexes\n");
        rt_timer_spin(1*TIME_UNIT);
    }
}

void TASK_D()
{
    rt_mutex_create(&mut_d_a, "mutex_a");
    rt_mutex_create(&mut_d_b, "mutex_b");
    RT_TASK low, high;
    rt_task_create(&low, "low", 0, 1, T_CPU(1));
    rt_task_create(&high, "high", 0, 2, T_CPU(1));
    rt_task_start(low, D_low_pri_task, NULL);
    rt_task_start(high, D_high_pri_task, NULL);
    rt_task_join(&low);
    rt_task_join(&high);
    unsigned long duration = 20 * 1000 * 1000 * 1000; // 20 second timeout
    unsigned long endTime = rt_timer_read() + duration;
    while(1)
    {
        if(rt_timer_read() > endTime)
        {
            rt_printf("Time expired\n");
            rt_task_delete(NULL);
        }
        if(rt_task_yield())
        {
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
    }
}

void E_low_pri_task()
{
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    while (1)
    {
        rt_mutex_acquire(&mut_d_a, timeout);
        rt_printf("low_pri got mutex A\n");
        rt_timer_spin(3*TIME_UNIT);
        rt_mutex_acquire(&mut_d_b, timeout);
        rt_task_set_priority(3);
        rt_printf("low_pri got mutex B\n")
        rt_timer_spin(3*TIME_UNIT);
        rt_mutex_release(&mut_d_b);
        rt_mutex_release(&mut_d_a);
        rt_task_set_priority(NULL, 1);
        rt_printf("low_pri released both mutexes\n")
        rt_timer_spin(1*TIME_UNIT);
    }
}

void E_high_pri_task()
{
    RTIME timeout = 1000*1000*1000*10;//redundantly long
    while (1)
    {
        rt_timer_sleep(1*TIME_UNIT);
        rt_mutex_acquire(&mut_d_b, timeout);
        rt_printf("high pri got mutex A\n");
        rt_timer_spin(1*TIME_UNIT);
        rt_mutex_acquire(&mut_d_a, timeout);
        rt_task_set_priority(3);
        rt_printf("high pri got mutex B\n");
        rt_timer_spin(2*TIME_UNIT);
        rt_mutex_release(&mut_d_a);
        rt_mutex_release(&mut_d_b);
        rt_task_set_priority(NULL, 2);
        rt_printf("high pri released both mutexes\n");
        rt_timer_spin(1*TIME_UNIT);
    }
}

void task_E()
{
    rt_mutex_create(&mut_e_a, "mutex_a");
    rt_mutex_create(&mut_e_b, "mutex_b");
    RT_TASK low, high;
    rt_task_create(&low, "low", 0, 1, T_CPU(1));
    rt_task_create(&high, "high", 0, 2, T_CPU(1));
    rt_task_start(low, E_low_pri_task, NULL);
    rt_task_start(high, E_high_pri_task, NULL);
    unsigned long duration = 20 * 1000 * 1000 * 1000; // 20 second timeout
    unsigned long endTime = rt_timer_read() + duration;
    while(1)
    {
        if(rt_timer_read() > endTime)
        {
            rt_printf("Time expired\n");
            rt_task_delete(NULL);
        }
        if(rt_task_yield())
        {
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
    }
}



int main()
{
    rt_print_auto_init(1);
    mlockall(MCL_CURRENT|MCL_FUTURE);
    // int revar = task_A();
    // rt_task_sleep(10*TIME_UNIT);
    //return revar;
    //task_B();
    //task_C();
    //task_D();
    //task_E();
    unsigned long duration = 20 * 1000 * 1000 * 1000; // 20 second timeout
    unsigned long endTime = rt_timer_read() + duration;
    while(1)
    {
        if(rt_timer_read() > endTime)
        {
            rt_printf("Time expired\n");
            rt_task_delete(NULL);
        }
        if(rt_task_yield())
        {
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
    }
    return 0;   
}