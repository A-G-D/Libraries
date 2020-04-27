library AllocList /* Depreciated, use Alloc + LinkedList instead


    */uses /*

    */Table                 /*
    */optional ErrorMessage /*


    */
    module AllocList

        debug private boolean _allocated
        private thistype _prev
        private thistype _next
        private thistype recycler

        debug method operator allocated takes nothing returns boolean
            debug return this._allocated
        debug endmethod

        method operator prev takes nothing returns thistype
            return this._prev
        endmethod
        method operator next takes nothing returns thistype
            return this._next
        endmethod

        static method allocate takes nothing returns thistype
            local thistype this = thistype(0).recycler
            local thistype last = thistype(0)._prev
            static if LIBRARY_ErrorMessage then
                debug call ThrowError(integer(this) > 8190, "Alloc", "allocate()", "thistype", 0, "Overflow")
            endif
            if this.recycler == 0 then
                set this = this + 1
                set thistype(0).recycler = this
            else
                set thistype(0).recycler = this.recycler
                set this.recycler = 0
            endif
            set this._prev = last
            set this._next = 0
            set thistype(0)._prev = this
            set last._next = this
            debug set this._allocated = true
            return this
        endmethod

        method deallocate takes nothing returns nothing
            local thistype prev = this._prev
            local thistype next = this._next
            static if LIBRARY_ErrorMessage then
                debug call ThrowError(not this._allocated, "AllocList", "deallocate()", "thistype", this, "Double-free")
            endif
            debug set this._allocated = false
            set next._prev = prev
            set prev._next = next
            set this.recycler = thistype(0).recycler
            set thistype(0).recycler = this
        endmethod

    endmodule

    module AllocListT

        debug private static key allocK
        private static key prevK
        private static key nextK
        private static key recyclerK

        debug method operator allocated takes nothing returns boolean
            debug return Table(allocK).boolean[this]
        debug endmethod

        method operator prev takes nothing returns thistype
            return Table(prevK)[this]
        endmethod
        method operator next takes nothing returns thistype
            return Table(nextK)[this]
        endmethod

        static method allocate takes nothing returns thistype
            local integer this = Table(recyclerK)[0]
            local integer last = Table(prevK)[0]
            static if LIBRARY_ErrorMessage then
                debug call ThrowError(this < 0, "Alloc", "allocate()", "thistype", 0, "Overflow")
            endif
            if Table(recyclerK)[this] == 0 then
                set this = this + 1
                set Table(recyclerK)[0] = this
            else
                set Table(recyclerK)[0] = Table(recyclerK)[this]
                set Table(recyclerK)[this] = 0
            endif
            set Table(prevK)[this] = last
            set Table(nextK)[this] = 0
            set Table(prevK)[0] = this
            set Table(nextK)[last] = this
            debug set Table(allocK).boolean[this] = true
            return this
        endmethod

        method deallocate takes nothing returns nothing
            local integer prev = Table(prevK)[this]
            local integer next = Table(nextK)[this]
            static if LIBRARY_ErrorMessage then
                debug call ThrowError(not Table(allocK).boolean[this], "AllocList", "deallocate()", "thistype", this, "Double-free")
            endif
            set Table(prevK)[next] = prev
            set Table(nextK)[prev] = next
            set Table(recyclerK)[this] = Table(recyclerK)[0]
            set Table(recyclerK)[0] = this
            debug call Table(allocK).boolean.remove(this)
        endmethod

    endmodule


endlibrary
