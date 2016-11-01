library LoopCode /*


    */uses /*

    */Timer


    //! novjass

    |================|
    | Written by AGD |
    |================|

        |-----|
        | API |
        |-----|

          function interface LoopCode takes nothing returns nothing/*
            - Interface of functions to be registered in the loop

        */function RegisterLoopCode takes LoopCode c, real timeout returns boolean/*
            - Registers a code to run every <timeout> seconds and returns a boolean value depending
              on the success of the operation

        */function RegisterLoopCodeCounted takes LoopCode c, real timeout, integer executionCount returns boolean/*
            - Registers a code to run every <timeout> seconds <executionCount> times

        */function RegisterLoopCodeTimed takes LoopCode c, real timeout, real duration returns boolean/*
            - Registers a code to run every <timeout> seconds for <duration> seconds

        */function RegisterLoopCodeEvaluated takes LoopCode c, real timeout, filterfunc filter returns boolean/*
            - Registers a code to run every <timeout> seconds until <filter> returns false

        */function RemoveLoopCode takes LoopCode c returns boolean/*
            - Unregisters a code from the loop and returns a boolean value depending
              on the success of the operation

        */function SetCodeTimeout takes LoopCode c, real timeout returns boolean/*
            - Sets a new loop timeout for a code

    *///! endnovjass

    //=================================== Configuration ====================================

    globals

        ////////////////////////////////////////////////////////////////////////
        // The minimum possible timeout of the loop                           //
        ////////////////////////////////////////////////////////////////////////
        private constant real TIMEOUT = 0.03125
        ////////////////////////////////////////////////////////////////////////
        // If true, the code timeout will be automatically set to the minimum //
        // amount if the input timeout value is less than <TIMEOUT>           //
        ////////////////////////////////////////////////////////////////////////
        private constant boolean AUTO_ADJUST = true

    endglobals

    //================================ End of Configuration ================================

    globals
        private LoopCode array codes
        private Timer loopTimer
        private code func
        private boolean success
        private integer id = 0
        private integer count = 0
        private real array codeTimeout
        private real array elapsed
        private real array exDurElapsed
        private real array exDuration
        private integer array exCount
        private integer array exDone
        private integer array index
        private boolean array check
        private trigger array condition
    endglobals

    //======================================================================================

    function interface LoopCode takes nothing returns nothing

    static if DEBUG_MODE then
        private function Debug takes string msg returns nothing
            call DisplayTimedTextToPlayer(GetLocalPlayer(), 0, 0, 60, "|CFFFFCC00[LoopCode] : |R" + msg)
        endfunction
    endif

    function RegisterLoopCode takes LoopCode c, real timeout returns boolean
        static if AUTO_ADJUST then
            if timeout < TIMEOUT then
                set timeout = TIMEOUT
                debug call Debug("Entered code execution timeout is less than the minimum value (" + R2S(TIMEOUT) + "), auto-adjusting timeout to (" + R2S(TIMEOUT) + ")")
            endif
        else
            if timeout < TIMEOUT then
                debug call Debug("ERROR: Entered code execution timeout is less than the minimum value (" + R2S(TIMEOUT) + ")")
                return false
            endif
        endif
        if not check[c] then
            debug if timeout - (timeout/TIMEOUT)*TIMEOUT > 0.00 then
                debug call Debug("WARNING: Entered code timeout is not divisible by " + R2S(TIMEOUT) + ", this code's execution interval will not be even")
            debug endif
            set count = count + 1
            set elapsed[count] = 0.00
            set codeTimeout[count] = timeout
            set codes[count] = c
            set index[c] = count
            set check[c] = true
            if count == 1 then
                call loopTimer.start(TIMEOUT, true, func)
                debug call Debug("There is one code instance registered, starting to run timer")
            endif
            return true
        endif
        debug call Debug("ERROR: Attempt to double register a code")
        return false
    endfunction

    function RemoveLoopCode takes LoopCode c returns boolean
        local integer i = index[c]
        if check[c] then
            debug call Debug("Removing a code from the loop")
            call DestroyTrigger(condition[i])
            set check[c] = false
            set index[codes[count]] = i
            set codes[i] = codes[count]
            set codeTimeout[i] = codeTimeout[count]
            set exDone[i] = exDone[count]
            set exCount[i] = exCount[count]
            set exDurElapsed[i] = exDurElapsed[count]
            set exDuration[i] = exDuration[count]
            set condition[i] = condition[count]
            if id >= i then
                set id = id - 1
            endif
            set count = count - 1
            if count == 0 then
                call loopTimer.pause()
                debug call Debug("There are no code instances running, stopping timer")
            endif
            return true
        endif
        debug call Debug("ERROR: Attempt to remove a null or an already removed code")
        return false
    endfunction

    function SetCodeTimeout takes LoopCode c, real timeout returns boolean
        local integer i = index[c]
        if check[c] then
            static if AUTO_ADJUST then
                if codeTimeout[i] >= TIMEOUT then
                    set codeTimeout[i] = timeout
                else
                    set codeTimeout[i] = TIMEOUT
                    debug call Debug("Entered code execution timeout is less than the minimum value (" + R2S(TIMEOUT) + "), auto-adjusting timeout to (" + R2S(TIMEOUT) + ")")
                endif
                return true
            else
                if codeTimeout[i] >= TIMEOUT then
                    set codeTimeout[i] = timeout
                    return true
                endif
                debug call Debug("ERROR: Entered code execution timeout is less than the minimum value (" + R2S(TIMEOUT) + ")")
                return false
            endif
        endif
        debug call Debug("ERROR: Specified code is not registered")
        return false
    endfunction

    function RegisterLoopCodeCounted takes LoopCode c, real timeout, integer executionCount returns boolean
        if executionCount > 0 then
            set success = RegisterLoopCode(c, timeout)
            set exCount[count] = executionCount
            return success
        endif
        debug call Debug("ERROR: Entered code execution count is less than 1")
        return false
    endfunction

    function RegisterLoopCodeTimed takes LoopCode c, real timeout, real duration returns boolean
        if duration > TIMEOUT then
            set success = RegisterLoopCode(c, timeout)
            set exDuration[count] = duration
            return success
        endif
        debug call Debug("ERROR: Entered code execution duration is less than the minimum timeout (" + R2S(TIMEOUT) + ")")
        return false
    endfunction

    function RegisterLoopCodeEvaluated takes LoopCode c, real timeout, filterfunc filter returns boolean
        set success = RegisterLoopCode(c, timeout)
        set condition[count] = CreateTrigger()
        call TriggerAddCondition(condition[count], filter)
        return success
    endfunction

    private function RunLoop takes nothing returns nothing
        set id = 0
        loop
            set id = id + 1
            set elapsed[id] = elapsed[id] + TIMEOUT
            if elapsed[id] >= codeTimeout[id] then
                if (exDuration[id] == 0 and exCount[id] == 0 and condition[id] == null) or exDurElapsed[id] < exDuration[id] or exDone[id] < exCount[id] or TriggerEvaluate(condition[id]) then
                    call codes[id].evaluate()
                    set exDone[id] = exDone[id] + 1
                    set exDurElapsed[id] = exDurElapsed[id] + codeTimeout[id]
                else
                    call RemoveLoopCode(codes[id])
                endif
                set elapsed[id] = elapsed[id] - codeTimeout[id]
            endif
            exitwhen id == count
        endloop
    endfunction

    private module Init
        static method onInit takes nothing returns nothing
            set func = function RunLoop
            set loopTimer = Timer.new()
        endmethod
    endmodule

    private struct S extends array
        implement Init
    endstruct


endlibrary