#ifndef _ROBOT_HPP
#define _ROBOT_HPP

#include "Entity.hpp"
#include <RadioTx.hpp>
#include <RadioRx.hpp>

class Robot : public Entity
{
    public:
        Robot(NxScene& scene);
        ~Robot();
        
        /** @return the world angle */
        float getAngle() const;
        
        virtual void position(float x, float y); 
        
        /** set control data */
        void radio(Packet::RadioTx::Robot& data);
        /** get robot information data */
        Packet::RadioRx::Robot radio() const;

    private:
        void initRoller();
        void initKicker();
        void initWheels();

        static NxConvexMesh* cylinder(const float length, const float radius,
                const unsigned int sides);

    private:
        NxActor* _roller;
        NxActor* _kicker;
        
        NxActor* _wheels[4];
        NxRevoluteJoint* _motors[4];
        
        Packet::RadioTx::Robot _tx;
        Packet::RadioRx::Robot _rx;

        /** center of roller from ground */
        const static float RollerHeight = .03;
        /** center of roller from center of robot */
        const static float RollerOffset = .065;
        /** roller length */
        const static float RollerLength = .07;
        /** radius of the roller */
        const static float RollerRadius = .01;

        /** width of the kicker face */
        const static float KickerFaceWidth = .05;
        /** height of the kicker face */
        const static float KickerFaceHeight = .005;
        /** depth of the kicker plate */
        const static float KickerLength = .03;
};

#endif /* _ROBOT_HPP */
