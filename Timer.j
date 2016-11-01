library Timer /* v1.0


*///! novjass

    Credits:

        - AGD (Author)
        - Vexorian (For the TimerUtils where I got my inspiration of making my own version of the system)
        - Wieltol (For the nice idea of protecting timers)

    |=====|
    | API |
    |=====|

        struct Timer

            integer data/*
            - the Timer custom data

          */static method new takes nothing returns Timer/*
            - allocates a new Timer

          */static method newEx takes integer data returns Timer/*
            - allocates a new Timer with a custom data initialized
              to <data>

          */static method freeExpired takes nothing returns nothing/*
            - frees the expiring Timer

          */static method stopExpired takes nothing returns nothing/*
            - stops the expiring Timer

          */static method operator expired takes nothing returns Timer/*
            - gets the expiring Timer

          */static method operator expiredData takes nothing returns integer/*
            - gets the custom data of the expiring Timer

          */static method operator expiredData= takes integer data returns nothing/*
            - sets the custom data of the expiring Timer

          */method start takes real timeout, boolean periodic, code handlerFunction returns nothing/*
            - starts a Timer instance, running <handlerFunction> after <timeout> seconds periodically
              or not depending on the <periodic> boolean value

          */method free takes nothing returns nothing/*
            - frees and stops a Timer instance

          */method stop takes nothing returns nothing/*
            - stops a Timer instance

          */method pause takes nothing returns nothing/*
            - pauses a Timer instance

          */method resume takes nothing returns nothing/*
            - resumes a Timer instance


*///! endnovjass

    private keyword Init

    struct Timer extends array

        private static integer startHandle
        private timer timers
        private timer T
        private thistype recycler
        integer data

        debug private static method debug takes string msg returns nothing
            debug call DisplayTimedTextToPlayer(GetLocalPlayer(), 0, 0, 60, "|CFFFFCC00[Timer] :|R " + msg)
        debug endmethod

        static method new takes nothing returns thistype
            local thistype this = thistype(0).recycler
            set thistype(0).recycler = .recycler
            debug set .recycler = -1
            debug call debug("Allocating Timer [" + I2S(this) + "]")
            set .T = .timers
            return this
        endmethod

        static method newEx takes integer data returns thistype
            local thistype this = new()
            set .data = data
            return this
        endmethod

        method free takes nothing returns nothing
            debug if .recycler != -1 then
                debug call debug("ERROR: Attempt to double free Timer [" + I2S(this) + "]")
                debug return
            debug endif
            set .recycler = thistype(0).recycler
            set thistype(0).recycler = this
            debug call debug("Freeing Timer [" + I2S(this) + "]")
            call PauseTimer(.T)
            set .T = null
            set .data = 0
        endmethod

        method start takes real timeout, boolean periodic, code handlerFunction returns nothing
            call TimerStart(.T, timeout, periodic, handlerFunction)
        endmethod

        method stop takes nothing returns nothing
            call TimerStart(.T, 0, false, null)
        endmethod

        method pause takes nothing returns nothing
            call PauseTimer(.T)
        endmethod

        method resume takes nothing returns nothing
            call ResumeTimer(.T)
        endmethod

        static method operator expired takes nothing returns thistype
            return thistype(GetHandleId(GetExpiredTimer()) - startHandle)
        endmethod

        static method freeExpired takes nothing returns nothing
            call expired.free()
        endmethod

        static method stopExpired takes nothing returns nothing
            call TimerStart(GetExpiredTimer(), 0, false, null)
        endmethod

        static method operator expiredData takes nothing returns integer
            return expired.data
        endmethod

        static method operator expiredData= takes integer data returns nothing
            set expired.data = data
        endmethod

        implement Init

    endstruct

    private module Init
        private static method onInit takes nothing returns nothing
            local thistype this = thistype(1)
            set thistype(1).timers = CreateTimer()
            set startHandle = GetHandleId(thistype(1).timers) - 1
            set thistype(0).recycler = 1
            set thistype(1).recycler = 2
            set thistype(8191).recycler = 0
            loop
                set this = this + 1
                set .timers = CreateTimer()
                set .recycler = this + 1
                exitwhen this == 8190
            endloop
            debug call debug("TimerStruct ready!")
        endmethod
    endmodule


endlibrary