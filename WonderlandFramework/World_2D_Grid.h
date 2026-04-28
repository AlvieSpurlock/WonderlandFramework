#pragma once
#include "Consumable.h"

#include <random>
enum class RoomType
{
    None,
    Bedroom,
    Bathroom,
    LivingRoom,
    Kitchen,
    Hallway,
    Storage
};

enum class TileObject
{
    None,
    Desk,
    Bed,
    Chair,
    Bookshelf,
    Chest,
    Painting,
    Plant,
    Rug,
    Toilet,
    Sink,
    Bathtub,
    Sofa,
    CoffeeTable,
    TV,
    DiningTable,
    Stove,
    Fridge,
    Counter,
    LampPost,
    RoadMarking,
    Item
};

enum class WallDetail
{
    None,
    Window,
    Painting,
    Torch
};

struct Tile
{
    int X = 0;
    int Y = 0;
    bool IsBlocked = false;
    bool IsCover = false;
    bool IsWall = false;
    RoomType Room = RoomType::None;
    TileObject Object = TileObject::None;
    WallDetail Detail = WallDetail::None;
    std::string Name = "";
    Consumable* Item = nullptr;
};

struct RoomRegion
{
    int X1 = 0;
    int Y1 = 0;
    int X2 = 0;
    int Y2 = 0;
    RoomType Type = RoomType::None;
};

struct BaseStructures
{
    std::vector<std::vector<Tile>> VerticalWall;
    std::vector<std::vector<Tile>> HorizontalWall;
    std::vector<std::vector<Tile>> Block;
    std::vector<std::vector<Tile>> Railing;
    std::vector<std::vector<Tile>> Fences;
    std::vector<std::vector<Tile>> Room;
    std::vector<std::vector<Tile>> House;

    void InitGrid(std::vector<std::vector<Tile>>& iGrid, int MaxHeight, int MaxWidth)
    {
        iGrid.resize(MaxWidth);
        for (auto& Y : iGrid)
        {
            Y.resize(MaxHeight);
        }
        for (int X = 0; X < MaxWidth; X++)
        {
            for (int Y = 0; Y < MaxHeight; Y++)
            {
                iGrid[X][Y].X = X; iGrid[X][Y].Y = Y;
            }
        }
    }

    void PlaceWall(std::vector<std::vector<Tile>>& Grid, int X, int Y, const std::string& Name = "Wall")
    {
        Grid[X][Y].IsBlocked = true;
        Grid[X][Y].IsWall = true;
        Grid[X][Y].Name = Name;
    }

    void PlaceObject(std::vector<std::vector<Tile>>& Grid, int X, int Y, TileObject Obj, const std::string& Name, bool Blocked = true)
    {
        if (Grid[X][Y].IsBlocked || Grid[X][Y].Object != TileObject::None) { return; }
        Grid[X][Y].Object = Obj;
        Grid[X][Y].Name = Name;
        Grid[X][Y].IsBlocked = Blocked;
    }

    void FurnishRoom(std::vector<std::vector<Tile>>& Grid, RoomRegion& R, std::mt19937& RNG)
    {
        int X1 = R.X1; int Y1 = R.Y1;
        int X2 = R.X2; int Y2 = R.Y2;
        int RW = X2 - X1; int RH = Y2 - Y1;
        if (RW < 3 || RH < 3) { return; }

        std::uniform_int_distribution<int> CoinFlip(0, 1);
        std::uniform_int_distribution<int> IXDist(X1 + 1, X2 - 2);
        std::uniform_int_distribution<int> IYDist(Y1 + 1, Y2 - 2);

        // Mark room type on tiles
        for (int X = X1; X < X2; X++)
        {
            for (int Y = Y1; Y < Y2; Y++)
            { Grid[X][Y].Room = R.Type; }
        }

        if (R.Type == RoomType::Bedroom)
        {
            // Bed against top wall
            PlaceObject(Grid, X1 + 1, Y1 + 1, TileObject::Bed, "Bed");
            if (X1 + 2 < X2 - 1)
            { PlaceObject(Grid, X1 + 2, Y1 + 1, TileObject::Bed, "Bed"); }


            // Desk in corner
            PlaceObject(Grid, X2 - 2, Y2 - 2, TileObject::Desk, "Desk");

            PlaceObject(Grid, X2 - 2, Y2 - 3, TileObject::Chair, "Chair", false);
           
            
            // Painting on wall
            Grid[X1][Y1 + RH / 2].Detail = WallDetail::Painting;

            Grid[X1][Y1 + RH / 2].Name = "Painting";


            // Optional plant
            if (CoinFlip(RNG) == 1)
            { PlaceObject(Grid, X2 - 2, Y1 + 1, TileObject::Plant, "Plant", false); }
            
            
            // Window
            Grid[X1 + RW / 2][Y1].Detail = WallDetail::Window;
            Grid[X1 + RW / 2][Y1].Name = "Window";
        }
        else if (R.Type == RoomType::Bathroom)
        {
            // Toilet in corner
            PlaceObject(Grid, X1 + 1, Y1 + 1, TileObject::Toilet, "Toilet");
            
            
            // Sink next to toilet
            PlaceObject(Grid, X1 + 1, Y1 + 2, TileObject::Sink, "Sink");
            
            
            // Bathtub along wall
            PlaceObject(Grid, X2 - 2, Y1 + 1, TileObject::Bathtub, "Bathtub");
            if (Y1 + 2 < Y2 - 1)
            { PlaceObject(Grid, X2 - 2, Y1 + 2, TileObject::Bathtub, "Bathtub"); }
            
            
            // Window for ventilation
            Grid[X1 + RW / 2][Y1].Detail = WallDetail::Window;
            Grid[X1 + RW / 2][Y1].Name = "Window";
        }
        else if (R.Type == RoomType::LivingRoom)
        {
            // Sofa arrangement
            PlaceObject(Grid, X1 + 1, Y2 - 2, TileObject::Sofa, "Sofa", false);
            if (X1 + 2 < X2 - 1)
            { PlaceObject(Grid, X1 + 2, Y2 - 2, TileObject::Sofa, "Sofa", false); }
            
            
            // Coffee table
            PlaceObject(Grid, X1 + 2, Y2 - 3, TileObject::CoffeeTable, "Coffee Table");
            
            
            // TV on opposite wall
            PlaceObject(Grid, X1 + RW / 2, Y1 + 1, TileObject::TV, "TV");
            
            
            // Rug in center
            PlaceObject(Grid, X1 + RW / 2, Y1 + RH / 2, TileObject::Rug, "Rug", false);
            
            
            // Plants in corners
            PlaceObject(Grid, X2 - 2, Y2 - 2, TileObject::Plant, "Plant", false);
            
            
            // Paintings on walls
            Grid[X1][Y1 + RH / 2].Detail = WallDetail::Painting;
            Grid[X1][Y1 + RH / 2].Name = "Painting";
            Grid[X2 - 1][Y1 + RH / 2].Detail = WallDetail::Painting;
            Grid[X2 - 1][Y1 + RH / 2].Name = "Painting";
            
            
            // Windows
            Grid[X1 + RW / 3][Y1].Detail = WallDetail::Window;
            Grid[X1 + RW / 3][Y1].Name = "Window";
            Grid[X1 + 2 * RW / 3][Y1].Detail = WallDetail::Window;
            Grid[X1 + 2 * RW / 3][Y1].Name = "Window";
        }
        else if (R.Type == RoomType::Kitchen)
        {
            // Counter along top wall
            for (int X = X1 + 1; X < X2 - 1; X++)
            { PlaceObject(Grid, X, Y1 + 1, TileObject::Counter, "Counter"); }
            
            
            // Stove and fridge on counter
            PlaceObject(Grid, X1 + 1, Y1 + 1, TileObject::Stove, "Stove");
            
            PlaceObject(Grid, X2 - 2, Y1 + 1, TileObject::Fridge, "Fridge");
            
            
            // Dining table in center
            PlaceObject(Grid, X1 + RW / 2, Y1 + RH / 2, TileObject::DiningTable, "Dining Table");
            
            
            // Chairs around table
            if (X1 + RW / 2 - 1 > X1)
            { PlaceObject(Grid, X1 + RW / 2 - 1, Y1 + RH / 2, TileObject::Chair, "Chair", false); }
            
            if (X1 + RW / 2 + 1 < X2 - 1)
            { PlaceObject(Grid, X1 + RW / 2 + 1, Y1 + RH / 2, TileObject::Chair, "Chair", false); }
            
            
            // Sink on counter
            PlaceObject(Grid, X1 + RW / 2, Y1 + 1, TileObject::Sink, "Sink");
            
            
            // Window above counter
            Grid[X1 + RW / 2][Y1].Detail = WallDetail::Window;
            Grid[X1 + RW / 2][Y1].Name = "Window";
        }
        else if (R.Type == RoomType::Storage)
        {
            // Chests along walls
            for (int X = X1 + 1; X < X2 - 1 && X < X1 + 4; X++)
            { PlaceObject(Grid, X, Y1 + 1, TileObject::Chest, "Chest"); }
            
            
            // Bookshelf
            PlaceObject(Grid, X1 + 1, Y2 - 2, TileObject::Bookshelf, "Bookshelf");
            
            
            // Rug
            PlaceObject(Grid, X1 + RW / 2, Y1 + RH / 2, TileObject::Rug, "Rug", false);
        }

        // Random item scatter
        std::uniform_int_distribution<int> ItemCountDist(0, 3);
        int ItemCount = ItemCountDist(RNG);
        for (int I = 0; I < ItemCount; I++)
        {
            int IX = IXDist(RNG); int IY = IYDist(RNG);
            if (!Grid[IX][IY].IsBlocked && Grid[IX][IY].Object == TileObject::None)
            {
                Grid[IX][IY].Object = TileObject::Item;
                Grid[IX][IY].Name = "Item";
            }
        }
    }

    void GenerateHouse(std::vector<std::vector<Tile>>& Grid, int OffsetX, int OffsetY, int TotalWidth, int TotalHeight, int RoomCount, int OpeningCount, std::mt19937& RNG)
    {
        //Layout Zones
        int StreetHeight = 3;
        int FrontYardHeight = 4;
        int HouseHeight = TotalHeight - StreetHeight - FrontYardHeight - 4;
        int BackYardHeight = 3;
        int AlleyHeight = 1;

        int StreetY = OffsetY;
        int FrontYardY = StreetY + StreetHeight;
        int HouseY = FrontYardY + FrontYardHeight;
        int BackYardY = HouseY + HouseHeight;
        int AlleyY = BackYardY + BackYardHeight;

        InitGrid(House, TotalHeight, TotalWidth);

        // Street
        for (int X = 0; X < TotalWidth; X++)
        {
            for (int Y = StreetY; Y < StreetY + StreetHeight; Y++)
            {
                House[X][Y].Name = "Road";
                // Road markings in center of street
                if (Y == StreetY + StreetHeight / 2 && X % 4 == 0)
                {
                    House[X][Y].Object = TileObject::RoadMarking;
                    House[X][Y].Name = "Road Marking";
                }
            }
            // Lamp posts at street edge
            if (X % 8 == 0)
            {
                House[X][StreetY + StreetHeight - 1].Object = TileObject::LampPost;
                House[X][StreetY + StreetHeight - 1].Name = "Lamp Post";
                House[X][StreetY + StreetHeight - 1].IsBlocked = true;
            }
        }

        // Front Yard with Fences
        for (int X = 0; X < TotalWidth; X++)
        {
            for (int Y = FrontYardY; Y < FrontYardY + FrontYardHeight; Y++)
            { House[X][Y].Name = "Front Yard"; }


            // Fence along street edge
            House[X][FrontYardY].IsBlocked = true;
            House[X][FrontYardY].Name = "Fence";
        }
        // Fence gate opening
        std::uniform_int_distribution<int> GateDist(TotalWidth / 4, 3 * TotalWidth / 4);
        int GateX = GateDist(RNG);
        House[GateX][FrontYardY].IsBlocked = false;
        House[GateX][FrontYardY].Name = "Gate";

        // House Exterior Walls
        for (int X = 0; X < TotalWidth; X++)
        {
            PlaceWall(House, X, HouseY);
            PlaceWall(House, X, HouseY + HouseHeight - 1);
        }
        for (int Y = HouseY; Y < HouseY + HouseHeight; Y++)
        {
            PlaceWall(House, 0, Y);
            PlaceWall(House, TotalWidth - 1, Y);
        }

        // Front Door
        std::uniform_int_distribution<int> DoorDist(TotalWidth / 4, 3 * TotalWidth / 4);
        int FrontDoorX = DoorDist(RNG);
        House[FrontDoorX][HouseY].IsBlocked = false;
        House[FrontDoorX][HouseY].Name = "Front Door";

        // Back Door
        int BackDoorX = DoorDist(RNG);
        House[BackDoorX][HouseY + HouseHeight - 1].IsBlocked = false;
        House[BackDoorX][HouseY + HouseHeight - 1].Name = "Back Door";

        // Interior room generation
        int InteriorWidth = TotalWidth - 2;
        int InteriorHeight = HouseHeight - 2;
        int VerticalWalls = (RoomCount - 1) / 2;
        int HorizontalWalls = (RoomCount - 1) - VerticalWalls;
        int MinSpacingX = std::max(3, InteriorWidth / (VerticalWalls + 1));
        int MinSpacingY = std::max(3, InteriorHeight / (HorizontalWalls + 1));

        std::vector<int> VerticalDividers;
        std::vector<int> HorizontalDividers;

        // Vertical dividers
        std::vector<int> UsedVertical;
        for (int V = 0; V < VerticalWalls; V++)
        {
            int Placement = 0;
            bool Valid = false;
            while (!Valid)
            {
                std::uniform_int_distribution<int> pDist(1, InteriorWidth - 1);
                Placement = pDist(RNG);
                Valid = true;
                for (int Used : UsedVertical)
                {
                    if (abs(Placement - Used) < MinSpacingX)
                    { Valid = false; break; }
                }
            }
            UsedVertical.push_back(Placement);
            VerticalDividers.push_back(Placement + 1);
            std::uniform_int_distribution<int> sDist(HouseY + 1, HouseY + InteriorHeight / 2);
            int StartY = sDist(RNG);
            std::uniform_int_distribution<int> eDist(StartY, HouseY + InteriorHeight - 1);
            int EndY = eDist(RNG);
            std::uniform_int_distribution<int> gDist(StartY, EndY);
            int Gap = gDist(RNG);
            for (int Index = StartY; Index <= EndY; Index++)
            {
                if (Index == Gap) { continue; }
                PlaceWall(House, Placement + 1, Index);
            }
        }

        // Horizontal dividers
        std::vector<int> UsedHorizontal;
        for (int H = 0; H < HorizontalWalls; H++)
        {
            int Placement = 0;
            bool Valid = false;
            while (!Valid)
            {
                std::uniform_int_distribution<int> pDist(HouseY + 1, HouseY + InteriorHeight - 1);
                Placement = pDist(RNG);
                Valid = true;
                for (int Used : UsedHorizontal)
                {
                    if (abs(Placement - Used) < MinSpacingY)
                    { Valid = false; break; }
                }
            }
            UsedHorizontal.push_back(Placement);
            HorizontalDividers.push_back(Placement);
            std::uniform_int_distribution<int> sDist(1, InteriorWidth / 2);
            int StartX = sDist(RNG);
            std::uniform_int_distribution<int> eDist(StartX, InteriorWidth);
            int EndX = eDist(RNG);
            std::uniform_int_distribution<int> gDist(StartX, EndX);
            int Gap = gDist(RNG);
            for (int Index = StartX; Index <= EndX; Index++)
            {
                if (Index == Gap) { continue; }
                PlaceWall(House, Index, Placement);
            }
        }

        // Build room regions
        std::sort(VerticalDividers.begin(), VerticalDividers.end());
        std::sort(HorizontalDividers.begin(), HorizontalDividers.end());
        std::vector<int> XBounds = { 1 };
        for (int V : VerticalDividers) { XBounds.push_back(V); }
        XBounds.push_back(TotalWidth - 1);
        std::vector<int> YBounds = { HouseY + 1 };
        for (int H : HorizontalDividers) { YBounds.push_back(H); }
        YBounds.push_back(HouseY + HouseHeight - 1);


        // Assign room types — ensure at least one of each key room
        std::vector<RoomType> RequiredRooms = { RoomType::Kitchen, RoomType::LivingRoom, RoomType::Bathroom };
        std::vector<RoomType> OptionalRooms = { RoomType::Bedroom, RoomType::Storage, RoomType::Bedroom };
        std::vector<RoomRegion> Regions;

        int RegionIndex = 0;
        for (size_t Xi = 0; Xi < XBounds.size() - 1; Xi++)
        {
            for (size_t Yi = 0; Yi < YBounds.size() - 1; Yi++)
            {
                RoomRegion R;
                R.X1 = XBounds[Xi]; R.Y1 = YBounds[Yi];
                R.X2 = XBounds[Xi + 1]; R.Y2 = YBounds[Yi + 1];
                if (RegionIndex < (int)RequiredRooms.size())
                { R.Type = RequiredRooms[RegionIndex]; }
                else if (RegionIndex - (int)RequiredRooms.size() < (int)OptionalRooms.size())
                { R.Type = OptionalRooms[RegionIndex - RequiredRooms.size()]; }
                else
                { R.Type = RoomType::Bedroom; }
                Regions.push_back(R);
                RegionIndex++;
            }
        }

        // Furnish all rooms
        for (auto& R : Regions)
        { FurnishRoom(House, R, RNG); }

        // Back Yard with Railings
        for (int X = 0; X < TotalWidth; X++)
        {
            for (int Y = BackYardY; Y < BackYardY + BackYardHeight; Y++)
            { House[X][Y].Name = "Back Yard"; }
            House[X][BackYardY].IsBlocked = true;
            House[X][BackYardY].Name = "Railing";
            House[X][BackYardY + BackYardHeight - 1].IsBlocked = true;
            House[X][BackYardY + BackYardHeight - 1].Name = "Railing";
        }

        // Alley
        for (int X = 0; X < TotalWidth; X++)
        {
            for (int Y = AlleyY; Y < AlleyY + AlleyHeight; Y++)
            { House[X][Y].Name = "Alley"; }
        }

        std::cout << "House Generated!\n"
            << "Total Size: " << TotalWidth << "x" << TotalHeight << "\n"
            << "Rooms: " << RoomCount << "\n"
            << "Street: Y " << StreetY << " to " << StreetY + StreetHeight << "\n"
            << "Front Yard: Y " << FrontYardY << " to " << FrontYardY + FrontYardHeight << "\n"
            << "House: Y " << HouseY << " to " << HouseY + HouseHeight << "\n"
            << "Back Yard: Y " << BackYardY << " to " << BackYardY + BackYardHeight << "\n"
            << "Alley: Y " << AlleyY << "\n";
    }
};


class Map
{
private:
    int TotalWidth = 0;
    int TotalHeight = 0;
    int RoadWidth = 3;
    BaseStructures BS;
public:
    std::vector<std::vector<Tile>> Grid;


    //-----[Obligatory]-----\\
    //Constructor & Deconstructor

    Map() {}
    Map(int HouseWidth, int HouseHeight, int RoomCount, std::mt19937& RNG)
    {
        Generate(HouseWidth, HouseHeight, RoomCount, RNG);
    }


    //-----[Accessors]-----\\

    int GetTotalWidth() const { return TotalWidth; }
    int GetTotalHeight() const { return TotalHeight; }
    Tile& GetTile(int X, int Y) { return Grid[X][Y]; }
    bool InBounds(int X, int Y) const { return X >= 0 && Y >= 0 && X < TotalWidth && Y < TotalHeight; }


    //-----[Generation]-----\\

    void Generate(int HouseWidth, int HouseHeight, int RoomCount, std::mt19937& RNG)
    {
        TotalWidth = (HouseWidth * 2) + RoadWidth;
        TotalHeight = (HouseHeight * 2) + RoadWidth;
        Grid.resize(TotalWidth);
        for (auto& Col : Grid)
        {
            Col.resize(TotalHeight);
        }
        for (int X = 0; X < TotalWidth; X++)
        {
            for (int Y = 0; Y < TotalHeight; Y++)
            {
                Grid[X][Y].X = X; Grid[X][Y].Y = Y;
            }
        }
        int H2X = HouseWidth + RoadWidth;
        int H2Y = HouseHeight + RoadWidth;
        BS.GenerateHouse(Grid, 0, 0, HouseWidth, HouseHeight, RoomCount, 2, RNG); // Top Left
        BS.GenerateHouse(Grid, H2X, 0, HouseWidth, HouseHeight, RoomCount, 2, RNG); // Top Right
        BS.GenerateHouse(Grid, 0, H2Y, HouseWidth, HouseHeight, RoomCount, 2, RNG); // Bottom Left
        BS.GenerateHouse(Grid, H2X, H2Y, HouseWidth, HouseHeight, RoomCount, 2, RNG); // Bottom Right
        GenerateCrossroads(RNG);
    }

    void GenerateCrossroads(std::mt19937& RNG)
    {
        int RoadStartX = TotalWidth / 2 - RoadWidth / 2;
        int RoadStartY = TotalHeight / 2 - RoadWidth / 2;

        // Vertical road strip
        for (int X = RoadStartX; X < RoadStartX + RoadWidth; X++)
        {
            for (int Y = 0; Y < TotalHeight; Y++)
            {
                Grid[X][Y].Name = "Road";
                Grid[X][Y].IsBlocked = false;
                Grid[X][Y].IsWall = false;
                if (X == RoadStartX + RoadWidth / 2 && Y % 4 == 0)
                {
                    Grid[X][Y].Object = TileObject::RoadMarking;
                    Grid[X][Y].Name = "Road Marking";
                }
            }
        }

        // Horizontal road strip
        for (int Y = RoadStartY; Y < RoadStartY + RoadWidth; Y++)
        {
            for (int X = 0; X < TotalWidth; X++)
            {
                Grid[X][Y].Name = "Road";
                Grid[X][Y].IsBlocked = false;
                Grid[X][Y].IsWall = false;
                if (Y == RoadStartY + RoadWidth / 2 && X % 4 == 0)
                {
                    Grid[X][Y].Object = TileObject::RoadMarking;
                    Grid[X][Y].Name = "Road Marking";
                }
            }
        }

        // Lamp posts at intersection corners
        Grid[RoadStartX - 1][RoadStartY - 1].Object = TileObject::LampPost;
        Grid[RoadStartX - 1][RoadStartY - 1].Name = "Lamp Post";
        Grid[RoadStartX - 1][RoadStartY - 1].IsBlocked = true;

        Grid[RoadStartX + RoadWidth][RoadStartY - 1].Object = TileObject::LampPost;
        Grid[RoadStartX + RoadWidth][RoadStartY - 1].Name = "Lamp Post";
        Grid[RoadStartX + RoadWidth][RoadStartY - 1].IsBlocked = true;

        Grid[RoadStartX - 1][RoadStartY + RoadWidth].Object = TileObject::LampPost;
        Grid[RoadStartX - 1][RoadStartY + RoadWidth].Name = "Lamp Post";
        Grid[RoadStartX - 1][RoadStartY + RoadWidth].IsBlocked = true;

        Grid[RoadStartX + RoadWidth][RoadStartY + RoadWidth].Object = TileObject::LampPost;
        Grid[RoadStartX + RoadWidth][RoadStartY + RoadWidth].Name = "Lamp Post";
        Grid[RoadStartX + RoadWidth][RoadStartY + RoadWidth].IsBlocked = true;
    }

    //-----[Movement]-----\\

    bool CanMoveTo(int X, int Y)
    {
        if (!InBounds(X, Y))
        {
            std::cout << "Out of bounds!\n"; return false;
        }
        if (Grid[X][Y].IsBlocked)
        {
            std::cout << "Blocked by: " << Grid[X][Y].Name << "\n"; return false;
        }
        return true;
    }

    //-----[Surroundings]-----\\

    void DescribeSurroundings(int X, int Y)
    {
        std::vector<std::pair<std::string, std::string>> Surroundings;
        auto Check = [&](int DX, int DY, std::string Dir)
            {
                int NX = X + DX; int NY = Y + DY;
                if (!InBounds(NX, NY)) { return; }
                Tile& T = Grid[NX][NY];
                if (T.Name != "") { Surroundings.push_back({ Dir, T.Name }); }
            };
        Check(-1, 0, "West");
        Check(1, 0, "East");
        Check(0, -1, "North");
        Check(0, 1, "South");
        Check(-1, -1, "Northwest");
        Check(1, -1, "Northeast");
        Check(-1, 1, "Southwest");
        Check(1, 1, "Southeast");

        std::cout << "You are in: " << Grid[X][Y].Name << "\n";
        for (auto& S : Surroundings)
        {
            std::cout << S.first << ": " << S.second << "\n";
        }
    }

    //-----[Ollama Description]-----\\

    std::string BuildOllamaContext(int X, int Y)
    {
        std::string Context = "You are describing a game world to a player in second person present tense. Keep it under 3 sentences. ";
        Context += "The player is standing on a " + Grid[X][Y].Name + ". ";
        Context += "Nearby they can see: ";
        auto Check = [&](int DX, int DY, std::string Dir) -> std::string
            {
                int NX = X + DX; int NY = Y + DY;
                if (!InBounds(NX, NY)) { return ""; }
                if (Grid[NX][NY].Name != "")
                {
                    return Dir + " is a " + Grid[NX][NY].Name + ". ";
                }
                return "";
            };
        Context += Check(-1, 0, "To the west");
        Context += Check(1, 0, "To the east");
        Context += Check(0, -1, "To the north");
        Context += Check(0, 1, "To the south");
        return Context;
    }
};