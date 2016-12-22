library Alloc /*


    */requires /*

    */Debug /*


    Author: AGD

    |-----|
    | API |
    |-----|

        module Alloc
        module AllocT

            debug readonly boolean allocated

            static method allocate takes nothing returns thistype
                - Allocates a new instance of the struct

            method deallocate takes nothing returns nothing
                - Deallocates an instance of the struct

    */

    /*=======================================================================*/

    module Alloc

        debug readonly boolean allocated
        private static thistype alloc = 0
        private static thistype array recycler

        implement Debug

        static method allocate takes nothing returns thistype
            debug if recycler[0] == 0 then
                debug call debug("|CFFFFCC00[AllocList]|R Attempt to allocate more than 8190 instances.")
                debug return 0
            debug endif
            set alloc = recycler[0]
            set recycler[0] = recycler[alloc]
            debug set alloc.allocated = true
            return alloc
        endmethod

        method deallocate takes nothing returns nothing
            debug if not .allocated then
                debug call debug("|CFFFFCC00[AllocList]|R Attempt to double-free instance.")
                debug return
            debug endif
            set recycler[this] = recycler[0]
            set recycler[0] = this
            debug set .allocated = false
        endmethod

        private static method onInit takes nothing returns nothing
            set recycler[8190] = 0
            loop
                set recycler[alloc] = alloc + 1
                set alloc = alloc + 1
                exitwhen alloc == 8190
            endloop
        endmethod

    endmodule

    /*=======================================================================*/

    module AllocT

        debug private static Table isAllocated
        private static Table recycler
        private static integer instanceCount = 0
        private static integer recyclerCount = 0

        implement Debug

        static method allocate takes nothing returns thistype
            local thistype this
            if recyclerCount == 0 then
                set this = instanceCount + 1
                set instanceCount = this
            else
                set recyclerCount = recyclerCount - 1
                set this = recycler[recyclerCount]
            endif
            debug set isAllocated.boolean[this] = true
            return this
        endmethod

        method deallocate takes nothing returns nothing
            debug if not isAllocated.boolean[this] then
                debug call debug("|CFFFFCC00[AllocListT] |R Attempt to double-free instance.")
                debug return
            debug endif
            set recycler[recyclerCount] = this
            set recyclerCount = recyclerCount + 1
            debug set isAllocated.boolean[this] = false
        endmethod

        debug method operator allocated takes nothing returns boolean
            debug return isAllocated.boolean[this]
        debug endmethod

        private static method onInit takes nothing returns nothing
            set recycler = Table.create()
            debug set isAllocated = Table.create()
        endmethod

    endmodule


endlibrary
