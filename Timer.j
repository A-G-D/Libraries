library Timer /*


    */uses /*

    */Initializer /*


    *///! novjass

    Author: AGD

    Credits:
        - Vexorian (For the TimerUtils where I got my inspiration of making my own version of the system)
        - Wieltol (For the nice idea of encapsullating timers)

    |=====|
    | API |
    |=====|

        struct Timer/*

          */Timer data  /* the Timer custom data

          */static method operator new takes nothing returns Timer/*
                - allocates a new Timer

          */static method newEx takes integer data returns Timer/*
                - allocates a new Timer with a custom data initialized
                  to <data>

          */static method operator expired takes nothing returns Timer/*
                - gets the expiring Timer

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

    globals
    /*  The total number of timers in the stack
        Maximum allowed value: 8190
        Suggested value range: 250 to 500        */
        private integer MAX_TIMER_COUNT = 256
    endglobals

    struct Timer extends array

        private static integer startHandle
        private static thistype alloc
        private timer timers
        private thistype recycler
        thistype data

        debug private static method debug takes string msg returns nothing
            debug call DisplayTimedTextToPlayer(GetLocalPlayer(), 0, 0, 60, "|CFFFFCC00[Timer]|R " + msg)
        debug endmethod

        static method operator new takes nothing returns thistype
            debug if thistype(0).recycler == 0 then
                debug call debug("|CFFFF0000ERROR: Attempt to allocate more than " + I2S(MAX_TIMER_COUNT) + " instances|R")
                debug return 0
            debug endif
            set alloc = thistype(0).recycler
            set thistype(0).recycler = alloc.recycler
            debug set alloc.recycler = -1
            debug call debug("Allocating Timer [" + I2S(alloc) + "]")
            return alloc
        endmethod

        static method newEx takes integer data returns thistype
            set new.data = data
            return alloc
        endmethod

        method free takes nothing returns nothing
            debug if .recycler != -1 then
                debug call debug("|CFFFF0000ERROR: Attempt to double free Timer [" + I2S(this) + "]|R")
                debug return
            debug endif
            set .recycler = thistype(0).recycler
            set thistype(0).recycler = this
            call PauseTimer(.timers)
            set .data = 0
            debug call debug("Freeing Timer [" + I2S(this) + "]")
        endmethod

        method start takes real timeout, boolean periodic, code c returns nothing
            debug if .recycler != -1 then
                debug call debug("|CFFFF0000ERROR: Attempt to start freed Timer [" + I2S(this) + "]|R")
                debug return
            debug endif
            call TimerStart(.timers, timeout, periodic, c)
        endmethod

        method stop takes nothing returns nothing
            debug if .recycler != -1 then
                debug call debug("|CFFFF0000ERROR: Attempt to stop freed Timer [" + I2S(this) + "]|R")
                debug return
            debug endif
            call TimerStart(.timers, 0, false, null)
        endmethod

        method pause takes nothing returns nothing
            debug if .recycler != -1 then
                debug call debug("|CFFFF0000ERROR: Attempt to pause freed Timer [" + I2S(this) + "]|R")
                debug return
            debug endif
            call PauseTimer(.timers)
        endmethod

        method resume takes nothing returns nothing
            debug if .recycler != -1 then
                debug call debug("|CFFFF0000ERROR: Attempt to resume freed Timer [" + I2S(this) + "]|R")
                debug return
            debug endif
            call ResumeTimer(.timers)
        endmethod

        static method operator expired takes nothing returns thistype
            return GetHandleId(GetExpiredTimer()) - startHandle
        endmethod

        private static method init takes nothing returns nothing
            local thistype this = thistype(1)
            set MAX_TIMER_COUNT = IMinBJ(8190, MAX_TIMER_COUNT)
            set thistype(1).timers = CreateTimer()
            set startHandle = GetHandleId(thistype(1).timers) - 1
            set thistype(0).recycler = 1
            set thistype(1).recycler = 2
            set thistype(MAX_TIMER_COUNT).recycler = 0
            loop
                set this = this + 1
                set .timers = CreateTimer()
                exitwhen this == MAX_TIMER_COUNT
                set .recycler = this + 1
            endloop
            debug call debug("TimerStruct ready! (" + I2S(this) + " Timers available in stock)")
        endmethod

        implement Initializer

    endstruct


endlibrary
