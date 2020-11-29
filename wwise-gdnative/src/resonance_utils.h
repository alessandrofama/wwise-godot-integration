#include <cmath>

static inline void convertAmplitudeFromDb(const float& inDb, float& outAmplitude)
{
    outAmplitude = pow(10.0f, 0.05f * inDb);
}

static inline void scaleVector(const Vector3& inVectorA, const Vector3& inVectorB, Vector3& outVector)
{
    outVector.x = inVectorA.x * inVectorB.x;
    outVector.y = inVectorA.y * inVectorB.y;
    outVector.z = inVectorA.z * inVectorB.z;
}

static inline bool hasPoint(const AABB& bounds, const Vector3& relativePosition, const Quat& rotationInverse)
{
    float minX = bounds.position.x - bounds.size.x * 0.5f;
    float maxX = bounds.position.x + bounds.size.x * 0.5f;
    float minY = bounds.position.y - bounds.size.y * 0.5f;
    float maxY = bounds.position.y + bounds.size.y * 0.5f;
    float minZ = bounds.position.z - bounds.size.z * 0.5f;
    float maxZ = bounds.position.z + bounds.size.z * 0.5f;

    float x = rotationInverse.xform(relativePosition).x;
    float y = rotationInverse.xform(relativePosition).y;
    float z = rotationInverse.xform(relativePosition).z;

    return (x > minX && x < maxX && y > minY && y < maxY && z > minZ && z < maxZ) ? true : false;
}

enum SurfaceMaterial
{
    Transparent = 0,
    AcousticCeilingTiles = 1,
    BrickBare = 2,
    BrickPainted = 3,
    ConcreteBlockCoarse = 4,
    ConcreteBlockPainted = 5,
    CurtainHeavy = 6,
    FiberglassInsulation = 7,
    GlassThin = 8,
    GlassThick = 9,
    Grass = 10,
    LinoleumOnConcrete = 11,
    Marble = 12,
    Metal = 13,
    ParquetOnConcrete = 14,
    PlasterRough = 15,
    PlasterSmooth = 16,
    PlywoodPanel = 17,
    PolishedConcreteOrTile = 18,
    Sheetrock = 19,
    WaterOrIceSurface = 20,
    WoodCeiling = 21,
    WoodPanel = 22
};

struct RoomProperties
{
    float positionX;
    float positionY;
    float positionZ;

    // Rotation (quaternion) of the room in world space.
    float rotationX;
    float rotationY;
    float rotationZ;
    float rotationW;

    // Size of the shoebox room in world space.
    float dimensionsX;
    float dimensionsY;
    float dimensionsZ;

    // Material name of each surface of the shoebox room.
    SurfaceMaterial materialLeft;
    SurfaceMaterial materialRight;
    SurfaceMaterial materialBottom;
    SurfaceMaterial materialTop;
    SurfaceMaterial materialFront;
    SurfaceMaterial materialBack;

    // User defined uniform scaling factor for reflectivity. This parameter has no effect when set
    // to 1.0f.
    float reflectionScalar;

    // User defined reverb tail gain multiplier. This parameter has no effect when set to 0.0f.
    float reverbGain;

    // Adjusts the reverberation time across all frequency bands. RT60 values are multiplied by this
    // factor. Has no effect when set to 1.0f.
    float reverbTime;

    // Controls the slope of a line from the lowest to the highest RT60 values (increases high
    // frequency RT60s when positive, decreases when negative). Has no effect when set to 0.0f.
    float reverbBrightness;
};

static inline void getRoomProperties(const Spatial* room, RoomProperties& outRoomProperties) 
{
	Vector3 position = room->get_global_transform().origin;
	Quat rotation = Quat(room->get_global_transform().basis.orthonormalized());
	Vector3 scale;	scaleVector(room->get_scale(), static_cast<Vector3>(room->get("size")), scale);
	outRoomProperties.positionX = position.x;
    outRoomProperties.positionY = position.y;
    outRoomProperties.positionZ = position.z;
    outRoomProperties.rotationX = rotation.x;
    outRoomProperties.rotationY = rotation.y;
    outRoomProperties.rotationZ = rotation.z;
    outRoomProperties.rotationW = rotation.w;
    outRoomProperties.dimensionsX = scale.x;
    outRoomProperties.dimensionsY = scale.y;
    outRoomProperties.dimensionsZ = scale.z;
    outRoomProperties.materialLeft = static_cast<SurfaceMaterial>((int)room->get("left_wall"));
    outRoomProperties.materialRight = static_cast<SurfaceMaterial>((int)room->get("right_wall"));
    outRoomProperties.materialBottom = static_cast<SurfaceMaterial>((int)room->get("_floor"));
    outRoomProperties.materialTop = static_cast<SurfaceMaterial>((int)room->get("ceiling"));
    outRoomProperties.materialFront = static_cast<SurfaceMaterial>((int)room->get("front_wall"));
    outRoomProperties.materialBack = static_cast<SurfaceMaterial>((int)room->get("back_wall"));
    convertAmplitudeFromDb(room->get("reverb_gain_db"), outRoomProperties.reverbGain);
    outRoomProperties.reverbTime = (float)room->get("reverb_time");
    outRoomProperties.reverbBrightness = (float)room->get("reverb_brightness");
    outRoomProperties.reflectionScalar = (float)room->get("reflectivity");
}
