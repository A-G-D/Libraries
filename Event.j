library Event /*


    */requires /*

    */Table         /* http://www.hiveworkshop.com/forums/jass-resources-412/snippet-new-table-188084/
    */LinkedList    /*
    */Alloc         /*
    */Initializer   /*

    *///! novjass

    |=========|
    | Credits |
    |=========|

        Author: AGD
        Dependencies: Bribe (Table)


    |-----|
    | API |
    |-----|

        struct Event

            boolean enabled             /* Determines if the Event instance is enabled or not */
            readonly boolean check      /* Checks the evaluation of an Event */
            readonly Event fired        /* The currently fired Event instance

          */static method create takes nothing returns Event/*
            - Creates a new Event

          */method destroy takes nothing returns nothing/*
            - Destroys an Event

          */method clear takes nothing returns Event/*
            - Clears all the codes and triggers attached to an Event

          */method register takes code c returns Event/*
            - Registers a code to an Event

          */method unregister takes code c returns Event/*
            - Unregisters a code from an Event

          */method registerTrigger takes trigger t returns Event/*
            - Registers a trigger to an Event

          */method unregisterTrigger takes trigger t returns Event/*
            - Unregisters a trigger from an Event

          */method evaluate takes nothing returns Event/*
            - Evaluates all codes registered to the Event

          */method execute takes nothing returns Event/*
            - Fires all triggers registered to the Event

          */method fire takes nothing returns Event/*
            - Fires all codes and triggers registered to the Event

    *///! endnovjass

    private struct TriggerList extends array
        implement LinkedListT
    endstruct

    struct Event extends array

        readonly static thistype fired = 0
        private static real eventTrigger = 0.00
        private static HashTable table
        private static Table tList

        implement AllocT

        static method create takes nothing returns thistype
            local thistype this = allocate()
            local Table t = table[this]
            set t.trigger[0] = CreateTrigger()
            set t[1] = TriggerList.create()
            set t[2] = t[1]
            set t.boolean[4] = true
            return this
        endmethod

        method destroy takes nothing returns nothing
            local Table t = table[this]
            call .deallocate()
            call DestroyTrigger(t.trigger[0])
            call TriggerList(t[1]).destroy()
            call t.flush()
        endmethod

        method clear takes nothing returns thistype
            local Table t = table[this]
            local boolean flag = t.boolean[4]
            call TriggerClearConditions(t.trigger[0])
            call TriggerList(t[1]).destroy()
            call t.flush()
            set t.boolean[4] = flag
            return this
        endmethod

        method operator enabled takes nothing returns boolean
            return table[this].boolean[4]
        endmethod

        method operator enabled= takes boolean flag returns nothing
            set table[this].boolean[4] = flag
        endmethod

        method register takes code c returns thistype
            local Table t = table[this]
            local filterfunc filter = Filter(c)
            debug if t.triggercondition.has(GetHandleId(filter)) then
                debug call debug("|CFFFF0000ERROR:|R Attempt to double register a code to the event [" + I2S(this) + "]")
                debug return this
            debug endif
            set t.triggercondition[GetHandleId(filter)] = TriggerAddCondition(t.trigger[0], filter)
            set filter = null
            return this
        endmethod

        method unregister takes code c returns thistype
            local Table t = table[this]
            local integer i = GetHandleId(Filter(c))
            debug if not t.triggercondition.has(i) then
                debug call debug("|CFFFF0000ERROR:|R Attempt to remove a null or an already removed code from event [" + I2S(this) + "]")
                debug return this
            debug endif
            call TriggerRemoveCondition(t.trigger[0], t.triggercondition[i])
            call t.triggercondition.remove(i)
            return this
        endmethod

        method registerTrigger takes trigger trig returns thistype
            local Table t = table[this]
            local TriggerList tNew = TriggerList.allocate()
            call TriggerList(t[2]).insertNode(tNew)
            set t[2] = tNew
            set tList.trigger[tNew] = trig
            set t[GetHandleId(trig)] = tNew
            return this
        endmethod

        method unregisterTrigger takes trigger trig returns thistype
            local Table t = table[this]
            local TriggerList node = t[GetHandleId(trig)]
            call node.removeNode()
            call tList.trigger.remove(node)
            if node == t[2] then
                set t[2] = TriggerList(t[2]).next
            endif
            return this
        endmethod

        private method runAllTriggers takes nothing returns nothing
            local trigger t
            set this = TriggerList(this).next
            loop
                exitwhen this == 0
                set t = tList.trigger[this]
                if IsTriggerEnabled(t) and TriggerEvaluate(t) then
                    call TriggerExecute(t)
                endif
                set this = TriggerList(this).next
            endloop
            set t = null
        endmethod

        method evaluate takes nothing returns thistype
            local Table t = table[this]
            if t.boolean[4] then
                set fired = this
                set t.boolean[3] = TriggerEvaluate(t.trigger[0])
                set fired = 0
            endif
            return this
        endmethod

        method execute takes nothing returns thistype
            local Table t = table[this]
            if t.boolean[4] then
                set fired = this
                call thistype(t[1]).runAllTriggers()
                set fired = 0
            endif
            return this
        endmethod

        method fire takes nothing returns thistype
            call .evaluate()
            return .execute()
        endmethod

        method operator check takes nothing returns boolean
            local Table t = table[this]
            local boolean b = t.boolean[3]
            set t.boolean[3] = false
            return b
        endmethod

        private static method init takes nothing returns nothing
            set table = HashTable.create()
            set tList = table[0]
        endmethod

        implement Initializer

    endstruct


endlibrary
