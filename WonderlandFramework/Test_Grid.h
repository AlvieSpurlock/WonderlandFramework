#pragma once
#define NOMINMAX
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
#include <map>
#include <windows.h>
#include <thread>
#include <wtypes.h>


//-----[Character Marker]-----\\

struct CharacterMarker
{
    std::string Name;
    int X = 0;
    int Y = 0;
};


//-----[Color Map]-----\\

static COLORREF GetEffectColor(const std::string& Effect)
{
    if (Effect == "None")     return RGB(40, 40, 40);
    if (Effect == "Poison")   return RGB(50, 180, 50);
    if (Effect == "Fire")     return RGB(220, 80, 20);
    if (Effect == "Bleed")    return RGB(180, 20, 20);
    if (Effect == "Shock")    return RGB(220, 220, 20);
    if (Effect == "Wet")      return RGB(20, 120, 220);
    if (Effect == "Frozen")   return RGB(100, 200, 255);
    if (Effect == "Stunned")  return RGB(180, 80, 220);
    if (Effect == "Burning")  return RGB(255, 140, 0);
    if (Effect == "Sepsis")   return RGB(80, 100, 20);
    if (Effect == "Steam")    return RGB(180, 180, 180);
    if (Effect == "Brittle")  return RGB(160, 130, 90);
    return RGB(40, 40, 40);
}

static COLORREF GetCharacterColor(const std::string& Name)
{
    if (Name == "Hero")    return RGB(255, 255, 255);
    if (Name == "Villain") return RGB(255, 60, 60);
    return RGB(100, 200, 100);
}


//-----[Grid State]-----\\

struct GridState
{
    std::vector<CharacterMarker> Characters;
    std::vector<std::tuple<int, int, std::string>> Tiles;
};

static GridState ReadGridState(const std::string& FilePath)
{
    GridState State;
    std::ifstream File(FilePath);
    if (!File.is_open()) { return State; }
    std::string Line;
    std::string Mode = "";
    while (std::getline(File, Line))
    {
        if (Line == "CHARACTERS") { Mode = "chars"; continue; }
        if (Line == "TILES") { Mode = "tiles"; continue; }
        std::stringstream SS(Line);
        std::string Token;
        std::vector<std::string> Parts;
        while (std::getline(SS, Token, ','))
        {
            Parts.push_back(Token);
        }
        if (Mode == "chars" && Parts.size() == 3)
        {
            State.Characters.push_back({ Parts[0], std::stoi(Parts[1]), std::stoi(Parts[2]) });
        }
        if (Mode == "tiles" && Parts.size() == 3)
        {
            State.Tiles.push_back({ std::stoi(Parts[0]), std::stoi(Parts[1]), Parts[2] });
        }
    }
    return State;
}


//-----[Draw Grid]-----\\

static void DrawGrid(HWND hWnd, const std::string& FilePath, int GridWidth, int GridHeight, int TileSize)
{
    HDC DC = GetDC(hWnd);
    GridState State = ReadGridState(FilePath);

    //-----[Build Effect Map]-----\\

    std::map<std::pair<int, int>, std::string> EffectMap;
    for (auto& T : State.Tiles)
    {
        EffectMap[{std::get<0>(T), std::get<1>(T)}] = std::get<2>(T);
    }

    //-----[Draw Tiles]-----\\

    for (int X = 0; X < GridWidth; X++)
    {
        for (int Y = 0; Y < GridHeight; Y++)
        {
            auto It = EffectMap.find({ X, Y });
            std::string Effect = (It != EffectMap.end()) ? It->second : "None";
            COLORREF Color = GetEffectColor(Effect);
            HBRUSH Brush = CreateSolidBrush(Color);
            HPEN Pen = CreatePen(PS_SOLID, 1, RGB(60, 60, 60));
            SelectObject(DC, Brush);
            SelectObject(DC, Pen);
            Rectangle(DC, X * TileSize, Y * TileSize, (X + 1) * TileSize, (Y + 1) * TileSize);
            DeleteObject(Brush);
            DeleteObject(Pen);
        }
    }

    //-----[Draw Characters]-----\\

    for (auto& C : State.Characters)
    {
        int CX = C.X * TileSize + TileSize / 2;
        int CY = C.Y * TileSize + TileSize / 2;
        int Radius = TileSize / 3;
        HBRUSH Brush = CreateSolidBrush(GetCharacterColor(C.Name));
        HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
        SelectObject(DC, Brush);
        SelectObject(DC, Pen);
        Ellipse(DC, CX - Radius, CY - Radius, CX + Radius, CY + Radius);
        DeleteObject(Brush);
        DeleteObject(Pen);
        SetTextColor(DC, RGB(255, 255, 255));
        SetBkMode(DC, TRANSPARENT);
        TextOutA(DC, CX - Radius, CY - Radius - 14, C.Name.c_str(), C.Name.size());
    }

    ReleaseDC(hWnd, DC);
}


//-----[Window Proc]-----\\

static std::string GFilePath = "grid_state.txt";
static int GGridWidth = 20;
static int GGridHeight = 20;
static int GTileSize = 32;

static LRESULT CALLBACK GridWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT PS;
        BeginPaint(hWnd, &PS);
        DrawGrid(hWnd, GFilePath, GGridWidth, GGridHeight, GTileSize);
        EndPaint(hWnd, &PS);
        return 0;
    }
    case WM_TIMER:
        InvalidateRect(hWnd, nullptr, FALSE);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hWnd, Msg, wParam, lParam);
}


//-----[Grid Thread]-----\\
//Runs GDI window on separate thread so engine keeps ticking

static void GridThread()
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    WNDCLASSA WC = {};
    WC.lpfnWndProc = GridWndProc;
    WC.hInstance = hInstance;
    WC.lpszClassName = "WonderlandGrid";
    WC.hbrBackground = CreateSolidBrush(RGB(20, 20, 20));
    RegisterClassA(&WC);

    HWND hWnd = CreateWindowA("WonderlandGrid", "Wonderland Framework — Grid Debug",
        WS_OVERLAPPEDWINDOW,
        100, 100,
        GGridWidth * GTileSize + 16,
        GGridHeight * GTileSize + 39,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, SW_SHOW);
    SetTimer(hWnd, 1, 100, nullptr);

    MSG Msg = {};
    while (GetMessage(&Msg, nullptr, 0, 0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}


//-----[Grid Visualizer]-----\\
//Writes grid state to file every tick

class GridVisualizer
{
private:
    std::string FilePath = "grid_state.txt";

public:

    //-----[Obligatory]-----\\
    //Constructor & Deconstructor

    GridVisualizer() {}
    GridVisualizer(const std::string& FP) { FilePath = FP; }
    ~GridVisualizer() {}


    //-----[Init]-----\\
    //Writes initial valid state file before launching

    void Init(const std::vector<CharacterMarker>& Characters, int GridWidth, int GridHeight)
    {
        std::ofstream File(FilePath);
        if (!File.is_open()) { return; }

        //-----[Write Characters]-----\\

        File << "CHARACTERS\n";
        for (auto& C : Characters)
        {
            File << C.Name << "," << C.X << "," << C.Y << "\n";
        }

        //-----[Write Tiles]-----\\

        File << "TILES\n";
        for (int X = 0; X < GridWidth; X++)
        {
            for (int Y = 0; Y < GridHeight; Y++)
            {
                File << X << "," << Y << ",None\n";
            }
        }
        File.close();
    }


    //-----[Tick Update]-----\\
    //Call this every tick from your engine

    void OnTick(
        const std::vector<CharacterMarker>& Characters,
        const std::vector<std::tuple<int, int, std::string>>& TileUpdates)
    {
        std::ofstream File(FilePath);
        if (!File.is_open()) { return; }

        //-----[Write Characters]-----\\

        File << "CHARACTERS\n";
        for (auto& C : Characters)
        {
            File << C.Name << "," << C.X << "," << C.Y << "\n";
        }

        //-----[Write Tiles]-----\\

        File << "TILES\n";
        for (auto& T : TileUpdates)
        {
            File << std::get<0>(T) << "," << std::get<1>(T) << "," << std::get<2>(T) << "\n";
        }

        File.close();
    }


    //-----[Launch]-----\\
    //Launches GDI grid window on separate thread

    void Launch()
    {
        std::thread(GridThread).detach();
    }
};