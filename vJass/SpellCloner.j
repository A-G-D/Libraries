library SpellCloner /* v1.0.1 https://www.hiveworkshop.com/threads/324157/


	*/uses /*

	*/SpellEvent		/*  https://www.hiveworkshop.com/threads/301895/
	*/optional Table    /*  https://www.hiveworkshop.com/threads/188084/

	*///! novjass

	|-----|
	| API |
	|-----|

		module SpellClonerHeader/*
			- Implement this module at the top of your spell struct

		  */static method initSpellConfiguration takes integer abilId returns integer/*
                - Call this method at the top of you onSpellStart() method to initialize
                  the correct local configuration of your spell based on the activation
                  ability id
                - Returns the struct type id of the struct containing the configuration

          */static method loadSpellConfiguration takes integer configStructId returns nothing/*
                - Call this method with the value returned by initSpellConfiguration() as the
                  parameter
                - Like initSpellConfiguration(), loads the correct local configuration of the
                  spell, but based on the typeid of the configuration struct


	  */module SpellClonerFooter/*
            - Implement this module at the bottom of your spell struct, below your SpellEvent implementation

          */static method create takes integer configStructId, integer abilId, integer spellEventType, code configurationFunc returns thistype/*
                - Creates a new local configuration instance for the spell (Return value is obsolete)


	*///! endnovjass

    private keyword SpellConfigList

	globals
		private trigger evaluator = CreateTrigger()
        private integer array eventIndex
        private SpellConfigList array configStructNode
	endglobals

	private module Init
        static if LIBRARY_Table then
            readonly static TableArray table
        else
            static constant hashtable table = InitHashtable()
        endif
        private static method onInit takes nothing returns nothing
            static if LIBRARY_Table then
                set table = TableArray[JASS_MAX_ARRAY_SIZE]
            endif
            set eventIndex[EVENT_SPELL_CAST]    = 1
            set eventIndex[EVENT_SPELL_CHANNEL] = 2
            set eventIndex[EVENT_SPELL_EFFECT]  = 3
            set eventIndex[EVENT_SPELL_ENDCAST] = 4
            set eventIndex[EVENT_SPELL_FINISH]  = 5
        endmethod
	endmodule

	private struct S extends array
		implement Init
	endstruct

    private function SaveInt takes integer index, integer key, integer value returns nothing
        static if LIBRARY_Table then
            set S.table[index][key] = value
        else
            call SaveInteger(S.table, index, key, value)
        endif
    endfunction
    private function LoadInt takes integer index, integer key returns integer
        static if LIBRARY_Table then
            return S.table[index][key]
        else
            return LoadInteger(S.table, index, key)
        endif
    endfunction

	private struct SpellConfigList extends array
		thistype current
		readonly thistype prev
		readonly thistype next
        readonly integer structId
		readonly boolexpr configExpr

		private static thistype node = 0

        method evaluateExpr takes nothing returns nothing
            call TriggerAddCondition(evaluator, this.configExpr)
            call TriggerEvaluate(evaluator)
            call TriggerClearConditions(evaluator)
        endmethod

		method insert takes integer id, boolexpr expr returns thistype
			local thistype next = this.next
            set node = node + 1
            set node.structId = id
            set node.configExpr = expr
			set node.prev = this
			set node.next = next
			set next.prev = node
			set this.next = node
            return node
		endmethod

		static method create takes nothing returns thistype
			set node = node + 1
			set node.prev = node
			set node.next = node
			set node.current = node
			return node
		endmethod
	endstruct

	private function InitSpellConfiguration takes integer spellStructId, integer abilId returns integer
        local SpellConfigList configList = LoadInt(spellStructId*5 + eventIndex[Spell.EVENT_TYPE], abilId)
        local integer configStructId
		set configList.current = configList.current.next
        set configStructId = configList.current.structId
        call configList.current.evaluateExpr()
		if configList.current.next == configList then
			set configList.current = configList
		endif
        return configStructId
	endfunction

	private function CloneSpell takes integer spellStructId, integer configStructId, integer abilId, integer eventType, code configFunc returns nothing
        local SpellConfigList configList
        local integer eventId = 0x10
        loop
            exitwhen eventId == 0
            if eventType >= eventId then
                set eventType = eventType - eventId
                set configList = LoadInt(spellStructId*5 + eventIndex[eventId], abilId)
                if configList == 0 then
                    set configList = SpellConfigList.create()
                    call SaveInt(spellStructId*5 + eventIndex[eventId], abilId, configList)
                endif
                set configStructNode[configStructId] = configList.prev.insert(configStructId, Filter(configFunc))
            endif
            set eventId = eventId/2
        endloop
	endfunction

	module SpellClonerHeader
		static constant integer SPELL_ID = 0
		static constant integer SPELL_EVENT_TYPE = 0

		static method initSpellConfiguration takes integer abilId returns integer
			return InitSpellConfiguration(thistype.typeid, abilId)
		endmethod
        static method loadSpellConfiguration takes integer configStructId returns nothing
            call SpellConfigList(configStructNode[configStructId]).evaluateExpr()
        endmethod
	endmodule

	module SpellClonerFooter
		static method create takes integer configStructId, integer abilId, integer spellEventType, code configurationFunc returns thistype
			call CloneSpell(thistype.typeid, configStructId, abilId, spellEventType, configurationFunc)
			call registerSpellEvent(abilId, spellEventType)
            return 0
		endmethod
	endmodule


endlibrary
