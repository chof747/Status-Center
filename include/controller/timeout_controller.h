#ifndef TIMEOUT_CONTROLLER_H
#define TIMEOUT_CONTROLLER_H

class TimeOutController
{
    public:

        TimeOutController(unsigned long timeout);
        void restartTimer();
        bool checkTimeForTimeOut();


    private:
        unsigned long referenceTime;

        /**
         * Timeout in milliseconds
         */
        unsigned long timeout;

};


#endif //TIMEOUT_CONTROLLER_H