#pragma once
#include "AHZActorInfo.h"
#include <mutex>

static std::recursive_mutex mtx;

class SafeEnemyDataHolder
{
public:
    SafeEnemyDataHolder() = default;
    ~SafeEnemyDataHolder() = default;

    CAHZActorData GetData()
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        CAHZActorData                         data;
        data = m_data;
        return data;
    };

    void SetData(const CAHZActorData& data)
    {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        m_data = data;
    };

private:
    CAHZActorData m_data;
};


class AHZEnemyHealthUpdateHook
{
public:
    static void          Install();
    static CAHZActorData GetCurrentEnemyData();

private:
    static SafeEnemyDataHolder ahzEnemyData;
    static uint64_t            lastRefHandle;
    static void                EnemyHealthUpdateHook_impl(uint32_t* refHandle, RE::NiPointer<RE::TESObjectREFR>* refrOut);
};
