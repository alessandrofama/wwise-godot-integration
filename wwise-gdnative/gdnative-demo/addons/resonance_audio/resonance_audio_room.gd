extends Spatial

enum SurfaceMaterial {
		Transparent = 0,              #< Transparent
		AcousticCeilingTiles = 1,     #< Acoustic ceiling tiles
		BrickBare = 2,                #< Brick, bare
		BrickPainted = 3,             #< Brick, painted
		ConcreteBlockCoarse = 4,      #< Concrete block, coarse
		ConcreteBlockPainted = 5,     #< Concrete block, painted
		CurtainHeavy = 6,             #< Curtain, heavy
		FiberglassInsulation = 7,     #< Fiberglass insulation
		GlassThin = 8,                #< Glass, thin
		GlassThick = 9,               #< Glass, thick
		Grass = 10,                   #< Grass
		LinoleumOnConcrete = 11,      #< Linoleum on concrete
		Marble = 12,                  #< Marble
		Metal = 13,                   #< Galvanized sheet metal
		ParquetOnConcrete = 14,       #< Parquet on concrete
		PlasterRough = 15,            #< Plaster, rough
		PlasterSmooth = 16,           #< Plaster, smooth
		PlywoodPanel = 17,            #< Plywood panel
		PolishedConcreteOrTile = 18,  #< Polished concrete or tile
		Sheetrock = 19,               #< Sheetrock
		WaterOrIceSurface = 20,       #< Water or ice surface
		WoodCeiling = 21,             #< Wood ceiling
		WoodPanel = 22                #< Wood panel
	}

# Room surface material in negative x direction.
export(SurfaceMaterial) var left_wall = SurfaceMaterial.ConcreteBlockCoarse

# Room surface material in positive x direction.
export(SurfaceMaterial) var right_wall = SurfaceMaterial.ConcreteBlockCoarse

# Room surface material in negative y direction.
export(SurfaceMaterial) var _floor = SurfaceMaterial.ParquetOnConcrete

# Room surface material in positive y direction.
export(SurfaceMaterial) var ceiling = SurfaceMaterial.PlasterRough

# Room surface material in negative z direction.
export(SurfaceMaterial) var back_wall = SurfaceMaterial.ConcreteBlockCoarse

# Room surface material in positive z direction.
export(SurfaceMaterial) var front_wall = SurfaceMaterial.ConcreteBlockCoarse

# Reflectivity scalar for each surface of the room.
export(float) var reflectivity = 1.0

# Reverb gain modifier in decibels.
export(float) var reverb_gain_db = 0.0

# Reverb brightness modifier.
export(float) var reverb_brightness = 0.0;

# Reverb time modifier.
export(float) var reverb_time = 1.0;

# Size of the room (normalized with respect to scale of the game object).
export(Vector3) var size = Vector3.ONE

export(AK.BUSSES._enum) var room_effects_bus;

func get_listener() -> Spatial: 
	return get_tree().get_root().find_node("AkListener", true, false) as Spatial

func _ready():
	Wwise.update_audio_room(self, Wwise.is_listener_inside_room(self, get_listener()))
	
func _exit_tree():
	Wwise.update_audio_room(self, false)

func _process(delta):
	Wwise.update_audio_room(self, Wwise.is_listener_inside_room(self, get_listener()))
