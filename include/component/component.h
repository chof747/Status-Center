#ifndef COMPONENT_H
#define COMPONENT_H

class Component
{

    public:
        virtual void setup() = 0;
        virtual void loop()  = 0;

};

#endif //COMPONENT_G