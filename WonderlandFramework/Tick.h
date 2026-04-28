#pragma once
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
class Tick
{
private:
    int TickDelay = 100;
    float TickSpeed = 1.0f;
    int CurrentTick = 0;
   
    bool IsRunning = false;
    
    std::vector<std::function<void(int)>> Listeners;

public:
    //-----[Obligatory]-----\\
    //Constructor & Deconstructor
    
    Tick() {}
    
    Tick(int TD, float TS)
    {
        TickDelay = TD;
        TickSpeed = TS;
    }
    
    
    //-----[Accessors]-----\\
    //Funcitons to Access Private Vars
    
    int GetCurrentTick() const { return CurrentTick; }
    bool GetIsRunning() const { return IsRunning; }
    
    
    //-----[Mutators]-----\\
    //Funcitons to Alter Private Vars
   
    void Register(std::function<void(int)> Listener)
    { Listeners.push_back(Listener); }

    void Start() { IsRunning = true; }

    void Stop() { IsRunning = false; }
    
    void Update()
    {
        int TrueTickSpeed = static_cast<int>(TickDelay * TickSpeed);
        std::this_thread::sleep_for(std::chrono::milliseconds(TrueTickSpeed));
        CurrentTick++;
        for (auto& Listener : Listeners)
        { Listener(CurrentTick); }
    }
    
    void Run()
    {
        IsRunning = true;
        while (IsRunning)
        { Update(); }
    }
};