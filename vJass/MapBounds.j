library MapBounds uses Initializer


    //! novjass

    |=====|
    | API |
    |=====|

        struct MapBounds extends array
        struct WorldBounds extends array

            readonly static real centerX
            readonly static real centerY
            readonly static real minX
            readonly static real minY
            readonly static real maxX
            readonly static real maxY
            readonly static rect rect
            readonly static region region

            static method getBoundedX takes real x returns real
            static method getBoundedY takes real y returns real/*
                - Returns a coordinate that is inside the bounds

          */static method containsX takes real x returns boolean
            static method containsY takes real y returns boolean/*
                - Checks if the bound contains the input coordinate
    */

    //! endnovjass

    private module Methods
        static method getBoundedX takes real x returns real
            if x < minX then
                return minX
            elseif x > maxX then
                return maxX
            endif
            return x
        endmethod
        static method getBoundedY takes real y returns real
            if y < minY then
                return minY
            elseif y > maxY then
                return maxY
            endif
            return y
        endmethod

        static method containsX takes real x returns boolean
            return getBoundedX(x) == x
        endmethod
        static method containsY takes real y returns boolean
            return getBoundedY(y) == y
        endmethod
    endmodule

    struct MapBounds extends array

        readonly static real centerX
        readonly static real centerY
        readonly static real minX
        readonly static real minY
        readonly static real maxX
        readonly static real maxY
        readonly static rect rect
        readonly static region region

        implement Methods

        private static method init takes nothing returns nothing
            set region = CreateRegion()
            set rect = bj_mapInitialPlayableArea
            set minX = GetRectMinX(rect)
            set minY = GetRectMinY(rect)
            set maxX = GetRectMaxX(rect)
            set maxY = GetRectMaxY(rect)
            set centerX = (minX + maxX)/2.00
            set centerY = (minY + maxY)/2.00
            call RegionAddRect(region, rect)
        endmethod
        implement Initializer

    endstruct

    struct WorldBounds extends array

        readonly static real centerX
        readonly static real centerY
        readonly static real minX
        readonly static real minY
        readonly static real maxX
        readonly static real maxY
        readonly static rect rect
        readonly static region region

        implement Methods

        private static method init takes nothing returns nothing
            set region = CreateRegion()
            set rect = GetWorldBounds()
            set minX = GetRectMinX(rect)
            set minY = GetRectMinY(rect)
            set maxX = GetRectMaxX(rect)
            set maxY = GetRectMaxY(rect)
            set centerX = (minX + maxX)/2.00
            set centerY = (minY + maxY)/2.00
            call RegionAddRect(region, rect)
        endmethod
        implement Initializer

    endstruct


endlibrary
