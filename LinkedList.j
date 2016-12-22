library LinkedList /*


    */requires /*

    */Alloc /*


    Author: AGD

    |-----|
    | API |
    |-----|

        module LinkedList
        module LinkedListT

            debug readonly boolean head
            thistype prev
            thistype next

            static method create takes nothing returns thistype
                - Creates a head node

            method insertNode takes integer node returns nothing
                - Inserts a node next to this instance

            method removeNode takes nothing returns nothing
                - Removes this node from the list

            method clear takes nothing returns nothing

    */

    /*=======================================================================*/

    module LinkedList

        debug readonly boolean head
        readonly thistype prev
        readonly thistype next

        implement Alloc

        static method create takes nothing returns thistype
            local thistype this = allocate()
            debug set .head = true
            set .prev = 0
            set .next = 0
            return this
        endmethod

        method insertNode takes integer node returns nothing
            local thistype alloc = allocate()
            local thistype listNext = .next
            set alloc.prev = this
            set alloc.next = listNext
            set listNext.prev = alloc
            set .next = alloc
        endmethod

        method removeNode takes nothing returns nothing
            local thistype listPrev = .prev
            local thistype listNext = .next
            set listNext.prev = listPrev
            set listPrev.next = listNext
            call .deallocate()
        endmethod

        method clear takes nothing returns nothing
            local thistype listHead = this
            debug if not .head then
                debug call debug("|CFFFF0000ERROR:|R Attempt to clear an instance which is not a head")
                debug return
            debug endif
            set this = .next
            loop
                exitwhen this == 0
                call .removeNode()
                set this = listHead.next
            endloop
        endmethod

        method destroy takes nothing returns nothing
            debug if not .head then
                debug call debug("|CFFFF0000ERROR:|R Attempt to destroy an instance which is not a head")
                debug return
            debug endif
            debug set .head = false
            call .clear()
            call .deallocate()
        endmethod

    endmodule

    /*=======================================================================*/

    module LinkedListT

        debug private static Table headT
        private static Table prevT
        private static Table nextT

        implement AllocT

        static method create takes nothing returns thistype
            local integer this = allocate()
            debug set headT.boolean[this] = true
            set prevT[this] = 0
            set nextT[this] = 0
            return this
        endmethod

        method insertNode takes integer node returns nothing
            local integer listNext = nextT[this]
            set prevT[node] = this
            set nextT[node] = listNext
            set prevT[listNext] = node
            set nextT[this] = node
        endmethod

        method removeNode takes nothing returns nothing
            local integer listPrev = prevT[this]
            local integer listNext = nextT[this]
            set prevT[listNext] = listPrev
            set nextT[listPrev] = listNext
            call .deallocate()
        endmethod

        method clear takes nothing returns nothing
            local integer listHead = this
            debug if not headT.boolean[this] then
                debug call debug("|CFFFF0000ERROR:|R Attempt to clear an instance which is not a head")
                debug return
            debug endif
            set this = nextT[this]
            loop
                exitwhen this == 0
                call .removeNode()
                set this = nextT[listHead]
            endloop
        endmethod

        method destroy takes nothing returns nothing
            debug if not headT.boolean[this] then
                debug call debug("|CFFFF0000ERROR:|R Attempt to destroy an instance which is not a head")
                debug return
            debug endif
            debug set headT.boolean[this] = false
            call .clear()
            call .deallocate()
        endmethod

        method operator prev takes nothing returns thistype
            return prevT[this]
        endmethod

        method operator next takes nothing returns thistype
            return nextT[this]
        endmethod

        debug method operator head takes nothing returns boolean
            debug return headT.boolean[this]
        debug endmethod

        private static method onInit takes nothing returns nothing
            debug set headT = Table.create()
            set prevT = Table.create()
            set nextT = Table.create()
        endmethod

    endmodule


endlibrary
