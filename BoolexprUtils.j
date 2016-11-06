//! zinc
library BoolexprUtils {

    public boolexpr BOOLEXPR_TRUE, BOOLEXPR_FALSE;

    module Init {
        static method onInit() {
            BOOLEXPR_TRUE = Filter(function() -> boolean {return true;});
            BOOLEXPR_FALSE = Filter(function() -> boolean {return false;});
        }
    }

    struct S extends array {module Init;}

}
//! endzinc
