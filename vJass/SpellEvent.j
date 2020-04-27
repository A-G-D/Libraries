library SpellEvent /* v1.4.1 https://www.hiveworkshop.com/threads/301895/


    */uses /*

    */optional Table                    /*  https://www.hiveworkshop.com/threads/188084/
    */optional RegisterPlayerUnitEvent  /*  https://www.hiveworkshop.com/threads/250266/
    */optional ResourcePreloader        /*  https://www.hiveworkshop.com/threads/287358/
    */optional ErrorMessage             /*  https://github.com/nestharus/JASS/blob/master/jass/Systems/ErrorMessage/main.j


    *///! novjass

    /*
        A library that eases and expands the possibilities of custom spells development.

        Core Features:
            1. Two-phase spell event handlers
            2. Ability to manually trigger a spell event
            3. Event response values overriding
            4. Spell development template

            1.) Spell event handlers are grouped into two: generic handlers that runs for every spell, and ability-specific
            handlers. All generic handlers run first. Within them, you can do things such as changing the event parameters
            (caster, target, etc.) as well as preventing the ability-specific handlers from running. The second phase are
            the specific handlers which are for the spell developers to define the mechanics of the spell.

            Note: Generic handlers only run for spells that have existing ability-specific handlers. This is because
            generic handlers are intended as custom-spell modifier, not an ability handler. If you want to catch an event
            that runs for just any ability (including normal OE abilities), you can easily use (in fact, you should) the
            blizzard native events instead.

            2.) You can invoke a spell event to run and define the parameters manually. This removes the need for dummy
            casters in most cases.

            3.) Within the generic event handlers, you can override the event parameters. The change will only affect the
            ability-specific handlers.

            4.) This library provides a framework for the flow of spell through the use of modules. This removes from the
            spell developers the additional work of manual spell event registration, spell instance allocation, and other
            minute tasks such as storing and looping through each active spell instance.

    */

    |=========|
    | Credits |
    |=========|
    /*
        - AGD (Author)
        - Bribe, Nestharus (SpellEffectEvent concept)
		- Anitarf (Original SpellEvent Idea)

    */
    |=========|
    | Structs |
    |=========|
    /*

      */struct Spell extends array/*

          */static constant thistype        GENERIC         /*  You can also use this like 'Spell.GENERIC.registerEventHandlers()'

        Event Responses:
          */readonly static integer         ABILITY_ID      /*
		  */readonly static integer         EVENT_TYPE      /*
		  */readonly static integer         ORDER_TYPE      /*
          */readonly static integer         LEVEL           /*
          */readonly static player          TRIGGER_PLAYER  /*
          */readonly static unit            TRIGGER_UNIT    /*
          */readonly static unit            TARGET_UNIT     /*
		  */readonly static item            TARGET_ITEM     /*
		  */readonly static destructable    TARGET_DEST     /*
          */readonly static real            TARGET_X        /*  Returns the x-coordinate of the caster if the spell is a 'No-target' ability
          */readonly static real            TARGET_Y        /*  Returns the y-coordinate of the caster if the spell is a 'No-target' ability

        Fields:
          */readonly integer abilityId/*
                - Rawcode of the activation ability for the spell

        Methods:

          */static method   operator []                     takes integer spellId                                                           returns Spell/*
                - Returns a Spell instance based on the given activation-ability rawcode which can be used for event handler registrations

		  */method          setEventFlag                    takes integer eventType, boolean flag                                           returns nothing/*
		  */method          getEventFlag                    takes integer eventType                                                         returns boolean/*
				- Disables/Enables certain event types from running for a Spell (These flags are <true> by default)

          */method          executeNoTargetEvent            takes integer eventType, integer level, unit caster                             returns nothing/*
          */method          executePointTargetEvent         takes integer eventType, integer level, unit caster, real targetX, real targetY returns nothing/*
          */method          executeSingleTargetEvent        takes integer eventType, integer level, unit caster, widget target              returns nothing/*
                - Manually triggers a spell event

          */static method   overrideNoTargetParams          takes integer level, unit caster                                                returns nothing/*
          */static method   overridePointTargetParams       takes integer level, unit caster, real targetX, real targetY                    returns nothing/*
          */static method   overrideSingleTargetParams      takes integer level, unit caster, widget target                                 returns nothing/*
                - Overrides the values of the event response variables (Only effective when called inside a generic event handler)
                - The values are only overriden in the ability-specific spell event handlers

          */method          registerEventHandler            takes integer eventType, code handler                                           returns nothing/*
          */method          unregisterEventHandler          takes integer eventType, code handler                                           returns nothing/*
          */method          clearEventHandlers              takes integer eventType                                                         returns nothing/*
          */method          clearHandlers                   takes nothing                                                                   returns nothing/*
                - Manages ability-specific spell event handlers

          */static method   registerGenericEventHandler     takes integer eventType, code handler                                           returns nothing/*
          */static method   unregisterGenericEventHandler   takes integer eventType, code handler                                           returns nothing/*
          */static method   clearGenericEventHandlers       takes integer eventType                                                         returns nothing/*
          */static method   clearGenericHandlers            takes nothing                                                                   returns nothing/*
                - Manages generic spell event handlers

    */
    |===========|
    | Variables |
    |===========|
    /*
        Spell Event Types

      */constant integer EVENT_SPELL_CAST/*
      */constant integer EVENT_SPELL_CHANNEL/*
      */constant integer EVENT_SPELL_EFFECT/*
      */constant integer EVENT_SPELL_ENDCAST/*
      */constant integer EVENT_SPELL_FINISH/*

		Spell Order Types

	  */constant integer SPELL_ORDER_TYPE_TARGET/*
	  */constant integer SPELL_ORDER_TYPE_POINT/*
	  */constant integer SPELL_ORDER_TYPE_IMMEDIATE/*

    */
    |===========|
    | Functions |
    |===========|
    /*
        Equivalent functions for the methods above

        (Event Responses)
      */constant function GetEventSpellAbilityId    takes nothing                                                   returns integer/*
	  */constant function GetEventSpellEventType    takes nothing                                                   returns integer/*
	  */constant function GetEventSpellOrderType    takes nothing                                                   returns integer/*
      */constant function GetEventSpellLevel        takes nothing                                                   returns integer/*
      */constant function GetEventSpellUser         takes nothing                                                   returns player/*
      */constant function GetEventSpellCaster       takes nothing                                                   returns unit/*
      */constant function GetEventSpellTargetUnit   takes nothing                                                   returns unit/*
      */constant function GetEventSpellTargetItem   takes nothing                                                   returns item/*
      */constant function GetEventSpellTargetDest   takes nothing                                                   returns destructable/*
      */constant function GetEventSpellTargetX      takes nothing                                                   returns real/*
      */constant function GetEventSpellTargetY      takes nothing                                                   returns real/*

	  */function SetSpellEventFlag                  takes integer abilId, integer eventType, boolean flag           returns nothing/*
	  */function GetSpellEventFlag                  takes integer abilId, integer eventType                         returns boolean/*

      */function SpellExecuteNoTargetEvent          takes integer abilId, integer eventType, integer level, unit caster                                returns nothing/*
      */function SpellExecutePointTargetEvent       takes integer abilId, integer eventType, integer level, unit caster, real targetX, real targetY    returns nothing/*
      */function SpellExecuteSingleTargetEvent      takes integer abilId, integer eventType, integer level, unit caster, widget target                 returns nothing/*

      */function SpellOverrideNoTargetParams        takes integer level, unit caster                                returns nothing/*
      */function SpellOverridePointTargetParams     takes integer level, unit caster, real targetX, real targetY    returns nothing/*
      */function SpellOverrideSingleTargetParams    takes integer level, unit caster, widget target                 returns nothing/*

      */function SpellRegisterEventHandler          takes integer spellId, integer eventType, code handler          returns nothing/*
      */function SpellUnregisterEventHandler        takes integer spellId, integer eventType, code handler          returns nothing/*
      */function SpellClearEventHandlers            takes integer spellId, integer eventType                        returns nothing/*
      */function SpellClearHandlers                 takes integer spellId                                           returns nothing/*
	  
      */function SpellRegisterGenericEventHandler   takes integer eventType, code handler                           returns nothing/*
      */function SpellUnregisterGenericEventHandler takes integer eventType, code handler                           returns nothing/*
      */function SpellClearGenericEventHandlers     takes integer eventType                                         returns nothing/*
      */function SpellClearGenericHandlers          takes nothing                                                   returns nothing/*

    */
    |=========|
    | Modules |
    |=========|
    /*
        Automates spell event handler registration at map initialization
        Modules <SpellEvent> and <SpellEventEx> cannot both be implemented in the same struct

      */module SpellEvent/*

            > Uses a single timer for all active spell instances. Standard module designed for
              periodic spells with high-frequency timeout (<= 0.5 seconds)

        Fields:

          */readonly thistype prev/*
          */readonly thistype next/*
                - Spell instances links
                - Readonly attribute is only effective outside the implementing struct, though
                  users are also not supposed to change these values from inside the struct

		Public methods:
		  */static method registerSpellEvent takes integer spellId, integer eventType returns nothing/*
				- Manually registers a spell rawcode to trigger spell events
                - Can be used for spells that involves more than one abilityId

        Member interfaces:
            - Should be declared above the module implementation

          */static integer SPELL_ID             /*  Ability rawcode
          */static integer SPELL_EVENT_TYPE     /*  Spell event type
          */static real    SPELL_PERIOD         /*  Spell periodic actions execution period

          */method onSpellStart     takes nothing   returns thistype/*
                - Runs right after the spell event fires
                - Returning zero or a negative value will not run the periodic operations for that instance
                - You can return a value different from the original value of 'this'
                - The value returned will be added to the list of instances that will run onSpellPeriodic().
          */method onSpellPeriodic  takes nothing   returns boolean/*
                - Runs periodically after the spell event fires until it returns false
          */method onSpellEnd       takes nothing   returns nothing/*
                - Runs after method onSpellPeriodic() returns false


      */module SpellEventEx/*

            > Uses 1 timer for each active spell instance. A module specifically designed for
              periodic spells with low-frequency timeout (> 0.5 seconds) as it does not affect
              the accuracy of the first 'tick' of the periodic operations. Here, you always
              need to manually allocate/deallocate you spell instances.

		Public methods:
		  */static method registerSpellEvent takes integer spellId, integer eventType returns nothing/*
				- Manually registers a spell rawcode to trigger spell events
                - Can be used for spells that involves more than one abilityId

        Member interfaces:
            - Should be declared above the module implementation

          */static integer SPELL_ID             /*  Ability rawcode
          */static integer SPELL_EVENT_TYPE     /*  Spell event type
          */static real    SPELL_PERIOD         /*  Spell periodic actions execution period

          */static method   onSpellStart        takes nothing   returns thistype/*
                - Runs right after the spell event fires
                - User should manually allocate the spell instance and use it as a return value of this method
                - Returning zero or a negative value will not run the periodic operations for that instance
          */method          onSpellPeriodic     takes nothing   returns boolean/*
                - Runs periodically after the spell event fires until it returns false
          */method          onSpellEnd          takes nothing   returns nothing/*
                - Runs after method onSpellPeriodic() returns false
                - User must manually deallocate the spell instance inside this method


      */module SpellEventGeneric/*

        Member interfaces (All optional):
            - Should be declared above the module implementation

          */static method onSpellEvent takes nothing returns nothing/*
                - Runs on any generic spell event

          */static method onSpellCast takes nothing returns nothing/*
          */static method onSpellChannel takes nothing returns nothing/*
          */static method onSpellEffect takes nothing returns nothing/*
          */static method onSpellEndcast takes nothing returns nothing/*
          */static method onSpellFinish takes nothing returns nothing/*
                - Runs on certain spell events


    *///! endnovjass

    /*=================================== SYSTEM CODE ===================================*/

    globals
        constant integer EVENT_SPELL_CAST               = 0x1
        constant integer EVENT_SPELL_CHANNEL            = 0x2
        constant integer EVENT_SPELL_EFFECT             = 0x4
        constant integer EVENT_SPELL_ENDCAST            = 0x8
        constant integer EVENT_SPELL_FINISH             = 0x10

		constant integer SPELL_ORDER_TYPE_TARGET        = 0x12
		constant integer SPELL_ORDER_TYPE_POINT         = 0x123
		constant integer SPELL_ORDER_TYPE_IMMEDIATE     = 0x1234
    endglobals

    globals
        private integer tempOrderType                   = 0
        private integer tempLevel                       = 0
        private player tempTriggerPlayer                = null
        private unit tempTriggerUnit                    = null
        private widget tempTarget                       = null
        private real tempTargetX                        = 0.00
        private real tempTargetY                        = 0.00
        private boolexpr bridgeExpr

		private integer array eventType
        private integer array eventIndex
    endglobals

	private keyword Init

    static if DEBUG_MODE then
        private function IsValidEventType takes integer eventType returns boolean
            return eventType > 0 and eventType <= (EVENT_SPELL_CAST + EVENT_SPELL_CHANNEL + EVENT_SPELL_EFFECT + EVENT_SPELL_ENDCAST + EVENT_SPELL_FINISH)
        endfunction

        private function IsEventSingleFlag takes integer eventType returns boolean
            return eventType == EVENT_SPELL_CAST    or/*
                */ eventType == EVENT_SPELL_CHANNEL or/*
                */ eventType == EVENT_SPELL_EFFECT  or/*
                */ eventType == EVENT_SPELL_ENDCAST or/*
                */ eventType == EVENT_SPELL_FINISH
        endfunction

        private function AssertError takes boolean condition, string methodName, string structName, integer instance, string message returns nothing
            static if LIBRARY_ErrorMessage then
                call ThrowError(condition, SCOPE_PREFIX, methodName, structName, instance, message)
            endif
        endfunction
    endif

    /*===================================================================================*/

    private struct Hashtable extends array

        static if LIBRARY_Table then
            readonly static TableArray table
            static method save takes integer index, integer key, integer value returns nothing
                set table[index][key] = value
            endmethod
            static method load takes integer index, integer key returns integer
                return table[index][key]
            endmethod
            static method remove takes integer index, integer key returns nothing
                call table[index].remove(key)
            endmethod
            static method saveTriggerCondition takes integer index, integer key, triggercondition value returns nothing
                set table[index].triggercondition[key] = value
            endmethod
            static method loadTriggerCondition takes integer index, integer key returns triggercondition
                return table[index].triggercondition[key]
            endmethod
            static method removeHandle takes integer index, integer key returns nothing
                call table[index].handle.remove(key)
            endmethod
            static method flushChild takes integer index returns nothing    
                call table[index].flush()
            endmethod
            static method init takes nothing returns nothing
                set table = TableArray[JASS_MAX_ARRAY_SIZE]
            endmethod
        else
            readonly static hashtable table = InitHashtable()
            static method save takes integer index, integer key, integer value returns nothing
                call SaveInteger(table, index, key, value)
            endmethod
            static method load takes integer index, integer key returns integer
                return LoadInteger(table, index, key)
            endmethod
            static method remove takes integer index, integer key returns nothing
                call RemoveSavedInteger(table, index, key)
            endmethod
            static method saveTriggerCondition takes integer index, integer key, triggercondition value returns nothing
                call SaveTriggerConditionHandle(table, index, key, value)
            endmethod
            static method loadTriggerCondition takes integer index, integer key returns triggercondition
                return LoadTriggerConditionHandle(table, index, key)
            endmethod
            static method removeHandle takes integer index, integer key returns nothing
                call RemoveSavedHandle(table, index, key)
            endmethod
            static method flushChild takes integer index returns nothing
                call FlushChildHashtable(table, index)
            endmethod
        endif

    endstruct

    /*
    *   One Allocator for the whole library. Yes, it would be unlikely for this system to
    *   reach JASS_MAX_ARRAY_SIZE instances of allocated nodes at a single time.
    *
    *   Need to use custom Alloc because of the updated value for JASS_MAX_ARRAY_SIZE
    *   Credits to MyPad for the allocation algorithm
    */
    private struct Node extends array
		private static thistype array stack
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
            debug call AssertError(stack[this] > 0, "deallocate()", "thistype", this, "Double-free")
			set stack[this] = stack[0]
			set stack[0] = this
		endmethod
    endstruct

	private struct NodeList extends array

        boolexpr expr
        readonly thistype prev
        readonly thistype next

        static method allocate takes nothing returns thistype
            return Node.allocate()
        endmethod
        method deallocate takes nothing returns nothing
            call Node(this).deallocate()
        endmethod

        method insert takes thistype node returns nothing
            local thistype next = this.next
            set node.prev = this
            set node.next = next
            set next.prev = node
            set this.next = node
        endmethod
        method remove takes nothing returns nothing
            set this.next.prev = this.prev
            set this.prev.next = this.next
        endmethod

        static method create takes nothing returns thistype
            local thistype node = allocate()
            set node.prev = node
            set node.next = node
            return node
        endmethod
        method destroy takes nothing returns nothing
            local thistype node = this.next
            loop
                exitwhen node == this
                call node.remove()
                call node.deallocate()
                set node = node.next
            endloop
            set this.prev = 0
            set this.next = 0
            call this.deallocate()
        endmethod

	endstruct

	private struct Handler extends array

		readonly trigger trigger

        boolean overrideParams
        boolean disableBackExpr
		private integer handlerCount
        private integer index
        private static NodeList array genericList

        private method operator list takes nothing returns NodeList
            return this
        endmethod

        /*
        *   You might think that the process of registering handlers are expensive in performance
        *   due to constant rebuilding of triggerconditions each time, but setting up proper spell
        *   handlers are seldom done (often only once per spell) and a large part of them are done
        *   at map initialization.
        */
        private method updateHandlerList takes NodeList list returns nothing
            local NodeList node = list.next
            if list == genericList[this.index] and node == genericList[this.index].prev then
                return
            endif
            loop
                exitwhen node == list
                call Hashtable.saveTriggerCondition(this, GetHandleId(node.expr), TriggerAddCondition(this.trigger, node.expr))
                set node = node.next
            endloop
        endmethod

        method updateHandlers takes nothing returns nothing
			call TriggerClearConditions(this.trigger)
            call this.updateHandlerList(genericList[this.index])
            call this.updateHandlerList(this.list)
		endmethod

        static method bridge takes nothing returns boolean
            local integer triggerId = GetHandleId(GetTriggeringTrigger())
            local thistype node = Hashtable.load(0, triggerId)
            local trigger tempTrig
            if node.disableBackExpr then
                if node.list.next != node.list then
                    set tempTrig = node.trigger
                    set node.trigger = CreateTrigger()
                    call Hashtable.save(0, GetHandleId(node.trigger), node)
                    call node.updateHandlerList(genericList[node.index])
                    call node.updateHandlerList(node.list)
                    call TriggerClearConditions(tempTrig)
                    call DestroyTrigger(tempTrig)
                    call Hashtable.remove(0, triggerId)
                    set tempTrig = null
                endif
                return false
            endif
            return node.list.next != node.list and node.overrideParams
        endmethod

		static method create takes integer eventIndex returns thistype
			local thistype node = NodeList.create()
            set node.index = eventIndex
			set node.handlerCount = 0
			return node
		endmethod
		method destroy takes nothing returns nothing
			if this.handlerCount > 0 then
				call DestroyTrigger(this.trigger)
				set this.trigger = null
				set this.handlerCount = 0
			endif
            set this.index = 0
            call Hashtable.flushChild(this)
			call this.list.destroy()
		endmethod

        static method registerGeneric takes integer eventIndex, boolexpr expr returns nothing
            local NodeList node = NodeList.allocate()
            set node.expr = expr
            call genericList[eventIndex].prev.prev.insert(node)
            call Hashtable.save(genericList[eventIndex], -GetHandleId(expr), node)
        endmethod
        static method unregisterGeneric takes integer eventIndex, integer exprId returns nothing
            local NodeList node = Hashtable.load(genericList[eventIndex], -exprId)
            call Hashtable.remove(genericList[eventIndex], -exprId)
            set node.expr = null
            call node.remove()
            call node.deallocate()
        endmethod
        static method clearGeneric takes integer eventIndex returns nothing
            local NodeList list = genericList[eventIndex]
			local NodeList node = list.next
			loop
				exitwhen node == list.prev
                set node.expr = null
				call node.remove()
				call node.deallocate()
				set node = node.next
			endloop
            call Hashtable.flushChild(list)
        endmethod

        method register takes boolexpr expr returns nothing
            local integer exprId = GetHandleId(expr)
			local NodeList node = NodeList.allocate()
			if this.handlerCount == 0 then
				set this.trigger = CreateTrigger()
                call Hashtable.save(0, GetHandleId(this.trigger), this)
                call this.updateHandlers()
			endif
			set this.handlerCount = this.handlerCount + 1
			set node.expr = expr
            call this.list.prev.insert(node)
            call Hashtable.save(this, -exprId, node)
            call Hashtable.saveTriggerCondition(this, exprId, TriggerAddCondition(this.trigger, expr))
		endmethod
        method unregister takes integer exprId returns nothing
            local NodeList node = Hashtable.load(this, -exprId)
			set node.expr = null
			call node.remove()
			set this.handlerCount = this.handlerCount - 1
			if this.handlerCount == 0 then
                call Hashtable.remove(0, GetHandleId(this.trigger))
                call Hashtable.flushChild(this)
				call DestroyTrigger(this.trigger)
				set this.trigger = null
			else
                call TriggerRemoveCondition(this.trigger, Hashtable.loadTriggerCondition(this, exprId))
                call Hashtable.remove(this, -exprId)
                call Hashtable.removeHandle(this, exprId)
			endif
			call node.deallocate()
		endmethod
        method clear takes nothing returns nothing
			local NodeList node = this.list.next
			loop
				exitwhen node == this.list
                call TriggerRemoveCondition(this.trigger, Hashtable.loadTriggerCondition(this, GetHandleId(node.expr)))
				call node.remove()
				call node.deallocate()
				set node = node.next
			endloop
            call Hashtable.flushChild(this)
            call DestroyTrigger(this.trigger)
            set this.trigger = null
		endmethod

        debug static method hasGenericExpr takes integer eventIndex, boolexpr expr returns boolean
            debug return Hashtable.load(genericList[eventIndex], -GetHandleId(expr)) != 0
        debug endmethod
        debug method hasExpr takes boolexpr expr returns boolean
            debug return Hashtable.load(this, -GetHandleId(expr)) != 0
        debug endmethod

		method operator enabled= takes boolean flag returns nothing
			if flag then
				call EnableTrigger(this.trigger)
			else
				call DisableTrigger(this.trigger)
			endif
		endmethod
		method operator enabled takes nothing returns boolean
			return IsTriggerEnabled(this.trigger)
		endmethod

        private static method initGenericList takes integer eventIndex returns nothing
            set genericList[eventIndex] = NodeList.create()
            call genericList[eventIndex].insert(NodeList.allocate())
            set genericList[eventIndex].next.expr = bridgeExpr
        endmethod

        static method init takes nothing returns nothing
            call initGenericList(eventIndex[EVENT_SPELL_CAST])
            call initGenericList(eventIndex[EVENT_SPELL_CHANNEL])
            call initGenericList(eventIndex[EVENT_SPELL_EFFECT])
            call initGenericList(eventIndex[EVENT_SPELL_ENDCAST])
            call initGenericList(eventIndex[EVENT_SPELL_FINISH])
        endmethod

	endstruct

    /*===================================================================================*/

    struct Spell extends array

        readonly static integer         ABILITY_ID      = 0
		readonly static integer			EVENT_TYPE		= 0
		readonly static integer			ORDER_TYPE		= 0
        readonly static integer         LEVEL           = 0
        readonly static player          TRIGGER_PLAYER  = null
        readonly static unit            TRIGGER_UNIT    = null
        readonly static unit            TARGET_UNIT     = null
		readonly static item            TARGET_ITEM     = null
		readonly static destructable    TARGET_DEST     = null
        readonly static real            TARGET_X        = 0.00
        readonly static real            TARGET_Y        = 0.00

        readonly integer abilityId

        private static integer spellCount = 0
        private static Node spellKey
        private static Handler array eventHandler

        static method operator GENERIC takes nothing returns thistype
            return Hashtable.load(spellKey, 0)
        endmethod

        static method operator [] takes integer abilId returns thistype
            local thistype this = Hashtable.load(spellKey, abilId)
			local integer offset
            if this == 0 then
                debug call AssertError(spellCount > R2I(JASS_MAX_ARRAY_SIZE/5), "Spell[]", "thistype", 0, "Overflow")
                static if LIBRARY_ResourcePreloader then
                    call PreloadAbility(abilId)
                endif
                set spellCount = spellCount + 1
                set thistype(spellCount).abilityId = abilId
                call Hashtable.save(spellKey, abilId, spellCount)
				set offset = (spellCount - 1)*5
				set eventHandler[offset + eventIndex[EVENT_SPELL_CAST]]     = Handler.create(eventIndex[EVENT_SPELL_CAST])
				set eventHandler[offset + eventIndex[EVENT_SPELL_CHANNEL]]  = Handler.create(eventIndex[EVENT_SPELL_CHANNEL])
				set eventHandler[offset + eventIndex[EVENT_SPELL_EFFECT]]   = Handler.create(eventIndex[EVENT_SPELL_EFFECT])
				set eventHandler[offset + eventIndex[EVENT_SPELL_ENDCAST]]  = Handler.create(eventIndex[EVENT_SPELL_ENDCAST])
				set eventHandler[offset + eventIndex[EVENT_SPELL_FINISH]]   = Handler.create(eventIndex[EVENT_SPELL_FINISH])
                return spellCount
            endif
            return this
        endmethod

        static method registerGenericEventHandler takes integer eventType, code handler returns nothing
			local boolexpr expr = Filter(handler)
            local integer eventId = 0x10
            local integer node
            debug call AssertError(not IsValidEventType(eventType), "registerGenericEventHandler()", "thistype", 0, "Invalid Spell Event Type (" + I2S(eventType) + ")")
            loop
                exitwhen eventId == 0
                if eventType >= eventId then
                    set eventType = eventType - eventId
                    debug call AssertError(Handler.hasGenericExpr(eventIndex[eventId], expr), "registerGenericEventHandler()", "thistype", 0, "EventType(" + I2S(eventType) + "): Code is already registered")
                    call Handler.registerGeneric(eventIndex[eventId], expr)
                    set node = spellCount
                    loop
                        exitwhen node == 0
                        set node = node - 1
                        call eventHandler[node*5 + eventIndex[eventId]].updateHandlers()
                    endloop
                endif
                set eventId = eventId/2
            endloop
			set expr = null
        endmethod
        static method unregisterGenericEventHandler takes integer eventType, code handler returns nothing
			local boolexpr expr = Filter(handler)
            local integer eventId = 0x10
            local integer node
            debug call AssertError(not IsValidEventType(eventType), "unregisterGenericEventHandler()", "thistype", 0, "Invalid Spell Event Type (" + I2S(eventType) + ")")
            loop
                exitwhen eventId == 0
                if eventType >= eventId then
                    set eventType = eventType - eventId
                    debug call AssertError(Handler.hasGenericExpr(eventIndex[eventId], expr), "unregisterGenericEventHandler()", "thistype", 0, "EventType(" + I2S(eventType) + "): Code is not registered")
                    call Handler.unregisterGeneric(eventIndex[eventId], GetHandleId(expr))
                    set node = spellCount
                    loop
                        exitwhen node == 0
                        set node = node - 1
                        call eventHandler[node*5 + eventIndex[eventId]].updateHandlers()
                    endloop
                endif
                set eventId = eventId/2
            endloop
			set expr = null
        endmethod
        static method clearGenericEventHandlers takes integer eventType returns nothing
            local integer eventId = 0x10
            local integer node
            debug call AssertError(not IsValidEventType(eventType), "clearGenericEventHandlers()", "thistype", 0, "Invalid Spell Event Type (" + I2S(eventType) + ")")
            loop
                exitwhen eventId == 0
                if eventType >= eventId then
                    set eventType = eventType - eventId
                    call Handler.clearGeneric(eventIndex[eventId])
                    set node = spellCount
                    loop
                        exitwhen node == 0
                        set node = node - 1
                        call eventHandler[node*5 + eventIndex[eventId]].updateHandlers()
                    endloop
                endif
                set eventId = eventId/2
            endloop
        endmethod
        static method clearGenericHandlers takes nothing returns nothing
            call clearGenericEventHandlers(EVENT_SPELL_CAST + EVENT_SPELL_CHANNEL + EVENT_SPELL_EFFECT + EVENT_SPELL_ENDCAST + EVENT_SPELL_FINISH)
        endmethod

        method registerEventHandler takes integer eventType, code handler returns nothing
            local boolexpr expr = Filter(handler)
            local integer offset = (this - 1)*5
            local integer eventId = 0x10
            debug call AssertError((this) < 1 or (this) > spellCount, "registerEventHandler()", "thistype", this, "Invalid Spell instance")
            debug call AssertError(not IsValidEventType(eventType), "registerEventHandler()", "thistype", this, "Invalid Spell Event Type (" + I2S(eventType) + ")")
            if this == GENERIC then
                call registerGenericEventHandler(eventType, handler)
            else
                loop
                    exitwhen eventId == 0
                    if eventType >= eventId then
                        set eventType = eventType - eventId
                        debug call AssertError(eventHandler[offset + eventIndex[eventId]].hasExpr(expr), "registerEventHandler()", "thistype", this, "EventType(" + I2S(eventType) + "): Code is already registered")
                        call eventHandler[offset + eventIndex[eventId]].register(expr)
                    endif
                    set eventId = eventId/2
                endloop
            endif
            set expr = null
        endmethod
        method unregisterEventHandler takes integer eventType, code handler returns nothing
            local boolexpr expr = Filter(handler)
            local integer offset = (this - 1)*5
            local integer eventId = 0x10
            debug call AssertError((this) < 1 or (this) > spellCount, "unregisterEventHandler()", "thistype", this, "Invalid Spell instance")
            debug call AssertError(not IsValidEventType(eventType), "unregisterEventHandler()", "thistype", this, "Invalid Spell Event Type (" + I2S(eventType) + ")")
            if this == GENERIC then
                call unregisterGenericEventHandler(eventType, handler)
            else
                loop
                    exitwhen eventId == 0
                    if eventType >= eventId then
                        set eventType = eventType - eventId
                        debug call AssertError(not eventHandler[offset + eventIndex[eventId]].hasExpr(expr), "registerEventHandler()", "thistype", this, "EventType(" + I2S(eventType) + "): Code is already unregistered")
                        call eventHandler[offset + eventIndex[eventId]].unregister(GetHandleId(expr))
                    endif
                    set eventId = eventId/2
                endloop
            endif
            set expr = null
        endmethod
        method clearEventHandlers takes integer eventType returns nothing
            local integer offset = (this - 1)*5
            local integer eventId = 0x10
            debug call AssertError((this) < 1 or (this) > spellCount, "SpellEvent", "clearEventHandlers()", this, "Invalid Spell instance")
            debug call AssertError(not IsValidEventType(eventType), "SpellEvent", "clearEventHandlers()", this, "Invalid Spell Event Type (" + I2S(eventType) + ")")
            if this == GENERIC then
                call clearGenericEventHandlers(eventType)
            else
                loop
                    exitwhen eventId == 0
                    if eventType >= eventId then
                        set eventType = eventType - eventId
                        call eventHandler[offset + eventIndex[eventId]].clear()
                    endif
                    set eventId = eventId/2
                endloop
            endif
        endmethod
        method clearHandlers takes nothing returns nothing
            debug call AssertError((this) < 1 or (this) > spellCount, "clearHandlers()", "thistype", this, "Invalid Spell instance")
            if this == GENERIC then
                call this.clearGenericHandlers()
            else
                call this.clearEventHandlers(EVENT_SPELL_CAST + EVENT_SPELL_CHANNEL + EVENT_SPELL_EFFECT + EVENT_SPELL_ENDCAST + EVENT_SPELL_FINISH)
            endif
        endmethod

		method setEventFlag takes integer eventType, boolean flag returns nothing
            debug call AssertError(not IsEventSingleFlag(eventType), "setEventFlag()", "thistype", this, "Spell Event Type does not contain a single flag (" + I2S(eventType) + ")")
			set eventHandler[(this - 1)*5 + eventIndex[eventType]].enabled = flag
		endmethod
		method getEventFlag takes integer eventType returns boolean
            debug call AssertError(not IsEventSingleFlag(eventType), "getEventFlag()", "thistype", this, "Spell Event Type does not contain a single flag (" + I2S(eventType) + ")")
			return eventHandler[(this - 1)*5 + eventIndex[eventType]].enabled
		endmethod

        method operator handlersDisabled= takes boolean disabled returns nothing
            if Spell.ABILITY_ID != 0 then
                set eventHandler[(this - 1)*5 + eventIndex[Spell.EVENT_TYPE]].disableBackExpr = disabled
            endif
        endmethod
        method operator handlersDisabled takes nothing returns boolean
            if Spell.ABILITY_ID == 0 then
                return false
            endif
            return eventHandler[(this - 1)*5 + eventIndex[Spell.EVENT_TYPE]].disableBackExpr
        endmethod

        private static method onOverrideParams takes nothing returns nothing
            if Handler.bridge() then
                set ORDER_TYPE		= tempOrderType
                set LEVEL           = tempLevel
                set TRIGGER_PLAYER  = GetOwningPlayer(tempTriggerUnit)
                set TRIGGER_UNIT    = tempTriggerUnit
                set TARGET_X        = tempTargetX
                set TARGET_Y        = tempTargetY

                static if LIBRARY_Table then
                    set Hashtable.table[0].widget[0] = tempTarget
                    set TARGET_UNIT = Hashtable.table[0].unit[0]
                    set TARGET_ITEM = Hashtable.table[0].item[0]
                    set TARGET_DEST = Hashtable.table[0].destructable[0]
                else
                    call SaveWidgetHandle(Hashtable.table, 0, 0, tempTarget)
                    set TARGET_UNIT = LoadUnitHandle(Hashtable.table, 0, 0)
                    set TARGET_ITEM = LoadItemHandle(Hashtable.table, 0, 0)
                    set TARGET_DEST = LoadDestructableHandle(Hashtable.table, 0, 0)
                endif

                set tempOrderType   = 0
                set tempLevel       = 0
                set tempTriggerUnit = null
                set tempTargetX     = 0.00
                set tempTargetY     = 0.00
                set tempTarget      = null
            endif
        endmethod

        private static method overrideParams takes integer orderType, integer level, unit triggerUnit, widget target, real targetX, real targetY returns nothing
            if ABILITY_ID != 0 then
                set Handler(Hashtable.load(0, GetHandleId(GetTriggeringTrigger()))).overrideParams = true

                set tempOrderType           = orderType
                set tempLevel               = level
                set tempTriggerPlayer       = GetOwningPlayer(triggerUnit)
                set tempTriggerUnit         = triggerUnit
                set tempTargetX             = targetX
                set tempTargetY             = targetY
                set tempTarget              = target
            endif
        endmethod

        static method overrideNoTargetParams takes integer level, unit triggerUnit returns nothing
            call overrideParams(SPELL_ORDER_TYPE_IMMEDIATE, level, triggerUnit, null, GetUnitX(triggerUnit), GetUnitY(triggerUnit))
        endmethod
        static method overridePointTargetParams takes integer level, unit triggerUnit, real targetX, real targetY returns nothing
            call overrideParams(SPELL_ORDER_TYPE_POINT, level, triggerUnit, null, targetX, targetY)
        endmethod
        static method overrideSingleTargetParams takes integer level, unit triggerUnit, widget target returns nothing
            call overrideParams(SPELL_ORDER_TYPE_TARGET, level, triggerUnit, target, GetWidgetX(target), GetWidgetY(target))
        endmethod

        private static method executeEventHandler takes Handler eventHandler, integer currentId, boolean manualExecute, integer eventFlag, integer orderType, integer level, unit triggerUnit, widget target, real targetX, real targetY returns nothing

            local boolean disableBackExpr       = eventHandler.disableBackExpr
            local boolean overrideParams        = eventHandler.overrideParams
            local integer prevId                = ABILITY_ID
            local integer prevEventType			= EVENT_TYPE
            local integer prevOrderType			= ORDER_TYPE
            local integer prevLevel             = LEVEL
            local player prevTriggerPlayer      = TRIGGER_PLAYER
            local unit prevTriggerUnit          = TRIGGER_UNIT
            local real prevTargetX              = TARGET_X
            local real prevTargetY              = TARGET_Y
            local unit prevTargetUnit           = TARGET_UNIT
            local item prevTargetItem           = TARGET_ITEM
            local destructable prevTargetDest   = TARGET_DEST
            local location tempLoc

            set ABILITY_ID                      = currentId

            if manualExecute then
                set EVENT_TYPE                  = eventFlag
                set ORDER_TYPE                  = orderType
                set LEVEL                       = level
                set TRIGGER_PLAYER              = GetOwningPlayer(triggerUnit)
                set TRIGGER_UNIT                = triggerUnit
                set TARGET_X                    = targetX
                set TARGET_Y                    = targetY

                static if LIBRARY_Table then
                    set Hashtable.table[0].widget[0] = target
                    set TARGET_UNIT             = Hashtable.table[0].unit[0]
                    set TARGET_ITEM             = Hashtable.table[0].item[0]
                    set TARGET_DEST             = Hashtable.table[0].destructable[0]
                else
                    call SaveWidgetHandle(Hashtable.table, 0, 0, target)
                    set TARGET_UNIT             = LoadUnitHandle(Hashtable.table, 0, 0)
                    set TARGET_ITEM             = LoadItemHandle(Hashtable.table, 0, 0)
                    set TARGET_DEST             = LoadDestructableHandle(Hashtable.table, 0, 0)
                endif
            else
                set EVENT_TYPE                  = eventType[GetHandleId(GetTriggerEventId())]
                set TRIGGER_PLAYER              = GetTriggerPlayer()
                set TRIGGER_UNIT                = GetTriggerUnit()
                set LEVEL                       = GetUnitAbilityLevel(TRIGGER_UNIT, ABILITY_ID)
                set TARGET_UNIT                 = GetSpellTargetUnit()
                set TARGET_ITEM                 = GetSpellTargetItem()
                set TARGET_DEST                 = GetSpellTargetDestructable()

                if TARGET_UNIT != null then
                    set TARGET_X                = GetUnitX(TARGET_UNIT)
                    set TARGET_Y                = GetUnitY(TARGET_UNIT)
                    set ORDER_TYPE              = SPELL_ORDER_TYPE_TARGET
                elseif TARGET_ITEM != null then
                    set TARGET_X                = GetItemX(TARGET_ITEM)
                    set TARGET_Y                = GetItemY(TARGET_ITEM)
                    set ORDER_TYPE              = SPELL_ORDER_TYPE_TARGET
                elseif TARGET_DEST != null then
                    set TARGET_X                = GetWidgetX(TARGET_DEST)
                    set TARGET_Y                = GetWidgetY(TARGET_DEST)
                    set ORDER_TYPE              = SPELL_ORDER_TYPE_TARGET
                else
                    set tempLoc = GetSpellTargetLoc()
                    if tempLoc == null then
                    /* Special Case (for some no-target spells) */
                        set TARGET_X            = GetUnitX(TRIGGER_UNIT)
                        set TARGET_Y            = GetUnitY(TRIGGER_UNIT)
                        set ORDER_TYPE          = SPELL_ORDER_TYPE_IMMEDIATE
                    else
                        call RemoveLocation(tempLoc)
                        set tempLoc = null
                        set TARGET_X            = GetSpellTargetX()
                        set TARGET_Y            = GetSpellTargetY()
                        set ORDER_TYPE          = SPELL_ORDER_TYPE_POINT
                    endif
                endif
            endif

            set eventHandler.disableBackExpr = false
            set eventHandler.overrideParams = false
            call TriggerEvaluate(eventHandler.trigger)
            set eventHandler.disableBackExpr = disableBackExpr
            set eventHandler.overrideParams = overrideParams

            set ABILITY_ID                      = prevId
            set EVENT_TYPE                      = prevEventType
            set ORDER_TYPE                      = prevOrderType
            set LEVEL                           = prevLevel
            set TRIGGER_PLAYER                  = prevTriggerPlayer
            set TRIGGER_UNIT                    = prevTriggerUnit
            set TARGET_X                        = prevTargetX
            set TARGET_Y                        = prevTargetY
            set TARGET_UNIT                     = prevTargetUnit
            set TARGET_ITEM                     = prevTargetItem
            set TARGET_DEST                     = prevTargetDest

            set prevTriggerPlayer               = null
            set prevTriggerUnit                 = null
            set prevTargetUnit                  = null
            set prevTargetItem                  = null
            set prevTargetDest                  = null

        endmethod

        private method executeEvent takes integer eventType, integer orderType, integer level, unit triggerUnit, widget target, real targetX, real targetY returns nothing
            local Handler handler = eventHandler[(this - 1)*5 + eventIndex[eventType]]
            if handler != 0 and handler.enabled then
                call executeEventHandler(handler, this.abilityId, true, eventType, orderType, level, triggerUnit, target, targetX, targetY)
            endif
        endmethod

        method executeNoTargetEvent takes integer eventType, integer level, unit triggerUnit returns nothing
            debug call AssertError(not IsEventSingleFlag(eventType), "executeNoTargetEvent()", "thistype", this, "Spell Event Type does not contain a single flag (" + I2S(eventType) + ")")
            call this.executeEvent(eventType, SPELL_ORDER_TYPE_IMMEDIATE, level, triggerUnit, null, GetUnitX(triggerUnit), GetUnitY(triggerUnit))
        endmethod
        method executePointTargetEvent takes integer eventType, integer level, unit triggerUnit, real targetX, real targetY returns nothing
            debug call AssertError(not IsEventSingleFlag(eventType), "executePointTargetEvent()", "thistype", this, "Spell Event Type does not contain a single flag (" + I2S(eventType) + ")")
            call this.executeEvent(eventType, SPELL_ORDER_TYPE_POINT, level, triggerUnit, null, targetX, targetY)
        endmethod
        method executeSingleTargetEvent takes integer eventType, integer level, unit triggerUnit, widget target returns nothing
            debug call AssertError(not IsEventSingleFlag(eventType), "executeSingleTargetEvent()", "thistype", this, "Spell Event Type does not contain a single flag (" + I2S(eventType) + ")")
            call this.executeEvent(eventType, SPELL_ORDER_TYPE_TARGET, level, triggerUnit, target, GetWidgetX(target), GetWidgetY(target))
        endmethod

        private static method onSpellEvent takes integer eventIndex returns nothing
            local integer id = GetSpellAbilityId()
            local Handler handler = eventHandler[(Hashtable.load(spellKey, id) - 1)*5 + eventIndex]
            if handler != 0 and handler.enabled then
                call executeEventHandler(handler, id, false, 0, 0, 0, null, null, 0.00, 0.00)
            endif
        endmethod

        private static method onSpellCast takes nothing returns nothing
            call onSpellEvent(eventIndex[EVENT_SPELL_CAST])
        endmethod
        private static method onSpellChannel takes nothing returns nothing
            call onSpellEvent(eventIndex[EVENT_SPELL_CHANNEL])
        endmethod
        private static method onSpellEffect takes nothing returns nothing
            call onSpellEvent(eventIndex[EVENT_SPELL_EFFECT])
        endmethod
        private static method onSpellEndcast takes nothing returns nothing
            call onSpellEvent(eventIndex[EVENT_SPELL_ENDCAST])
        endmethod
        private static method onSpellFinish takes nothing returns nothing
            call onSpellEvent(eventIndex[EVENT_SPELL_FINISH])
        endmethod

        private static method registerEvent takes playerunitevent whichEvent, code handler returns nothing
            static if LIBRARY_RegisterPlayerUnitEvent then
                call RegisterAnyPlayerUnitEvent(whichEvent, handler)
            else
                local trigger t = CreateTrigger()
                call TriggerRegisterAnyUnitEventBJ(t, whichEvent)
                call TriggerAddCondition(t, Filter(handler))
                set t = null
            endif
        endmethod

        private static method init takes nothing returns nothing
            /*
            *   This bridge boolexpr executes in after all the generic spell handlers
            *   before transitioning into the ability-specific spell handlers.
            *   This boolexpr is responsible for disabling the ability-specific handlers
            *   (if requested) as well as implementing the change/overriding of the
            *   event parameters.
            */
            local code bridgeFunc = function thistype.onOverrideParams
            set bridgeExpr = Filter(bridgeFunc)

            set spellKey = Node.allocate()
            set spellCount = spellCount + 1
            call Hashtable.save(spellKey, 0, spellCount)

            set eventIndex[EVENT_SPELL_CAST]    = 1
            set eventIndex[EVENT_SPELL_CHANNEL] = 2
            set eventIndex[EVENT_SPELL_EFFECT]  = 3
            set eventIndex[EVENT_SPELL_ENDCAST] = 4
            set eventIndex[EVENT_SPELL_FINISH]  = 5
			set eventType[GetHandleId(EVENT_PLAYER_UNIT_SPELL_CAST)]	= EVENT_SPELL_CAST
			set eventType[GetHandleId(EVENT_PLAYER_UNIT_SPELL_CHANNEL)]	= EVENT_SPELL_CHANNEL
			set eventType[GetHandleId(EVENT_PLAYER_UNIT_SPELL_EFFECT)]	= EVENT_SPELL_EFFECT
			set eventType[GetHandleId(EVENT_PLAYER_UNIT_SPELL_ENDCAST)]	= EVENT_SPELL_ENDCAST
			set eventType[GetHandleId(EVENT_PLAYER_UNIT_SPELL_FINISH)]	= EVENT_SPELL_FINISH
            call registerEvent(EVENT_PLAYER_UNIT_SPELL_CAST, function thistype.onSpellCast)
            call registerEvent(EVENT_PLAYER_UNIT_SPELL_CHANNEL, function thistype.onSpellChannel)
            call registerEvent(EVENT_PLAYER_UNIT_SPELL_EFFECT, function thistype.onSpellEffect)
            call registerEvent(EVENT_PLAYER_UNIT_SPELL_ENDCAST, function thistype.onSpellEndcast)
            call registerEvent(EVENT_PLAYER_UNIT_SPELL_FINISH, function thistype.onSpellFinish)
        endmethod
        implement Init

    endstruct

	private module Init
		private static method onInit takes nothing returns nothing
            static if LIBRARY_Table then
                call Hashtable.init()
            endif
			call init()
            call Handler.init()
		endmethod
	endmodule

    /*===================================================================================*/

    constant function GetEventSpellAbilityId takes nothing returns integer
        return Spell.ABILITY_ID
    endfunction
	constant function GetEventSpellEventType takes nothing returns integer
		return Spell.EVENT_TYPE
	endfunction
	constant function GetEventSpellOrderType takes nothing returns integer
		return Spell.ORDER_TYPE
	endfunction
    constant function GetEventSpellLevel takes nothing returns integer
        return Spell.LEVEL
    endfunction
    constant function GetEventSpellUser takes nothing returns player
        return Spell.TRIGGER_PLAYER
    endfunction
    constant function GetEventSpellCaster takes nothing returns unit
        return Spell.TRIGGER_UNIT
    endfunction
    constant function GetEventSpellTargetUnit takes nothing returns unit
        return Spell.TARGET_UNIT
    endfunction
	constant function GetEventSpellTargetItem takes nothing returns item
		return Spell.TARGET_ITEM
	endfunction
	constant function GetEventSpellTargetDest takes nothing returns destructable
		return Spell.TARGET_DEST
	endfunction
    constant function GetEventSpellTargetX takes nothing returns real
        return Spell.TARGET_X
    endfunction
    constant function GetEventSpellTargetY takes nothing returns real
        return Spell.TARGET_Y
    endfunction

	function SetSpellEventFlag takes integer abilId, integer eventType, boolean flag returns nothing
		debug call AssertError(not IsEventSingleFlag(eventType), "SetSpellEventFlag()", "", 0, "Spell(" + I2S(abilId) + "): Invalid Spell Event Type (" + I2S(eventType) + ")")
		call Spell[abilId].setEventFlag(eventType, flag)
	endfunction
	function GetSpellEventFlag takes integer abilId, integer eventType returns boolean
		debug call AssertError(not IsEventSingleFlag(eventType), "GetSpellEventFlag()", "", 0, "Spell(" + I2S(abilId) + "): Invalid Spell Event Type (" + I2S(eventType) + ")")
		return Spell[abilId].getEventFlag(eventType)
	endfunction

    function SpellExecuteNoTargetEvent takes integer abilId, integer eventType, integer level, unit caster returns nothing
        call Spell[abilId].executeNoTargetEvent(eventType, level, caster)
    endfunction
    function SpellExecutePointTargetEvent takes integer abilId, integer eventType, integer level, unit caster, real targetX, real targetY returns nothing
        call Spell[abilId].executePointTargetEvent(eventType, level, caster, targetX, targetY)
    endfunction
    function SpellExecuteSingleTargetEvent takes integer abilId, integer eventType, integer level, unit caster, widget target returns nothing
        call Spell[abilId].executeSingleTargetEvent(eventType, level, caster, target)
    endfunction

    function SpellOverrideNoTargetParams takes integer level, unit caster returns nothing
        call Spell.overrideNoTargetParams(level, caster)
    endfunction
    function SpellOverridePointTargetParams takes integer level, unit caster, real targetX, real targetY returns nothing
        call Spell.overridePointTargetParams(level, caster, targetX, targetY)
    endfunction
    function SpellOverrideSingleTargetParams takes integer level, unit caster, widget target returns nothing
        call Spell.overrideSingleTargetParams(level, caster, target)
    endfunction

    function SpellRegisterEventHandler takes integer abilId, integer eventType, code handler returns nothing
		debug call AssertError(not IsValidEventType(eventType), "SpellRegisterEventHandler()", "", 0, "Spell(" + I2S(abilId) + "): Invalid Spell Event Type (" + I2S(eventType) + ")")
        call Spell[abilId].registerEventHandler(eventType, handler)
    endfunction
    function SpellUnregisterEventHandler takes integer abilId, integer eventType, code handler returns nothing
		debug call AssertError(not IsValidEventType(eventType), "SpellUnregisterEventHandler()", "", 0, "Spell(" + I2S(abilId) + "): Invalid Spell Event Type (" + I2S(eventType) + ")")
        call Spell[abilId].unregisterEventHandler(eventType, handler)
    endfunction
    function SpellClearEventHandlers takes integer abilId, integer eventType returns nothing
        debug call AssertError(not IsValidEventType(eventType), "SpellClearEventHandler()", "", 0, "Spell(" + I2S(abilId) + "): Invalid Spell Event Type (" + I2S(eventType) + ")")
        call Spell[abilId].clearEventHandlers(eventType)
    endfunction
    function SpellClearHandlers takes integer abilId returns nothing
        call Spell[abilId].clearHandlers()
    endfunction

    function SpellRegisterGenericEventHandler takes integer eventType, code handler returns nothing
		debug call AssertError(not IsValidEventType(eventType), "SpellRegisterGenericEventHandler()", "", 0, "Invalid Spell Event Type (" + I2S(eventType) + ")")
        call Spell.registerGenericEventHandler(eventType, handler)
    endfunction
    function SpellUnregisterGenericEventHandler takes integer eventType, code handler returns nothing
		debug call AssertError(not IsValidEventType(eventType), "SpellUnregisterGenericEventHandler()", "", 0, "Invalid Spell Event Type (" + I2S(eventType) + ")")
        call Spell.unregisterGenericEventHandler(eventType, handler)
    endfunction
    function SpellClearGenericEventHandlers takes integer eventType returns nothing
        debug call AssertError(not IsValidEventType(eventType), "SpellClearGenericEventHandlers()", "", 0, "Invalid Spell Event Type (" + I2S(eventType) + ")")
        call Spell.clearGenericEventHandlers(eventType)
    endfunction
    function SpellClearGenericHandlers takes nothing returns nothing
        call Spell.clearGenericHandlers()
    endfunction

    /*===================================================================================*/

    private function DestroyTimerEx takes timer whichTimer returns nothing
        call PauseTimer(whichTimer)
        call DestroyTimer(whichTimer)
    endfunction

	private function OnSpellEventEx takes integer node, real period, code callback returns nothing
		local timer periodicTimer
		if node > 0 then
			set periodicTimer = CreateTimer()
            call Hashtable.save(0, GetHandleId(periodicTimer), node)
			call TimerStart(periodicTimer, period, true, callback)
			set periodicTimer = null
		endif
	endfunction

	private function RegisterSpell takes integer spellId, integer eventType, code onSpellEvent returns nothing
		if spellId != 0 then
			call Spell[spellId].registerEventHandler(eventType, onSpellEvent)
		endif
	endfunction

    module SpellEvent

        readonly thistype prev
        readonly thistype next
        private boolean replacement

        private static method onPeriodic takes nothing returns nothing
            local thistype node = thistype(0).next
            if node == 0 then
            /*
            *   For some reason, some guy tried to manually remove his node from the supposed
            *   readonly linked-list, without realizing that he ALMOST messed up the system..
            */
                call DestroyTimerEx(GetExpiredTimer())
                return
            endif
            loop
                exitwhen node == 0
                if not node.onSpellPeriodic() then
                    call node.onSpellEnd()
                    set node.next.prev = node.prev
                    set node.prev.next = node.next
                    if node.replacement then
                        set node.replacement = false
                    else
                        call Node(node).deallocate()
                    endif
                    if thistype(0).next == 0 then
                        call DestroyTimerEx(GetExpiredTimer())
                    endif
                endif
                set node = node.next
            endloop
        endmethod

        private static method onSpellEvent takes nothing returns nothing
            local thistype node = Node.allocate()
            local thistype last = thistype(0).prev
            local boolean prevEmpty = thistype(0).next == 0
            /*
            *   Add the new node into the list
            */
            set thistype(0).prev = node
            set last.next = node
            set node.prev = last
            set node.next = 0
            set last = node.onSpellStart()
            if last != node then
            /*
            *   If the user returned a different node than the one he was given,
            *   remove and deallocate the earlier node and replace it with the
            *   new node from the user.
            */
                set node.next.prev = node.prev
                set node.prev.next = node.next
                call Node(node).deallocate()
                if last > 0 then
                    set last.replacement = true
                    set node = thistype(0).prev
                    set thistype(0).prev = last
                    set node.next = last
                    set last.prev = node
                    set last.next = 0
                endif
            endif
            /*
            *   We need to use this kind of check in case the user returned 0
            *   but manually added some node in the list inside onSpellStart()
            */
            if prevEmpty and thistype(0).next != 0 then
                call TimerStart(CreateTimer(), SPELL_PERIOD, true, function thistype.onPeriodic)
            endif
        endmethod

        private static method onInit takes nothing returns nothing
			call RegisterSpell(SPELL_ID, SPELL_EVENT_TYPE, function thistype.onSpellEvent)
        endmethod

        static method registerSpellEvent takes integer spellId, integer eventType returns nothing
            call Spell[spellId].registerEventHandler(eventType, function thistype.onSpellEvent)
        endmethod

    endmodule

    module SpellEventEx

        private static method onPeriodic takes nothing returns nothing
            local timer expired = GetExpiredTimer()
            local integer handleId = GetHandleId(expired)
            local thistype node = Hashtable.load(0, handleId)
            if not node.onSpellPeriodic() then
                call node.onSpellEnd()
                call Hashtable.remove(0, handleId)
                call DestroyTimerEx(expired)
            endif
            set expired = null
        endmethod

        private static method onSpellEvent takes nothing returns nothing
			call OnSpellEventEx(onSpellStart(), SPELL_PERIOD, function thistype.onPeriodic)
        endmethod

        private static method onInit takes nothing returns nothing
			call RegisterSpell(SPELL_ID, SPELL_EVENT_TYPE, function thistype.onSpellEvent)
        endmethod

        static method registerSpellEvent takes integer spellId, integer eventType returns nothing
            call Spell[spellId].registerEventHandler(eventType, function thistype.onSpellEvent)
        endmethod

    endmodule

    module SpellEventGeneric
        private static method onSpellResponse takes nothing returns nothing
            static if thistype.onSpellEvent.exists then
                call onSpellEvent()
            endif
            static if thistype.onSpellCast.exists then
                if Spell.EVENT_TYPE == EVENT_SPELL_CAST then
                    call onSpellCast()
                endif
            endif
            static if thistype.onSpellChannel.exists then
                if Spell.EVENT_TYPE == EVENT_SPELL_CHANNEL then
                    call onSpellChannel()
                endif
            endif
            static if thistype.onSpellEffect.exists then
                if Spell.EVENT_TYPE == EVENT_SPELL_EFFECT then
                    call onSpellEffect()
                endif
            endif
            static if thistype.onSpellEndcast.exists then
                if Spell.EVENT_TYPE == EVENT_SPELL_ENDCAST then
                    call onSpellEndcast()
                endif
            endif
            static if thistype.onSpellFinish.exists then
                if Spell.EVENT_TYPE == EVENT_SPELL_FINISH then
                    call onSpellFinish()
                endif
            endif
        endmethod
        private static method onInit takes nothing returns nothing
            call SpellRegisterGenericEventHandler(EVENT_SPELL_CAST + EVENT_SPELL_CHANNEL + EVENT_SPELL_EFFECT + EVENT_SPELL_ENDCAST + EVENT_SPELL_FINISH, function thistype.onSpellResponse)
        endmethod
    endmodule


endlibrary
