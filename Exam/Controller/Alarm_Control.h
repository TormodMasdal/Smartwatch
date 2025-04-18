#ifndef ALARM_CONTROL_H
#define ALARM_CONTROL_H

class Alarm_Control
{
public:
    int Set_Alarm(int &time);
    void Snooze_Alarm(int &time);
    void Mute_Alarm(bool &buzz);
    void Disable_Alarm(bool &enabled);
    void Enable_Alarm(bool &enabled);
};

#endif