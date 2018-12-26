#pragma once

typedef enum 
{
    LIGHT_ID_AMBIENT=-1,
    LIGHT_ID_1=0,
    LIGHT_ID_2,
    LIGHT_ID_3,
    LIGHT_ID_4,
    LIGHT_ID_5,
    LIGHT_ID_6,
    LIGHT_ID_7,
    LIGHT_ID_8,
    MAX_LIGHT
} LightID;

typedef  enum 
{
    LIGHT_TYPE_DIRECTIONAL,
    LIGHT_TYPE_POINT,
    LIGHT_TYPE_SPOT
} LightType;

typedef  enum 
{
    LIGHT_SPACE_VIEW,
    LIGHT_SPACE_LOCAL
} LightSpace;

typedef  enum
{
	LIGHT_ATTENUATION_NONE,
	LIGHT_ATTENUATION_LINEAR,
	LIGHT_ATTENUATION_POWEROF2
} LightAttenuation;


class LightParams
{
public:
    //light enabled
    bool Enabled;
    //type directional,point,spot
    LightType Type;
    //local or view space
    LightSpace Space;
	// Attenuation type
	LightAttenuation Attenuation;

    //color 0-255 RGB
    int ColorR;
    int ColorG;
    int ColorB;

    //position
    double PosX;
    double PosY;
    double PosZ;

    //direction
    double DirX;
    double DirY;
    double DirZ;
    
    LightParams():
	Enabled(false), Type(LIGHT_TYPE_DIRECTIONAL), Space(LIGHT_SPACE_VIEW),
	Attenuation(LIGHT_ATTENUATION_NONE),
	ColorR(255), ColorG(255), ColorB(255), PosX(0), PosY(0), PosZ(0),
	DirX(0), DirY(0), DirZ(0)
    {}

protected:
private:
};

class Light
{
public:
	Light();
	~Light();
};
