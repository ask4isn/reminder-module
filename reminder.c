#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>

#define MAX_EVENTS 16
#define MAX_MSG_LEN 128

static unsigned long event_times[MAX_EVENTS];
static char *event_msgs[MAX_EVENTS];
static int time_count;
static int msg_count;

module_param_array(event_times, ulong, &time_count, 0444);
MODULE_PARM_DESC(event_times, "задержки (сек.)");

module_param_array(event_msgs, charp, &msg_count, 0444);
MODULE_PARM_DESC(event_msgs, "сообщения (до 127 символов)");

struct reminder {
    struct hrtimer timer;
    char *msg;
};

static struct reminder *reminders = NULL;
static spinlock_t reminder_lock;

static enum hrtimer_restart reminder_handler(struct hrtimer *t)
{
    struct reminder *r = container_of(t, struct reminder, timer);
    unsigned long flags;
    spin_lock_irqsave(&reminder_lock, flags);
    pr_info("reminder: %s\n", r->msg ? r->msg : "(null)");
    spin_unlock_irqrestore(&reminder_lock, flags);
    return HRTIMER_NORESTART;
}

static int __init reminder_init(void)
{
    int i;
    if (time_count <= 0 || time_count > MAX_EVENTS) {
        pr_err("reminder: неверное кол-во времени: %d\n", time_count);
        return -EINVAL;
    }
    if (msg_count != time_count) {
        pr_err("reminder: сообщения и время не совпадают (%d != %d)\n", msg_count, time_count);
        return -EINVAL;
    }
    reminders = kzalloc(sizeof(struct reminder) * time_count, GFP_KERNEL);
    if (!reminders) {
        pr_err("reminder: не хватает памяти под напоминания :(\n");
        return -ENOMEM;
    }
    spin_lock_init(&reminder_lock);
    for (i = 0; i < time_count; i++) {
        ktime_t kt = ktime_set(event_times[i], 0);
        reminders[i].msg = kstrdup(event_msgs[i], GFP_KERNEL);
        if (!reminders[i].msg) {
            pr_err("reminder: не смог скопировать сообщение %d\n", i);
            goto fail_alloc;
        }
        if (strlen(reminders[i].msg) >= MAX_MSG_LEN) {
            reminders[i].msg[MAX_MSG_LEN-1] = '\0';
            pr_warn("reminder: сообщение #%d уменьшено до 127 символов\n", i);
        }
        hrtimer_init(&reminders[i].timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        reminders[i].timer.function = reminder_handler;
        hrtimer_start(&reminders[i].timer, kt, HRTIMER_MODE_REL);
        pr_info("reminder #%d: \"%s\" через %lu секунд\n",
            i, reminders[i].msg, event_times[i]);
    }
    return 0;

fail_alloc:
    while (--i >= 0) {
        hrtimer_cancel(&reminders[i].timer);
        kfree(reminders[i].msg);
    }
    kfree(reminders);
    return -ENOMEM;
}

static void __exit reminder_exit(void)
{
    int i;
    if (!reminders) {
        return;
    }
    for (i = 0; i < time_count; i++) {
        hrtimer_cancel(&reminders[i].timer);
        kfree(reminders[i].msg);
    }
    kfree(reminders);
    pr_info("reminder: модуль выгружен\n");
}

module_init(reminder_init);
module_exit(reminder_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Аманов Азамат");
MODULE_DESCRIPTION("Модуль для напоминаний");