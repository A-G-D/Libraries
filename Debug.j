library Debug /* by AGD

    |-----|
    | API |
    |-----|

        module Debug
            - Implement at the top of your struct to gain helpful debugger method

    */
    module Debug
        debug static method debug takes string msg returns nothing
            debug call DisplayTimedTextToPlayer(GetLocalPlayer(), 0, 0, 60, "|CFFFFCC00[" + "thistype" + "] : |R" + msg)
        debug endmethod
    endmodule

endlibrary
