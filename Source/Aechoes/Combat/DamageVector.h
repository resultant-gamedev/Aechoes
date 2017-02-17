#pragma once

#include "DamageVector.generated.h"

UENUM(BlueprintType)
enum class DamageType : uint8
{
    NEUTRAL,
    EARTH,
    FIRE,
    WATER,
    AIR,
	HEAL,
	ALL, //used when filtering damage types. Not applicable as actual dtype
	NONE //Same as above
};

USTRUCT(Atomic, BlueprintType)
struct FDamageVector
{
    GENERATED_USTRUCT_BODY()


    int32 amount;

    DamageType damageType;

    static FDamageVector make(int32 amt, DamageType type)
    {
        FDamageVector vect;

        vect.amount = amt;
        vect.damageType = type;

        return vect;
    }

};