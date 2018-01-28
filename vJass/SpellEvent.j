library SpellEvent /* v1.01


    */uses /*

    */Table                    /*
    */Timer                    /*
    */Process                  /*
    */RegisterPlayerUnitEvent  /*
    */Initializer              /*


    *///! novjass

    /*
        Pros:
            - Minimizes handle creation when registering a spell event handler
            - Constant time 'O(1)' operation when executing a spell event
            - Shortens spell registration code
            - Shortens and gives better structure and readability to your spell struct

    */

    |=========|
    | Credits |
    |=========|
    /*
        - AGD (Author)
        - Bribe (SpellEffectEvent concept)

    */
    |============|
    | Struct API |
    |============|

        struct Spell extends array/*

        Event Types:
          */static constant integer CAST_EVENT/*
          */static constant integer CHANNEL_EVENT/*
          */static constant integer EFFECT_EVENT/*
          */static constant integer ENDCAST_EVENT/*
          */static constant integer FINISH_EVENT/*
          */static constant integer ITEM_USE_EVENT/*

        Event Responses:
          */readonly static integer abilityId/*     Returns the item rawcode if the event type is 'ITEM_USE_EVENT'
          */readonly static player  user/*
          */readonly static unit    caster/*
          */readonly static unit    target/*
          */readonly static real    targetX/*
          */readonly static real    targetY/*
          */readonly static real    level/*         Returns '0' if the event type is 'ITEM_USE_EVENT'

        Methods:

          */static method operator () takes integer spellId returns Spell/*
                - Returns a Spell instance based on the given spellID for event handler
                  registrations
                - You can input the item rawcode instead of an ability rawcode if you want
                  to register to an 'Spell.ITEM_USE_EVENT'

          */method registerEventHandler takes integer eventType, code handler returns nothing/*
          */method unregisterEventHandler takes integer eventType, code handler returns nothing/*
                - Registers/Unregisters a handler for/from a certain spell event type

    */
    |==============|
    | Function API |
    |==============|
    /*
        Equivalent functions for the methods above

      */function RegisterSpellEventHandler takes integer eventType, integer spellId, code handler returns nothing/*
      */function UnregisterSpellEventHandler takes integer eventType, integer spellId, code handler returns nothing/*

    */
    |=========|
    | Modules |
    |=========|
    /*
        > Automates spell event registration at map init
          Implement either of these two modules at the bottom of your struct

      */module SpellEvent/*
      */module SpellEventEx/*
            - 

        Fields:

          */readonly thistype prev/*
          */readonly thistype next/*
                - readonly access is only effective from outside the implementing struct, though
                  users are also not supposed to change these values

        Method interfaces:
        - Should be above the module implementation
        - For a more thorough explanation, see SpellEvent Template

          */static method spellId takes nothing returns integer/*
                - Must return the ability rawcode (item rawcode if the event type is 'Spell.ITEM_USE_EVENT')
          */static method spellEventType takes nothing returns integer/*
                - Must return the spell event type
          */static method spellPeriod takes nothing returns real/*
                - Must return the spell periodic actions execution timeout
          */method onSpellStart takes nothing returns nothing/* (if the module used is SpellEvent)
          */static method onSpellStart takes nothing returns thistype/* (if the module used is SpellEventEx)
                - Runs right after the spell event fires
                - User should manually allocate the spell instance use it as a return value if the module
                  used is SpellEventEx
          */method onSpellPeriodic takes nothing returns boolean/*
                - Runs periodically after the spell event fires until
                  it returns true
          */method onSpellEnd takes nothing returns nothing/*
                - Runs after method onSpellPeriodic() returns true
                - Must manually deallocate the spell instance if the module used is SpellEventEx


    *///! endnovjass

    private keyword Init

    globals
        private TableArray eventProcess
        private Table table
    endglobals

    /*=================================== SYSTEM CODE ===================================*/

    struct Spell extends array

        /*
        *   Arbitrary values are used so that users are forced to use the globals instead
        *   of manually typing the integers ;D
        */
        static constant integer CAST_EVENT      = 0x1234 + 0x123 * 1
        static constant integer CHANNEL_EVENT   = 0x1234 + 0x123 * 2
        static constant integer EFFECT_EVENT    = 0x1234 + 0x123 * 3
        static constant integer ENDCAST_EVENT   = 0x1234 + 0x123 * 4
        static constant integer FINISH_EVENT    = 0x1234 + 0x123 * 5
        static constant integer ITEM_USE_EVENT  = 0x1234 + 0x123 * 6

        readonly static integer abilityId    = 0
        readonly static player  user         = null
        readonly static unit    caster       = null
        readonly static unit    target       = null
        readonly static real    targetX      = 0.00
        readonly static real    targetY      = 0.00
        readonly static integer level        = 0

        private static method getEventIndex takes integer eventType returns integer
            return (eventType - 0x1234)/0x123
        endmethod

        method registerEventHandler takes integer eventType, code handler returns nothing
            local integer index = getEventIndex(eventType)
            local Process process = eventProcess[index][this]
            if process == 0 then
                set process = Process.create()
                set eventProcess[index][this] = process
            endif
            call process.register(handler)
        endmethod
        method unregisterEventHandler takes integer eventType, code handler returns nothing
            local integer index = getEventIndex(eventType)
            local Process process = eventProcess[index][this]
            if process != 0 then
                call process.unregister(handler)
                if process.empty then
                    call process.destroy()
                    call eventProcess[index].remove(this)
                endif
            endif
        endmethod

        private static method executeEventHandler takes Process process, integer currentId, unit currentTarget, real currentTargetX, real currentTargetY returns nothing

            local integer prevId    = abilityId
            local player prevUser   = user
            local unit prevCaster   = caster
            local unit prevTarget   = target
            local real prevTargetX  = targetX
            local real prevTargetY  = targetY
            local integer prevLevel = level

            set abilityId           = currentId
            set user                = GetTriggerPlayer()
            set caster              = GetTriggerUnit()
            set target              = currentTarget
            set targetX             = currentTargetX
            set targetY             = currentTargetY
            set level               = GetUnitAbilityLevel(caster, abilityId)

            call process.execute()

            set abilityId           = prevId
            set user                = prevUser
            set caster              = prevCaster
            set target              = prevTarget
            set targetX             = prevTargetX
            set targetY             = prevTargetY
            set level               = prevLevel

            set prevUser            = null
            set prevCaster          = null
            set prevTarget          = null

        endmethod

        private static method onSpellEvent takes integer eventIndex returns nothing
            local integer id = GetSpellAbilityId()
            local Process process = eventProcess[eventIndex][id]
            if process != 0 then
                call executeEventHandler(process, id, GetSpellTargetUnit(), GetSpellTargetX(), GetSpellTargetY())
            endif
        endmethod

        private static method onSpellCast takes nothing returns nothing
            call onSpellEvent(1)
        endmethod
        private static method onSpellChannel takes nothing returns nothing
            call onSpellEvent(2)
        endmethod
        private static method onSpellEffect takes nothing returns nothing
            call onSpellEvent(3)
        endmethod
        private static method onSpellEndcast takes nothing returns nothing
            call onSpellEvent(4)
        endmethod
        private static method onSpellFinish takes nothing returns nothing
            call onSpellEvent(5)
        endmethod

        private static method onItemUse takes nothing returns nothing
            local integer id = GetItemTypeId(GetManipulatedItem())
            local Process process = eventProcess[6][id]
            local unit target
            if process != 0 then
                set target = GetOrderTargetUnit()
                call executeEventHandler(process, id, target, GetUnitX(target), GetUnitY(target))
                set target = null
            endif
        endmethod

        private static method init takes nothing returns nothing
            set eventProcess = TableArray[7]
            set table = eventProcess[0]
            call RegisterPlayerUnitEvent(EVENT_PLAYER_UNIT_SPELL_CAST, function thistype.onSpellCast)
            call RegisterPlayerUnitEvent(EVENT_PLAYER_UNIT_SPELL_CHANNEL, function thistype.onSpellChannel)
            call RegisterPlayerUnitEvent(EVENT_PLAYER_UNIT_SPELL_EFFECT, function thistype.onSpellEffect)
            call RegisterPlayerUnitEvent(EVENT_PLAYER_UNIT_SPELL_ENDCAST, function thistype.onSpellEndcast)
            call RegisterPlayerUnitEvent(EVENT_PLAYER_UNIT_SPELL_FINISH, function thistype.onSpellFinish)
            call RegisterPlayerUnitEvent(EVENT_PLAYER_UNIT_USE_ITEM, function thistype.onItemUse)
        endmethod
        implement Initializer

    endstruct

    /*===================================================================================*/

    function RegisterSpellEventHandler takes integer eventType, integer abilId, code handler returns nothing
        call Spell[abilId].registerEventHandler(eventType, handler)
    endfunction
    function UnregisterSpellEventHandler takes integer eventType, integer abilId, code handler returns nothing
        call Spell[abilId].unregisterEventHandler(eventType, handler)
    endfunction

    /*===================================================================================*/

    private module LinkNode
        set thistype(0).prev = node
        set last.next = node
        set node.prev = last
        set node.next = 0
    endmodule

    private module UnlinkNode
        set node.next.prev = node.prev
        set node.prev.next = node.next
        if thistype(0).next == 0 then
            call Timer.expired.free()
        endif
    endmodule

    module SpellEvent

        readonly thistype prev
        readonly thistype next
        private thistype recycler

        private static method onPeriodic takes nothing returns nothing
            local thistype node = thistype(0).next
            loop
                exitwhen node == 0
                if not node.onSpellPeriodic() then
                    call node.onSpellEnd()
                    implement UnlinkNode
                    set node.recycler = thistype(0).recycler
                    set thistype(0).recycler = node
                endif
                set node = node.next
            endloop
        endmethod

        private static method onSpellEvent takes nothing returns nothing
            local thistype last = thistype(0).prev
            local thistype node = thistype(0).recycler
            set thistype(0).recycler = node.recycler
            if thistype(0).next == 0 then
                call Timer.new.start(spellPeriod(), true, function thistype.onPeriodic)
            endif
            implement LinkNode
            call node.onSpellStart()
        endmethod

        private static method onInit takes nothing returns nothing
            local thistype node = 0
            loop
                exitwhen node == 8190
                set node.recycler = node + 1
                set node = node + 1
            endloop
            set node.recycler = 0
            call Spell(spellId()).registerEventHandler(spellEventType(), function thistype.onSpellEvent)
        endmethod

    endmodule

    module SpellEventEx

        readonly thistype prev
        readonly thistype next

        private static method onPeriodic takes nothing returns nothing
            local thistype node = thistype(0).next
            loop
                exitwhen node == 0
                if not node.onSpellPeriodic() then
                    call node.onSpellEnd()
                    implement UnlinkNode
                endif
                set node = node.next
            endloop
        endmethod

        private static method onSpellEvent takes nothing returns nothing
            local thistype last
            local thistype node = onSpellStart()
            if node > 0 then
                set last = thistype(0).prev
                if thistype(0).next == 0 then
                    call Timer.new.start(spellPeriod(), true, function thistype.onPeriodic)
                endif
                implement LinkNode
            endif
        endmethod

        private static method onInit takes nothing returns nothing
            call Spell(spellId()).registerEventHandler(spellEventType(), function thistype.onSpellEvent)
        endmethod

    endmodule


endlibrary
