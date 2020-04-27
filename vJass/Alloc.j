library Alloc /* v1.0.0 


    */uses /*

    */optional ErrorMessage /*

    *///! novjass

    |=========|
    | CREDITS |
    |=========|
    /*
        - AGD (Author)
        - MyPad (Allocation algorithm)
    */

    |-----|
    | API |
    |-----|
    /*
      */module Alloc/*

          */debug readonly boolean allocated/*

          */static method allocate takes nothing returns thistype/*
          */method deallocate takes nothing returns nothing/*

    *///! endnovjass

    /*===========================================================================*/

    static if DEBUG_MODE then
        private function AssertError takes boolean condition, string methodName, string structName, integer node, string message returns nothing
            static if LIBRARY_ErrorMessage then
                call ThrowError(condition, "Alloc", methodName, structName, node, message)
            endif
        endfunction
    endif

    module Alloc
        private static thistype array stack
        debug method operator allocated takes nothing returns boolean
            debug return stack[this] == 0
        debug endmethod
        static method allocate takes nothing returns thistype
            local thistype node = stack[0]
            if stack[node] == 0 then
                debug call AssertError(node == (JASS_MAX_ARRAY_SIZE - 1), "allocate()", "thistype", node, "Overflow")
                set node = node + 1
                set stack[0] = node
            else
                set stack[0] = stack[node]
                set stack[node] = 0
            endif
            return node
        endmethod
        method deallocate takes nothing returns nothing
            debug call AssertError(this == 0, "deallocate()", "thistype", 0, "Null node")
            debug call AssertError(not this.allocated, "deallocate()", "thistype", this, "Double-free")
            set stack[this] = stack[0]
            set stack[0] = this
        endmethod
    endmodule


endlibrary
