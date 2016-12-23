//! zinc
library BoolexprUtils {

    public boolexpr BOOLEXPR_TRUE, BOOLEXPR_FALSE;

    function onInit() {
        BOOLEXPR_TRUE = Filter(function() -> boolean {return true;});
        BOOLEXPR_FALSE = Filter(function() -> boolean {return false;});
    }

}
//! endzinc
